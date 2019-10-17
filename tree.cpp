#include "tree.h"
#include "treenode.h"
template<class T>
void visit(const T& Value);
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
bool Tree<T>::IsEmpty() const
{
    return (root != nullptr ? false : true);
}

template<class T>
Treenode<T>* Tree<T>::Root()
{
    return root;
}

template<class T>
void Tree<T>::Create()
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
        PerCreateTree(root->LeftChild);
        PerCreateTree(root->RightChild);
    }
}

template<class T>
void Tree<T>::PerOrder()
{
    if (root != NULL)
    {
        visit(root->Value());
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
        visit(root->Value());
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
        visit(root->Value());
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
        visit(pointer->Value());
        if(pointer->LeftChild() != NULL)
            aque.push(pointer->LeftChild());
        if(pointer->RightChild() != NULL)
            aque.push(pointer->RightChild());
    }
}

template<class T>
void Tree<T>::DeleteTree(){
    if(root->LeftChild() != NULL){
    DeleteBinaryTree(root->LeftChild());
    }
    if (root->RightChild() != NULL)
    {
        DeleteBinaryTree(root->RightChild());
    }
    delete root;
    root = NULL;
}

template<class T>
Treenode<T>* Tree<T>::Find(T ele)
{
    Treenode<T> *bst = root;
    while(bst)
    {
        if(ele > bst->Value())
        {
            //set 查找
            //sleep
            bst = bst->LeftChild();
        }else if(ele < bst->Value())
        {
            //set 查找
            //sleep
            bst = bst->RightChild();
        }else
        {
            //set 访问成功
            return bst;
            break;
        }
    }
    return nullptr;
}

template<class T>
Treenode<T>* Tree<T>::FindMax()
{
    Treenode<T> *bst = root;
    if(bst)
    {
        while(bst->LeftChild())
        {
            //set 查找
            //sleep
            bst = bst->LeftChild();
        }
    }
    //set 显示
    return bst;
}

template<class T>
Treenode<T>* Tree<T>::FindMin()
{
    Treenode<T> *bst = root;
    if(bst)
    {
        while(bst->RightChild())
        {
            //set 查找
            //sleep
            bst = bst->RightChild();
        }
    }
    //set 显示
    return bst;
}

template<class T>
Treenode<T>* Tree<T>::Insert(T ele, Treenode<T> *bst)
{
    if(bst)
    {
        bst = new Treenode<T>(ele);
        //设置创建
        //sleep
    }else if(ele > bst->Value())
    {
        //set bst->right显示
        //sleep
        bst->SetRightChild(Insert(ele,bst->RightChild));
    }else if(ele < bst->Value())
    {
        //set bst->left显示
        //sleep
        bst->SetLeftChild(Insert(ele,bst->LeftChild));
    }
    return bst;
}

template<class T>
Treenode<T>* Tree<T>::Delete(T ele, Treenode<T> *bst)
{
    Treenode<T> *tp;
    if(!bst)
    {
        //set 查找失败
    }else if(ele < bst->Value())
    {
        //set 显示
        //sleep
        bst->SetLeftChild(Delete(ele, bst->LeftChild));
    }else if(ele > bst->Value())
    {
        //set 显示
        //sleep
        bst->SetRightChild(Delete(ele,bst->RightChild));
    }else
    {
        //左右子都存在
        if(bst->LeftChild() && bst->RightChild())
        {
            tp=FindMin(bst->RightChild());
            //set 显示
            //sleep
            bst->setValue(tp->Value());
            bst->SetRightChild(Delete(bst->Value(),bst->SetRightChild()));
        }else
        {
            tp=bst;
            if(!bst->LeftChild())
            {
                //set 显示
                //sleep
                bst=bst->RightChild();
            }else if(!bst->RightChild())
            {
                //set 显示
                //sleep
                bst=bst->LeftChild();
            }
            delete tp;
        }
    }
    return bst;
}
