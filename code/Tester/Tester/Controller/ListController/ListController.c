#include "ListController.h"


void initListController(PLIST_CONTROLLER listController)
{
	int i;
	for (i = 0; i < MAX_STRUCTS;i++)
	{
		CreateList(&listController->lists[i]);
	}
	listController->currentList = -1;
	listController->nrOfLists = 0;
}

STATUS_CODE ListControllerCreate(PLIST_CONTROLLER *listController)
{
	STATUS_CODE status;

	status = SUCCES;

	if (NULL == listController)
	{
		status = INVALID_PCONTROLLER_POINTER;
		goto CleanUp;
	}

	(*listController) = (PLIST_CONTROLLER)malloc(sizeof(LIST_CONTROLLER));
	if(NULL == (*listController))
	{
		status = MALLOC_ERROR;
		goto CleanUp;
	}

	initListController(*listController);
	(*listController)->currentList = -1;
	(*listController)->nrOfLists = 0;
	CleanUp:
	return status;
}

STATUS_CODE ListControllerRead(PLIST_CONTROLLER listController, PPARSER parser)
{
	STATUS_CODE status;
	LIST list;
	int listSize;
	int nr;
	int oldCurrentList;


	oldCurrentList = listController->currentList;
	listSize = 0;
	status = SUCCES;

	if(listController->nrOfLists == MAX_STRUCTS)
	{
		status = NUMBER_OF_STRUCTURES_EXCEEDED;
		goto Exit;
	}

	status = CreateList(&list);
	if(SUCCES != status)
	{
		goto CleanUp;
	}

	listController->currentList = listController->nrOfLists;
	(listController->nrOfLists)++;
	listController->lists[listController->currentList] = list;

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		// TODO:ParserFlush(parser);
		ListDestroy(&listController->lists[listController->currentList]);
		listController->currentList = oldCurrentList;
		(listController->nrOfLists)--;
		//status = SUCCES;
		goto Exit;
	}

	while (!IsExitClause(status))
	{
		listSize++;
		if (list.size > MAX_CAPACITY)
		{
			status = CAPACITY_LIMIT_REACHED;
			goto CleanUp;
		}
		ListAdd(&listController->lists[listController->currentList], nr);
		if (SUCCES != status)
		{
			goto CleanUp;
		}
		status = ParseNextInt(parser, &nr);
		if (INVALID_INPUT == status)
		{
			// TODO:ParserFlush(parser);
			ListDestroy(&listController->lists[listController->currentList]);
			listController->currentList = oldCurrentList;
			(listController->nrOfLists)--;
			//status = SUCCES;
			goto Exit;
		}
		if (SUCCES == status) {
			status = SkipSpaces(parser);
			if (END_LINE == status || END_FILE == status)
			{
				ListAdd(&listController->lists[listController->currentList], nr);
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
		ListDestroy(&listController->lists[listController->currentList]);
		listController->currentList = oldCurrentList;
		(listController->nrOfLists)--;
	}
	if (MALLOC_ERROR == status)
	{
		goto Exit;//tratam in interfata malloc-error
	}
Exit:
	return status;
}

STATUS_CODE ListControllerDestroy(PLIST_CONTROLLER *listController)
{
	STATUS_CODE status;
	int i;

	status = SUCCES;

	if (NULL == listController)
	{
		status = INVALID_PCONTROLLER_POINTER;
		goto CleanUp;
	}

	for (i = 0; i < (*listController)->nrOfLists; i++)
	{
		ListDestroy(&((*listController)->lists[i]));
	}
	free(*listController);
	*listController = NULL;

CleanUp:
	return status;
}

STATUS_CODE ListControllerGoTo(PLIST_CONTROLLER listController, PPARSER parser)
{
	STATUS_CODE status;

	int nr;
	status = SUCCES;
	nr = -1;

	if (NULL == listController)
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
		listController->currentList = nr;
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
		listController->currentList = nr;
		goto CleanUp;
	}
	else if (status != INVALID_PARSER_POINTER)
	{
		status = INVALID_INPUT;//need flush
	}
	listController->currentList = nr;

CleanUp:
	return status;
}

STATUS_CODE ListControllerPrint(PLIST_CONTROLLER listController, PPARSER parser, FILE *out)
{
	STATUS_CODE status;

	status = SUCCES;
	if (NULL == listController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (NULL == parser)
	{
		status = INVALID_PARSER_POINTER;
		goto CleanUp;
	}

	if (listController->currentList >= listController->nrOfLists || listController->nrOfLists == 0 || listController->lists[listController->currentList].first ==NULL || listController->lists[listController->currentList].last == NULL)
	{
		status = CURRENT_STRUCTURE_UNDEFINED;
		goto CleanUp;
	}
	ListPrint((listController->lists[listController->currentList]), out);

CleanUp:
	return status;
}

STATUS_CODE ListControllerAdd(PLIST_CONTROLLER listController, PPARSER parser)
{
	STATUS_CODE status;
	int nr;

	status = SUCCES;

	if (NULL == listController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}
	if (NULL == parser)
	{
		status = INVALID_PARSER_POINTER;
		goto CleanUp;
	}

	if (listController->currentList >= listController->nrOfLists || listController->nrOfLists == 0)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}

	if(listController->lists[listController->currentList].size == MAX_CAPACITY)
	{
		status = CAPACITY_LIMIT_REACHED;
		goto CleanUp;
	}

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		//ParserFlush(parser);
		//@ TODO:status = SUCCES;
		goto CleanUp;
	}
	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		ListAdd(&(listController->lists[listController->currentList]), nr);
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
		ListAdd(&(listController->lists[listController->currentList]), nr);
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

STATUS_CODE ListControllerMerge(PLIST_CONTROLLER listController)
{
	STATUS_CODE status;
	status = SUCCES;
	if (NULL == listController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (listController->currentList >= listController->nrOfLists || listController->nrOfLists == 0)
	{
		status = CURRENT_STRUCTURE_UNDEFINED;
		goto CleanUp;
	}
	if(listController->currentList == 0)
	{
		status = PREVIOUS_STRUCTURE_UNDEFINED;
		goto CleanUp;
	}
	if (listController->nrOfLists == MAX_STRUCTS)
	{
		status = NUMBER_OF_STRUCTURES_EXCEEDED;
		goto CleanUp;
	}

	status = ListMerge(
		&(listController->lists[listController->currentList - 1]),
		&(listController->lists[listController->currentList]),
		&(listController->lists[listController->nrOfLists]));
	if(SUCCES == status)
	{
		listController->currentList = listController->nrOfLists;
		listController->nrOfLists++;
	}
	CleanUp:
	return status;
}

STATUS_CODE ListControllerMergeSort(PLIST_CONTROLLER listController)
{
	STATUS_CODE status;
	status = SUCCES;
	if (NULL == listController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (listController->currentList >= listController->nrOfLists || listController->nrOfLists == 0)
	{
		status = CURRENT_STRUCTURE_UNDEFINED;
		goto CleanUp;
	}

	status = ListMergeSort(&listController->lists[listController->currentList]);

CleanUp:
	return status;
}