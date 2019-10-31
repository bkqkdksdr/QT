#ifndef BST_H
#define BST_H

#include <QWidget>
#include <QFile>
#include <QTextBrowser>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QIntValidator>
#include <graphwidget.h>

#include "uidefine.h"
#include "lnode.h"
namespace Ui {
class BST;
}

class BST : public QWidget
{
    Q_OBJECT

public:
    explicit BST(QWidget *parent = nullptr);
    ~BST();

private slots:
    void on_pushButtonInit_clicked();

    void on_pushButtonInsert_clicked();

    void on_pushButtonClear_clicked();

    void on_pushButtonRandomInsert5_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonLocate_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_addNode_clicked();

    void on_deleteNode_clicked();

    void on_searchNode_clicked();

    void on_divTree_clicked();

    void on_mergerTree_clicked();

private:
    Ui::BST *ui;
    QGraphicsScene *scene2;
    GraphWidget *widget;
    int sleepTime;//演示延时时间

    void initTextBrowser();//初始化文本显示区
    void initUI();//初始化UI控件
    void initSceneView();//初始化视图框架
    void adjustController();//操作之后调整右侧输入

    QPoint getLNodePos(int nodeNumber);
    void addLNodeGraphicsItem(LNode *pl, QPoint coord);
    void adjustLNodeArrow(LNode *pLNode, int nodeNumber);
    void adjustLNodePos(LNode *pLNode, QPoint coord);
    void setBSTNormalBrush();
    void closeEvent(QCloseEvent *event);

    void initBST();
    void insertLNode(int pos, QString elem);
    void deleteLNode(int pos, QString &elem);
    bool locateLNode(int &pos,QString elem);
    void destorySelf();


private:
     //设置显示模式、正常、遍历、选中、删除
    const static QBrush normalBursh, visitedBrush, markBrush;
    const static QFont headLabelFont, dataFont;
    const static QIntValidator dataValidator;
};

#endif // BST_H
