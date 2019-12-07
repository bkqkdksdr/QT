#include <QDebug>
#include <math.h>
#include "mycqueue.h"
#include "ui_mycqueue.h"
#include "mycqueue.h"
void sleep(int msec);

const QBrush MyCQueue::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush MyCQueue::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush MyCQueue::markBrush=QBrush(Qt::GlobalColor::green);
const QBrush MyCQueue::deleteBrush=QBrush(Qt::GlobalColor::red);

const QFont MyCQueue::headLabelFont=QFont("Consolas");
const QFont MyCQueue::dataFont=QFont("Consolas",8);

const QIntValidator MyCQueue::dataValidator(-999999999,999999999);

MyCQueue::MyCQueue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyCQueue)
{
    //初始化数据
    ui->setupUi(this);
    initTextBrowser();
    initUI();

    countNode=0;
    size=0;
    front=0;
    tail=0;
    head=nullptr;
    scene=nullptr;
    frontLabel=nullptr;
    rearLabel=nullptr;
    frontArrow=nullptr;
    rearArrow=nullptr;
    sleepTime=MAX_SLEEP_TIME>>1;
    ui->horizontalSlider->setValue(MAX_SLIDER>>1);
    srand(time_t(nullptr));

    this->setWindowTitle("循环链表");
}
//析构函数
MyCQueue::~MyCQueue()
{
    destorySelf();
    delete ui;
}
//初始化文本显示区
void MyCQueue::initTextBrowser(){
    QFile file(":/html/html/MyCQueue.HTML");
    file.open(QIODevice::ReadOnly);
    QString htmlString=file.readAll();
    ui->textBrowser->setHtml(htmlString);
    file.close();
}
//初始化UI控件
void MyCQueue::initUI()
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
    ui->lineEditLocate->setPlaceholderText("队首值：Int");
    ui->lineEditnumber->setPlaceholderText("队列个数");
    ui->lineEditInsert->setValidator(&MyCQueue::dataValidator);
    ui->lineEditLocate->setValidator(&MyCQueue::dataValidator);
    ui->lineEditnumber->setValidator(&MyCQueue::dataValidator);
    ui->lineEditState->setText("请选择操作");

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAX_SLIDER);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
}
//初始化视图框架
void MyCQueue::initSceneView()
{
    //为view配置相应scene
    scene=new QGraphicsScene;
    scene->setSceneRect(0,0,SCENE_MAX_W,SCENE_MAX_H);
    ui->graphicsView->setScene(scene);
    //配置箭头和指针
    frontLabel=scene->addText("front",headLabelFont);
    rearLabel=scene->addText("rear",headLabelFont);
    adjustArrow();
    adjustLabel();
}
//操作之后调整右侧输入和显示控件
void MyCQueue::adjustController()
{
    //节点个数可能会改变，需要考虑一些操作的合理性
    adjustArrow();
    adjustLabel();

    if(size==0){//队列空
        ui->pushButtonLocate->setEnabled(false);
        ui->pushButtonDelete->setEnabled(false);
        ui->pushButtonInsert->setEnabled(true);
        ui->pushButtonRandomInsert5->setEnabled(true);
    }
    else if(size>=CQUEUE_MAXSIZE-1)//队列满
    {
        ui->pushButtonInsert->setEnabled(false);
        ui->pushButtonRandomInsert5->setEnabled(false);
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Queue Full");
    }else
    {
        ui->pushButtonLocate->setEnabled(true);
        ui->pushButtonDelete->setEnabled(true);
        ui->pushButtonInsert->setEnabled(true);
        ui->pushButtonRandomInsert5->setEnabled(true);
    }
    //显示队列个数
    ui->lineEditLocate->clear();
    ui->lineEditnumber->setText(QString::number(size));
}
//计算节点的SCENE坐标
QPoint MyCQueue::getLNodePos(int nodeNumber)
{
    int n = nodeNumber/3;
    int m = nodeNumber%3;
    QPoint point;
    switch(n)
    {
        case 0:
            point = QPoint(SCENE_MAX_W/2+50,m*NODE_H+100);
        break;
        case 1:
            point = QPoint(SCENE_MAX_W/2+50-(m*NODE_H),250);
        break;
        case 2:
            point = QPoint(275,250-m*NODE_H);
        break;
        case 3:
            point = QPoint(275+m*NODE_H,100);
        break;
        default:
            point = QPoint(SCENE_MAX_W/2+200,(nodeNumber+1)*NODE_H+50);
        break;

    }
    return point;
}
//添加节点的GraphicsItem
void MyCQueue::addLNodeGraphicsItem(LNode *pl, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    pl->valueRect    =scene->addRect(x,y+SPACING,CQ_W,CQ_H,QPen(),MyCQueue::normalBursh);
    pl->valueText    =scene->addText(pl->data,MyCQueue::dataFont);
    pl->valueText->setPos(x,y+SPACING+5);


}
//调整节点的箭头
void MyCQueue::adjustArrow()
{
    if(frontArrow)
    {
        scene->removeItem(frontArrow);
        delete frontArrow;
        frontArrow = nullptr;
    }
    int n=front/3;
    QPoint fpoint = getLNodePos(front);
    switch(n)
    {
        case 0:
            frontArrow = new NewArrow(ARROW_LEN,3,1);
            fpoint = QPoint(fpoint.x()+CQ_W*2,fpoint.y()+CQ_H/2);
        break;
        case 1:
            frontArrow = new NewArrow(ARROW_LEN,0,1);
            fpoint = QPoint(fpoint.x()+CQ_W/4*3,fpoint.y()+CQ_H/4*9);
        break;
        case 2:
            frontArrow = new NewArrow(ARROW_LEN,1,1);
            fpoint = QPoint(fpoint.x()-CQ_W,fpoint.y()+CQ_H);
        break;
        case 3:
            frontArrow = new NewArrow(ARROW_LEN,2,1);
            fpoint = QPoint(fpoint.x()+CQ_W/4,fpoint.y()-CQ_H/4*3);
        break;
    }
    scene->addItem(frontArrow);
    frontArrow->setPos(fpoint);
    sleep(sleepTime);
    if(rearArrow)
    {
        scene->removeItem(rearArrow);
        delete rearArrow;
        rearArrow = nullptr;
    }
    int m=tail/3;
    QPoint rpoint = getLNodePos(tail);
    switch(m)
    {
        case 0:
            rearArrow = new NewArrow(ARROW_LEN*2,3,1);
            rpoint = QPoint(rpoint.x()+CQ_W*3,rpoint.y()+CQ_H/4*3);
        break;
        case 1:
            rearArrow = new NewArrow(ARROW_LEN*2,0,1);
            rpoint = QPoint(rpoint.x()+CQ_W/4,rpoint.y()+CQ_H/4*9+ARROW_LEN);
        break;
        case 2:
            rearArrow = new NewArrow(ARROW_LEN*2,1,1);
            rpoint = QPoint(rpoint.x()-CQ_W-ARROW_LEN,rpoint.y()+CQ_H/2);
        break;
        case 3:
            rearArrow = new NewArrow(ARROW_LEN*2,2,1);
            rpoint = QPoint(rpoint.x()+CQ_W/4*3,rpoint.y()-CQ_H/4*3-ARROW_LEN);
        break;
    }
    scene->addItem(rearArrow);
    rearArrow->setPos(rpoint);
}
//调整节点scene坐标
void MyCQueue::adjustLabel()
{
    int n=front/3;
    QPoint fpoint = getLNodePos(front);
    switch(n)
    {
        case 0:
            fpoint = QPoint(fpoint.x()+CQ_W*2,fpoint.y()+CQ_H/8);
        break;
        case 1:
            fpoint = QPoint(fpoint.x()+CQ_W/4*2,fpoint.y()+CQ_H/4*9);
        break;
        case 2:
            fpoint = QPoint(fpoint.x()-CQ_W/4*9,fpoint.y()+CQ_H/8*5);
        break;
        case 3:
            fpoint = QPoint(fpoint.x()-CQ_W/2,fpoint.y()-CQ_H/4*5);
        break;
    }
    frontLabel->setPos(fpoint);
    sleep(sleepTime);
    int m=tail/3;
    QPoint rpoint = getLNodePos(tail);
    switch(m)
    {
        case 0:
            rpoint = QPoint(rpoint.x()+CQ_W*3,rpoint.y()+CQ_H/8*3);
        break;
        case 1:
            rpoint = QPoint(rpoint.x(),rpoint.y()+CQ_H/4*9+ARROW_LEN);
        break;
        case 2:
            rpoint = QPoint(rpoint.x()-CQ_W*2-ARROW_LEN,rpoint.y()+CQ_H/8);
        break;
        case 3:
            rpoint = QPoint(rpoint.x(),rpoint.y()-CQ_H/4*5-ARROW_LEN);
        break;
    }
    rearLabel->setPos(rpoint);
}
//设置链表所有节点显示模式
void MyCQueue::setMyCQueueNormalBrush()
{
    for(LNode *pLNode=head;pLNode;pLNode=pLNode->next)
        pLNode->setNodeStatus(MyCQueue::normalBursh);
}
//创建链表初始化
void MyCQueue::initMyCQueue()
{
    size=0;
    front=0;
    tail=0;
    countNode=0;
    head=nullptr;
    scene=nullptr;
    frontLabel=nullptr;
    rearLabel=nullptr;
    frontArrow=nullptr;
    rearArrow=nullptr;
    initSceneView();

    head=new LNode("###",nullptr);
    QString temp = "###";
    addLNodeGraphicsItem(head, getLNodePos(0));
    for(int i=1;i<CQUEUE_MAXSIZE;i++)
    {
        inittwelve(countNode+1,temp);
        adjustController();
    }

}
//插入节点到链表
void MyCQueue::insertLNode(int pos, QString elem)
{
    LNode *pLNode=head;

    for(int i=0;i<pos;++i)
    {
        pLNode->setNodeStatus(MyCQueue::normalBursh);
        pLNode=pLNode->next;
    }
    pLNode->setNodeStatus(MyCQueue::markBrush);
    scene->removeItem(pLNode->valueText);
    pLNode->setValue(elem);
    pLNode->valueText=scene->addText(elem,MyCQueue::dataFont);
    pLNode->valueText->setPos(getLNodePos(tail).x(),getLNodePos(tail).y()+SPACING+5);
}

void MyCQueue::inittwelve(int pos, QString elem)
{
    if(countNode==CQUEUE_MAXSIZE) return;
    LNode *pInsertNode=nullptr;
    LNode *pLNode=head;

    for(int i=0;i<pos-1;++i)
    {
        pLNode=pLNode->next;
    }
    pInsertNode=new LNode(elem,pLNode->next);
    pLNode->next=pInsertNode;
    ++countNode;
    addLNodeGraphicsItem(pInsertNode,getLNodePos(pos));
}
//删除链表节点
void MyCQueue::deleteLNode(int pos, QString &elem)
{
    LNode *pLNode=head;

    for(int i=0;i<pos;++i)
    {
        pLNode->setNodeStatus(MyCQueue::normalBursh);
        pLNode=pLNode->next;
    }
    pLNode->setNodeStatus(MyCQueue::deleteBrush);
    scene->removeItem(pLNode->valueText);
    pLNode->setValue(elem);
    pLNode->valueText=scene->addText(elem,MyCQueue::dataFont);
    pLNode->valueText->setPos(getLNodePos(front).x(),getLNodePos(front).y()+SPACING+5);
}
//查找链表节点
bool MyCQueue::locateLNode(int &pos, QString elem)
{
    LNode *pLNode=head;
    for(pos=1;pLNode&&pLNode->next&&pLNode->next->data!=elem;++pos)
    {
        pLNode=pLNode->next;
    }
    sleep(sleepTime);
    //找到相应的节点
    if(pLNode&&pLNode->next){
        pLNode->next->setNodeStatus(MyCQueue::markBrush);
        return true;
    }
    return false;
}
//返回栈顶元素
QString MyCQueue::topNode()
{
    LNode *pLNode=head;

    for(int i=0;i<front;++i)
    {
        pLNode->setNodeStatus(MyCQueue::normalBursh);
        pLNode=pLNode->next;
    }
    pLNode->setNodeStatus(MyCQueue::visitedBrush);
    return pLNode->data;
}
//释放申请的内存空间
void MyCQueue::destorySelf()
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
    if(frontArrow)
    {
        scene->removeItem(frontArrow);
        delete frontArrow;
    }
    if(frontLabel)
    {
        scene->removeItem(frontLabel);
        delete frontLabel;
    }
    sleep(sleepTime);
    if(rearArrow)
    {
        scene->removeItem(rearArrow);
        delete rearArrow;
    }
    if(rearLabel)
    {
        scene->removeItem(rearLabel);
        delete rearLabel;
    }

    scene=nullptr;

    countNode=0;
}
void MyCQueue::closeEvent(QCloseEvent *event)
{
    destorySelf();
    initUI();
}
//槽函数
//创建按钮
void MyCQueue::on_pushButtonInit_clicked()
{
    destorySelf();

    initMyCQueue();

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
void MyCQueue::on_pushButtonInsert_clicked()
{
    setMyCQueueNormalBrush();
    sleep(sleepTime);
    QString edit=ui->lineEditInsert->text();
    //若无输入或未输入
    if(edit.isEmpty())
    {
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Please Input!");
        return ;
    }
    insertLNode(tail,edit);
    size++;
    tail = (tail+1)%12;
    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Insert Success!");
}
//push_5按钮
void MyCQueue::on_pushButtonRandomInsert5_clicked()
{
    for(int i=0;i<5;i++)
        {
        if(size<CQUEUE_MAXSIZE-1){
            setMyCQueueNormalBrush();
            sleep(sleepTime);
            insertLNode(tail,QString::number(rand()%999999999));
            size++;
            tail = (tail+1)%12;
            adjustController();
            ui->lineEditState->setPalette(Qt::GlobalColor::green);
            ui->lineEditState->setText("Push Success!");
            }
        }

}
//pop按钮
void MyCQueue::on_pushButtonDelete_clicked()
{
    setMyCQueueNormalBrush();
    sleep(sleepTime);
    QString deleteData="###";
    deleteLNode(front,deleteData);
    size--;
    front = (front+1)%12;
    //调整右侧控件状态及值
    adjustController();
    if(size)
    {
        ui->lineEditState->setPalette(Qt::GlobalColor::green);
        ui->lineEditState->setText("Pop Success!");
    }else {
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("Queue Empty!");
}

}
//top按钮
void MyCQueue::on_pushButtonLocate_clicked()
{
    setMyCQueueNormalBrush();
    sleep(sleepTime);
    QString edit = topNode();
    ui->lineEditLocate->setText(edit);
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Find Successs!");
}
//演示延迟调试
void MyCQueue::on_horizontalSlider_valueChanged(int value)
{
    sleepTime=MAX_SLEEP_TIME/(value+1);
}
//清除按钮
void MyCQueue::on_pushButtonClear_clicked()
{
    destorySelf();
    initUI();
}

void MyCQueue::on_saveButton_clicked()
{
    QFile writeFile("F:\\Qt\\Documents\\Data_2_0\\resource\\mycqueue.txt");
    writeFile.open(QIODevice::Text|QIODevice::WriteOnly);
    QTextStream in(&writeFile);
    int count = 11;
    in<< front << "\n";
    in << tail << "\n";
    int number;
    LNode *pLNode=head;
    if(pLNode->data == "###") number = -1;
    else number = pLNode->data.toInt();
    in<< number << "\n";
    qDebug() << "sdsd" << endl;
    for(int i = 0; i < count; i++)
    {
        pLNode = pLNode->next;
        if(pLNode->data == "###") number = -1;
        else number = pLNode->data.toInt();
        in<< number << "\n";
    }

    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Save Success!");
    //QString data = QString(file.readAll());

    //data.toLatin1().data()

    writeFile.close();
}

void MyCQueue::on_loadButton_clicked()
{
    destorySelf();
    initMyCQueue();
    ui->pushButtonClear->setEnabled(true);
    ui->pushButtonInsert->setEnabled(true);
    ui->pushButtonRandomInsert5->setEnabled(true);
    ui->lineEditInsert->setEnabled(true);
    ui->pushButtonLocate->setEnabled(true);
    ui->lineEditLocate->setEnabled(true);
    qDebug()<< " ****" << endl;
    QFile readFile("F:\\Qt\\Documents\\Data_2_0\\resource\\mycqueue.txt");
    readFile.open(QIODevice::ReadOnly);
    int number;
    QString temp = QString(readFile.readLine());
    QString elem;
    front = temp.toInt();
    temp = QString(readFile.readLine());
    tail = temp.toInt();
    size = 0;
    qDebug()<< front <<" ****" << tail << endl;
    LNode *pLNode=head;
    for(int i = 0; i < 12; i++)
    {
        temp = QString(readFile.readLine());
        number = temp.toInt();
        if(number == -1) elem = "###";
        else {
            elem = temp;
            size++;
        }
        scene->removeItem(pLNode->valueText);
        pLNode->setValue(elem);
        pLNode->valueText=scene->addText(elem,MyCQueue::dataFont);
        pLNode->valueText->setPos(getLNodePos(i).x(),getLNodePos(i).y()+SPACING+5);
        sleep(sleepTime);
        pLNode=pLNode->next;
    }

    adjustController();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Load Success!");

    readFile.close();
}
