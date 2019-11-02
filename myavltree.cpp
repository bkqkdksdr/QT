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
#include <QDialogButtonBox>
#include <QGraphicsView>
#include <QFormLayout>

const QBrush MyAVLTree::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush MyAVLTree::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush MyAVLTree::markBrush=QBrush(Qt::GlobalColor::green);

const QFont MyAVLTree::headLabelFont=QFont("Consolas");
const QFont MyAVLTree::dataFont=QFont("Consolas",8);

const QIntValidator MyAVLTree::dataValidator(-999999999,999999999);

MyAVLTree::MyAVLTree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyAVLTree),
    timerId(0)
{
    ui->setupUi(this);
    initTextBrowser();
    initUI();

    scene = nullptr;
    ui->horizontalSlider->setValue(MAX_SLIDER>>1);
    srand(time_t(nullptr));

    this->setWindowTitle("avl");
}

MyAVLTree::~MyAVLTree()
{
    delete ui;
}
//初始化文本显示区
void MyAVLTree::initTextBrowser(){
    QFile file(":/html/html/LinkList.html");
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

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(MAX_SLIDER);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBelow);
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
}

void MyAVLTree::destorySelf()
{
    //widget->destoryTree();
}
void MyAVLTree::on_pushButtonInit_clicked()
{
    initMyAVLTree();
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
        QMessageBox::about(nullptr,tr("提醒"),tr("该数字已经存在"));
        return ;
    }
    drawTree();
}

void MyAVLTree::itemMoved()
{
    if(!timerId)
        timerId = startTimer(1000 / 25);
}

void MyAVLTree::EmptyNodeAndEdge()
{
    QList<NewNode *> nodes;
    foreach(QGraphicsItem *item, scene->items())
    {
        if(NewNode *node = qgraphicsitem_cast<NewNode *>(item))
            scene->removeItem(node);
        if(NewEdge *edge = qgraphicsitem_cast<NewEdge *>(item))
            scene->removeItem(edge);
    }
}
void MyAVLTree::drawTree()
{
    EmptyNodeAndEdge();
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
            //item->setPos(-150 + QRandomGenerator::global()->bounded(300), -150 + QRandomGenerator::global()->bounded(300));
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
