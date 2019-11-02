#ifndef AVLTREE_H
#define AVLTREE_H

#define LH +1
#define EH 0
#define RH -1

enum Status
{
    OK,ERROR,YES,NO
};

typedef struct AVLNode{
    int data;
    int bf;//平衡因子
    struct AVLNode *lchild, *rchild;
}* AVLTree;

void InOrderTraverse(AVLTree T);
void L_rotation(AVLTree &T);
void R_rotation(AVLTree &T);
void L_balance(AVLTree &T);
void R_balance(AVLTree &T);
Status L_D_balance(AVLTree &T);
Status R_D_balance(AVLTree &T);

Status InsertAVL(AVLTree &T,int e, Status &taller);
Status DeleteAVL(AVLTree &T,int e, Status &shorter);

void DestoryAVL(AVLTree &T);
AVLTree SearchAVL(AVLTree T, int key);

Status isBalanced(AVLTree T);

int max(int a,int b);

int GetDepth(AVLTree T);

void MergeAVL(AVLTree &T1, AVLTree T2);

void DivAVL(AVLTree R, AVLTree &T1, AVLTree &T2, int e);

#endif // AVLTREE_H
