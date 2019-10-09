#include <QDebug>
#include <math.h>
#include "myqueue.h"
#include "ui_myqueue.h"
#include "MyQueue.h"
void sleep(unsigned int msec);

const QBrush MyQueue::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush MyQueue::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush MyQueue::markBrush=QBrush(Qt::GlobalColor::green);
const QBrush MyQueue::deleteBrush=QBrush(Qt::GlobalColor::red);

const QFont MyQueue::dataFont=QFont("Consolas",8);

const QIntValidator MyQueue::dataValidator(-999999999,999999999);

MyQueue::MyQueue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyQueue)
{
    //初始化数据
    ui->setupUi(this);
    initTextBrowser();
    initUI();

    countNode=0;
    head=nullptr;
    tail=nullptr;
    scene=nullptr;
    sleepTime=MAX_SLEEP_TIME>>1;
    ui->horizontalSlider->setValue(MAX_SLIDER>>1);
    srand(time_t(nullptr));

    this->setWindowTitle("队列");
}
//析构函数
MyQueue::~MyQueue()
{
    destorySelf();
    delete ui;
}
//初始化文本显示区
void MyQueue::initTextBrowser(){
    QFile file(":/html/html/MyQueue.HTML");
    file.open(QIODevice::ReadOnly);
    QString htmlString=file.readAll();
    ui->textBrowser->setHtml(htmlString);
    file.close();
}
//初始化UI控件
void MyQueue::initUI()
{
    //设置背景图片
    QPixmap background(":/ico/resource/background.png");
    QPalette palette;
    palette.setBrush(QPalette::Background,background);
    this->setPalette(palette);
    //未创建时，除创建按钮外一律无效
    ui->pushButtonClear->setEnabled(false);
    ui->pushButtonInsert->setEnabled(false);
    ui->pushButtonRandomInsert5->setEnabled(false);
    ui->lineEditInsert->setEnabled(false);

    ui->pushButtonDelete->setEnabled(false);

    ui->pushButtonLocate->setEnabled(false);
    ui->lineEditLocate->setEnabled(false);
    ui->lineEditnumber->setEnabled(false);
    ui->lineEditState->setEnabled(false);

    ui->lineEditInsert->setFont(dataFont);
    ui->lineEditLocate->setFont(dataFont);
    ui->lineEditnumber->setFont(dataFont);
    ui->lineEditState->setFont(dataFont);
    ui->lineEditInsert->setPlaceholderText("插入值：Int");
    ui->lineEditLocate->setPlaceholderText("栈顶值：Int");
    ui->lineEditnumber->setPlaceholderText("队列个数");
    ui->lineEditInsert->setValidator(&MyQueue::dataValidator);
    ui->lineEditLocate->setValidator(&MyQueue::dataValidator);
    ui->lineEditnumber->setValidator(&MyQueue::dataValidator);
    ui->lineEditState->setText("请选择操作");

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAX_SLIDER);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
}
//初始化视图框架
void MyQueue::initSceneView()
{
    //为view配置相应scene
    scene=new QGraphicsScene;
    scene->setSceneRect(0,0,SCENE_MAX_W,SCENE_MAX_H);
    ui->graphicsView->setScene(scene);
}
//操作之后调整右侧输入和显示控件
void MyQueue::adjustController()
{
    //节点个数可能会改变，需要考虑一些操作的合理性
    ui->pushButtonDelete->setEnabled(countNode);
    if(countNode==0){
        ui->pushButtonLocate->setEnabled(false);
        ui->pushButtonDelete->setEnabled(false);
    }
    else
    {
        ui->pushButtonLocate->setEnabled(true);
        ui->pushButtonDelete->setEnabled(true);
    }

    ui->lineEditLocate->clear();
    ui->lineEditnumber->setText(QString::number(countNode));

}
//计算节点的SCENE坐标
QPoint MyQueue::getLNodePos(int nodeNumber)
{
    if(nodeNumber==-1) //tail
        return QPoint(SCENE_MAX_W/2,NODE_H+QUEUE_V_OFFSET);
    else             //head
        return QPoint(SCENE_MAX_W/2+QUEUE_H_OFFSET,(nodeNumber+1)*NODE_H+QUEUE_V_OFFSET);
}
//添加节点的GraphicsItem
void MyQueue::addLNodeGraphicsItem(LNode *pl, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    //qDebug()<<"x="<<x<<"y="<<y<<"\n";//将信息输出到控制台
    pl->valueRect    =scene->addRect(x,y+SPACING,VALUE_RECT_W,RECT_H,QPen(),MyQueue::markBrush);
    pl->pointerRect  =scene->addRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pl->valueText    =scene->addText(pl->data,MyQueue::dataFont);
    pl->valueText->setPos(x,y+SPACING+5);
    if(pl->next==nullptr)
    {
        pl->pointerText=scene->addText(" ^",MyQueue::dataFont);
        pl->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
    }
}
//调整节点的箭头
void MyQueue::adjustLNodeArrow(LNode *pLNode, int nodeNumber)
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
        QPoint point3(SCENE_MAX_W/2+QUEUE_ARR_OFFSET,point2.y());

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
//调整尾部箭头
void MyQueue::adjustTailArrow(LNode *pLNode, int nodeNumber)
{
    for(auto &a:pLNode->arrowVector)
        scene->removeItem(a);
    if(nodeNumber == 0)
        return ;

    QPoint myCoord=getLNodePos(-1);
    QPoint nextCoord=getLNodePos(nodeNumber);

    NewArrow *pArrow;

    //节点是一行中最后的节点
    QPoint point1(myCoord.x()+ARROW_H_OFFSET,myCoord.y()+ARROW_V_OFFSET);
    QPoint point2(point1.x(),point1.y()+NODE_H/2+nextCoord.y()-myCoord.y());
    QPoint point3(SCENE_MAX_W/2+QUEUE_ARR_OFFSET,point2.y());

    pArrow=new NewArrow(nextCoord.y()-myCoord.y()+NODE_H/2,2,0);
    scene->addItem(pArrow);
    pArrow->setPos(point1);
    pLNode->arrowVector.push_back(pArrow);

    pArrow=new NewArrow(point2.x()-point3.x(),3,0);
    scene->addItem(pArrow);
    pArrow->setPos(point2);
    pLNode->arrowVector.push_back(pArrow);

    pArrow=new NewArrow(SPACING,0);
    scene->addItem(pArrow);
    pArrow->setPos(point3);
    pLNode->arrowVector.push_back(pArrow);

}
//调整节点scene坐标
void MyQueue::adjustLNodePos(LNode *pLNode, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    pLNode->valueRect->setRect(x,y+SPACING,VALUE_RECT_W,RECT_H);
    pLNode->pointerRect->setRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pLNode->valueText->setPos(x,y+SPACING+5);
    if(pLNode->pointerText)
        pLNode->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
}
//设置链表所有节点显示模式
void MyQueue::setMyQueueNormalBrush()
{
    for(LNode *pLNode=head;pLNode;pLNode=pLNode->next)
        pLNode->setNodeStatus(MyQueue::normalBursh);
    tail->setNodeStatus(MyQueue::normalBursh);
}
//创建链表初始化
void MyQueue::initMyQueue()
{
    initSceneView();
    sleep(sleepTime);
    head=new LNode("Head",nullptr);
    addLNodeGraphicsItem(head, getLNodePos(0));
    sleep(sleepTime);
    tail=new LNode("Tail",nullptr);
    addLNodeGraphicsItem(tail, getLNodePos(-1));
}
//插入节点到链表
void MyQueue::insertLNode(int pos, QString elem)
{
    LNode *pInsertNode=nullptr;
    LNode *pLNode=head;

    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(MyQueue::visitedBrush);
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
    pLNode=head;

    adjustTailArrow(tail,countNode);
}
//删除链表节点
void MyQueue::deleteLNode(int pos, QString &elem)
{
    LNode *pDeleteNode=nullptr;
    LNode *pLNode=head;
    //找到前驱结点
    for(int i=0;i<pos-1;++i)
    {
        pLNode=pLNode->next;
    }
    pDeleteNode=pLNode->next;
    pDeleteNode->setNodeStatus(MyQueue::deleteBrush);
    sleep(sleepTime);
    pLNode->next=pDeleteNode->next;
    elem=pDeleteNode->data;
    //删除节点，移出图形item
    pDeleteNode->removeAll(scene);
    delete pDeleteNode;
    --countNode;

    if(pLNode->next==nullptr)
    {
        QPoint coord=getLNodePos(pos-1);
        pLNode->pointerText=scene->addText(" ^",MyQueue::dataFont);
        pLNode->pointerText->setPos(coord.x()+VALUE_RECT_W, coord.y()+SPACING+5);
    }

    for(--pos;pos<=countNode;++pos)
    {
        sleep(sleepTime);

        adjustLNodePos(pLNode,getLNodePos(pos));
        adjustLNodeArrow(pLNode,pos);
        pLNode=pLNode->next;
    }

    adjustTailArrow(tail,countNode);
}
//查找链表节点
bool MyQueue::locateLNode(int &pos, QString elem)
{
    LNode *pLNode=head;
    for(pos=1;pLNode&&pLNode->next&&pLNode->next->data!=elem;++pos)
    {
        pLNode=pLNode->next;
    }
    sleep(sleepTime);
    //找到相应的节点
    if(pLNode&&pLNode->next){
        pLNode->next->setNodeStatus(MyQueue::markBrush);
        return true;
    }
    return false;
}
//返回队首节点
QString MyQueue::frontNode()
{
    LNode *pLNode=head;
    //for(int pos=1;pLNode&&pLNode->next;++pos)

    pLNode=pLNode->next;

    sleep(sleepTime);
    pLNode->setNodeStatus(MyQueue::markBrush);
    return pLNode->data;
}
//释放申请的内存空间
void MyQueue::destorySelf()
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
    tail->removeAll(scene);
    delete tail;
    sleep(sleepTime);
    scene=nullptr;

    countNode=0;
}
void MyQueue::closeEvent(QCloseEvent *event)
{
    destorySelf();
}
//创建节点
void MyQueue::on_pushButtonInit_clicked()
{
    destorySelf();

    initMyQueue();

    ui->pushButtonClear->setEnabled(true);
    ui->pushButtonInsert->setEnabled(true);
    ui->pushButtonRandomInsert5->setEnabled(true);
    ui->lineEditInsert->setEnabled(true);
    ui->pushButtonLocate->setEnabled(true);
    ui->lineEditLocate->setEnabled(true);
    ui->lineEditnumber->setEnabled(true);
    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Create Success!");

}
//EnQueue按钮
void MyQueue::on_pushButtonInsert_clicked()
{
    setMyQueueNormalBrush();
    sleep(sleepTime);
    QString edit=ui->lineEditInsert->text();
    //若无输入或未输入
    if(edit.isEmpty())
    {
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Please Input!");
        return ;
    }

    insertLNode(countNode+1,edit);
    //调整右侧控件状态及值
    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Insert Success!");
}
//EnQueue_5按钮
void MyQueue::on_pushButtonRandomInsert5_clicked()
{
    for(int i=0;i<5;i++)
        {
            setMyQueueNormalBrush();
            sleep(sleepTime);
            insertLNode(countNode+1,QString::number(rand()%999999999));
            adjustController();
            ui->lineEditState->setPalette(Qt::GlobalColor::green);
            ui->lineEditState->setText("Insert Success!");
        }
}
//DeQueue按钮
void MyQueue::on_pushButtonDelete_clicked()
{
    setMyQueueNormalBrush();
    sleep(sleepTime);
    QString deleteData;
    deleteLNode(1,deleteData);
    //调整右侧控件状态及值
    adjustController();
    //ui->lineEditDelete->setText(deleteData);
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Delete Success!");
}
//front按钮 
void MyQueue::on_pushButtonLocate_clicked()
{
    setMyQueueNormalBrush();
    sleep(sleepTime);
    QString edit = frontNode();
    ui->lineEditLocate->setText(edit);
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Find Successs!");
}
//演示延迟按钮
void MyQueue::on_horizontalSlider_valueChanged(int value)
{
    sleepTime=MAX_SLEEP_TIME/(value+1);
}
//清除按钮
void MyQueue::on_pushButtonClear_clicked()
{
    destorySelf();
    initUI();
}
