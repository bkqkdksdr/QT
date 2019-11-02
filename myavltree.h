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

private:
    Ui::MyAVLTree *ui;
    QGraphicsScene *scene;
    int timerId;
    void EmptyNodeAndEdge();
    void initTextBrowser();
    void initUI();
    void initSceneView();
    void initMyAVLTree();
    void destorySelf();

private:
    //设置显示模式、正常、遍历、选中、删除
   const static QBrush normalBursh, visitedBrush, markBrush;
   const static QFont headLabelFont, dataFont;
   const static QIntValidator dataValidator;
};

#endif // MYAVLTREE_H
