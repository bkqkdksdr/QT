#ifndef NEWARROW_H
#define NEWARROW_H
#define SIZE1_WEIGHT 6
#define SIZE1_HEIGHT 3
#include <QGraphicsItem>

//新的自定义箭头，用于绘图
class NewArrow : public QGraphicsItem
{
    QRectF mygroundRect;//画图区域
    QLine line1,line2,line3;//箭头由三条线构成
    int direction;//箭头方向
    int arrowsize;//箭头尺寸
public:
    explicit NewArrow(int lenght, int dir=1, int size=1);//默认数值
    ~NewArrow();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget=nullptr) Q_DECL_OVERRIDE;//防止写错虚函数
    QRectF boundingRect() const Q_DECL_OVERRIDE;
};

#endif // NEWARROW_H
