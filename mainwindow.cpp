#include "mainwindow.h"
#include "ui_mainwindow.h"
//mainwindow tianjiazhushi
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    this->setWindowTitle("BDQKDKSDR");
    QIcon exeIcon(":/ico/resource/exe.ico");
    this->setWindowIcon(exeIcon);

    QPixmap background(":/ico/resource/background.png");
    QPalette palette;
    palette.setBrush(QPalette::Background,background);
    this->setPalette(palette);

    ui->pushButton1->setToolTip("<img src=':/tip/resource/linklist.png' /> ");
    ui->pushButton2->setToolTip("<img src=':/tip/resource/mystack.PNG' /> ");
    ui->pushButton3->setToolTip("<img src=':/tip/resource/myqueue.PNG' /> ");
    ui->pushButton4->setToolTip("<img src=':/tip/resource/mycqueue.PNG' /> ");
}

void MainWindow::on_pushButton1_clicked()
{
   linkList.show();
}

void MainWindow::on_pushButton2_clicked()
{
    myStack.show();
}

void MainWindow::on_pushButton3_clicked()
{
    myQueue.show();
}

void MainWindow::on_pushButton4_clicked()
{
    myCQueue.show();
}
