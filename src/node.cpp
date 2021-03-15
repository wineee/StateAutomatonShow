#include "edge.h"
#include "node.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QtDebug>
#include <graphwidget.h>

QVector<Node *> Node::nodeList;

Node::Node(GraphWidget *graphWidget, QString name, int nodeCas)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable); // 可被鼠标拖动
    setFlag(ItemSendsGeometryChanges); // itemChange()通知发生位置移动
    setCacheMode(DeviceCoordinateCache); // 加速渲染
    setZValue(10); // 放上层 容易点到

    m_nodeName = name;
    m_nodeCas = nodeCas;

    nodeList.push_back(this);
}

Node::~Node() {
    nodeList.removeAll(this);
    foreach (Edge * p, edgeList)
        delete p;
    edgeList.clear();
}

void Node::setName(QString name) {
    m_nodeName = name;
}
QString Node::getName() {
    return m_nodeName;
}

void Node::setCas(int newCas) {
    m_nodeCas = newCas;
}

int Node::getCas() {
    return m_nodeCas;
}

void Node::addEdge(Edge *edge) {
    edgeList << edge;
    edge->adjust(); // 调整边的位置
}

void Node::removeEdge(Edge *edge) {
    edgeList.removeAll(edge);
    qDebug() << "rmEdge" << edgeList.size() << " \n";
}


QVector<Edge *> Node::edges() const {
    return edgeList;
}

QVector<Node *> Node::getNodeList() {
    return nodeList;
}

//void Node::setautoAdjust(int newAutoAdjust) {
//    this->autoAdjust = newAutoAdjust;
//}


void Node::calculateForces()
{
    // 未被scene托管 或 鼠标选中的点 不发生移动
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    // 计算节点间的斥力
    qreal xvel = 0;
    qreal yvel = 0;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        Node *node = qgraphicsitem_cast<Node *>(item); // 强制转换
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 1e-7) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    // 计算节点间聚集的力 节点间连线的长度决定聚集的力的大小
    const int k = 30;
    double weight = (edgeList.size() + 1) * k;
    for (const Edge *edge : qAsConst(edgeList))
        if (edge->sourceNode() == edge->destNode()) weight -= k; // 自环不影响
    for (const Edge *edge : qAsConst(edgeList)) {
        if (edge->sourceNode() == edge->destNode()) continue;
        QPointF vec;
        if (edge->sourceNode() == this) {
            vec = mapToItem(edge->destNode(), 0, 0);
        }
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

    // 设置当 < 0.1 时候，就认为是 0
    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    // 保证新的坐标位置仍然在我们设定的边界内
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}


// 根据newPos更新位置
bool Node::advancePosition()
{
    if (newPos == pos())
        return false;
    setPos(newPos);
    return true;
}

// paint 绘制的图像必须在 boundingRect 函数之中
QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

// 用来检测碰撞
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

// 画出圆点
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    painter->setPen(QPen(Qt::black, 0));
    painter->setBrush(Qt::white);

    painter->drawEllipse(-10, -10, 20, 20);

    if (m_nodeCas & 1) { //
        painter->setBrush(Qt::lightGray);
        painter->drawEllipse(-10, -10, 20, 20);
    }

    if (m_nodeCas & 2) {
       painter->drawEllipse(-7, -7, 14, 14);
    }

    QRectF textRect(-10, -10, 20, 20);
    painter->setPen(Qt::black);

    int flags = Qt::TextWordWrap | Qt::AlignCenter; // 自动换行，中间对齐
    painter->drawText(textRect, flags, m_nodeName);
}

// 拖动点位置, 更新相连的边
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (Edge *edge : qAsConst(edgeList))
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

// 鼠标点击事件
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

// 鼠标释放事件
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
