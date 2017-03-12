#include "HeapController.h"

void initHeapController(PHEAP_CONTROLLER heapController)
{
	int i;

	for (i = 0; i < MAX_STRUCTS; i++)
	{
		heapController->heaps[i] = NULL;
	}
}

STATUS_CODE HeapControllerCreate(PHEAP_CONTROLLER *heapController)
{
	STATUS_CODE status;

	status = SUCCES;

	if (NULL == heapController)
	{
		status = INVALID_PCONTROLLER_POINTER;
		goto CleanUp;
	}
	(*heapController) = (PHEAP_CONTROLLER)malloc(sizeof(HEAP_CONTROLLER));
	if (NULL == (*heapController))
	{
		status = MALLOC_ERROR;
		goto CleanUp;
	}
	initHeapController(*heapController);
	(*heapController)->currentHeap = -1;
	(*heapController)->nrOfHeaps = 0;

CleanUp:
	return status;
}

STATUS_CODE HeapControllerDestroy(PHEAP_CONTROLLER *heapController)
{
	STATUS_CODE status;
	int i;

	status = SUCCES;

	if (NULL == heapController)
	{
		status = INVALID_PCONTROLLER_POINTER;
		goto CleanUp;
	}

	for (i = 0; i < (*heapController)->nrOfHeaps; i++)
	{
		if (NULL != ((*heapController))->heaps[i])
		{
			MyHeapDestroy(&(((*heapController))->heaps[i]));
		}
	}
	free(*heapController);
	*heapController = NULL;

CleanUp:
	return status;
}

STATUS_CODE HeapControllerRead(PHEAP_CONTROLLER heapController, PPARSER parser)
{
	STATUS_CODE status;
	PMY_HEAP heap;
	int heapSize;
	int nr;
	int oldCurrentVector;

	oldCurrentVector = heapController->currentHeap;
	heapSize = 0;
	heap = NULL;
	status = SUCCES;

	if (heapController->nrOfHeaps == MAX_STRUCTS)
	{
		status = NUMBER_OF_STRUCTURES_EXCEEDED;
		goto Exit;
	}
	status = MyHeapCreate(&heap);
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	heapController->currentHeap = heapController->nrOfHeaps;
	(heapController->nrOfHeaps)++;
	heapController->heaps[heapController->currentHeap] = heap;

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		MyHeapDestroy(&heapController->heaps[heapController->currentHeap]);
		heapController->currentHeap = oldCurrentVector;
		(heapController->nrOfHeaps)--;
		goto Exit;
	}
	while (!IsExitClause(status))
	{
		heapSize++;
		if (heapSize > MAX_CAPACITY)
		{
			status = CAPACITY_LIMIT_REACHED;
			goto CleanUp;
		}

		MyHeapAdd(heapController->heaps[heapController->currentHeap], nr);

		if (SUCCES != status)
		{
			goto CleanUp;
		}
		status = ParseNextInt(parser, &nr);
		if (SUCCES == status) {
			status = SkipSpaces(parser);
			if (END_LINE == status || END_FILE == status)
			{
				MyHeapAdd(heapController->heaps[heapController->currentHeap], nr);
				goto CleanUp;
			}
		}
		if (INVALID_INPUT == status)
		{
			MyHeapDestroy(&heapController->heaps[heapController->currentHeap]);
			heapController->currentHeap = oldCurrentVector;
			(heapController->nrOfHeaps)--;
			goto Exit;
		}
	}
	if (END_FILE_NO_NUMBER == status || END_FILE == status)
	{
		status = END_FILE;
	}
	if (END_LINE_NO_NUMBER == status || END_LINE)
	{
		status = SUCCES;
	}
	goto Exit;
CleanUp:
	if (STRUCTS_LIMIT_REACHED == status)
	{
		MyHeapDestroy(&heapController->heaps[heapController->currentHeap]);
		heapController->currentHeap = oldCurrentVector;
		heapController->nrOfHeaps--;
	}
	if (MALLOC_ERROR == status)
	{
		goto Exit;//tratam in interfata malloc-error
	}
Exit:
	return status;
}

STATUS_CODE HeapControllerGoTo(PHEAP_CONTROLLER heapController, PPARSER parser)
{
	STATUS_CODE status;

	int nr;
	status = SUCCES;
	nr = -1;

	if (NULL == heapController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}
	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		//ParserFlush(parser);
		//@ TODO:status = SUCCES;
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
		heapController->currentHeap = nr;
		goto CleanUp;
	}
	if (INVALID_INPUT_END_OF_LINE == status || INVALID_INPUT_END_OF_FILE == status)
	{
		goto CleanUp;
	}
	//status = ParseNextInt(parser, &aux);
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
		heapController->currentHeap = nr;
		goto CleanUp;
	}
	else if (status != INVALID_PARSER_POINTER)
	{
		status = INVALID_INPUT;//need flush
	}
	heapController->currentHeap = nr;

CleanUp:
	return status;
}

STATUS_CODE HeapControllerInsert(PHEAP_CONTROLLER heapController, PPARSER parser)
{
	STATUS_CODE status;
	int nr;

	status = SUCCES;

	if (NULL == heapController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}
	if (NULL == parser)
	{
		status = INVALID_PARSER_POINTER;
		goto CleanUp;
	}

	if (heapController->currentHeap >= heapController->nrOfHeaps || 0 == heapController->nrOfHeaps)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}

	if (heapController->heaps[heapController->currentHeap]->size == MAX_CAPACITY)
	{
		status = CAPACITY_LIMIT_REACHED;
		goto CleanUp;
	}

	status = ParseNextInt(parser, &nr);



	if (INVALID_INPUT == status)
	{
		goto CleanUp;
	}
	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		MyHeapAdd(heapController->heaps[heapController->currentHeap], nr);
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
	//status = ParseNextInt(parser, &aux);
	status = SkipSpaces(parser);
	if (status == END_LINE || status == END_FILE)//nu am avut sfarsit dupa numar deci verificam sa fie doar spatii
	{
		MyHeapAdd(heapController->heaps[heapController->currentHeap], nr);
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

STATUS_CODE HeapControllerRemove(PHEAP_CONTROLLER heapController, PPARSER parser, FILE* out)
{
	STATUS_CODE status;
	int ret;

	status = SUCCES;

	if (NULL == heapController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (heapController->currentHeap >= heapController->nrOfHeaps ||  0 == heapController->nrOfHeaps)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}

	status = MyHeapDelete(heapController->heaps[heapController->currentHeap],&ret);
	if(SUCCES == status)
	{
		fprintf(out, "%d\n", ret);
		goto CleanUp;
	}
	else if(ELEMENT_NOT_FOUND == status)
	{
		fprintf(out, "NOT_FOUND\n");
		status = SUCCES;
		goto CleanUp;
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