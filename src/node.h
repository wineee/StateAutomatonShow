#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QVector>
#include <QString>

class Edge;
class GraphWidget;

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget, QString name, int NodeCas=0);
    ~Node();

    void addEdge(Edge *edge);
    void removeEdge(Edge *edge);
    QVector<Edge *> edges() const;
    static QVector<Node *> getNodeList();

    enum { Type = UserType + 1 };
    int type() const override { return Type; }
    // 基类转子类需要

    void calculateForces();
    bool advancePosition();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setName(QString name);
    QString getName();
    void setCas(int newCas);
    int getCas();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QVector<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
    QString m_nodeName;
    int m_nodeCas; // 二进制 低1位表示是否为初态 低2位表示是否为终态
    static QVector<Node *> nodeList; // 全局记录所有点
};

#endif // NODE_H
