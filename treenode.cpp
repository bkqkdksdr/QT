#include "treenode.h"

template<class T>
Treenode<T>::Treenode()
{
    leftchild = rightchild = nullptr;
}

template<class T>
Treenode<T>::Treenode(const T& ele)
{
    info  = ele;
    leftchild = rightchild = nullptr;
}

template<class T>
Treenode<T>::Treenode(const T& ele, Treenode<T> *l, Treenode<T> *r)
{
    info = ele;
    leftchild = l;
    rightchild = r;
}

template<class T>
T Treenode<T>::Value() const
{
    return info;
}

template<class T>
Treenode<T> * Treenode<T>::LeftChild() const
{
    return leftchild;
}

template<class T>
Treenode<T> * Treenode<T>::RightChild() const
{
    return rightchild;
}

template<class T>
void Treenode<T>::SetLeftChild(Treenode<T> *l)
{
    leftchild = l;
}

template<class T>
void Treenode<T>::SetRightChild(Treenode<T> *r)
{
    rightchild = r;
}

template<class T>
void Treenode<T>::SetValue(const T& val)
{
    info = val;
}

template<class T>
bool Treenode<T>::IsLeaf()
{
    if(leftchild == nullptr && rightchild == nullptr)return true;
    else return false;
}
