#ifndef MYAVLTREE_H
#define MYAVLTREE_H

#include <QWidget>
#include <QFile>
#include <QTextBrowser>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QIntValidator>
#include "avltree.h"
struct trace{
    int value;
    bool insert;
};
class NewNode;

namespace Ui {
class MyAVLTree;
}

class MyAVLTree : public QWidget
{
    Q_OBJECT

public:
    explicit MyAVLTree(QWidget *parent = nullptr);
    ~MyAVLTree();

    AVLTree root;
    std::vector<AVLTree> trees;
    std::vector<trace> traces;
    void itemMoved();
    void drawTree();
    void paintTree(NewNode* &root, qreal &centerX, qreal centerY);
public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
    void drawBackground(QPainter *painter, const QRectF &rect);
    void scaleView(qreal scaleFactor);

private slots:
    void on_pushButtonInit_clicked();

    void on_addNode_clicked();

    void on_deleteNode_clicked();

    void on_searchNode_clicked();

    void on_addNode5_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButtonclear_clicked();

    void on_PreOrder_clicked();

    void on_InOrder_clicked();

    void on_LastOrder_clicked();

    void on_LevelOrder_clicked();

    void on_saveButton_clicked();

    void on_loadButton_clicked();

private:
    Ui::MyAVLTree *ui;
    QGraphicsScene *scene;
    int timerId;
    int sleepTime;
    int de;
    void EmptyNodeAndEdge(int state);
    void initTextBrowser();
    void initUI();
    void initSceneView();
    void initMyAVLTree();
    void destorySelf();
    void adjustContronller();
    void closeEvent(QCloseEvent *event);
    void count(AVLTree &T, int &numbers);
    void PreOrder(AVLTree T);
    void InOrder(AVLTree T);
    void LastOrder(AVLTree T);
    void LevelOrder(AVLTree T);
    void Setvisited(AVLTree T);
    void RefleshNode();

private:
    //设置显示模式、正常、遍历、选中、删除
   const static QBrush normalBursh, visitedBrush, markBrush;
   const static QFont headLabelFont, dataFont;
   const static QIntValidator dataValidator;
};

#endif // MYAVLTREE_H
