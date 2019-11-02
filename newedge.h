#ifndef NEWEDGE_H
#define NEWEDGE_H

#include <QGraphicsItem>
class NewNode;

class NewEdge : public QGraphicsItem
{
public:
    NewEdge(NewNode *sourceNode, NewNode *destNode);

    NewNode *sourceNode() const;
    NewNode *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    NewNode *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif // NEWEDGE_H
