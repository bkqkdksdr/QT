#include <QDebug>
#include <math.h>
#include "bst.h"
#include "ui_bst.h"

void sleep(unsigned int msec);

const QBrush BST::normalBursh=QBrush(Qt::GlobalColor::darkGray);
const QBrush BST::visitedBrush=QBrush(Qt::GlobalColor::yellow);
const QBrush BST::markBrush=QBrush(Qt::GlobalColor::green);
const QBrush BST::deleteBrush=QBrush(Qt::GlobalColor::red);

const QFont BST::dataFont=QFont("Consolas",8);

const QIntValidator BST::dataValidator(-999999999,999999999);

BST::BST(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BST)
{
    ui->setupUi(this);
}

BST::~BST()
{
    delete ui;
}
