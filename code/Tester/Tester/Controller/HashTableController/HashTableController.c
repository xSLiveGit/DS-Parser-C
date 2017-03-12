#include "HashTableController.h"

static int HashFunction(int key)
{
	return (key % NUMBER_OF_LISTS);
}

void initHashTableController(PHASH_TABLE_CONTROLLER hashTableController)
{
//	int i;
//	for (i = 0; i < MAX_STRUCTS; i++)
//	{
//		
//		HashTableCreate(&hashTableController->hashTable[i],HashFunction);
//	}
	hashTableController->currentHashTable = -1;
	hashTableController->nrOfHashTable = 0;
}
 
STATUS_CODE HashTableControllerCreate(PHASH_TABLE_CONTROLLER *hashTableController)
{
	STATUS_CODE status;

	status = SUCCES;

	if (NULL == hashTableController)
	{
		status = INVALID_PCONTROLLER_POINTER;
		goto CleanUp;
	}

	(*hashTableController) = (PHASH_TABLE_CONTROLLER)malloc(sizeof(HASH_TABLE_CONTROLLER));
	if (NULL == (*hashTableController))
	{
		status = MALLOC_ERROR;
		goto CleanUp;
	}

	initHashTableController(*hashTableController);
	(*hashTableController)->currentHashTable = -1;
	(*hashTableController)->nrOfHashTable = 0;
CleanUp:
	return status;
}

STATUS_CODE HashTableControllerRead(PHASH_TABLE_CONTROLLER hashTableController, PPARSER parser)
{
	STATUS_CODE status;
	HASH_TABLE hashTable;
	int hashSize;
	int nr;
	int oldCurrentHashTable;


	oldCurrentHashTable = hashTableController->currentHashTable;
	hashSize = 0;
	status = SUCCES;

	if (hashTableController->nrOfHashTable == MAX_STRUCTS)
	{
		status = NUMBER_OF_STRUCTURES_EXCEEDED;
		goto Exit;
	}

	status = HashTableCreate(&hashTable, &HashFunction);
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	hashTableController->currentHashTable = hashTableController->nrOfHashTable;
	(hashTableController->nrOfHashTable)++;
	hashTableController->hashTable[hashTableController->currentHashTable] = hashTable;

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		// TODO:ParserFlush(parser);
		HashTableDestroy(&hashTableController->hashTable[hashTableController->currentHashTable]);
		hashTableController->currentHashTable = oldCurrentHashTable;
		(hashTableController->nrOfHashTable)--;
		//status = SUCCES;
		goto Exit;
	}

	while (!IsExitClause(status))
	{
		hashSize++;
		if (hashSize > MAX_CAPACITY)
		{
			status = CAPACITY_LIMIT_REACHED;
			goto CleanUp;
		}
		HashTableAdd(&hashTableController->hashTable[hashTableController->currentHashTable], nr);
		if (SUCCES != status)
		{
			goto CleanUp;
		}
		status = ParseNextInt(parser, &nr);
		if (INVALID_INPUT == status)
		{
			// TODO:ParserFlush(parser);
			HashTableDestroy(&hashTableController->hashTable[hashTableController->currentHashTable]);
			hashTableController->currentHashTable = oldCurrentHashTable;
			(hashTableController->nrOfHashTable)--;
			//status = SUCCES;
			goto Exit;
		}
		if (SUCCES == status) {
			status = SkipSpaces(parser);
			if (END_LINE == status || END_FILE == status)
			{
				HashTableAdd(&hashTableController->hashTable[hashTableController->currentHashTable], nr);
				goto CleanUp;
			}
		}
		//			status = SkipSpaces(parser);
		//			if(SUCCES!= status && INVALID_PARSER_POINTER!=status)
		//			{
		//				status = SUCCES;
		//				goto CleanUp;
		//			}
	}

	goto Exit;
CleanUp:
	if (STRUCTS_LIMIT_REACHED == status)
	{
		HashTableDestroy(&hashTableController->hashTable[hashTableController->currentHashTable]);
		hashTableController->currentHashTable = oldCurrentHashTable;
		(hashTableController->nrOfHashTable)--;
	}
	if (MALLOC_ERROR == status)
	{
		goto Exit;//tratam in interfata malloc-error
	}
Exit:
	return status;
}


STATUS_CODE HashTableControllerSearch(PHASH_TABLE_CONTROLLER hashTableController, PPARSER parser, FILE* out)
{
	STATUS_CODE status;
	BOOLEAN exist;
	int nr;
	int pos = -1;

	status = SUCCES;
	exist = FALSE;

	if (NULL == hashTableController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (hashTableController->currentHashTable >= hashTableController->nrOfHashTable || hashTableController->nrOfHashTable == 0)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		goto CleanUp;
	}
	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		status = HashTableFind(&hashTableController->hashTable[hashTableController->currentHashTable], nr, &exist);
		if (status == SUCCES)
		{
			if (exist == TRUE)
			{
				fprintf(out, "FOUND\n");
			}
			else
			{
				fprintf(out, "NOT_FOUND\n");
			}
		}
		goto CleanUp;
	}
	if (INVALID_INPUT_END_OF_FILE == status || INVALID_INPUT_END_OF_LINE == status || INVALID_INPUT == status)
	{
		goto CleanUp;
	}
	if (END_LINE_NO_NUMBER == status || END_FILE == status)
	{
		goto CleanUp;
	}
	status = SkipSpaces(parser);
	if (status == END_LINE || status == END_FILE)//nu am avut sfarsit dupa numar deci verificam sa fie doar spatii
	{
		status = HashTableFind(&hashTableController->hashTable[hashTableController->currentHashTable], nr,&exist);
		if (status == SUCCES)
		{
			if(exist == TRUE)
			{
				fprintf(out, "FOUND\n");
			}
			else
			{
				fprintf(out, "NOT_FOUND\n");
			}
		}
		goto CleanUp;
	}
	else if (status != INVALID_PARSER_POINTER)
	{
		status = INVALID_INPUT;//need flush
	}

CleanUp:
	if (END_LINE_NO_NUMBER == status)
	{
		status = INVALID_INPUT_END_OF_LINE;
	}
	if (END_FILE_NO_NUMBER == status)
	{
		status = INVALID_INPUT_END_OF_FILE;
	}
	return status;
}

STATUS_CODE HashTableControllerGoTo(PHASH_TABLE_CONTROLLER hashTableController, PPARSER parser)
{
	STATUS_CODE status;

	int nr;
	status = SUCCES;
	nr = -1;

	if (NULL == hashTableController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}
	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		goto CleanUp;
	}
	if (END_LINE_NO_NUMBER == status)
	{
		status = INVALID_INPUT_END_OF_LINE;
	}
	if (END_FILE_NO_NUMBER == status)
	{
		status = INVALID_INPUT_END_OF_FILE;
	}

	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		if (nr < 0 || nr > 9)
		{
			if (NUMBER_READED_END_OF_FILE == status)
			{
				status = INDEX_OUT_OF_BOUNDS_END_FILE;
			}
			else
			{
				status = INDEX_OUT_OF_BOUNDS;
			}
			goto CleanUp;
		}
		hashTableController->currentHashTable = nr;
		goto CleanUp;
	}
	if (INVALID_INPUT_END_OF_LINE == status || INVALID_INPUT_END_OF_FILE == status)
	{
		goto CleanUp;
	}
	if (INVALID_INPUT == status)
	{
		status = SkipSpaces(parser);
		if (status == END_LINE || status == NO_SPACES_WITH_END_LINE)
		{
			status = INVALID_INPUT_END_OF_LINE;
		}
		else if (END_FILE == status || NO_SPACES_WITH_END_FILE)
		{
			status = INVALID_INPUT_END_OF_LINE;
		}
		goto CleanUp;
	}
	status = SkipSpaces(parser);
	if (status == END_LINE || status == END_FILE)//nu am avut sfarsit dupa numar deci verificam sa fie doar spatii
	{
		if (nr < 0 || nr > 9)
		{
			if (status == END_LINE)
				status = INDEX_OUT_OF_BOUNDS;
			else status = INDEX_OUT_OF_BOUNDS;
			goto CleanUp;
		}
		hashTableController->currentHashTable = nr;
		goto CleanUp;
	}
	else if (status != INVALID_PARSER_POINTER)
	{
		status = INVALID_INPUT;//need flush
	}
	hashTableController->currentHashTable = nr;

CleanUp:
	return status;
}

STATUS_CODE HashTableControllerDestroy(PHASH_TABLE_CONTROLLER *hashTableController)
{
	STATUS_CODE status;
	int i;

	status = SUCCES;

	if (NULL == hashTableController)
	{
		status = INVALID_PCONTROLLER_POINTER;
		goto CleanUp;
	}

//	for (i = 0; i < (*hashTableController)->nrOfHashTable; i++)
//	{
//		HashTableDestroy(&((*hashTableController)->hashTable[i]));
//	}
//	free(*hashTableController);
//	*hashTableController = NULL;

CleanUp:
	return status;
}