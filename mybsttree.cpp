#include "mybsttree.h"
#include "ui_mybsttree.h"
#include "newbedge.h"
#include "newbnode.h"
#include "uidefine.h"

#include <math.h>
#include <QInputDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSpinBox>
#include <QTime>
#include <QDialogButtonBox>
#include <QGraphicsView>
#include <QFormLayout>
#include <QDebug>
#include <QQueue>
#include <iostream>
#include <fstream>
#include <string>
void sleep(int msec);

const QBrush MyBSTTree::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush MyBSTTree::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush MyBSTTree::markBrush=QBrush(Qt::GlobalColor::green);

const QFont MyBSTTree::headLabelFont=QFont("Consolas");
const QFont MyBSTTree::dataFont=QFont("Consolas",8);

const QIntValidator MyBSTTree::dataValidator(-999999999,999999999);

MyBSTTree::MyBSTTree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyBSTTree)
{
    ui->setupUi(this);
    initTextBrowser();
    initUI();

    timerId = 0;
    scene = nullptr;
    sleepTime=MAX_SLEEP_TIME>>1;
    ui->horizontalSlider->setValue(MAX_SLIDER>>1);
    srand(time_t(nullptr));

    this->setWindowTitle("BST树");
}

MyBSTTree::~MyBSTTree()
{
    delete ui;
}
//初始化文本显示区
void MyBSTTree::initTextBrowser(){
    QFile file(":/html/html/MyBSTTree.HTML");
    file.open(QIODevice::ReadOnly);
    QString htmlString=file.readAll();
    ui->textBrowser->setHtml(htmlString);
    file.close();
}

//初始化UI控件
void MyBSTTree::initUI()
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
}
//初始化视图框架
void MyBSTTree::initSceneView()
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
void MyBSTTree::initMyBSTTree()
{
    initSceneView();
}

void MyBSTTree::destorySelf()
{
    if(trees.size())
    {
        trees.clear();
        EmptyNodeAndEdge(1);
    }
    adjustContronller();
}

void MyBSTTree::adjustContronller()
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
        }else
        {
            ui->pushButtonclear->setEnabled(false);
            ui->InOrder->setEnabled(false);
            ui->PreOrder->setEnabled(false);
            ui->LastOrder->setEnabled(false);
            ui->LevelOrder->setEnabled(false);
            ui->deleteNode->setEnabled(false);
            ui->searchNode->setEnabled(false);
        }
    }
}
void MyBSTTree::count(BSTTree &T, int &numbers)
{
    if(nullptr == T) return;
    count(T->lchild,numbers);
    numbers++;
    count(T->rchild,numbers);
}
void MyBSTTree::PreOrder(BSTTree T)
{
    sleep(sleepTime);
    if(nullptr == T) return;
    Setvisited(T);
    PreOrder(T->lchild);
    PreOrder(T->rchild);
}
void MyBSTTree::InOrder(BSTTree T)
{
    sleep(sleepTime);
    if(nullptr == T) return;
    InOrder(T->lchild);
    Setvisited(T);
    InOrder(T->rchild);
}
void MyBSTTree::LastOrder(BSTTree T)
{
    if(nullptr == T) return;
    LastOrder(T->lchild);
    LastOrder(T->rchild);
    Setvisited(T);
    sleep(sleepTime);
}
void MyBSTTree::LevelOrder(BSTTree T)
{
    QQueue<BSTTree> q;
    if(nullptr == T) return;
    q.enqueue(T);
    while(!q.empty())
    {
        BSTTree temp = q.dequeue();
        Setvisited(temp);
        sleep(sleepTime);
        if(temp->lchild)
            q.enqueue(temp->lchild);
        if(temp->rchild)
            q.enqueue(temp->rchild);
     }
}
void MyBSTTree::Setvisited(BSTTree T)
{
    std::vector<BSTTree> result;
    foreach (BSTTree t, trees) {
        BSTTree s;
        s = SearchBST(t,T->data);
        if(nullptr!=s)result.push_back(s);
    }
    //更新点
    QList<NewbNode *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (NewbNode *node = qgraphicsitem_cast<NewbNode *>(item))
        {
            node->isVisited=std::find(result.begin(),result.end(),node->node)!=result.end();
            node->update();
        }
    }
}
void MyBSTTree::on_pushButtonInit_clicked()
{
    destorySelf();
    initMyBSTTree();

    ui->addNode->setEnabled(true);
    ui->deleteNode->setEnabled(true);
    ui->searchNode->setEnabled(true);
    ui->addNode5->setEnabled(true);
    ui->pushButtonclear->setEnabled(true);

    adjustContronller();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Create Success");
}

void MyBSTTree::on_addNode_clicked()
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
    BSTStatus s;
    if(InsertBST(trees.at(number),input, s) == BSTNO)
    {
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("该数字已经存在");
        return ;
    }
    drawTree();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Insert Success");
}

void MyBSTTree::itemMoved()
{
    if(!timerId)
        timerId = startTimer(1000 / 25);
}

void MyBSTTree::EmptyNodeAndEdge(int state)
{
    QList<NewbNode *> nodes;
    foreach(QGraphicsItem *item, scene->items())
    {
        if(NewbNode *node = qgraphicsitem_cast<NewbNode *>(item))
            scene->removeItem(node);
        if(NewbEdge *edge = qgraphicsitem_cast<NewbEdge *>(item))
            scene->removeItem(edge);
        if(state)sleep(sleepTime);
    }
}
void MyBSTTree::drawTree()
{
    EmptyNodeAndEdge(0);
    qreal x = 20;
    int depth = 0;
    foreach(BSTTree t,trees)
    {
        int t_d = GetDepth(t);
        depth = depth<t_d?t_d:depth;
        NewbNode *n = new NewbNode(this, t);
        paintTree(n, x, 20);
    }
    scene->setSceneRect(0, 0, x-20, 40*depth);
    adjustContronller();
}

void MyBSTTree::paintTree(NewbNode *&root, qreal &centerX, qreal centerY)
{
    if(root->node == nullptr) return;
    if(root->node->lchild != nullptr)
    {
        NewbNode *lnode = new NewbNode(this, root->node->lchild);
        scene->addItem(new NewbEdge(root,lnode));
        paintTree(lnode, centerX, centerY+40);
    }
    scene->addItem(root);
    root->setPos(centerX,centerY);
    centerX += 40;
    if(root->node->rchild != nullptr)
    {
        NewbNode  *rnode = new NewbNode(this, root->node->rchild);
        scene->addItem(new NewbEdge(root, rnode));
        paintTree(rnode, centerX, centerY+40);
    }
}

void MyBSTTree::keyPressEvent(QKeyEvent *event)
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

void MyBSTTree::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    return;

    QList<NewbNode *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (NewbNode *node = qgraphicsitem_cast<NewbNode *>(item))
            nodes << node;
    }

    foreach (NewbNode *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (NewbNode *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

#if QT_CONFIG(wheelevent)
void MyBSTTree::wheelEvent(QWheelEvent *event)
{
    double temp = pow((double)2, -event->delta() / 240.0);
    scaleView(qreal(temp));
}
#endif

void MyBSTTree::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
}

void MyBSTTree::scaleView(qreal scaleFactor)
{
    qreal factor = ui->graphicsView->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    ui->graphicsView->scale(scaleFactor, scaleFactor);
}
void MyBSTTree::shuffle()
{
    foreach (QGraphicsItem *item, scene->items()) {
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
         if (qgraphicsitem_cast<NewbNode *>(item))
            item->setPos(-150 + rand()%300, -150 + rand()%300);
    }
}

void MyBSTTree::zoomIn()
{
    scaleView(qreal(1.2));
}

void MyBSTTree::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void MyBSTTree::on_deleteNode_clicked()
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
    BSTStatus s;
    if(DeleteBST(trees.at(number),input,s) == BSTNO){
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("该数字不存在");
        return;
    }
    drawTree();
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Delete Success");
}

void MyBSTTree::on_searchNode_clicked()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    bool flag = false;
    int input = QInputDialog::getInt(nullptr,tr("搜索数字"),tr("请输入0-99的数字"),std::rand()%100,0,100,2,&flag);
    if(!flag){
        return;
    }
    std::vector<BSTTree> result;
    foreach (BSTTree t, trees) {
        BSTTree s;
        s = SearchBST(t,input);
        if(nullptr!=s)result.push_back(s);
    }
    if(result.size()==0){
        ui->lineEditState->setPalette(Qt::GlobalColor::red);
        ui->lineEditState->setText("该数字不存在");
        return;
    }
    //更新点
    RefleshNode();

    QList<NewbNode *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (NewbNode *node = qgraphicsitem_cast<NewbNode *>(item))
        {
            node->isSearched=std::find(result.begin(),result.end(),node->node)!=result.end();
            node->update();
        }
    }
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Searched Success");
}

void MyBSTTree::on_addNode5_clicked()
{
    qsrand(QTime(0, 0 ,0).secsTo(QTime::currentTime()));
    int input = 0;
    if(trees.empty())
    {
        trees.push_back(nullptr);
    }
    BSTStatus s;
    for(int i = 0; i < 5; i++)
    {
        input = std::rand()%100;
        if(InsertBST(trees.at(0),input, s) == BSTNO)
        {
            i--;
        }
        drawTree();
        ui->lineEditState->setPalette(Qt::GlobalColor::green);
        ui->lineEditState->setText("Insert Success");
        sleep(sleepTime);
    }
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("Insert5 Success");
}


void MyBSTTree::on_horizontalSlider_valueChanged(int value)
{
    sleepTime=MAX_SLEEP_TIME/(value+1);
}

void MyBSTTree::on_pushButtonclear_clicked()
{
    destorySelf();
    initUI();
}

void MyBSTTree::closeEvent(QCloseEvent *event)
{
    destorySelf();
    initUI();
    adjustContronller();
}

void MyBSTTree::on_PreOrder_clicked()
{
    RefleshNode();
    ui->lineEditState->setPalette(Qt::GlobalColor::white);
    ui->lineEditState->setText("PreOrder");
    PreOrder(trees.at(0));
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("PreOrder success");
}

void MyBSTTree::on_InOrder_clicked()
{
    RefleshNode();
    ui->lineEditState->setPalette(Qt::GlobalColor::white);
    ui->lineEditState->setText("IBSTNOrder");
    InOrder(trees.at(0));
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("IBSTNOrder success");
}

void MyBSTTree::on_LastOrder_clicked()
{
    RefleshNode();
    ui->lineEditState->setPalette(Qt::GlobalColor::white);
    ui->lineEditState->setText("LastOrder");
    LastOrder(trees.at(0));
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("LastOrder success");
}
void MyBSTTree::RefleshNode()
{
    QList<NewbNode *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (NewbNode *node = qgraphicsitem_cast<NewbNode *>(item))
        {
            node->isSearched = false;
            node->update();
        }
    }
    EmptyNodeAndEdge(0);
    drawTree();
}


void MyBSTTree::on_LevelOrder_clicked()
{
    RefleshNode();
    ui->lineEditState->setPalette(Qt::GlobalColor::white);
    ui->lineEditState->setText("LevelOrder");
    LevelOrder(trees.at(0));
    ui->lineEditState->setPalette(Qt::GlobalColor::green);
    ui->lineEditState->setText("LevelOrder success");
}
