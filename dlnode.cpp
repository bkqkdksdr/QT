#include "dlnode.h"


DLNode::DLNode(QString dt, DLNode *nt)
{
    data=dt;
    next=nt;
    pointerRect=valueRect=prePointerRect=nullptr;
    valueText=pointerText=prePointerText=nullptr;
}

DLNode::~DLNode()
{

}

void DLNode::setNodeStatus(QBrush brush)
{
    valueRect->setBrush(brush);
}

void DLNode::removeAll(QGraphicsScene *scene)
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

    if(prePointerRect)
    {
        scene->removeItem(prePointerRect);
        delete prePointerRect;
        prePointerRect=nullptr;
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

    if(prePointerText)
    {
        scene->removeItem(prePointerText);
        delete prePointerText;
        prePointerText=nullptr;
    }

    for(auto &a:arrowVector)
        scene->removeItem(a);
    arrowVector.clear();

    for(auto &a:preArrowVector)
        scene->removeItem(a);
    preArrowVector.clear();
}
