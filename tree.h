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
    bool IsEmpty() const;
    Treenode<T>* Root();
    void Create();
    void PerCreateTree(Treenode<T>* &root);
    void PerOrder();
    void InOrder();
    void PostOrder();
    void LevelOrder();
    void DeleteTree();
    Treenode<T>* Find(T ele);
    Treenode<T>* FindMax();
    Treenode<T>* FindMin();
    Treenode<T>* Insert(T ele, Treenode<T> *bst);
    Treenode<T>* Delete(T ele, Treenode<T> *bst);
};

#endif // TREE_H
