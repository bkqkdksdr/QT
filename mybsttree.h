#ifndef MYBSTTREE_H
#define MYBSTTREE_H

#include <QWidget>
#include <QFile>
#include <QTextBrowser>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QIntValidator>
#include <bsttree.h>

class NewbNode;

namespace Ui {
class MyBSTTree;
}

class MyBSTTree : public QWidget
{
    Q_OBJECT

public:
    explicit MyBSTTree(QWidget *parent = nullptr);
    ~MyBSTTree();

    BSTTree root;
    std::vector<BSTTree> trees;
    void itemMoved();
    void drawTree();
    void paintTree(NewbNode* &root, qreal &centerX, qreal centerY);
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

private:
    Ui::MyBSTTree *ui;
    QGraphicsScene *scene;
    int timerId;
    int sleepTime;
    int de;
    void EmptyNodeAndEdge(int state);
    void initTextBrowser();
    void initUI();
    void initSceneView();
    void initMyBSTTree();
    void destorySelf();
    void adjustContronller();
    void closeEvent(QCloseEvent *event);
    void count(BSTTree &T, int &numbers);
    void PreOrder(BSTTree T);
    void InOrder(BSTTree T);
    void LastOrder(BSTTree T);
    void LevelOrder(BSTTree T);
    void Setvisited(BSTTree T);
    void RefleshNode();

private:
    //设置显示模式、正常、遍历、选中、删除
   const static QBrush normalBursh, visitedBrush, markBrush;
   const static QFont headLabelFont, dataFont;
   const static QIntValidator dataValidator;
};

#endif // MYBSTTREE_H
