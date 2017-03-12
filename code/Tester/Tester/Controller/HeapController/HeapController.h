#ifndef _HEAP_CONTROLLER_H_
#define _HEAP_CONTROLLER_H_

#include "../ControllerStatus.h"
#include "../../Parser/Parser.h"
#include "stdlib.h"
#include "../HelperController/HelperController.h"
#include "Heap.h"

typedef struct
{
	PMY_HEAP heaps[MAX_STRUCTS];
	int currentHeap;
	int nrOfHeaps;
} HEAP_CONTROLLER, *PHEAP_CONTROLLER;

STATUS_CODE HeapControllerCreate(PHEAP_CONTROLLER *heapController);
STATUS_CODE HeapControllerDestroy(PHEAP_CONTROLLER *heapController);
STATUS_CODE HeapControllerGoTo(PHEAP_CONTROLLER heapController, PPARSER parser);
STATUS_CODE HeapControllerRead(PHEAP_CONTROLLER heapController, PPARSER parser);
STATUS_CODE HeapControllerInsert(PHEAP_CONTROLLER heapController, PPARSER parser);
STATUS_CODE HeapControllerRemove(PHEAP_CONTROLLER heapController, PPARSER parser,FILE* out);



#endif //!_HEAP_CONTROLLER_H_
