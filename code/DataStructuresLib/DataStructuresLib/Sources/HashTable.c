#include "../Headers/HashTable.h"

int HashTableCreate(HASH_TABLE *hashTable,int (*hashF)(int))
{
	int i;
	int statusCode;

	statusCode = SUCCES;
	for (i = 0; i < NUMBER_OF_LISTS; i++)
	{
		statusCode = CreateList(&hashTable->independentList[i]);
	}
	hashTable->hash_function = hashF;
	return statusCode;
}

int mfc(int a)
{
	return a%NUMBER_OF_LISTS;
}

int HashTableAdd(HASH_TABLE * hash, int value)
{
	int statusCode;
	int listNumber;

	listNumber = -1;
	statusCode = SUCCES;

	if (NULL == hash)
	{
		statusCode = INVALID_HASH_TABLE_POINTER;
		goto CleanUp;
	}
	
	hash->hash_function = mfc;
	listNumber = hash->hash_function(value);
	
	statusCode = ListAdd(&(hash->independentList[listNumber]), value);
CleanUp:
	return statusCode;;
}

int HashTableFind(HASH_TABLE * hash, int value, BOOLEAN * exist)
{
	int statusCode;
	int listNumber;

	statusCode = SUCCES;
	listNumber = -1;

	if (NULL == hash)
	{
		statusCode = INVALID_HASH_TABLE_POINTER;
		goto CleanUp;
	}

	if (NULL == exist)
	{
		statusCode = INVALID_RESULT_PARAMETER_POINTER;
		goto CleanUp;
	}
	hash->hash_function = mfc;
	listNumber = hash->hash_function(value);
	statusCode = ListFind(&(hash->independentList[listNumber]), value, exist);

CleanUp:
	return statusCode;
}

int HashTableDestroy(HASH_TABLE * hash)
{
	int i;
	int statusCode = SUCCES;

	for (i = 0; i < NUMBER_OF_LISTS; i++)
	{
		if(NULL != hash->independentList[i].first && NULL != hash->independentList[i].last)
		{
			statusCode = statusCode | ListDestroy(&(hash->independentList[i]));//0 - succes, !=0 - fail 0 | 1 = fail
		}
	}
	return statusCode;
}



