#include "newbnode.h"
#include "newbedge.h"
#include "MyBSTTree.h"
#include "mybsttree.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

NewbNode::NewbNode(MyBSTTree *myBSTTree,BSTTree node)
    :myBSTTree(myBSTTree)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    this->node = node;
}

void NewbNode::addEdge(NewbEdge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<NewbEdge *> NewbNode::edges() const
{
    return edgeList;
}

void NewbNode::calculateForces()
{
    if(scene() || scene()->mouseGrabberItem() == this)
    {
        newPos = pos();
        return ;
    }
    qreal xvel = 0;
    qreal yvel = 0;
    foreach(QGraphicsItem *item, scene()->items() )
    {
        NewbNode *node = qgraphicsitem_cast<NewbNode *>(item);
        if(!node) continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if(l > 0)
        {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    double weight = (edgeList.size() + 1) * 10;
    foreach(NewbEdge *edge, edgeList)
    {
        QPointF vec;
        if(edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0 , 0);
        else
            vec = mapToItem(edge->sourceNode(), 0 , 0);
        xvel -= vec.x() / weight;
        yvel += vec.x() / weight;
    }

    if(qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

bool NewbNode::advance()
{
    if(newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF NewbNode::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust, 23 + adjust, 23+adjust);
}

QPainterPath NewbNode::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void NewbNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRadialGradient gradient(-3, -3, 10);
    if(option->state & QStyle::State_Sunken)
    {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        switch(node->bf)
        {
        case EH:
            gradient.setColorAt(1, QColor(Qt::white).light(120));
            gradient.setColorAt(0, QColor(Qt::white).light(120));
            break;
        case LH:
            gradient.setColorAt(1, QColor(Qt::white).light(120));
            gradient.setColorAt(0, QColor(Qt::white).light(120));
            break;
        case RH:
            gradient.setColorAt(1, QColor(Qt::white).light(120));
            gradient.setColorAt(0, QColor(Qt::white).light(120));
            break;
        }
    }else
    {
        switch (node->bf)
        {
        case EH:
            gradient.setColorAt(1, Qt::white);
            gradient.setColorAt(0, Qt::white);
            break;
        case LH:
            gradient.setColorAt(1, Qt::white);
            gradient.setColorAt(0, Qt::white);
            break;
        case RH:
            gradient.setColorAt(1, Qt::white);
            gradient.setColorAt(0, Qt::white);
            break;
        }
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);

    QRectF textRect(-10, -10, 20, 20);
    QString message(QString::number(this->node->data));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(7);
    painter->setFont(font);
    if(isSearched || isVisited)
    {
        if(isSearched)
            painter->setPen(Qt::blue);
        if(isVisited)
            painter->setPen(Qt::yellow);
    }else
    {
        painter->setPen(Qt::black);
    }
    painter->drawText(textRect, Qt::AlignCenter, message);
}

QVariant NewbNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch(change)
    {
    case ItemPositionHasChanged:
        foreach(NewbEdge *edge, edgeList)
            edge->adjust();
        //one or never
        myBSTTree->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void NewbNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void NewbNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
