#ifndef BST_H
#define BST_H

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
class BST;
}

class BST : public QWidget
{
    Q_OBJECT

public:
    explicit BST(QWidget *parent = nullptr);
    ~BST();

private:
    Ui::BST *ui;
    QGraphicsScene *scene;

private:
     //设置显示模式、正常、遍历、选中、删除
     const static QBrush normalBursh, visitedBrush, markBrush, deleteBrush;
     const static QFont dataFont;
     const static QIntValidator dataValidator;
};

#endif // BST_H
