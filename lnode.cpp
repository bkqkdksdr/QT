#include "lnode.h"

LNode::LNode(QString dt, LNode *nt)
{
    data=dt;
    next=nt;
    pointerRect=valueRect=nullptr;
    pointerText=valueText=nullptr;
}

LNode::~LNode()
{

}

void LNode::setValueRect(QGraphicsRectItem *vRect)
{
    valueRect=vRect;
}
void LNode::setValue(QString dt)
{
    data=dt;
}

void LNode::setPointerRect(QGraphicsRectItem *pRect)
{
    pointerRect=pRect;
}

void LNode::setTextRect(QGraphicsTextItem *vText)
{
    valueText=vText;
}

void LNode::setArrowVector(std::vector<NewArrow *> aVector)
{
    arrowVector=aVector;
}

void LNode::setNodeStatus(QBrush brush)
{
    valueRect->setBrush(brush);
}

void LNode::removeAll(QGraphicsScene *scene)
{
    if(valueRect)
    {
        scene->removeItem(valueRect);
        delete valueRect;
        valueRect=nullptr;
    }

    if(pointerRect)
    {
        scene->removeItem(pointerRect);
        delete pointerRect;
        pointerRect=nullptr;
    }

    if(valueText)
    {
        scene->removeItem(valueText);
        delete valueText;
        valueText=nullptr;
    }

    if(pointerText)
    {
        scene->removeItem(pointerText);
        delete pointerText;
        pointerText=nullptr;
    }

    for(auto &a:arrowVector)
        scene->removeItem(a);
    arrowVector.clear();
}
