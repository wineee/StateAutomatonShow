#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include "addedgedlg.h"
#include "addnodedlg.h"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>
#include <QDebug>
#include <QMessageBox>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)/*,tmpInitState(nullptr)*/
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 250, 250); // 逻辑地址
    setScene(scene);
    setCacheMode(CacheBackground); // 缓存静态背景
    setViewportUpdateMode(BoundingRectViewportUpdate); // 设置视图的更新模式
    setRenderHint(QPainter::Antialiasing); // 抗锯齿
    setTransformationAnchor(AnchorUnderMouse); // 缩放时以鼠标为中心
    scale(qreal(1.5), qreal(1.5));
    setMinimumSize(400, 400);
    setWindowTitle(tr("StateAutomaton"));
}

// 主要用于文件恢复数据
void GraphWidget::addNode(QString name, int Cas, qreal x, qreal y) {
    Node *node = new Node(this, name, Cas);
    this->scene()->addItem(node);
    node->setPos(x, y);
}

void GraphWidget::addEdge(QString A, QString B, QString input, QString reInput) {
    Node *nodeA = nullptr, *nodeB = nullptr;
    foreach (Node * node, Node::getNodeList()) {
        if (node->getName() == A) {
            nodeA = node;
        }
        if (node->getName() == B) {
            nodeB = node;
        }
    }
    if (!nodeA || !nodeB) {
        qDebug() << "Err in addEdge";
        return;
    }
    this->scene()->addItem(new Edge(nodeA, nodeB, input, reInput));
}

void GraphWidget::setmode(QGRAPH_CAS newMode) {
    mode = newMode;
}

void GraphWidget::setautoAdjust(int newAutoAdjust) {
    this->autoAdjust = newAutoAdjust;
}

void GraphWidget::mousePressEvent(QMouseEvent *event) {
    switch (mode) {
    case QGRAPH_CAS::ADDNODE: { // 添加点
        addNodeDlg dlg;
        if (dlg.exec() == QDialog::Accepted) {
            if (dlg.getName().isEmpty()) {
                 QMessageBox::warning(this, tr("警告"), tr("输入不能为空!"));
                 return;
            }
            foreach (Node *tmp, Node::getNodeList())
                if (tmp->getName() == dlg.getName()) {
                    QMessageBox::warning(this, tr("警告"), tr("状态不能重复!"));
                    return;
                }
            Node *node = new Node(this, dlg.getName(), dlg.getCas());
            /*
             * NFA不限制初态个数
            if (dlg.getCas() & 1) { 
                if (tmpInitState != nullptr) {
                    tmpInitState->setCas(tmpInitState->getCas() & 2);
                    tmpInitState->hide(); tmpInitState->show();
                }
                tmpInitState = node;
            }
            */
            this->scene()->addItem(node);
            node->setPos(mapToScene(event->pos()));
        }
        break;
    }
    case QGRAPH_CAS::ADDEDGE1: { // 添加边， 顶点1
        QGraphicsItem *item = itemAt(event->pos());
        if (item->type() == QGraphicsItem::UserType + 1) {
            tmpNode1 = qgraphicsitem_cast<Node *>(item);
            qDebug() << "Get Edge 1 " << tmpNode1->getName();
            mode = QGRAPH_CAS::ADDEDGE2;
        }
        break;
    }
    case QGRAPH_CAS::ADDEDGE2: { // 添加边， 顶点2
        QGraphicsItem *item = itemAt(event->pos());
        if (item->type() == QGraphicsItem::UserType + 1) {
            tmpNode2 = qgraphicsitem_cast<Node *>(item);
            qDebug() << "Get Edge 2 " << tmpNode2->getName();
            addEdgeDlg dlg(tmpNode1->getName(), tmpNode2->getName(), nullptr);
            if (dlg.exec() == QDialog::Accepted) {
                if (dlg.getInputList().isEmpty()) {
                     QMessageBox::warning(this, tr("警告"), tr("输入不能为空!"));
                     return;
                }
                Edge *newEdge = new Edge(tmpNode1, tmpNode2, dlg.getInputList(), dlg.getReInputList());
                scene()->addItem(newEdge);
                newEdge->adjust();
            }
            mode = QGRAPH_CAS::ADDEDGE1;
        }
        break;
    }
    case QGRAPH_CAS::DELETE: { // 删除
        QGraphicsItem *item = itemAt(event->pos());
        delete item;
        break;
    }
    case QGRAPH_CAS::MODIFY: { // 修改
        QGraphicsItem *item = itemAt(event->pos());
        if (item->type() == QGraphicsItem::UserType + 1) { // 修改点
            Node *tmp = qgraphicsitem_cast<Node *>(item);
            addNodeDlg dlg(tmp->getName(), tmp->getCas());
            if (dlg.exec() == QDialog::Accepted) {
               if (dlg.getName().isEmpty()) {
                    QMessageBox::warning(this, tr("警告"), tr("输入不能为空!"));
                    return;
               }
               foreach (Node *t, Node::getNodeList())
                   if (t->getName() == dlg.getName() && t != tmp) {
                       QMessageBox::warning(this, tr("警告"), tr("无效修改!"));
                       return;
                    }
               tmp->setName(dlg.getName());
               tmp->setCas(dlg.getCas());
               tmp->hide(); tmp->show();
               /*
               if (dlg.getCas() & 1) {
                    if (tmpInitState != nullptr && tmpInitState != tmp) {
                       tmpInitState->setCas(tmpInitState->getCas() & 2);
                       tmpInitState->hide(); tmpInitState->show();
                   }
                   tmpInitState = tmp;
               }*/
            }
        } else if (item->type() == QGraphicsItem::UserType + 2) {  // 修改边
            Edge *tmp = qgraphicsitem_cast<Edge *>(item);
            addEdgeDlg dlg(tmp->sourceNode()->getName(), tmp->destNode()->getName(),
                           tmp->getInputList(), tmp->getReInputList());
            if (dlg.exec() == QDialog::Accepted) {
                tmp->setInputList(dlg.getInputList());
                tmp->setReInputList(dlg.getReInputList());
            }
        }
        break;
    }
    case QGRAPH_CAS::MOVE:
    default:
        QGraphicsView::mousePressEvent(event);
    }
}

// 键盘事件响应
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Equal:
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

// 定时器事件, 自动调整需要
 void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (this->autoAdjust == false) return;

    QVector<Node *> nodes;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    for (Node *node : qAsConst(nodes))
        node->calculateForces();

    bool itemsMoved = false;
    for (Node *node : qAsConst(nodes)) {
        if (node->advancePosition())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

// 滚轮放大缩小
#if QT_CONFIG(wheelevent)
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2., -event->angleDelta().y() / 240.0));
}
#endif

// 画出背景
void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
//    QRectF sceneRect = this->sceneRect();
//    painter->setBrush(Qt::NoBrush);
//    painter->setPen(Qt::DashLine);
//    painter->drawRect(sceneRect);
    QGraphicsView::drawBackground(painter, rect);
}

//缩放 scaleFactor 倍
void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100) // 防止视图过小 or 过大
        return;

    scale(scaleFactor, scaleFactor);
}

// 打乱顺序
void GraphWidget::shuffle()
{
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(QRandomGenerator::global()->bounded(250), QRandomGenerator::global()->bounded(250));
    }
}

// 放大
void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

// 缩小
void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
