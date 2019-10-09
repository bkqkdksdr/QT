#ifndef MyCQueue_H
#define MyCQueue_H

#include <QWidget>
#include <QFile>
#include <QTextBrowser>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QIntValidator>

#include "uidefine.h"
#include "lnode.h"

namespace Ui {
class MyCQueue;
}

class MyCQueue : public QWidget
{
    Q_OBJECT

public:
    explicit MyCQueue(QWidget *parent = nullptr);
    ~MyCQueue();

private slots:
    //槽函数
    void on_pushButtonInit_clicked();                       //创建按钮

    void on_pushButtonInsert_clicked();                     //push按钮

    void on_pushButtonRandomInsert5_clicked();              //push_5按钮

    void on_pushButtonDelete_clicked();                     //pop按钮

    void on_pushButtonLocate_clicked();                     //top按钮

    void on_horizontalSlider_valueChanged(int value);       //演示延迟速度按钮

    void on_pushButtonClear_clicked();                      //清除按钮

private:
    Ui::MyCQueue *ui;                                        //ui指针
    QGraphicsScene *scene;                                  //视图场景指针
    LNode *head;                                            //头节点指针
    QGraphicsTextItem *frontLabel;
    QGraphicsTextItem *rearLabel;
    NewArrow *frontArrow;
    NewArrow *rearArrow;
    int countNode;                                          //节点个数
    unsigned int sleepTime;                                 //演示延时时间
    int size;
    int front;
    int tail;
    void initTextBrowser();                                 //初始化文本显示区
    void initUI();                                          //初始化UI控件
    void initSceneView();                                   //初始化视图框架
    void adjustController();                                //操作之后调整右侧输入

    QPoint getLNodePos(int nodeNumber);                     //计算节点位置
    void addLNodeGraphicsItem(LNode *pl, QPoint coord);     //添加节点图象
    void adjustArrow();        //调整节点箭头
    void adjustLabel();       //调整节点位置
    void setMyCQueueNormalBrush();                           //设置所有节点显示模式
    void closeEvent(QCloseEvent *event);

    void initMyCQueue();                                     //初始化栈
    void insertLNode(int pos, QString elem);                //插入节点
    void inittwelve(int pos, QString elem);
    void deleteLNode(int pos, QString &elem);               //删除节点
    bool locateLNode(int &pos,QString elem);                //定位节点
    QString topNode();                                      //返回栈顶
    void destorySelf();                                     //清空栈

private:
     //设置显示模式、正常、遍历、选中、删除
     const static QBrush normalBursh, visitedBrush, markBrush, deleteBrush;
     const static QFont dataFont,headLabelFont;
     const static QIntValidator dataValidator;

};

#endif // MyCQueue_H
