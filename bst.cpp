#include <QDebug>
#include <math.h>
#include <QToolBar>
#include "bst.h"
#include "ui_bst.h"
#include "graphwidget.h"
void sleep(int msec);

const QBrush BST::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush BST::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush BST::markBrush=QBrush(Qt::GlobalColor::green);

const QFont BST::headLabelFont=QFont("Consolas");
const QFont BST::dataFont=QFont("Consolas",8);

const QIntValidator BST::dataValidator(-999999999,999999999);


BST::BST(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BST)
{
    ui->setupUi(this);
    initTextBrowser();
    initUI();

    scene2=nullptr;
    sleepTime=MAX_SLEEP_TIME>>1;
    ui->horizontalSlider->setValue(MAX_SLIDER>>1);
    srand(time_t(nullptr));

    this->setWindowTitle("单链表");
}

BST::~BST()
{
    destorySelf();
    delete ui;
}
//初始化文本显示区
void BST::initTextBrowser(){
    QFile file(":/html/html/LinkList.html");
    file.open(QIODevice::ReadOnly);
    QString htmlString=file.readAll();
    ui->textBrowser->setHtml(htmlString);
    //ui->textBrowser->setOpenLinks(true);
    //ui->textBrowser->setOpenExternalLinks(true);
    file.close();
}
//初始化UI控件
void BST::initUI()
{
    //设置图标
    QIcon exeIcon(":/ico/resource/exe.ico");
    this->setWindowIcon(exeIcon);
    //设置背景图片
    QPixmap background(":/ico/resource/background.png");
    QPalette palette;
    palette.setBrush(QPalette::Background,background);
    this->setPalette(palette);
    //未创建时，除创建按钮外一律无效

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAX_SLIDER);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
}
//初始化视图框架
void BST::initSceneView()
{
   /* //为view配置相应scene
    scene=new QGraphicsScene;
    //scene = new GraphWidget;
    scene->setSceneRect(0,0,SCENE_MAX_W,SCENE_MAX_H);
    ui->graphicsView->setScene(scene);*/

    scene2=new QGraphicsScene;
    widget = new GraphWidget(nullptr,scene2);
    ui->graphicsView2->setScene(scene2);


   // scene->addWidget(widget);
/*
    headLabel=scene->addText("L",headLabelFont);
    headLabel->setPos((VALUE_RECT_W-RECT_H)>>1,SPACING);
    sleep(sleepTime);
    headArrow=new NewArrow(ARROW_LEN);
    scene->addItem(headArrow);
    headArrow->setPos(ARROW_H_OFFSET,ARROW_V_OFFSET);*/
}
//操作之后调整右侧输入和显示控件
void BST::adjustController()
{

}

//设置链表所有节点显示模式
void BST::setBSTNormalBrush()
{

}
//创建链表初始化
void BST::initBST()
{
    initSceneView();
}
//插入节点到链表

//释放申请的内存空间
void BST::destorySelf()
{
    //widget->destoryTree();
}
//槽函数：点击创建
void BST::on_pushButtonInit_clicked()
{
    //若以及建立，需要清除重建
    //destorySelf();

    initBST();


    adjustController();
}


void BST::closeEvent(QCloseEvent *event)
{
    destorySelf();
    //initUI();
}

void BST::on_horizontalSlider_valueChanged(int value)
{
    sleepTime=MAX_SLEEP_TIME/(value+1);
}

void BST::on_addNode_clicked()
{
    widget->addNode();
}

void BST::on_deleteNode_clicked()
{
    widget->deleteNode();
}

void BST::on_searchNode_clicked()
{
    widget->searchNode();
}

void BST::on_divTree_clicked()
{
    widget->divTree();
}

void BST::on_mergerTree_clicked()
{
    widget->mergerTree();
}
