#ifndef BSTTREE_H
#define BSTTREE_H

#define LH +1
#define EH 0
#define RH -1

enum BSTStatus
{
    BSTOK,BSTERROR,BSTYES,BSTNO
};

typedef struct BSTNode{
    int data;
    int bf;//平衡因子
    struct BSTNode *lchild, *rchild;
}* BSTTree;

void InOrderTraverse(BSTTree T);
void L_rotation(BSTTree &T);
void R_rotation(BSTTree &T);
void L_balance(BSTTree &T);
void R_balance(BSTTree &T);

BSTStatus L_D_balance(BSTTree &T);
BSTStatus R_D_balance(BSTTree &T);

BSTStatus InsertBST(BSTTree &T,int e, BSTStatus &taller);
BSTStatus DeleteBST(BSTTree &T,int e, BSTStatus &shorter);

void DestoryBST(BSTTree &T);
BSTTree SearchBST(BSTTree T, int key);

BSTStatus isBalanced(BSTTree T);

int max(int a,int b);

int GetDepth(BSTTree T);

void MergeBST(BSTTree &T1, BSTTree T2);

void DivBST(BSTTree R, BSTTree &T1, BSTTree &T2, int e);


#endif // BSTTREE_H
