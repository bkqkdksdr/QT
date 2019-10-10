#include "tree.h"
template<class T>
void visit(const T& value);
//really
template<class T>
Tree<T>::Tree()
{
    root = nullptr;
}

template<class T>
Tree<T>::~Tree()
{
    DeleteTree();
}

template<class T>
bool Tree<T>::isempty() const
{
    return (root != nullptr ? false : true);
}

template<class T>
Treenode<T>* Tree<T>::Root()
{
    return root;
}

template<class T>
void Tree<T>::create()
{
    PerCreateTree(root);
}

template<class T>
void Tree<T>::PerCreateTree(Treenode<T>* &root)
{
    T item;
    cin >> item;
    if(item == "#")
            root = nullptr;
    else
    {
        root = new Treenode<T>();
        root->setValue(item);
        PerCreateTree(root->leftchild);
        PerCreateTree(root->rightchild);
    }
}

template<class T>
void Tree<T>::PerOrder()
{
    if (root != NULL)
    {
        visit(root->value());
        PreOrder(root->LeftChild());
        PreOrder(root->RightChild());
    }
}

template<class T>
void Tree<T>::InOrder()
{
    if (root != NULL)
    {
        InOrder(root->LeftChild());
        visit(root->value());
        InOrder(root->RightChild());
    }
}

template<class T>
void Tree<T>::PostOrder()
{
    if (root != NULL)
    {
        PostOrder(root->LeftChild());
        PostOrder(root->RightChild());
        visit(root->value());
    }
}

template<class T>
void Tree<T>::LevelOrder(){
    queue<Treenode<T> *> aque;
    Treenode<T> * pointer = root;
    if(pointer)
        aque.push(pointer);
    while(!aque.empty()){
        pointer =aque.front();
        aque.pop();
        visit(pointer->value());
        if(pointer->LeftChild() != NULL)
            aque.push(pointer->LeftChild());
        if(pointer->RightChild() != NULL)
            aque.push(pointer->RightChild());
    }
}

template<class T>
void Tree<T>::DeleteTree(){
    if(root->leftchild != NULL){
    DeleteBinaryTree(root->leftchild);
    }
    if (root->rightchild != NULL)
    {
        DeleteBinaryTree(root->rightchild);
    }
    delete root;
    root = NULL;
}
