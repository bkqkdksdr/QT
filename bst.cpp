#include "bst.h"
#include "ui_bst.h"

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
