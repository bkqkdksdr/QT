#ifndef NEWNODE_H
#define NEWNODE_H

#include <QGraphicsItem>
#include <QList>
#include "avltree.h"

class NewEdge;
class MyAVLTree;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class NewNode : public QGraphicsItem
{
public:
    NewNode(MyAVLTree *myAVLTree,AVLTree node);
    AVLTree node;

    void addEdge(NewEdge *edge);
    QList<NewEdge*> edges() const;

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
    QList<NewEdge*> edgeList;
    QPointF newPos;
    MyAVLTree *myAVLTree;
};

#endif // NEWNODE_H
