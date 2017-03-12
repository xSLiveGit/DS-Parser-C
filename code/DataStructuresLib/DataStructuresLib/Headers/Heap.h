#ifndef _HEAP_H_
#define _HEAP_H_
#include "ExitCodeStatus.h"
#include "DynamicVector.h"
#include <stdlib.h>
typedef struct {
	PDYNAMIC_VECTOR vector;
	int size;
} MY_HEAP, *PMY_HEAP;

int MyHeapCreate(PMY_HEAP* heap);

int MyHeapDestroy(PMY_HEAP *heap);

STATUS_CODE MyHeapAdd(MY_HEAP *heap,int value);

STATUS_CODE MyHeapDelete(MY_HEAP *heap,int* returned);

#endif // !_HEAP_H_

