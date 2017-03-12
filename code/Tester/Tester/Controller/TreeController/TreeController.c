#include "TreeController.h"

void initTreeController(PTREE_CONTROLLER treeController)
{
	int i;
	for (i = 0; i < MAX_STRUCTS; i++)
	{
		treeController->trees[i].root = NULL;
	}
}

STATUS_CODE TreeControllerCreate(PTREE_CONTROLLER *treeController)
{
	STATUS_CODE status;

	status = SUCCES;

	if (NULL == treeController)
	{
		status = INVALID_PCONTROLLER_POINTER;
		goto CleanUp;
	}
	(*treeController) = (PTREE_CONTROLLER)malloc(sizeof(TREE_CONTROLLER));
	if (NULL == (*treeController))
	{
		status = MALLOC_ERROR;
		goto CleanUp;
	}
	initTreeController(*treeController);
	(*treeController)->currentTree = -1;
	(*treeController)->nrOfTrees = 0;

CleanUp:
	return status;
}

STATUS_CODE TreeControllerRead(PTREE_CONTROLLER treeController, PPARSER parser)
{
	STATUS_CODE status;
	MY_TREE tree;
	int treeSize;
	int nr;
	int oldCurrentList;


	oldCurrentList = treeController->currentTree;
	treeSize = 0;
	status = SUCCES;

	if (treeController->nrOfTrees == MAX_STRUCTS)
	{
		status = NUMBER_OF_STRUCTURES_EXCEEDED;
		goto Exit;
	}

	status = TreeCreate(&tree);
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	treeController->currentTree = treeController->nrOfTrees;
	(treeController->nrOfTrees)++;
	treeController->trees[treeController->currentTree] = tree;

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		// TODO:ParserFlush(parser);
		TreeDestroy(&treeController->trees[treeController->currentTree]);
		treeController->currentTree = oldCurrentList;
		(treeController->nrOfTrees)--;
		//status = SUCCES;
		goto Exit;
	}

	while (!IsExitClause(status))
	{
		treeSize++;
		if (treeSize > MAX_CAPACITY)
		{
			status = CAPACITY_LIMIT_REACHED;
			goto CleanUp;
		}
		TreeInsert(&treeController->trees[treeController->currentTree], nr);
		if (SUCCES != status)
		{
			goto CleanUp;
		}
		status = ParseNextInt(parser, &nr);
		if (INVALID_INPUT == status)
		{
			// TODO:ParserFlush(parser);
			TreeDestroy(&treeController->trees[treeController->currentTree]);
			treeController->currentTree = oldCurrentList;
			(treeController->nrOfTrees)--;
			//status = SUCCES;
			goto Exit;
		}
		if (SUCCES == status) {
			status = SkipSpaces(parser);
			if (END_LINE == status || END_FILE == status)
			{
				TreeInsert(&treeController->trees[treeController->currentTree], nr);
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
		TreeDestroy(&treeController->trees[treeController->currentTree]);
		treeController->currentTree = oldCurrentList;
		(treeController->nrOfTrees)--;
	}
	if (MALLOC_ERROR == status)
	{
		goto Exit;//tratam in interfata malloc-error
	}
Exit:
	return status;
}

STATUS_CODE TreeControllerDestroy(PTREE_CONTROLLER *treeController)
{
	STATUS_CODE status;
	int i;

	status = SUCCES;

	if (NULL == treeController)
	{
		status = INVALID_PCONTROLLER_POINTER;
		goto CleanUp;
	}

	for (i = 0; i < (*treeController)->nrOfTrees; i++)
	{
		TreeDestroy(&((*treeController)->trees[i]));
	}
	free(*treeController);
	*treeController = NULL;

CleanUp:
	return status;
}

STATUS_CODE TreeControllerGoTo(PTREE_CONTROLLER treeController, PPARSER parser)
{
	STATUS_CODE status;

	int nr;
	status = SUCCES;
	nr = -1;

	if (NULL == treeController)
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
		treeController->currentTree = nr;
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
		treeController->currentTree = nr;
		goto CleanUp;
	}
	else if (status != INVALID_PARSER_POINTER)
	{
		status = INVALID_INPUT;//need flush
	}
	treeController->currentTree = nr;

CleanUp:
	return status;
}

STATUS_CODE TreeControllerInsert(PTREE_CONTROLLER treeController, PPARSER parser)
{
	STATUS_CODE status;
	int nr;

	status = SUCCES;

	if (NULL == treeController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}
	if (NULL == parser)
	{
		status = INVALID_PARSER_POINTER;
		goto CleanUp;
	}

	if (treeController->currentTree >= treeController->nrOfTrees || treeController->nrOfTrees == 0)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}

//	if(treeController->trees[treeController->currentTree].size == MAX_CAPACITY)
//	{
//		status = CAPACITY_LIMIT_REACHED;
//		goto CleanUp;
//	}

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		goto CleanUp;
	}
	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		TreeInsert(&(treeController->trees[treeController->currentTree]), nr);
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
		TreeInsert(&(treeController->trees[treeController->currentTree]), nr);
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

STATUS_CODE TreeControllerRemove(PTREE_CONTROLLER treeController, PPARSER parser, FILE* out)
{
	STATUS_CODE status;
	int ret;
	BOOLEAN deleted;

	deleted = FALSE;
	status = SUCCES;

	if (NULL == treeController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (treeController->currentTree >= treeController->nrOfTrees || 0 == treeController->nrOfTrees)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}

	status = ParseNextInt(parser, &ret);
	if (INVALID_INPUT == status)
	{
		// @TODO: ParserFlush(parser);
		//status = SUCCES;
		goto CleanUp;
	}
	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		status = TreeDelete(&(treeController->trees[treeController->currentTree]), ret);
		if (SUCCES == status)
		{
			//fprintf(out, "%d\n", ret);
		}
		else if (ELEMENT_NOT_FOUND == status)
		{
			fprintf(out, "NOT_FOUND\n");
			status = SUCCES;
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
	//status = ParseNextInt(parser, &aux);
	status = SkipSpaces(parser);
	if (status == END_LINE || status == END_FILE)//nu am avut sfarsit dupa numar deci verificam sa fie doar spatii
	{
		status = TreeDelete(&(treeController->trees[treeController->currentTree]), ret);
		if (SUCCES == status)
		{
			//fprintf(out, "%d\n", ret);
		}
		else if(ELEMENT_NOT_FOUND == status)
		{
			fprintf(out, "NOT_FOUND\n");
			status = SUCCES;
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

STATUS_CODE TreeControllerSearch(PTREE_CONTROLLER treeController, PPARSER parser, FILE* out)
{
	STATUS_CODE status;
	BOOLEAN exist;
	int nr;
	int pos = -1;

	status = SUCCES;
	exist = FALSE;

	if (NULL == treeController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (treeController->currentTree >= treeController->nrOfTrees || treeController->nrOfTrees == 0)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		// TODO:ParserFlush(parser);
		//status = INVALID_INPUT;
		goto CleanUp;
	}
	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		status = TreeFind(treeController->trees[treeController->currentTree], nr, &exist);
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
	//status = ParseNextInt(parser, &aux);-  FOLOSESC SKIP SPACES SA VAD DACA MAI AM CEVA PE LINIE
	status = SkipSpaces(parser);
	if (status == END_LINE || status == END_FILE)//nu am avut sfarsit dupa numar deci verificam sa fie doar spatii
	{
		status = TreeFind(treeController->trees[treeController->currentTree], nr, &exist);
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