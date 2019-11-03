#include "avltree.h"

#include <cstdio>
#include <cstdlib>
//中序遍历
void InOrderTraverse(AVLTree T)
{
    if(nullptr == T) return;
    InOrderTraverse(T->lchild);
    printf("%d",T->data);
    InOrderTraverse(T->rchild);
}

//左旋转
void L_rotation(AVLTree &T)
{
    AVLTree t;
    t = T->rchild;
    T->rchild = t->lchild;
    t->lchild = T;
    T = t;
}
void R_rotation(AVLTree &T)
{
    AVLTree t;
    t = T ->lchild;
    T->lchild = t->rchild;
    t->rchild = T;
    T = t;
}
/*插入之后树T 失衡时的平衡函数
 * 这里对三个节点定义
 * ROOT为原来的树T
 * RL树T的左孩子
 * RLR树T的左孩子的右孩子*/

void L_balance(AVLTree &T)
{
    AVLTree lc,rc;
    lc = T->lchild;
    //通过判断RL失衡的情况看是LL还是LR
    switch (lc->bf) {
        //平衡因子不为0
        //这种情况下平衡因子改变的节点只有ROOT和RL
        case LH:
        T->bf = lc->bf = EH;
        break;
        case RH://右子树高说明插入的节点的RLR上
        rc = lc->rchild;
        //判断RLR的平衡因子
        //这种情况下平衡因子发生改变的节点为上述定义的三个节点
        switch (rc->bf) {
            //RLR的左子树高，旋转之后该左子树接到了RL的右子树上
            //由于RL一定不存在右子树，且左子树高度一定为1，所以RL的平衡因子为1
            //ROOT右旋变成RLR的右子树，因此失去了左子树，并且由于RLR没有右子树，因此ROOT变成右高
            case LH:
            T->bf = RH;
            lc->bf = EH ;
            break;
            case EH:
            T->bf = lc->bf = EH;
            break;
            case RH:
            T->bf = EH;
            lc->bf = LH;
            break;
        }
        rc->bf = EH;//调整后RLR为新根，左右等高
        //先左旋RL，转换为LL，再右旋
        L_rotation(T->lchild);
        break;
    }
    R_rotation(T);
}
void R_balance(AVLTree &T)
{
    AVLTree lc,rc;
    rc = T->rchild;
    switch (rc->bf) {
        case RH:
        T->bf = rc->bf = EH;
        break;
        case LH:
        lc = rc->lchild;
        switch (lc->bf) {
            case RH:
            T->bf = LH;
            rc->bf = EH;
            break;
            case EH:
            T->bf = rc ->bf = EH;
            break;
            case LH:
            T->bf = EH;
            rc->bf = RH;
            break;
        }
        lc->bf = EH;
        R_rotation(T->rchild);
        break;
    }
    L_rotation(T);
}
Status L_D_balance(AVLTree &T)
{
    AVLTree lc,rc;
    lc = T->lchild;
    Status result;
    switch (lc->bf) {
        //当RL的平衡因子不为等高时，其旋转之后的新树的高度会降低
        //需要通知其祖先节点继续进行失衡调整
        //这里平衡因子变化的情况和插入的失衡函数一样
        case LH://LL
        T->bf = lc->bf = EH;
        result = YES;
        break;
        case RH://LR
        rc = lc->rchild;
        switch (rc->bf) {
            case LH:
            T->bf = RH;
            lc->bf = EH;
            break;
            case EH:
            T->bf = lc->bf = EH;
            break;
            case RH:
            T->bf = EH;
            lc->bf = LH;
            break;
        }
        rc->bf = EH;
        L_rotation(T->lchild);
        result = YES;
        break;
        //RL可能出现该树的平衡因子为EH的情况
        //这种时候将其当成LL的情况处理，将树T进行右旋操作
        //优选之后新树的高度没有改变
        case EH:
        T->bf = LH;
        lc->bf = RH;
        result = NO;
        break;
    }
    R_rotation(T);
    return result;
}
Status R_D_balance(AVLTree &T)
{
    AVLTree lc,rc;
    rc = T->rchild;
    Status result;
    switch (rc->bf) {
        case RH:
        T->bf = rc->bf = EH;
        result = YES;
        break;
        case LH:
        lc = rc->rchild;
        switch (lc->bf) {
            case RH:
            T->bf = LH;
            rc->bf = EH;
            break;
            case EH:
            T->bf = rc->bf = EH;
            break;
            case LH:
            T->bf = EH;
            rc->bf = RH;
            break;
        }
        lc->bf = EH;
        R_rotation(T->rchild);
        result = YES;
        break;
        case EH:
        T->bf = RH;
        rc->bf = LH;
        result = NO;
        break;
    }
    L_rotation(T);
    return result;
}

Status InsertAVL(AVLTree &T,int e, Status &taller)
{
    //如果为空则以该节点为根创建一棵新的树
    if(nullptr==T)
    {
        T=(AVLTree)malloc(sizeof(AVLNode));
        if(nullptr==T)return NO;
        T->data=e;
        T->bf=EH;
        taller=YES;
        T->lchild=T->rchild=nullptr;
    }
    //如果该节点已经存在，那么就直接返回
    else if (e==T->data)
    {
        taller = NO;
        return NO;
    }
    else if(e<T->data)
    {
        if(NO==InsertAVL(T->lchild,e,taller))return NO;
        if(YES==taller)//树变高了
        {
            switch(T->bf)
            {
                //本来左子树就高的话那么就失衡了
                case LH:L_balance(T);taller=NO;break;
                case RH:T->bf=EH;taller=NO;break;
                case EH:T->bf=LH;taller=YES;break;
            }
        }
    }
    else
    {
        if(NO==InsertAVL(T->rchild,e,taller))return NO;
        if(YES==taller)
        {
            switch(T->bf)
            {
                case RH:R_balance(T);taller=NO;break;
                case LH:T->bf=EH;taller=NO;break;
                case EH:T->bf=RH;taller=YES;break;
            }
        }
    }
    return YES;
}
Status DeleteAVL(AVLTree &T,int e, Status &shorter)
{
    //找不到则返回
    if(nullptr==T)
    {
        return NO;
    }
    else if (e<T->data)
    {
        if (NO==DeleteAVL(T->lchild,e,shorter))return NO;
        if(YES==shorter)
        {
            //判断祖先节点的高度
            switch(T->bf)
            {
                //如果本来左子树高的话，删除节点之后就应该变矮了
                case LH:T->bf=EH;shorter=YES;break;
                case EH:T->bf=RH;shorter=NO;break;
                case RH:shorter=R_D_balance(T);break;
            }
        }
    }
    else if(e>T->data)
    {
        if (NO==DeleteAVL(T->rchild,e,shorter))return NO;
        if(YES==shorter)
        {
            switch(T->bf)
            {
                case RH:T->bf=EH;shorter=YES;break;
                case EH:T->bf=LH;shorter=NO;break;
                case LH:shorter=L_D_balance(T);break;
            }
        }
    }
    else //找到要删除的节点
    {
        AVLTree p;
        p = T;
        //若左右子树都存在,那么寻找左子树的中序后继
        if(nullptr!=T->lchild && nullptr != T->rchild)
        {
                p = T->lchild;
                while (p->rchild != nullptr) {
                    p = p->rchild;
                }
                //并将后继的值赋给该节点
                T->data = p->data;
                //接下来通过遍历该节点的左子树，找到后继删除
                DeleteAVL(T->lchild,p->data,shorter);
                if (shorter==YES)
                {
                    switch(T->bf)
                    {
                        case LH:T->bf=EH;shorter=YES;break;
                        case EH:T->bf=RH;shorter=NO;break;
                        case RH:shorter=R_D_balance(T);break;
                    }
                }
        }
        else //选择左子树或者右子树接上
        {
            T = (nullptr==T->lchild)?T->rchild:T->lchild;
            free(p);
            shorter = YES;
        }
    }
    return YES;
}

void DestoryAVL(AVLTree &T)
{
    if(nullptr==T)return;
    if(nullptr!=T->lchild)DestoryAVL(T->lchild);
    if(nullptr!=T->rchild)DestoryAVL(T->rchild);
    free(T);
    T=nullptr;
}
AVLTree SearchAVL(AVLTree T, int key)
{
    if (nullptr==T)return nullptr;
    if (T->data == key) return T;
    if (T->data > key) return SearchAVL(T->lchild,key);
    else return SearchAVL(T->rchild,key);
}
//返回两个数之间的最大值


int GetDepth(AVLTree T)
{
    if (nullptr == T)return 0;
    return max(GetDepth(T->lchild),GetDepth(T->rchild))+1;
}
int max(int a, int b)
{
   if(a>b) return a;
   else return b;
}
Status isBalanced(AVLTree T)
{
    if(nullptr == T) return YES;
    if((T->lchild!= nullptr && T->data<T->lchild->data)||(T->rchild!= nullptr && T->data>T->rchild->data))return NO;
    if(NO == isBalanced(T->lchild)) return NO;
    if(NO == isBalanced(T->rchild)) return NO;
    int depthLeft = GetDepth(T->lchild);
    int depthRight = GetDepth(T->rchild);
    if (abs(depthLeft - depthRight) > 1)
        return NO;
    return YES;
}



void MergeAVL(AVLTree &T1, AVLTree T2)
{
    Status s;
    if(nullptr == T2)return;
    if (nullptr != T2->lchild) MergeAVL(T1,T2->lchild);
    InsertAVL(T1,T2->data,s);
    if (nullptr != T2->rchild) MergeAVL(T1,T2->rchild);
    return;
}

void DivAVL(AVLTree R, AVLTree &T1, AVLTree &T2, int e)
{
    if(nullptr==R)return;
    DivAVL(R->lchild,T1,T2,e);
    Status s;
    if(R->data>e)InsertAVL(T2,R->data,s);
    else InsertAVL(T1,R->data,s);
    DivAVL(R->rchild,T1,T2,e);
}
