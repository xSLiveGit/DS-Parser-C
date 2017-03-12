#ifndef _LIST_H_
#define _LIST_H_
#include <stdlib.h>

#include <stdio.h>
#include "ExitCodeStatus.h"
typedef struct _NOD{
	struct _NOD* prev;
	struct _NOD* next;
	int value;
}NOD,*PNOD;

typedef struct {
	PNOD first;
	PNOD last;
	int size;
} LIST,*PLIST;
BOOLEAN IsEmptyList(LIST);
int CreateNode(PNOD* nod,int);

int CreateList(LIST *);

int ListAdd(LIST*, int el);

int ListMerge(LIST*, LIST*, LIST *);

int ListPrint(LIST, FILE *fileDescriptor);

int ListMergeSort(LIST *);

int GetMiddle(PNOD head,PNOD* result);

int ListDestroy(PLIST list);

int ListFind(PLIST list,int value, BOOLEAN *exist);//ne to test
#endif //! _LIST_H_










