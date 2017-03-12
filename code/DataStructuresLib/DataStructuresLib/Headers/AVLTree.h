#ifndef _AVLTREE_H_
#define _AVLTREE_H_
#include <stdio.h>
#include "ExitCodeStatus.h"
#include <stdlib.h>
typedef struct _TNOD
{
	int value;
	struct _TNOD *left;
	struct _TNOD *right;
	int height;
} TNOD,*PTNOD;

typedef struct 
{
	PTNOD root;
	int size;
} MY_TREE,*PMY_TREE;

STATUS_CODE TreeInsert(MY_TREE *tree,int value);
STATUS_CODE TreeCreate(MY_TREE *tree);
STATUS_CODE TreeDelete(MY_TREE *tree,int value);
STATUS_CODE TreeFind(MY_TREE tree, int value,BOOLEAN *exist);
STATUS_CODE TreeDestroy(MY_TREE *tree);
//NEED TO TEST ALL TREE MODULE
#endif // !_AVLTREE_H_

