#ifndef _INTERFACE_H_
#define _INTERFACE_H_
#include "..\Controller\VectorController\VectorController.h"
#include "../Interface\Definitions.h"
#include "../StringHelper/StringHelper.h"
#include "../Controller/ListController/ListController.h"
#include "../Controller/HashTableController/HashTableController.h"
#include "../Controller/HeapController/HeapController.h"
#include "../Controller/TreeController/TreeController.h"
#define INVALID_POINTER -1
#define INVALID_PPOINTER -2
typedef struct
{
	PVECTOR_CONTROLLER vectorController;
	PLIST_CONTROLLER listController;
	PHASH_TABLE_CONTROLLER hashtableController;
	PHEAP_CONTROLLER heapController;
	PTREE_CONTROLLER treeController;
	PPARSER parser;
	FILE* outputFile;
}INTERFACE, *PINTERFACE;

STATUS_CODE InterfaceCreate(PINTERFACE * interface, char* file);
STATUS_CODE InterfaceDestroy(PINTERFACE *interface);
STATUS_CODE InterfaceRunTest(int nr);
STATUS_CODE InterfaceRunMultipleTests(char* startTest, char* endTest);
STATUS_CODE InterfaceRunAllTests();
#endif // !_INTERFACE_H_