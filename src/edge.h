#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode, QString inputListStr, QString reInputListStr = "");
    ~Edge();

    Node *sourceNode() const;
    Node *destNode() const;
    QString getInputList() const;
    QString getReInputList() const;
    void setInputList(QString newInputList);
    void setReInputList(QString newReInputList);

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Node *source, *dest;
    QString inputList, reInputList;
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize = 10; // 箭头大小
};

#endif // EDGE_H
