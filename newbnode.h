#ifndef NEWBNODE_H
#define NEWBNODE_H

#include <QGraphicsItem>
#include <QList>
#include "bsttree.h"

class NewbEdge;
class MyBSTTree;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class NewbNode : public QGraphicsItem
{
public:
    NewbNode(MyBSTTree *myBSTTree,BSTTree node);
    BSTTree node;

    void addEdge(NewbEdge *edge);
    QList<NewbEdge*> edges() const;

    bool isSearched = false;
    bool isVisited = false;

    enum{Type = UserType + 1};
    int type() const override {return Type;}

    void calculateForces();
    bool advance();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QList<NewbEdge*> edgeList;
    QPointF newPos;
    MyBSTTree *myBSTTree;
};

#endif // NEWBNODE_H
