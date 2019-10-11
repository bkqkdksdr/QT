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
    T Value() const;
    Treenode<T> * LeftChild() const;
    Treenode<T> * RightChild() const;
    void SetLeftChild(Treenode<T> *l);
    void SetRightChild(Treenode<T> *r);
    void SetValue(const T& val);
    bool IsLeaf();
};

#endif // TREENODE_H
