#ifndef LINKLIST_H
#define LINKLIST_H

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
class LinkList;
}

class LinkList : public QWidget
{
    Q_OBJECT

public:
    explicit LinkList(QWidget *parent = nullptr);
    ~LinkList();

private slots:
    void on_pushButtonInit_clicked();

    void on_pushButtonInsert_clicked();

    void on_pushButtonClear_clicked();

    void on_pushButtonRandomInsert5_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonLocate_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::LinkList *ui;//ui指针
    QGraphicsScene *scene;//视图场景指针
    QGraphicsTextItem *headLabel;//链表符号指针
    NewArrow *headArrow;//符号箭头指针
    LNode *head;//头节点指针
    int countNode;//节点个数
    unsigned int sleepTime;//演示延时时间

    void initTextBrowser();//初始化文本显示区
    void initUI();//初始化UI控件
    void initSceneView();//初始化视图框架
    void adjustController();//操作之后调整右侧输入

    QPoint getLNodePos(int nodeNumber);
    void addLNodeGraphicsItem(LNode *pl, QPoint coord);
    void adjustLNodeArrow(LNode *pLNode, int nodeNumber);
    void adjustLNodePos(LNode *pLNode, QPoint coord);
    void setLinkListNormalBrush();
    void closeEvent(QCloseEvent *event);

    void initLinkList();
    void insertLNode(int pos, QString elem);
    void deleteLNode(int pos, QString &elem);
    bool locateLNode(int &pos,QString elem);
    void destorySelf();

private:
     const static QBrush normalBursh, visitedBrush, markBrush;
     const static QFont headLabelFont, dataFont;
     const static QIntValidator dataValidator;
};

#endif // LINKLIST_H
