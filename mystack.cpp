#include <QDebug>
#include <math.h>
#include "mystack.h"
#include "ui_mystack.h"
#include "mystack.h"
void sleep(int msec);

const QBrush MyStack::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush MyStack::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush MyStack::markBrush=QBrush(Qt::GlobalColor::green);
const QBrush MyStack::deleteBrush=QBrush(Qt::GlobalColor::red);

const QFont MyStack::dataFont=QFont("Consolas",8);

const QIntValidator MyStack::dataValidator(-999999999,999999999);

MyStack::MyStack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyStack)
{
    //初始化数据
    ui->setupUi(this);
    initTextBrowser(); 
    initUI();

    countNode=0;
    head=nullptr;
    scene=nullptr;
    sleepTime=MAX_SLEEP_TIME>>1;
    ui->horizontalSlider->setValue(MAX_SLIDER>>1);
    srand(time_t(nullptr));

    this->setWindowTitle("栈");
}
//析构函数
MyStack::~MyStack()
{
    destorySelf();
    delete ui;
}
//初始化文本显示区
void MyStack::initTextBrowser(){
    QFile file(":/html/html/MyStack.html");
    file.open(QIODevice::ReadOnly);
    QString htmlString=file.readAll();
    ui->textBrowser->setHtml(htmlString);
    file.close();
}
//初始化UI控件
void MyStack::initUI()
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
    ui->lineEditnumber->setPlaceholderText("堆栈个数");
    ui->lineEditInsert->setValidator(&MyStack::dataValidator);
    ui->lineEditLocate->setValidator(&MyStack::dataValidator);
    ui->lineEditnumber->setValidator(&MyStack::dataValidator);
    ui->lineEditState->setText("请选择操作");

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAX_SLIDER);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
}
//初始化视图框架
void MyStack::initSceneView()
{
    //为view配置相应scene
    scene=new QGraphicsScene;
    scene->setSceneRect(0,0,SCENE_MAX_W,SCENE_MAX_H);
    ui->graphicsView->setScene(scene);
}
//操作之后调整右侧输入和显示控件
void MyStack::adjustController()
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
QPoint MyStack::getLNodePos(int nodeNumber)
{
    return QPoint(SCENE_MAX_W/2+STACK_H_OFFSET,(nodeNumber+1)*NODE_H+STACK_V_OFFSET);
}
//添加节点的GraphicsItem
void MyStack::addLNodeGraphicsItem(LNode *pl, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    //qDebug()<<"x="<<x<<"y="<<y<<"\n";//将信息输出到控制台
    pl->valueRect    =scene->addRect(x,y+SPACING,VALUE_RECT_W,RECT_H,QPen(),MyStack::markBrush);
    pl->pointerRect  =scene->addRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pl->valueText    =scene->addText(pl->data,MyStack::dataFont);
    pl->valueText->setPos(x,y+SPACING+5);
    if(pl->next==nullptr)
    {
        pl->pointerText=scene->addText(" ^",MyStack::dataFont);
        pl->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
    }
}
//调整节点的箭头
void MyStack::adjustLNodeArrow(LNode *pLNode, int nodeNumber)
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
        QPoint point3(SCENE_MAX_W/2+STACK_ARR_OFFSET,point2.y());

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
void MyStack::adjustLNodePos(LNode *pLNode, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    pLNode->valueRect->setRect(x,y+SPACING,VALUE_RECT_W,RECT_H);
    pLNode->pointerRect->setRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pLNode->valueText->setPos(x,y+SPACING+5);
    if(pLNode->pointerText)
        pLNode->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
}
//设置链表所有节点显示模式
void MyStack::setMyStackNormalBrush()
{
    for(LNode *pLNode=head;pLNode;pLNode=pLNode->next)
        pLNode->setNodeStatus(MyStack::normalBursh);
}
//创建链表初始化
void MyStack::initMyStack()
{
    initSceneView();
    sleep(sleepTime);
    head=new LNode("Top",nullptr);
    addLNodeGraphicsItem(head, getLNodePos(0));
}
//插入节点到链表
void MyStack::insertLNode(int pos, QString elem)
{
    LNode *pInsertNode=nullptr;
    LNode *pLNode=head;

    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);
        pLNode=pLNode->next;
        pLNode->setNodeStatus(MyStack::visitedBrush);
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
void MyStack::deleteLNode(int pos, QString &elem)
{
    LNode *pDeleteNode=nullptr;
    LNode *pLNode=head;
    //找到前驱结点
    for(int i=0;i<pos-1;++i)
    {
        pLNode=pLNode->next;
    }
    pDeleteNode=pLNode->next;
    pDeleteNode->setNodeStatus(MyStack::deleteBrush);
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
        pLNode->pointerText=scene->addText(" ^",MyStack::dataFont);
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
bool MyStack::locateLNode(int &pos, QString elem)
{
    LNode *pLNode=head;
    for(pos=1;pLNode&&pLNode->next&&pLNode->next->data!=elem;++pos)
    {
        pLNode=pLNode->next;
    }
    sleep(sleepTime);
    //找到相应的节点
    if(pLNode&&pLNode->next){
        pLNode->next->setNodeStatus(MyStack::markBrush);
        return true;
    }
    return false;
}
//返回栈顶元素
QString MyStack::topNode()
{
    LNode *pLNode=head->next;
    pLNode->setNodeStatus(MyStack::visitedBrush);
    sleep(sleepTime);
    pLNode->setNodeStatus(MyStack::markBrush);
    return pLNode->data;
}
//释放申请的内存空间
void MyStack::destorySelf()
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
    scene=nullptr;

    countNode=0;
}
void MyStack::closeEvent(QCloseEvent *event)
{
    destorySelf();
}
//槽函数
//创建按钮
void MyStack::on_pushButtonInit_clicked()
{
    destorySelf();

    initMyStack();

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
//push按钮
void MyStack::on_pushButtonInsert_clicked()
{
    setMyStackNormalBrush();
    sleep(sleepTime);
    QString edit=ui->lineEditInsert->text();
    //若无输入或未输入
    if(edit.isEmpty())
    {
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Please Input!");
        return ;
    }

    insertLNode(1,edit);
    //调整右侧控件状态及值
    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Insert Success!");
}
//push_5按钮
void MyStack::on_pushButtonRandomInsert5_clicked()
{
    for(int i=0;i<5;i++)
        {
            setMyStackNormalBrush();
            sleep(sleepTime);
            insertLNode(1,QString::number(rand()%999999999));
            adjustController();
            ui->lineEditState->setPalette(Qt::GlobalColor::green);
            ui->lineEditState->setText("Push Success!");
        }
}
//pop按钮
void MyStack::on_pushButtonDelete_clicked()
{
    setMyStackNormalBrush();
    sleep(sleepTime);
    QString deleteData;
    deleteLNode(1,deleteData);
    //调整右侧控件状态及值
    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Pop Success!");
}
//top按钮
void MyStack::on_pushButtonLocate_clicked()
{
    setMyStackNormalBrush();
    sleep(sleepTime);
    QString edit = topNode();
    ui->lineEditLocate->setText(edit);
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Find Successs!");
}
//演示延迟调试
void MyStack::on_horizontalSlider_valueChanged(int value)
{
    sleepTime=(unsigned int)MAX_SLEEP_TIME/(value+1);
}
//清除按钮
void MyStack::on_pushButtonClear_clicked()
{
    destorySelf();
    initUI();
}
