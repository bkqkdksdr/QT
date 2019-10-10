#ifndef BST_H
#define BST_H

#include <QWidget>

namespace Ui {
class BST;
}

class BST : public QWidget
{
    Q_OBJECT

public:
    explicit BST(QWidget *parent = nullptr);
    ~BST();

private:
    Ui::BST *ui;
};

#endif // BST_H
