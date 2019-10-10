#ifndef TREE_H
#define TREE_H
#include "iostream"
#include "queue"
#include "stack"
#include "vector"
#include "algorithm"
#include "treenode.h"
using namespace std;

template<class T>
class Tree
{
private:
    Treenode<T> *root;
public:
    Tree();
    ~Tree();
    bool isempty() const;
    Treenode<T>* Root();
    void create();
    void PerCreateTree(Treenode<T>* &root);
    void PerOrder();
    void InOrder();
    void PostOrder();
    void LevelOrder();
    void DeleteTree();
};

#endif // TREE_H
