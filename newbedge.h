#ifndef NEWBEDGE_H
#define NEWBEDGE_H
#include <QGraphicsItem>
class NewbNode;

class NewbEdge : public QGraphicsItem
{
public:
    NewbEdge(NewbNode *sourceNode, NewbNode *destNode);

    NewbNode *sourceNode() const;
    NewbNode *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    NewbNode *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif // NEWBEDGE_H
