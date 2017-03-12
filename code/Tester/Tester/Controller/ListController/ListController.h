#ifndef _LIST_CONTROLLER_H_
#define _LIST_CONTROLLER_H_
#include "../ControllerStatus.h"
#include "List.h"
#include "../../Parser/Parser.h"
#include "stdlib.h"
#include "../HelperController/HelperController.h"
typedef struct
{
	LIST lists[MAX_STRUCTS];
	int currentList;
	int nrOfLists;
} LIST_CONTROLLER,*PLIST_CONTROLLER;

STATUS_CODE ListControllerCreate(PLIST_CONTROLLER *listController);
STATUS_CODE ListControllerRead(PLIST_CONTROLLER listController, PPARSER parser);
STATUS_CODE ListControllerDestroy(PLIST_CONTROLLER *listController);
STATUS_CODE ListControllerGoTo(PLIST_CONTROLLER listController, PPARSER parser);
STATUS_CODE ListControllerPrint(PLIST_CONTROLLER vectorController, PPARSER parser, FILE* out);
STATUS_CODE ListControllerAdd(PLIST_CONTROLLER listController, PPARSER parser);
STATUS_CODE ListControllerMerge(PLIST_CONTROLLER listController);
STATUS_CODE ListControllerMergeSort(PLIST_CONTROLLER listController);
#endif //!_LIST_CONTROLLER_H_
