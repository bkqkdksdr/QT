#ifndef MYQUEUE_H
#define MYQUEUE_H

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
class MyQueue;
}

class MyQueue : public QWidget
{
    Q_OBJECT

public:
    explicit MyQueue(QWidget *parent = nullptr);
    ~MyQueue();

private slots:
    void on_pushButtonInit_clicked();                       //创建按钮

    void on_pushButtonInsert_clicked();                     //EnQueue按钮

    void on_pushButtonRandomInsert5_clicked();              //EnQueue_5按钮

    void on_pushButtonDelete_clicked();                     //DeQueue按钮

    void on_pushButtonLocate_clicked();                     //Front按钮

    void on_horizontalSlider_valueChanged(int value);       //演示延迟

    void on_pushButtonClear_clicked();                      //清除

private:
    Ui::MyQueue *ui;                                        //ui指针
    QGraphicsScene *scene;                                  //视图场景指针
    LNode *head;                                            //头节点指针
    LNode *tail;                                            //尾节点指针
    int countNode;                                          //节点个数
    int sleepTime;                                 //演示延时时间

    void initTextBrowser();                                 //初始化文本显示区
    void initUI();                                          //初始化UI控件
    void initSceneView();                                   //初始化视图框架
    void adjustController();                                //操作之后调整右侧输入

    QPoint getLNodePos(int nodeNumber);                     //计算节点位置
    void addLNodeGraphicsItem(LNode *pl, QPoint coord);     //添加节点图象
    void adjustLNodeArrow(LNode *pLNode, int nodeNumber);   //调整节点箭头
    void adjustTailArrow(LNode *pLNode, int nodeNumber);    //调整尾部节点
    void adjustLNodePos(LNode *pLNode, QPoint coord);       //调整节点位置
    void setMyQueueNormalBrush();                           //设置所有节点显示模式
    void closeEvent(QCloseEvent *event);

    void initMyQueue();                                     //初始化队列
    void insertLNode(int pos, QString elem);                //插入节点
    void deleteLNode(int pos, QString &elem);               //删除节点
    bool locateLNode(int &pos,QString elem);                //定位节点
    QString frontNode();                                    //返回队首
    void destorySelf();                                     //清空队列

private:
     //设置显示模式、正常、遍历、选中、删除
     const static QBrush normalBursh, visitedBrush, markBrush, deleteBrush;
     const static QFont dataFont;
     const static QIntValidator dataValidator;

};

#endif // MyQueue_H
