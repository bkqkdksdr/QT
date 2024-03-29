#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include "linklist.h"
#include "ui_linklist.h"
#include "newarrow.h"
//添加测试信息
void sleep(int msec);

const QBrush LinkList::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush LinkList::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush LinkList::markBrush=QBrush(Qt::GlobalColor::green);

const QFont LinkList::headLabelFont=QFont("Consolas");
const QFont LinkList::dataFont=QFont("Consolas",8);

const QIntValidator LinkList::dataValidator(-999999999,999999999);

LinkList::LinkList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkList)
{
    //初始化数据
    ui->setupUi(this);
    initTextBrowser();
    initUI();

    countNode=0;
    head=nullptr;
    headLabel=nullptr;
    headArrow=nullptr;
    scene=nullptr;
    sleepTime=MAX_SLEEP_TIME>>1;
    ui->horizontalSlider->setValue(MAX_SLIDER>>1);
    srand(time_t(nullptr));

    this->setWindowTitle("单链表");
}

LinkList::~LinkList()
{
    destorySelf();
    delete ui;
}
//初始化文本显示区
void LinkList::initTextBrowser(){
    QFile file(":/html/html/LinkList.html");
    file.open(QIODevice::ReadOnly);
    QString htmlString=file.readAll();
    ui->textBrowser->setHtml(htmlString);
    //ui->textBrowser->setOpenLinks(true);
    //ui->textBrowser->setOpenExternalLinks(true);
    file.close();
}
//初始化UI控件
void LinkList::initUI()
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
    ui->pushButtonClear->setEnabled(false);
    ui->pushButtonInsert->setEnabled(false);
    ui->pushButtonRandomInsert5->setEnabled(false);
    ui->comboBoxInsert->setEnabled(false);
    ui->lineEditInsert->setEnabled(false);

    ui->pushButtonDelete->setEnabled(false);
    ui->comboBoxDelete->setEnabled(false);
    ui->saveButton->setEnabled(false);

    ui->pushButtonLocate->setEnabled(false);
    ui->comboBoxLocate->setEnabled(false);
    ui->lineEditLocate->setEnabled(false);

    ui->lineEditState->setEnabled(false);

    ui->lineEditInsert->setFont(dataFont);
    //ui->lineEditDelete->setFont(dataFont);
    ui->lineEditLocate->setFont(dataFont);
    ui->lineEditState->setFont(dataFont);
    ui->lineEditInsert->setPlaceholderText("插入值：Int");
    ui->lineEditLocate->setPlaceholderText("查找值：Int");
    ui->lineEditInsert->setValidator(&LinkList::dataValidator);
    ui->lineEditLocate->setValidator(&LinkList::dataValidator);
    ui->lineEditState->setText("请选择操作");

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAX_SLIDER);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
    adjustController();
}
//初始化视图框架
void LinkList::initSceneView()
{
    //为view配置相应scene
    scene=new QGraphicsScene;
    scene->setSceneRect(0,0,SCENE_MAX_W,SCENE_MAX_H);
    ui->graphicsView->setScene(scene);
    //添加链表符号及指针箭头
    headLabel=scene->addText("L",headLabelFont);
    headLabel->setPos((VALUE_RECT_W-RECT_H)>>1,SPACING);
    sleep(sleepTime);
    headArrow=new NewArrow(ARROW_LEN);
    scene->addItem(headArrow);
    headArrow->setPos(ARROW_H_OFFSET,ARROW_V_OFFSET);
}
//操作之后调整右侧输入和显示控件
void LinkList::adjustController()
{
    //节点个数可能会改变，需要考虑一些操作的合理性
    ui->pushButtonDelete->setEnabled(countNode);
    ui->comboBoxDelete->setEnabled(countNode);
    ui->saveButton->setEnabled(countNode);
    ui->loadButton->setEnabled(!countNode);
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
    ui->lineEditInsert->setFont(dataFont);
    ui->lineEditLocate->setFont(dataFont);
    ui->lineEditInsert->setPlaceholderText("插入值：Int");
    ui->lineEditLocate->setPlaceholderText("查找值：Int");
    ui->lineEditInsert->setValidator(&LinkList::dataValidator);
    ui->lineEditLocate->setValidator(&LinkList::dataValidator);
    ui->comboBoxLocate->clear();
}
//计算节点的SCENE坐标
QPoint LinkList::getLNodePos(int nodeNumber)
{
    const static int rowN=SCENE_MAX_W/NODE_W;
    qDebug()<<rowN<<"getposx="<<(nodeNumber+1)%rowN*NODE_W<<" y="<<(nodeNumber+1)%rowN*NODE_H<<"\n";
    return QPoint((nodeNumber+1)%rowN*NODE_W,(nodeNumber+1)/rowN*NODE_H);
}
//添加节点的GraphicsItem
void LinkList::addLNodeGraphicsItem(LNode *pl, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    qDebug()<<"x="<<x<<"y="<<y<<"\n";//将信息输出到控制台
    pl->valueRect    =scene->addRect(x,y+SPACING,VALUE_RECT_W,RECT_H,QPen(),LinkList::markBrush);
    pl->pointerRect  =scene->addRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pl->valueText    =scene->addText(pl->data,LinkList::dataFont);
    pl->valueText->setPos(x,y+SPACING+5);
    if(pl->next==nullptr)
    {
        pl->pointerText=scene->addText(" ^",LinkList::dataFont);
        pl->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
    }
}
//调整节点的箭头
void LinkList::adjustLNodeArrow(LNode *pLNode, int nodeNumber)
{
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
    }
}
//调整节点scene坐标
void LinkList::adjustLNodePos(LNode *pLNode, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    pLNode->valueRect->setRect(x,y+SPACING,VALUE_RECT_W,RECT_H);
    pLNode->pointerRect->setRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pLNode->valueText->setPos(x,y+SPACING+5);
    if(pLNode->pointerText)
        pLNode->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
}
//设置链表所有节点显示模式
void LinkList::setLinkListNormalBrush()
{
    for(LNode *pLNode=head;pLNode;pLNode=pLNode->next)
        pLNode->setNodeStatus(LinkList::normalBursh);
}
//创建链表初始化
void LinkList::initLinkList()
{
    initSceneView();
    sleep(sleepTime);
    head=new LNode("头节点",nullptr);
    addLNodeGraphicsItem(head, getLNodePos(0));
}
//插入节点到链表
void LinkList::insertLNode(int pos, QString elem)
{
    LNode *pInsertNode=nullptr;
    LNode *pLNode=head;

    head->setNodeStatus(LinkList::visitedBrush);

    //找到前驱结点指针
    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(LinkList::visitedBrush);
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
    }
}
//删除链表节点
void LinkList::deleteLNode(int pos, QString &elem)
{
    LNode *pDeleteNode=nullptr;
    LNode *pLNode=head;

    head->setNodeStatus(LinkList::visitedBrush);

    //找到前驱结点
    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(LinkList::visitedBrush);
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
        pLNode->pointerText=scene->addText(" ^",LinkList::dataFont);
        pLNode->pointerText->setPos(coord.x()+VALUE_RECT_W, coord.y()+SPACING+5);
    }

    for(--pos;pos<=countNode;++pos)
    {
        sleep(sleepTime);

        adjustLNodePos(pLNode,getLNodePos(pos));
        adjustLNodeArrow(pLNode,pos);
        pLNode=pLNode->next;
    }
}
//查找链表节点
bool LinkList::locateLNode(int &pos, QString elem)
{
    LNode *pLNode=head;
    head->setNodeStatus(LinkList::visitedBrush);
    for(pos=1;pLNode&&pLNode->next&&pLNode->next->data!=elem;++pos)
    {
        sleep(sleepTime);
        pLNode=pLNode->next;
        pLNode->setNodeStatus(LinkList::visitedBrush);
    }
    sleep(sleepTime);
    //找到相应的节点
    if(pLNode&&pLNode->next){
        pLNode->next->setNodeStatus(LinkList::markBrush);
        return true;
    }
    return false;
}
//释放申请的内存空间
void LinkList::destorySelf()
{
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

    countNode=0;
}
//槽函数：点击创建
void LinkList::on_pushButtonInit_clicked()
{
    //若以及建立，需要清除重建
    destorySelf();

    initLinkList();

    ui->pushButtonClear->setEnabled(true);
    ui->pushButtonInsert->setEnabled(true);
    ui->pushButtonRandomInsert5->setEnabled(true);
    ui->comboBoxInsert->setEnabled(true);
    ui->lineEditInsert->setEnabled(true);
    ui->pushButtonLocate->setEnabled(true);
    ui->lineEditLocate->setEnabled(true);

    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Create Success!");
}

void LinkList::on_pushButtonInsert_clicked()
{
    setLinkListNormalBrush();
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
    ui->lineEditState->setText("Insert Success!");
}

void LinkList::on_pushButtonClear_clicked()
{
    destorySelf();
    initUI();
    adjustController();
}

void LinkList::closeEvent(QCloseEvent *event)
{
    destorySelf();
    initUI();
}
void LinkList::on_pushButtonRandomInsert5_clicked()
{
    for(int i=0;i<5;i++)
    {
        setLinkListNormalBrush();
        sleep(sleepTime);
        insertLNode(countNode+1,QString::number(rand()%999999999));
        adjustController();
        ui->lineEditState->setPalette(Qt::GlobalColor::green);
        ui->lineEditState->setText("Insert Success!");
    }
}

void LinkList::on_pushButtonDelete_clicked()
{
    setLinkListNormalBrush();
    sleep(sleepTime);
    QString deleteData;
    deleteLNode(ui->comboBoxDelete->currentText().toInt(),deleteData);
    //调整右侧控件状态及值
    adjustController();
    //ui->lineEditDelete->setText(deleteData);
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Delete Success!");
}


void LinkList::on_pushButtonLocate_clicked()
{
    setLinkListNormalBrush();
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
    }
}


void LinkList::on_horizontalSlider_valueChanged(int value)
{
    sleepTime=MAX_SLEEP_TIME/(value+1);
}

void LinkList::on_saveButton_clicked()
{
    QFile writeFile("F:\\Qt\\Documents\\Data_2_0\\resource\\linklist.txt");
    writeFile.open(QIODevice::Text|QIODevice::WriteOnly);
    QTextStream in(&writeFile);
    int count = countNode;
    in<< count <<"\n";
    int number;
    LNode *pLNode=head;

    for(int i = 0; i < count; i++)
    {
        pLNode = pLNode->next;
        number = pLNode->data.toInt();
        in<< number << "\n";
    }

    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Save Success!");
    //QString data = QString(file.readAll());

    //data.toLatin1().data()

    writeFile.close();
}

void LinkList::on_loadButton_clicked()
{
    destorySelf();
    initLinkList();
    ui->pushButtonClear->setEnabled(true);
    ui->pushButtonInsert->setEnabled(true);
    ui->pushButtonRandomInsert5->setEnabled(true);
    ui->comboBoxInsert->setEnabled(true);
    ui->lineEditInsert->setEnabled(true);
    ui->pushButtonLocate->setEnabled(true);
    ui->lineEditLocate->setEnabled(true);
    qDebug()<< " ****" << endl;
    QFile readFile("F:\\Qt\\Documents\\Data_2_0\\resource\\linklist.txt");
    readFile.open(QIODevice::ReadOnly);
    int count;
    int number;
    QString temp = QString(readFile.readLine());
    count = temp.toInt();
    qDebug() << count  << " count number" << endl;
    for(int i = 0; i < count; i++)
    {
        temp = QString(readFile.readLine());
        number = temp.toInt();
        insertLNode(countNode+1,QString::number(number));
        sleep(sleepTime);
    }

    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Load Success!");

    readFile.close();
}
