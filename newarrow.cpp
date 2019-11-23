#include "newarrow.h"
#include <QGraphicsScene>
#include <QPainter>
//length 长度
//dir 0 上 1 右 2 下 3 左
//size 0 无箭头 1 默认大小
NewArrow::NewArrow(int length, int dir, int size) : direction(dir), arrowsize(size)
{
    QPoint startp(0,0), endp, p1, p2;

    switch(dir) {
    case 0:
            p1=p2=endp=QPoint(0,-length);
            if(size)
            {
                p1+=QPoint(-SIZE1_HEIGHT,SIZE1_WEIGHT);p2+=QPoint(SIZE1_HEIGHT,SIZE1_WEIGHT);
            }
            mygroundRect=QRect(-SIZE1_HEIGHT,-length,SIZE1_HEIGHT<<1,length);
            break;
        case 1:
            p1=p2=endp=QPoint(length,0);
            if(size)
            {
                p1+=QPoint(-SIZE1_WEIGHT,-SIZE1_HEIGHT);p2+=QPoint(-SIZE1_WEIGHT,SIZE1_HEIGHT);
            }
            mygroundRect=QRect(0,-SIZE1_HEIGHT,length,SIZE1_HEIGHT<<1);
            break;
        case 2:
            p1=p2=endp=QPoint(0,length);
            if(size)
            {
                p1+=QPoint(-SIZE1_HEIGHT,-SIZE1_WEIGHT);p2+=QPoint(SIZE1_HEIGHT,-SIZE1_WEIGHT);
            }
            mygroundRect=QRect(-SIZE1_HEIGHT,0,SIZE1_HEIGHT<<1,length);
            break;
        case 3:
            p1=p2=endp=QPoint(-length,0);
            if(size)
            {
                p1+=QPoint(SIZE1_WEIGHT,SIZE1_HEIGHT);p2+=QPoint(SIZE1_WEIGHT,-SIZE1_HEIGHT);
            }
            mygroundRect=QRect(-length,-SIZE1_HEIGHT,length,SIZE1_HEIGHT<<1);
            break;
        default:
            break;
    }

    line1=QLine(startp,endp);
    line2=QLine(endp,p1);
    line3=QLine(endp,p2);
}

NewArrow::~NewArrow()
{

}

void NewArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawLine(line1);
    painter->drawLine(line2);
    painter->drawLine(line3);
}

QRectF NewArrow::boundingRect() const
{
    return mygroundRect;
}
