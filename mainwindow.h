#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>

#include <QIcon>
#include <QPixmap>
#include <QPalette>

#include "linklist.h"
#include "mystack.h"
#include "myqueue.h"
#include "mycqueue.h"

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

private:
    Ui::MainWindow *ui;
    LinkList linkList;
    MyStack myStack;
    MyQueue myQueue;
    MyCQueue myCQueue;
    void initUI();
};

#endif // MainWindow_H
