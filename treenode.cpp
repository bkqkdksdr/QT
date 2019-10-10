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
T Treenode<T>::value() const
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
void Treenode<T>::setLeftChild(Treenode<T> *l)
{
    leftchild = l;
}

template<class T>
void Treenode<T>::setRightChild(Treenode<T> *r)
{
    rightchild = r;
}

template<class T>
void Treenode<T>::setValue(const T& val)
{
    info = val;
}

template<class T>
bool Treenode<T>::isleaf()
{
    if(leftchild == nullptr && rightchild == nullptr)return true;
    else return false;
}
