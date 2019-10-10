#ifndef TREENODE_H
#define TREENODE_H
#include "queue"
#include "stack"
#include "vector"
#include "algorithm"
//#include "tree.h"
using namespace std;

template<class T>
class Treenode
{
public:
    T info;
    Treenode *leftchild, *rightchild;
public:
    Treenode();
    Treenode(const T& ele);
    Treenode(const T& ele, Treenode<T> *l, Treenode<T> *r);
    T value() const;
    Treenode<T> * LeftChild() const;
    Treenode<T> * RightChild() const;
    void setLeftChild(Treenode<T> *l);
    void setRightChild(Treenode<T> *r);
    void setValue(const T& val);
    bool isleaf();
};

#endif // TREENODE_H
