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
{/*
    //节点个数可能会改变，需要考虑一些操作的合理性
    ui->pushButtonDelete->setEnabled(countNode);
    ui->comboBoxDelete->setEnabled(countNode);

    if(ui->comboBoxDelete->count()!=countNode)
    {
        QStringList qStringList;
        for(int i=1;i<=countNode;++i)
            qStringList.push_back(QString::number(i));

        ui->comboBoxInsert->clear();
        ui->comboBoxInsert->addItems(qStringList);

        ui->comboBoxDelete->clear();
        ui->comboBoxDelete->addItems(qStringList);
    }
    if(ui->comboBoxInsert->count()!=countNode+1)
        ui->comboBoxInsert->addItem(QString::number(countNode+1));

    ui->comboBoxLocate->clear();*/
}
//计算节点的SCENE坐标
QPoint BST::getLNodePos(int nodeNumber)
{/*
    const static int rowN=SCENE_MAX_W/NODE_W;
    qDebug()<<rowN<<"getposx="<<(nodeNumber+1)%rowN*NODE_W<<" y="<<(nodeNumber+1)%rowN*NODE_H<<"\n";
    return QPoint((nodeNumber+1)%rowN*NODE_W,(nodeNumber+1)/rowN*NODE_H);*/
}
//添加节点的GraphicsItem
void BST::addLNodeGraphicsItem(LNode *pl, QPoint coord)
{/*
    int x=coord.x(), y=coord.y();
    qDebug()<<"x="<<x<<"y="<<y<<"\n";//将信息输出到控制台
    pl->valueRect    =scene->addRect(x,y+SPACING,VALUE_RECT_W,RECT_H,QPen(),BST::markBrush);
    pl->pointerRect  =scene->addRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pl->valueText    =scene->addText(pl->data,BST::dataFont);
    pl->valueText->setPos(x,y+SPACING+5);
    if(pl->next==nullptr)
    {
        pl->pointerText=scene->addText(" ^",BST::dataFont);
        pl->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
    }*/
}
//调整节点的箭头
void BST::adjustLNodeArrow(LNode *pLNode, int nodeNumber)
{/*
    for(auto &a:pLNode->arrowVector)
        scene->removeItem(a);
    if(pLNode->next==nullptr)
        return ;

    QPoint myCoord=getLNodePos(nodeNumber);
    QPoint nextCoord=getLNodePos(nodeNumber+1);

    NewArrow *pArrow;
    if(myCoord.y()==nextCoord.y())
    {
        //节点不是最后一行节点
        pArrow=new NewArrow(ARROW_LEN);
        scene->addItem(pArrow);
        pArrow->setPos(myCoord.x()+ARROW_H_OFFSET,myCoord.y()+ARROW_V_OFFSET);
        pLNode->arrowVector.push_back(pArrow);
    }
    else {
        //节点是一行中最后的节点
        QPoint point1(myCoord.x()+ARROW_H_OFFSET,myCoord.y()+ARROW_V_OFFSET);
        QPoint point2(point1.x(),point1.y()+NODE_H/2);
        QPoint point3(VALUE_RECT_W>>1,point2.y());

        pArrow=new NewArrow(NODE_H>>1,2,0);
        scene->addItem(pArrow);
        pArrow->setPos(point1);
        pLNode->arrowVector.push_back(pArrow);

        pArrow=new NewArrow(point2.x()-point3.x(),3,0);
        scene->addItem(pArrow);
        pArrow->setPos(point2);
        pLNode->arrowVector.push_back(pArrow);

        pArrow=new NewArrow(SPACING,2);
        scene->addItem(pArrow);
        pArrow->setPos(point3);
        pLNode->arrowVector.push_back(pArrow);
    }*/
}
//调整节点scene坐标
void BST::adjustLNodePos(LNode *pLNode, QPoint coord)
{/*
    int x=coord.x(), y=coord.y();
    pLNode->valueRect->setRect(x,y+SPACING,VALUE_RECT_W,RECT_H);
    pLNode->pointerRect->setRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pLNode->valueText->setPos(x,y+SPACING+5);
    if(pLNode->pointerText)
        pLNode->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);*/
}
//设置链表所有节点显示模式
void BST::setBSTNormalBrush()
{/*
    for(LNode *pLNode=head;pLNode;pLNode=pLNode->next)
        pLNode->setNodeStatus(BST::normalBursh);*/
}
//创建链表初始化
void BST::initBST()
{
    initSceneView();
   /* sleep(sleepTime);
    head=new LNode("头节点",nullptr);
    addLNodeGraphicsItem(head, getLNodePos(0));*/
}
//插入节点到链表
void BST::insertLNode(int pos, QString elem)
{/*
    LNode *pInsertNode=nullptr;
    LNode *pLNode=head;

    head->setNodeStatus(BST::visitedBrush);

    //找到前驱结点指针
    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(BST::visitedBrush);
    }
    sleep(sleepTime);
    if(pLNode->next==nullptr)
    {
        scene->removeItem(pLNode->pointerText);
        pLNode->pointerText=nullptr;
    }
    //新节点插入到链表中
    pInsertNode=new LNode(elem,pLNode->next);
    pLNode->next=pInsertNode;
    ++countNode;
    //添加图形item
    addLNodeGraphicsItem(pInsertNode,getLNodePos(pos));

    for(--pos;pos<=countNode;++pos)
    {
        sleep(sleepTime);

        adjustLNodePos(pLNode,getLNodePos(pos));
        adjustLNodeArrow(pLNode,pos);
        pLNode=pLNode->next;
    }*/
}
//删除链表节点
void BST::deleteLNode(int pos, QString &elem)
{/*
    LNode *pDeleteNode=nullptr;
    LNode *pLNode=head;

    head->setNodeStatus(BST::visitedBrush);

    //找到前驱结点
    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(BST::visitedBrush);
    }
    sleep(sleepTime);
    pDeleteNode=pLNode->next;
    pLNode->next=pDeleteNode->next;
    elem=pDeleteNode->data;
    //删除节点，移出图形item
    pDeleteNode->removeAll(scene);
    delete pDeleteNode;
    --countNode;

    if(pLNode->next==nullptr)
    {
        QPoint coord=getLNodePos(pos-1);
        pLNode->pointerText=scene->addText(" ^",BST::dataFont);
        pLNode->pointerText->setPos(coord.x()+VALUE_RECT_W, coord.y()+SPACING+5);
    }

    for(--pos;pos<=countNode;++pos)
    {
        sleep(sleepTime);

        adjustLNodePos(pLNode,getLNodePos(pos));
        adjustLNodeArrow(pLNode,pos);
        pLNode=pLNode->next;
    }*/
}
//查找链表节点
bool BST::locateLNode(int &pos, QString elem)
{/*
    LNode *pLNode=head;
    head->setNodeStatus(BST::visitedBrush);
    for(pos=1;pLNode&&pLNode->next&&pLNode->next->data!=elem;++pos)
    {
        sleep(sleepTime);
        pLNode=pLNode->next;
        pLNode->setNodeStatus(BST::visitedBrush);
    }
    sleep(sleepTime);
    //找到相应的节点
    if(pLNode&&pLNode->next){
        pLNode->next->setNodeStatus(BST::markBrush);
        return true;
    }
    return false;*/
}
//释放申请的内存空间
void BST::destorySelf()
{/*
    if(scene==nullptr)
        return ;
    LNode *pLNode=head, *qLNode;
    for(;pLNode;pLNode=qLNode)
    {
        sleep(sleepTime);
        qLNode=pLNode->next;
        pLNode->removeAll(scene);
        delete pLNode;
    }
    sleep(sleepTime);
    scene->removeItem(headLabel);
    scene->removeItem(headArrow);
    delete headLabel;
    delete headArrow;
    scene=nullptr;

    countNode=0;*/
}
//槽函数：点击创建
void BST::on_pushButtonInit_clicked()
{
    //若以及建立，需要清除重建
    //destorySelf();

    initBST();


    adjustController();
}

void BST::on_pushButtonInsert_clicked()
{/*
    setBSTNormalBrush();
    sleep(sleepTime);
    QString edit=ui->lineEditInsert->text();
    //若无输入或未输入
    if(edit.isEmpty())
    {
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Please Input!");
        return ;
    }

    insertLNode(ui->comboBoxInsert->currentText().toInt(),edit);
    //调整右侧控件状态及值
    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Insert Success!");*/
}

void BST::on_pushButtonClear_clicked()
{/*
    destorySelf();
    initUI();*/
}

void BST::closeEvent(QCloseEvent *event)
{/*
    destorySelf();
    //initUI();*/
}
void BST::on_pushButtonRandomInsert5_clicked()
{/*
    for(int i=0;i<5;i++)
    {
        setBSTNormalBrush();
        sleep(sleepTime);
        insertLNode(countNode+1,QString::number(rand()%999999999));
        adjustController();
        ui->lineEditState->setPalette(Qt::GlobalColor::green);
        ui->lineEditState->setText("Insert Success!");
    }*/
}

void BST::on_pushButtonDelete_clicked()
{/*
    setBSTNormalBrush();
    sleep(sleepTime);
    QString deleteData;
    deleteLNode(ui->comboBoxDelete->currentText().toInt(),deleteData);
    //调整右侧控件状态及值
    adjustController();
    //ui->lineEditDelete->setText(deleteData);
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Delete Success!");*/
}


void BST::on_pushButtonLocate_clicked()
{/*
    setBSTNormalBrush();
    sleep(sleepTime);
    QString edit=ui->lineEditLocate->text();

    //若输入无效或未输入
    if(edit.isEmpty())
    {
        adjustController();
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Please Input!");
        return;
    }

    //根据查找结果给出结果输出
    int pos;
    if(locateLNode(pos,edit))
    {
        ui->comboBoxLocate->addItem(QString::number(pos));
        ui->comboBoxLocate->setCurrentText(QString::number(pos));
        ui->lineEditState->setPalette(Qt::GlobalColor::green);
        ui->lineEditState->setText("Locate Success!");
    }
    else
    {
        adjustController();
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Locate Fail!");
    }*/
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
