#ifndef _HASH_TABLE_CONTROLLER_H_
#define _HASH_TABLE_CONTROLLER_H_

#include "../ControllerStatus.h"
#include <HashTable.h>
#include "../../Parser/Parser.h"
#include "stdlib.h"
#include "../HelperController/HelperController.h"

typedef struct
{
	HASH_TABLE hashTable[MAX_STRUCTS];
	int currentHashTable;
	int nrOfHashTable;
} HASH_TABLE_CONTROLLER, *PHASH_TABLE_CONTROLLER;



STATUS_CODE HashTableControllerCreate(PHASH_TABLE_CONTROLLER *hashTableController);
STATUS_CODE HashTableControllerRead(PHASH_TABLE_CONTROLLER hashTableController, PPARSER parser);
STATUS_CODE HashTableControllerSearch(PHASH_TABLE_CONTROLLER hashTableController, PPARSER parser, FILE* out);
STATUS_CODE HashTableControllerGoTo(PHASH_TABLE_CONTROLLER hashTableController, PPARSER parser);
STATUS_CODE HashTableControllerDestroy(PHASH_TABLE_CONTROLLER *hashTableController);

#endif //!_HASH_TABLE_CONTROLLER_H_