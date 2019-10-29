#ifndef LNODE_H
#define LNODE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QBrush>

#include "newarrow.h"

class LinkList;
class MyStack;
class MyQueue;
class MyCQueue;
class BST;

class LNode
{
public:
    LNode(QString dt, LNode *nt);  //构造函数
    ~LNode();
    void setValueRect(QGraphicsRectItem* vRect);//数值区域
    void setValue(QString dt);
    void setPointerRect(QGraphicsRectItem* pRect);
    void setTextRect(QGraphicsTextItem* vText);
    void setArrowVector(std::vector<NewArrow*> aVector);
    void setNodeStatus(QBrush brush);
    void removeAll(QGraphicsScene* scene);

public:
    QString data;//数据域
    LNode* next;//指针域
    //节点图形区域
    QGraphicsRectItem *valueRect, *pointerRect;
    QGraphicsTextItem *valueText, *pointerText;
    std::vector<NewArrow*> arrowVector;

public:
    friend class LinkList;
    friend class MyStack;
    friend class MyQueue;
    friend class MyCQueue;
    friend class BST;
};

#endif // LNODE_H
