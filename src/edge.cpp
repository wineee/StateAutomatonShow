#include "edge.h"
#include "node.h"

#include <QPainter>
#include <QtMath>
#include <QString>
#include <QtDebug>

Edge::Edge(Node *sourceNode, Node *destNode, QString inputListStr, QString reInputListStr)
    : source(sourceNode), dest(destNode),inputList(inputListStr),reInputList(reInputListStr)
{
    setAcceptedMouseButtons(Qt::NoButton); // 该类不需要接受鼠标的按钮事件
    source->addEdge(this);
    if(source != dest)
       dest->addEdge(this);
    adjust();
}

Edge::~Edge() {
    source->removeEdge(this);
    if (source != dest)
        dest->removeEdge(this);
}

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

QString Edge::getInputList() const {
    return inputList;
}
QString Edge::getReInputList() const {
    return reInputList;
}

void Edge::setInputList(QString newInputList) {
    inputList = newInputList;
}

void Edge::setReInputList(QString newReInputList) {
    reInputList = newReInputList;
}

void Edge::adjust() {
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();
    // 可能改变 QGraphicsItem 大小或者形状的时候，QGraphicsItem 子类函数中先调用 prepareGeometryChange

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

// paint 绘制的图像必须在 boundingRect 函数之中
QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal extra = 10;
    if (qFuzzyCompare(QLineF(sourcePoint, destPoint).length(), qreal(0.))) {
        return QRectF(sourcePoint.x(), sourcePoint.y()-6, 40, 40);
    }
    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

// 绘制边
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;
    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.))) {
        // 自环
        QRectF rectangle = QRectF(sourcePoint.x(), sourcePoint.y(), 30, 30);
        double startAngle = (180-18.44) * 16; // 起始角度  atan(1/3) = 18.44
        double spanAngle = (270+18.44*2) * 16; // 覆盖的角度，绘制方向为逆时针方向
        painter->drawArc(rectangle, startAngle, spanAngle);
        // 箭头
        double ArrowX = rectangle.x() + 10;
        double ArrowY = rectangle.y() + 0;
        painter->drawLine(ArrowX, ArrowY, ArrowX+5, ArrowY-4);
        painter->drawLine(ArrowX, ArrowY, ArrowX+5, ArrowY+5);
        // 文字
        int flags = Qt::TextWordWrap | Qt::AlignCenter;
        QFont font = painter->font();
        font.setItalic(true);
        font.setPointSize(8);
        painter->setFont(font);
        painter->drawText(rectangle, flags, inputList);
        return;
    }

    // 画出直线
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

     // 画出箭头
    double angle = std::atan2(-line.dy(), line.dx());
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize);
    if (!reInputList.isEmpty())
        painter->setPen(Qt::blue);
    painter->drawLine(line.p2(), destArrowP1);
    painter->drawLine(line.p2(), destArrowP2);

    if (!reInputList.isEmpty()) {  // 反向边箭头
        QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI / 3) * arrowSize);
        QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI - M_PI / 3) * arrowSize);
        painter->setPen(Qt::red);
        painter->drawLine(line.p1(), sourceArrowP1);
        painter->drawLine(line.p1(), sourceArrowP2);
        painter->setPen(Qt::black);
    }

    // 文字
    if (!reInputList.isEmpty())
        painter->setPen(Qt::blue);
    painter->translate(line.p1().x(), line.p1().y());
    painter->save();
    QRectF textRect = QRectF(0, 0, line.length(), 10);
    int flags = Qt::TextWordWrap | Qt::AlignCenter;
    painter->rotate(-angle * 180.0 / 3.14159265);
    painter->drawText(textRect, flags, inputList);
    painter->restore();

    // 反向边文字
    if (!reInputList.isEmpty()) {
        painter->setPen(Qt::red);
        QRectF textRect = QRectF(0, 0, line.length(), -10);
        painter->rotate(-angle * 180.0 / M_PI);
        painter->drawText(textRect, flags, reInputList);
    }
}
