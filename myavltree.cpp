#include "myavltree.h"
#include "ui_myavltree.h"
#include "newedge.h"
#include "newnode.h"
#include "uidefine.h"

#include <math.h>
#include <QInputDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSpinBox>
#include <QTime>
#include <QQueue>
#include <QDialogButtonBox>
#include <QGraphicsView>
#include <QFormLayout>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <QFile>
#include <QPainter>
using namespace std;
void sleep(int msec);

const QBrush MyAVLTree::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush MyAVLTree::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush MyAVLTree::markBrush=QBrush(Qt::GlobalColor::green);

const QFont MyAVLTree::headLabelFont=QFont("Consolas");
const QFont MyAVLTree::dataFont=QFont("Consolas",8);

const QIntValidator MyAVLTree::dataValidator(-999999999,999999999);

MyAVLTree::MyAVLTree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyAVLTree)
{
    ui->setupUi(this);
    initTextBrowser();
    initUI();

    timerId = 0;
    scene = nullptr;
    sleepTime=MAX_SLEEP_TIME>>1;
    ui->horizontalSlider->setValue(MAX_SLIDER>>1);
    srand(time_t(nullptr));

    this->setWindowTitle("AVL树");
}

MyAVLTree::~MyAVLTree()
{
    delete ui;
}
//初始化文本显示区
void MyAVLTree::initTextBrowser(){
    QFile file(":/html/html/MyAVLTree.html");
    file.open(QIODevice::ReadOnly);
    QString htmlString=file.readAll();
    ui->textBrowser->setHtml(htmlString);
    file.close();
}

//初始化UI控件
void MyAVLTree::initUI()
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

    ui->addNode->setEnabled(false);
    ui->deleteNode->setEnabled(false);
    ui->searchNode->setEnabled(false);
    ui->addNode5->setEnabled(false);
    ui->pushButtonclear->setEnabled(false);
    ui->InOrder->setEnabled(false);
    ui->PreOrder->setEnabled(false);
    ui->LastOrder->setEnabled(false);
    ui->LevelOrder->setEnabled(false);

    ui->lineEditState->setEnabled(false);
    ui->lineEditState->setFont(dataFont);
    ui->lineEditState->setText("请选择创建");

    ui->lineCount->setEnabled(false);
    ui->lineCount->setFont(dataFont);
    ui->lineCount->setText("0");

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAX_SLIDER);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
    adjustContronller();
}
//初始化视图框架
void MyAVLTree::initSceneView()
{
    scene=new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //ui->graphicsView->setCacheMode(CacheBackground);
   // ui->graphicsView->setViewportUpdateMode(BoundingRectViewportUpdate);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    //ui->graphicsView->setTransformationAnchor(AnchorUnderMouse);
    ui->graphicsView->scale(qreal(0.8), qreal(0.8));
    scene->setSceneRect(0,0,SCENE_MAX_W,SCENE_MAX_H);
    ui->graphicsView->setScene(scene);
}
void MyAVLTree::initMyAVLTree()
{
    initSceneView();
    traces.clear();
}

void MyAVLTree::destorySelf()
{
    if(trees.size())
    {
        trees.clear();
        EmptyNodeAndEdge(1);
    }
    adjustContronller();
}

void MyAVLTree::adjustContronller()
{
    if(!trees.size())
    {
        ui->pushButtonclear->setEnabled(false);
        ui->InOrder->setEnabled(false);
        ui->PreOrder->setEnabled(false);
        ui->LastOrder->setEnabled(false);
        ui->LevelOrder->setEnabled(false);
        ui->deleteNode->setEnabled(false);
        ui->searchNode->setEnabled(false);
        ui->loadButton->setEnabled(true);
        ui->saveButton->setEnabled(false);
    }else
    {
        int numbers = 0;
        count(trees.at(0),numbers);
        ui->lineCount->setText(QString::number(numbers));
        if(numbers)
        {
            ui->pushButtonclear->setEnabled(true);
            ui->InOrder->setEnabled(true);
            ui->PreOrder->setEnabled(true);
            ui->LastOrder->setEnabled(true);
            ui->LevelOrder->setEnabled(true);
            ui->addNode->setEnabled(true);
            ui->addNode5->setEnabled(true);
            ui->deleteNode->setEnabled(true);
            ui->searchNode->setEnabled(true);
            ui->saveButton->setEnabled(true);
            ui->loadButton->setEnabled(false);
        }else
        {
            ui->pushButtonclear->setEnabled(false);
            ui->InOrder->setEnabled(false);
            ui->PreOrder->setEnabled(false);
            ui->LastOrder->setEnabled(false);
            ui->LevelOrder->setEnabled(false);
            ui->deleteNode->setEnabled(false);
            ui->searchNode->setEnabled(false);
            ui->loadButton->setEnabled(true);
            ui->saveButton->setEnabled(false);
        }
    }
}
void MyAVLTree::count(AVLTree &T, int &numbers)
{
    if(nullptr == T) return;
    count(T->lchild,numbers);
    numbers++;
    count(T->rchild,numbers);
}
void MyAVLTree::PreOrder(AVLTree T)
{
    sleep(sleepTime);
    if(nullptr == T) return;
    Setvisited(T);
    PreOrder(T->lchild);
    PreOrder(T->rchild);
}
void MyAVLTree::InOrder(AVLTree T)
{
    sleep(sleepTime);
    if(nullptr == T) return;
    InOrder(T->lchild);
    Setvisited(T);
    InOrder(T->rchild);  
}
void MyAVLTree::LastOrder(AVLTree T)
{
    if(nullptr == T) return;
    LastOrder(T->lchild);
    LastOrder(T->rchild);
    Setvisited(T);
    sleep(sleepTime);
}
void MyAVLTree::LevelOrder(AVLTree T)
{
    QQueue<AVLTree> q;
    if(nullptr == T) return;
    q.enqueue(T);
    while(!q.empty())
    {
        AVLTree temp = q.dequeue();
        Setvisited(temp);
        sleep(sleepTime);
        if(temp->lchild)
            q.enqueue(temp->lchild);
        if(temp->rchild)
            q.enqueue(temp->rchild);
     }
}
void MyAVLTree::Setvisited(AVLTree T)
{
    std::vector<AVLTree> result;
    foreach (AVLTree t, trees) {
        AVLTree s;
        s = SearchAVL(t,T->data);
        if(nullptr!=s)result.push_back(s);
    }
    //更新点
    QList<NewNode *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (NewNode *node = qgraphicsitem_cast<NewNode *>(item))
        {
            node->isVisited=std::find(result.begin(),result.end(),node->node)!=result.end();
            node->update();
        }
    }
}
void MyAVLTree::on_pushButtonInit_clicked()
{
    destorySelf();
    initMyAVLTree();

    ui->addNode->setEnabled(true);
    ui->deleteNode->setEnabled(true);
    ui->searchNode->setEnabled(true);
    ui->addNode5->setEnabled(true);
    ui->pushButtonclear->setEnabled(true);

    adjustContronller();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Create Success");

}

void MyAVLTree::on_addNode_clicked()
{
    qsrand(QTime(0, 0 ,0).secsTo(QTime::currentTime()));
    bool flag = false;
    int number = 0;
    if(trees.size() > 1)
    {
        number = QInputDialog::getInt(nullptr,tr("选择树"),tr("输入树的编号(从左至右)"),
                                      std::rand()%trees.size(),0,int(trees.size())-1,2,&flag);
        if(!flag){
           return;
        }
    }
    int input = QInputDialog::getInt(nullptr,tr("插入数字"),tr("请输入0-99的数字"),std::rand()%100,0,100,2,&flag);
    if(!flag)
    {
        return ;
    }
    if(trees.empty())
    {
        trees.push_back(nullptr);
    }
    Status s;
    if(InsertAVL(trees.at(number),input, s) == NO)
    {
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("该数字已经存在");
        return ;
    }
    trace temp;
    temp.value=input;
    temp.insert=true;
    traces.push_back(temp);
    drawTree();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Insert Success");
}

void MyAVLTree::itemMoved()
{
    if(!timerId)
        timerId = startTimer(1000 / 25);
}

void MyAVLTree::EmptyNodeAndEdge(int state)
{
    QList<NewNode *> nodes;
    foreach(QGraphicsItem *item, scene->items())
    {
        if(NewNode *node = qgraphicsitem_cast<NewNode *>(item))
            scene->removeItem(node);
        if(NewEdge *edge = qgraphicsitem_cast<NewEdge *>(item))
            scene->removeItem(edge);
        if(state)sleep(sleepTime);
    }
}
void MyAVLTree::drawTree()
{
    EmptyNodeAndEdge(0);
    qreal x = 20;
    int depth = 0;
    foreach(AVLTree t,trees)
    {
        int t_d = GetDepth(t);
        depth = depth<t_d?t_d:depth;
        NewNode *n = new NewNode(this, t);
        paintTree(n, x, 20);
    }
    scene->setSceneRect(0, 0, x-20, 40*depth);
    adjustContronller();
}

void MyAVLTree::paintTree(NewNode *&root, qreal &centerX, qreal centerY)
{
    if(root->node == nullptr) return;
    if(root->node->lchild != nullptr)
    {
        NewNode *lnode = new NewNode(this, root->node->lchild);
        scene->addItem(new NewEdge(root,lnode));
        paintTree(lnode, centerX, centerY+40);
    }
    scene->addItem(root);
    root->setPos(centerX,centerY);
    centerX += 40;
    if(root->node->rchild != nullptr)
    {
        NewNode  *rnode = new NewNode(this, root->node->rchild);
        scene->addItem(new NewEdge(root, rnode));
        paintTree(rnode, centerX, centerY+40);
    }
}

void MyAVLTree::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        keyPressEvent(event);
    }
}

void MyAVLTree::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    return;

    QList<NewNode *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (NewNode *node = qgraphicsitem_cast<NewNode *>(item))
            nodes << node;
    }

    foreach (NewNode *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (NewNode *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

#if QT_CONFIG(wheelevent)
void MyAVLTree::wheelEvent(QWheelEvent *event)
{
    double temp = pow((double)2, -event->delta() / 240.0);
    scaleView(qreal(temp));
}
#endif

void MyAVLTree::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
}

void MyAVLTree::scaleView(qreal scaleFactor)
{
    qreal factor = ui->graphicsView->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    ui->graphicsView->scale(scaleFactor, scaleFactor);
}
void MyAVLTree::shuffle()
{
    foreach (QGraphicsItem *item, scene->items()) {
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
         if (qgraphicsitem_cast<NewNode *>(item))
            item->setPos(-150 + rand()%300, -150 + rand()%300);
    }
}

void MyAVLTree::zoomIn()
{
    scaleView(qreal(1.2));
}

void MyAVLTree::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void MyAVLTree::on_deleteNode_clicked()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int number = 0;
    bool flag = false;
    if(trees.size()>1){
        number = QInputDialog::getInt(nullptr,tr("选择树"),tr("请输入树的编号(从左到右)"),std::rand()%trees.size(),0,trees.size()-1,2,&flag);
        if(!flag){
            return;
        }
    }
    int input = QInputDialog::getInt(nullptr,tr("删除数字"),tr("请输入0-99的数字"),std::rand()%100,0,100,2,&flag);
    if(!flag){
        return;
    }
    Status s;
    if(DeleteAVL(trees.at(number),input,s) == NO){
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("该数字不存在");
        return;
    }
    trace temp;
    temp.value=input;
    temp.insert = false;
    traces.push_back(temp);
    drawTree();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Delete Success");
}

void MyAVLTree::on_searchNode_clicked()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    bool flag = false;
    int input = QInputDialog::getInt(nullptr,tr("搜索数字"),tr("请输入0-99的数字"),std::rand()%100,0,100,2,&flag);
    if(!flag){
        return;
    }
    std::vector<AVLTree> result;
    foreach (AVLTree t, trees) {
        AVLTree s;
        s = SearchAVL(t,input);
        if(nullptr!=s)result.push_back(s);
    }
    if(result.size()==0){
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("该数字不存在");
        return;
    }
    //更新点
    RefleshNode();

    QList<NewNode *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (NewNode *node = qgraphicsitem_cast<NewNode *>(item))
        {
            node->isSearched=std::find(result.begin(),result.end(),node->node)!=result.end();
            node->update();
        }
    }
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Searched Success");
}

void MyAVLTree::on_addNode5_clicked()
{
    qsrand(QTime(0, 0 ,0).secsTo(QTime::currentTime()));
    int input = 0;
    if(trees.empty())
    {
        trees.push_back(nullptr);
    }
    Status s;
    for(int i = 0; i < 5; i++)
    {
        input = std::rand()%100;
        if(InsertAVL(trees.at(0),input, s) == NO)
        {
            i--;
        }
        trace temp;
        temp.value=input;
        temp.insert = true;
        traces.push_back(temp);
        drawTree();
        ui->lineEditState->setPalette(Qt::GlobalColor::green);
        ui->lineEditState->setText("Insert Success");
        sleep(sleepTime);
    }
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Insert5 Success");
}


void MyAVLTree::on_horizontalSlider_valueChanged(int value)
{
    sleepTime=MAX_SLEEP_TIME/(value+1);
}

void MyAVLTree::on_pushButtonclear_clicked()
{
    destorySelf();
    initUI();
    adjustContronller();
}

void MyAVLTree::closeEvent(QCloseEvent *event)
{
    destorySelf();
    initUI();
    adjustContronller();
}

void MyAVLTree::on_PreOrder_clicked()
{
    RefleshNode();
    ui->lineEditState->setPalette(Qt::GlobalColor::white);
    ui->lineEditState->setText("PreOrder");
    PreOrder(trees.at(0));
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("PreOrder success");
}

void MyAVLTree::on_InOrder_clicked()
{
    RefleshNode();
    ui->lineEditState->setPalette(Qt::GlobalColor::white);
    ui->lineEditState->setText("InOrder");
    InOrder(trees.at(0));
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("InOrder success");
}

void MyAVLTree::on_LastOrder_clicked()
{
    RefleshNode();
    ui->lineEditState->setPalette(Qt::GlobalColor::white);
    ui->lineEditState->setText("LastOrder");
    LastOrder(trees.at(0));
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("LastOrder success");
}
void MyAVLTree::RefleshNode()
{
    QList<NewNode *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (NewNode *node = qgraphicsitem_cast<NewNode *>(item))
        {
            node->isSearched = false;
            node->update();
        }
    }
    EmptyNodeAndEdge(0);
    drawTree();
}


void MyAVLTree::on_LevelOrder_clicked()
{
    RefleshNode();
    ui->lineEditState->setPalette(Qt::GlobalColor::white);
    ui->lineEditState->setText("LevelOrder");
    LevelOrder(trees.at(0));
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("LevelOrder success");
}

void MyAVLTree::on_saveButton_clicked()
{
    QFile writeFile("F:\\Qt\\Documents\\Data_2_0\\resource\\myavltree.txt");
    writeFile.open(QIODevice::Text|QIODevice::WriteOnly);
    QTextStream in(&writeFile);
    in<<traces.size()<<endl;
    for(auto i:traces){
        in << i.value << endl << i.insert << endl;
    }
    adjustContronller();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Save Success!");
    //QString data = QString(file.readAll());

    //data.toLatin1().data()

    writeFile.close();
}

void MyAVLTree::on_loadButton_clicked()
{
    destorySelf();
    initMyAVLTree();
    qDebug() << "size"<< traces.size()<<  endl;
    QFile readFile("F:\\Qt\\Documents\\Data_2_0\\resource\\myavltree.txt");
    readFile.open(QIODevice::ReadOnly);
    int count;
    int value;
    bool insert;
    QString temp = QString(readFile.readLine());
    count = temp.toInt();
    qDebug() << count <<  endl;
    for(int i = 0; i < count; i++)
    {
        temp = QString(readFile.readLine());
        value = temp.toInt();
         qDebug() << value ;
        temp = QString(readFile.readLine());
        insert = temp.toInt();
         qDebug() << insert <<  endl;
        if(insert){
            int number = 0;
            int input = value;
            if(trees.empty())
            {
                trees.push_back(nullptr);
            }
            Status s;
            if(InsertAVL(trees.at(number),input, s) == NO)
            {
                ui->lineEditState->setPalette(Qt::GlobalColor::red);
                ui->lineEditState->setText("该数字已经存在");
                return ;
            }
            trace temp;
            temp.value=input;
            temp.insert=true;
            traces.push_back(temp);
        }else{
            int number = 0;
            int input = value;
            Status s;
            if(DeleteAVL(trees.at(number),input,s) == NO){
                ui->lineEditState->setPalette(Qt::GlobalColor::red);
                ui->lineEditState->setText("该数字不存在");
                return;
            }
            trace temp;
            temp.value=input;
            temp.insert = false;
            traces.push_back(temp);
        }
    }
    drawTree();
    adjustContronller();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Load Success!");

    readFile.close();
}
