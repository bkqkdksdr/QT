#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>

#include <QIcon>
#include <QPixmap>
#include <QPalette>

#include "linklist.h"
#include "clinklist.h"
#include "dlinklist.h"
#include "mystack.h"
#include "myqueue.h"
#include "mycqueue.h"
#include "myavltree.h"
#include "mybsttree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton1_clicked();

    void on_pushButton2_clicked();

    void on_pushButton3_clicked();

    void on_pushButton4_clicked();

    void on_pushButton5_clicked();

    void on_pushButton6_clicked();

    void on_pushButton7_clicked();

    void on_pushButton8_clicked();

private:
    Ui::MainWindow *ui;
    LinkList linkList;
    MyStack myStack;
    MyQueue myQueue;
    MyCQueue myCQueue;
    MyAVLTree myAVLTree;
    MyBSTTree myBSTTree;
    CLinkList clinkList;
    DLinkList dlinkList;
    void initUI();
};

#endif // MainWindow_H
