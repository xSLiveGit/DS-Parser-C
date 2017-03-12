#ifndef _HASH_TABLE_H_ 
#define _HASH_TABLE_H_
#define NUMBER_OF_LISTS 6011 // prime number
#include "List.h"
#include "ExitCodeStatus.h"

typedef struct {
	LIST independentList[NUMBER_OF_LISTS];
	int (*hash_function)(int);
} HASH_TABLE,*PHASH_TABLE;

int HashTableCreate(HASH_TABLE *hashTable, int(*hashF)(int));

int HashTableAdd(HASH_TABLE *hash, int value);

int HashTableFind(HASH_TABLE *hash, int value, BOOLEAN *exist);

int HashTableDestroy(HASH_TABLE *hash);

#endif // !_HASH_TABLE_H_ 
