#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QVector>
#include <abstractmaton.h>

#define QGRAPH_MOVE          0x00
#define QGRAPH_ADDNODE       0x01
#define QGRAPH_ADDEDGE1      0x02
#define QGRAPH_ADDEDGE2      0x03
#define QGRAPH_DELETE        0x04
#define QGRAPH_MODIFY        0x05

class Node;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr);

    void itemMoved();
    void setmode(int newMode);
    void setautoAdjust(int newAutoAdjust);
   // QPoint getPosofNode(Node *node);

    void addNode(QString name, int Cas, qreal x, qreal y);
    void addEdge(QString A, QString B, QString input, QString reInput);

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void scaleView(qreal scaleFactor);

private:
    int timerId = 0;
    int mode = 0;
    bool autoAdjust = false;
    Node *tmpNode1, *tmpNode2, *tmpInitState;
};

#endif // GRAPHWIDGET_H
