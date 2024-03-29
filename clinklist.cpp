#include <QDebug>
#include <math.h>
#include "clinklist.h"
#include "ui_clinklist.h"
#include "newarrow.h"

void sleep(int msec);

const QBrush CLinkList::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush CLinkList::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush CLinkList::markBrush=QBrush(Qt::GlobalColor::green);

const QFont CLinkList::headLabelFont=QFont("Consolas");
const QFont CLinkList::dataFont=QFont("Consolas",8);

const QIntValidator CLinkList::dataValidator(-999999999,999999999);

//构造函数
CLinkList::CLinkList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLinkList)
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

    this->setWindowTitle("循环链表");
}

//析构函数
CLinkList::~CLinkList()
{
    destroySelf();
    delete ui;
}

//初始设置文本显示区
void CLinkList::initTextBrowser(){
    //加载html文件
    QFile file(":/html/html/CLinkList.html");
    file.open(QIODevice::ReadOnly);
    QString htmlString=file.readAll();
    ui->textBrowser->setHtml(htmlString);
    //ui->textBrowser->setOpenLinks(true);
    //ui->textBrowser->setOpenExternalLinks(true);

    file.close();
}

//初始UI控件
void CLinkList::initUI()
{
    //设置图标
    QIcon exeIcon(":/ico/resource/exe.ico");
    this->setWindowIcon(exeIcon);
    //设置背景图片
    QPixmap background(":/ico/resource/background.png");
    QPalette palette;
    palette.setBrush(QPalette::Background,background);
    this->setPalette(palette);

    //未创建时，除创建按钮外，其余按键一律无效
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
    ui->lineEditInsert->setValidator(&CLinkList::dataValidator);
    ui->lineEditLocate->setValidator(&CLinkList::dataValidator);
    ui->lineEditState->setText("请选择操作");

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAX_SLIDER);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
    adjustController();
}


//初始化视图框架
void CLinkList::initSceneView()
{
    //为 view 配置相应 scene
    scene=new QGraphicsScene;
    scene->setSceneRect(0,0,SCENE_MAX_W,SCENE_MAX_H);
    ui->graphicsView->setScene(scene);

    //添加链表符号及指针箭头
    headLabel=scene->addText("CL",headLabelFont);
    headLabel->setPos((VALUE_RECT_W-RECT_H)>>1,SPACING);
    sleep(sleepTime);
    headArrow=new NewArrow(ARROW_LEN);
    scene->addItem(headArrow);
    headArrow->setPos(ARROW_H_OFFSET,ARROW_V_OFFSET);
}

//操作之后调整右侧输入和显示控件
void CLinkList::adjustController()
{
    //当经过插入或删除操作后，节点个数可能会改变，需考虑一些操作的合法性
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
    ui->lineEditInsert->setValidator(&CLinkList::dataValidator);
    ui->lineEditLocate->setValidator(&CLinkList::dataValidator);
    //ui->lineEditDelete->setText(" ");
    ui->comboBoxLocate->clear();
}

//计算节点的Scene坐标
QPoint CLinkList::getLNodePos(int nodeNumber)
{
    const static int rowN=SCENE_MAX_W/NODE_W;
    return QPoint((nodeNumber+1)%rowN*NODE_W,(nodeNumber+1)/rowN*NODE_H);
}

//添加节点的GraphicsItem
void CLinkList::addLNodeGraphicsItem(LNode *pl, QPoint coord)
{
    int x=coord.x(), y=coord.y();

    pl->valueRect   =scene->addRect(x,y+SPACING,VALUE_RECT_W,RECT_H,QPen(),CLinkList::markBrush);
    pl->pointerRect =scene->addRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pl->valueText   =scene->addText(pl->data,CLinkList::dataFont);
    pl->valueText->setPos(x,y+SPACING+5);
    if(pl->next==nullptr)
    {
        pl->pointerText=scene->addText(" ^",CLinkList::dataFont);
        pl->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
    }
}

//调整节点的箭头
void CLinkList::adjustLNodeArrow(LNode *pLNode, int nodeNumber)
{
    for(auto &a:pLNode->arrowVector)
        scene->removeItem(a);

    QPoint myCoord=getLNodePos(nodeNumber);
    QPoint nextCoord=getLNodePos(nodeNumber+1);

    NewArrow *pArrow;

    //若是指向头结点
    if(pLNode->next==head)
    {
        QPoint point1(myCoord.x()+ARROW_H_OFFSET,myCoord.y()+ARROW_V_OFFSET);
        QPoint point2(SCENE_MAX_W/NODE_W*NODE_W, point1.y());
        QPoint point3(point2.x(), 1);
        QPoint point4(NODE_W+(VALUE_RECT_W>>1), point3.y());

        pArrow=new NewArrow(point2.x()-point1.x(),1,0);
        scene->addItem(pArrow);
        pArrow->setPos(point1);
        pLNode->arrowVector.push_back(pArrow);

        pArrow=new NewArrow(point2.y()-point3.y(),0,0);
        scene->addItem(pArrow);
        pArrow->setPos(point2);
        pLNode->arrowVector.push_back(pArrow);

        pArrow=new NewArrow(point3.x()-point4.x(),3,0);
        scene->addItem(pArrow);
        pArrow->setPos(point3);
        pLNode->arrowVector.push_back(pArrow);

        pArrow=new NewArrow(SPACING-point4.y(),2,1);
        scene->addItem(pArrow);
        pArrow->setPos(point4);
        pLNode->arrowVector.push_back(pArrow);
        return ;
    }

    if(myCoord.y()==nextCoord.y())
    {
        //节点不是一行最后节点
        pArrow=new NewArrow(ARROW_LEN);
        scene->addItem(pArrow);
        pArrow->setPos(myCoord.x()+ARROW_H_OFFSET,myCoord.y()+ARROW_V_OFFSET);
        pLNode->arrowVector.push_back(pArrow);
    }
    else
    {
        //节点是一行中最后节点
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

//调整节点Scene坐标
void CLinkList::adjustLNodePos(LNode *pLNode, QPoint coord)
{
    int x=coord.x(), y=coord.y();
    pLNode->valueRect->setRect(x,y+SPACING,VALUE_RECT_W,RECT_H);
    pLNode->pointerRect->setRect(x+VALUE_RECT_W,y+SPACING,POINTER_RECT_W,RECT_H);
    pLNode->valueText->setPos(x,y+SPACING+5);
    if(pLNode->pointerText)
        pLNode->pointerText->setPos(x+VALUE_RECT_W, y+SPACING+5);
}

//设置链表所有节点显示模式
void CLinkList::setCLinkListNormalBrush()
{
    //head->setNodeStatus(CLinkList::normalBursh);
    for(LNode *pLNode=head->next;pLNode!=head;pLNode=pLNode->next)
        pLNode->setNodeStatus(CLinkList::normalBursh);
}

//创建链表初始化
void CLinkList::initCLinkList()
{
    initSceneView();
    sleep(sleepTime);
    head=new LNode("头结点", nullptr);
    head->next=head;

    addLNodeGraphicsItem(head, getLNodePos(0));
    adjustLNodeArrow(head,0);
}

//插入节点到链表
void CLinkList::insertLNode(int pos, QString elem)
{
    LNode *pInsertNode=nullptr;
    LNode *pLNode=head;

    head->setNodeStatus(CLinkList::visitedBrush);

    //找到前驱节点指针
    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(CLinkList::visitedBrush);
    }
    sleep(sleepTime);
    if(pLNode->next==nullptr){
        scene->removeItem(pLNode->pointerText);
        pLNode->pointerText=nullptr;
    }

    //新节点插入到链表中
    pInsertNode=new LNode(elem,pLNode->next);
    pLNode->next=pInsertNode;
    ++countNode;

    //添加图形Item
    addLNodeGraphicsItem(pInsertNode,getLNodePos(pos));

    for(--pos;pos<=countNode;++pos)
    {
        sleep(sleepTime);

        adjustLNodePos(pLNode,getLNodePos(pos));    //调整节点坐标位置
        adjustLNodeArrow(pLNode,pos);               //调整每个节点箭头
        pLNode=pLNode->next;
    }
}

//删除链表节点
void CLinkList::deleteLNode(int pos, QString &elem)
{
    LNode *pDeleteNode=nullptr;
    LNode *pLNode=head;

    head->setNodeStatus(CLinkList::visitedBrush);

    //找到前驱节点指针
    for(int i=0;i<pos-1;++i)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(CLinkList::visitedBrush);
    }
    sleep(sleepTime);
    pDeleteNode=pLNode->next;
    pLNode->next=pDeleteNode->next;
    elem=pDeleteNode->data;

    //删除节点，移除图形Item
    pDeleteNode->removeAll(scene);
    delete pDeleteNode;
    --countNode;

    if(pLNode->next==nullptr){
        QPoint coord=getLNodePos(pos-1);
        pLNode->pointerText=scene->addText(" ^",CLinkList::dataFont);
        pLNode->pointerText->setPos(coord.x()+VALUE_RECT_W, coord.y()+SPACING+5);
    }

    for(--pos;pos<=countNode;++pos)
    {
        sleep(sleepTime);

        adjustLNodePos(pLNode,getLNodePos(pos));    //调整节点坐标位置
        adjustLNodeArrow(pLNode,pos);               //调整节点箭头
        pLNode=pLNode->next;
    }
}

//查找链表节点
bool CLinkList::locateLNode(int &pos, QString elem)
{
    LNode *pLNode=head;

    head->setNodeStatus(CLinkList::visitedBrush);

    for(pos=1;pLNode->next!=head&&pLNode->next->data!=elem;++pos)
    {
        sleep(sleepTime);

        pLNode=pLNode->next;
        pLNode->setNodeStatus(CLinkList::visitedBrush);
    }
    sleep(sleepTime);
    //找到相应节点
    if(pLNode->next!=head){
        pLNode->next->setNodeStatus(CLinkList::markBrush);
        return true;
    }
    return false;
}

//释放申请的内存空间
void CLinkList::destroySelf()
{
    if(scene==nullptr)
        return ;

    LNode *pLNode=head->next, *qLNode;
    for(;pLNode!=head;pLNode=qLNode)
    {
        sleep(sleepTime);
        qLNode=pLNode->next;
        pLNode->removeAll(scene);       //移除每个节点的图形Item
        delete pLNode;      //释放内存
    }
    sleep(sleepTime);
    head->removeAll(scene);     //移除头结点图形Item
    delete head;                //释放内存
    scene->removeItem(headLabel);   //移除链表符号Item
    scene->removeItem(headArrow);   //移除链表符号后的箭头Item
    delete headLabel;       //释放内存
    delete headArrow;       //释放内存
    scene=nullptr;

    countNode=0;
}
void CLinkList::closeEvent(QCloseEvent *event)
{
    destroySelf();
    initUI();
}

//槽函数：点击创建
void CLinkList::on_pushButtonInit_clicked()
{
    //若已经建立，需要清除重建
    destroySelf();

    initCLinkList();

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

//槽函数：点击清空
void CLinkList::on_pushButtonClear_clicked()
{
    destroySelf();
    initUI();
    adjustController();
}

//槽函数：点击插入
void CLinkList::on_pushButtonInsert_clicked()
{
    setCLinkListNormalBrush();
    sleep(sleepTime);
    QString edit=ui->lineEditInsert->text();

    //若输入无效或未输入
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

//随机插入五个节点到链表末尾
void CLinkList::on_pushButtonRandomInsert5_clicked()
{
    for(int i=0;i<5;++i)
    {
        setCLinkListNormalBrush();
        sleep(sleepTime);
        insertLNode(countNode+1,QString::number(rand()%999999999));
        adjustController();
        ui->lineEditState->setPalette(Qt::GlobalColor::green);
        ui->lineEditState->setText("Insert Success!");
    }
}

//槽函数：点击删除
void CLinkList::on_pushButtonDelete_clicked()
{
    setCLinkListNormalBrush();sleep(sleepTime);
    QString deleteData;
    deleteLNode(ui->comboBoxDelete->currentText().toInt(),deleteData);

    //调整右侧控件状态及值
    adjustController();
    //ui->lineEditDelete->setText(deleteData);
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Delete Success!");
}

//槽函数：点击查找
void CLinkList::on_pushButtonLocate_clicked()
{
    setCLinkListNormalBrush();
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

//调整演示的速度快慢
void CLinkList::on_horizontalSlider_valueChanged(int value)
{
    sleepTime=MAX_SLEEP_TIME/(value+1);
}

void CLinkList::on_saveButton_clicked()
{
    QFile writeFile("F:\\Qt\\Documents\\Data_2_0\\resource\\clinklist.txt");
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

void CLinkList::on_loadButton_clicked()
{
    destroySelf();
    initCLinkList();
    ui->pushButtonClear->setEnabled(true);
    ui->pushButtonInsert->setEnabled(true);
    ui->pushButtonRandomInsert5->setEnabled(true);
    ui->comboBoxInsert->setEnabled(true);
    ui->lineEditInsert->setEnabled(true);
    ui->pushButtonLocate->setEnabled(true);
    ui->lineEditLocate->setEnabled(true);
    qDebug()<< " ****" << endl;
    QFile readFile("F:\\Qt\\Documents\\Data_2_0\\resource\\clinklist.txt");
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
