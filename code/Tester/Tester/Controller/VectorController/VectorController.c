#include "VectorController.h"



void initVectorController(PVECTOR_CONTROLLER controller)
{
	int i;

	for (i = 0; i < MAX_STRUCTS; i++)
	{
		controller->vectors[i] = NULL;
	}
}

STATUS_CODE VectorControllerCreate(PVECTOR_CONTROLLER * vectorController)
{
	STATUS_CODE status;
	
	status = SUCCES;

	if (NULL == vectorController)
	{
		status = INVALID_PCONTROLLER_POINTER;
		goto CleanUp;
	}
	(*vectorController) = (PVECTOR_CONTROLLER)malloc(sizeof(VECTOR_CONTROLLER));
	if (NULL == (*vectorController))
	{
		status = MALLOC_ERROR;
		goto CleanUp;
	}
	initVectorController(*vectorController);
	(*vectorController)->currentVector = -1;
	(*vectorController)->nrOfVectors = 0;

CleanUp:
	return status;
}

STATUS_CODE VectorControllerDestroy(PVECTOR_CONTROLLER *vectorController)
{
	STATUS_CODE status;
	int i;

	status = SUCCES;

	if (NULL == *vectorController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	for (i = 0; i < (*vectorController)->nrOfVectors; i++)
	{
		if(NULL != ((*vectorController))->vectors[i])
		{
			VectorDestroy(&((*vectorController)->vectors[i]));
		}
	}
	free(*vectorController);
	*vectorController = NULL;

CleanUp:
	return status;
}




STATUS_CODE VectorControllerRead(PVECTOR_CONTROLLER vectorController, PPARSER parser)
{
	STATUS_CODE status;
	PDYNAMIC_VECTOR vector;
	int vectorSize;
	int nr;
	int oldCurrentVector;

	oldCurrentVector = vectorController->currentVector;
	vectorSize= 0;
	vector = NULL;
	status = SUCCES;

	if (vectorController->nrOfVectors == MAX_STRUCTS)
	{
		status = NUMBER_OF_STRUCTURES_EXCEEDED;
		goto Exit;
	}
	status = VectorCreate(&vector);
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	vectorController->currentVector = vectorController->nrOfVectors;
	(vectorController->nrOfVectors)++;
	vectorController->vectors[vectorController->currentVector] = vector;

	status = ParseNextInt(parser, &nr);
	if(INVALID_INPUT == status)
	{
		// TODO:ParserFlush(parser);
		VectorDestroy(&vectorController->vectors[vectorController->currentVector]);
		vectorController->currentVector = oldCurrentVector;
		(vectorController->nrOfVectors)--;
		//status = SUCCES;
		goto Exit;
	}
	while (!IsExitClause(status))
	{
		vectorSize++;
		if (vector->size > MAX_CAPACITY)
		{
			status = CAPACITY_LIMIT_REACHED;
			goto CleanUp;
		}

		//TODO:VEZI DACA TRATEAZA SF DE LINIE/FISIER CORECT SAU TRB SA RETURNEZI END FILE/END LINE

		VectorAdd(vectorController->vectors[vectorController->currentVector], nr);

		if (SUCCES != status)
		{
			goto CleanUp;
		}
		status = ParseNextInt(parser, &nr);
		if (SUCCES == status){
			status = SkipSpaces(parser);
			if (END_LINE == status || END_FILE == status)
			{
				VectorAdd(vectorController->vectors[vectorController->currentVector], nr);
				goto CleanUp;
			}
		}
		if (INVALID_INPUT == status)
		{
			// TODO:ParserFlush(parser);
			VectorDestroy(&vectorController->vectors[vectorController->currentVector]);
			vectorController->currentVector = oldCurrentVector;
			(vectorController->nrOfVectors)--;
			//status = SUCCES;
			goto Exit;
		}
	}
	if(END_FILE_NO_NUMBER == status || END_FILE == status)  
	{
		status = END_FILE;
	}
	if(END_LINE_NO_NUMBER == status || END_LINE)
	{
		status = SUCCES;
	}
	goto Exit;
CleanUp:
	if (STRUCTS_LIMIT_REACHED == status)
	{
		VectorDestroy(&vectorController->vectors[vectorController->currentVector]);
		vectorController->currentVector = oldCurrentVector;
		vectorController->nrOfVectors--;
	}
	if (MALLOC_ERROR == status)
	{
		goto Exit;//tratam in interfata malloc-error
	}
Exit:
	return status;
}

/**
 * Returns STATUS_CODE: SUCCES
						INVALID_VECTOR_POINTER
						INDEX_OUT_OF_BOUNDS
						INDEX_OUT_OF_BOUNDS_END_LINE
 */

STATUS_CODE VectorControllerGoTo(PVECTOR_CONTROLLER vectorController,PPARSER parser)
{
	STATUS_CODE status;
	
	int nr;
	status = SUCCES;
	nr = -1;

	if (NULL == vectorController)
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
	if(END_LINE_NO_NUMBER == status)
	{
		status = INVALID_INPUT_END_OF_LINE;
	}
	if(END_FILE_NO_NUMBER == status)
	{
		status = INVALID_INPUT_END_OF_FILE;
	}
	
	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		if (nr < 0 || nr > 9)
		{
			if(NUMBER_READED_END_OF_FILE == status)
			{
				status = INDEX_OUT_OF_BOUNDS_END_FILE;
			}
			else
			{
				status = INDEX_OUT_OF_BOUNDS;
			}
			goto CleanUp;
		}
		vectorController->currentVector = nr;
		goto CleanUp;
	}
	if(INVALID_INPUT_END_OF_LINE == status || INVALID_INPUT_END_OF_FILE == status)
	{
		goto CleanUp;
	}
	//status = ParseNextInt(parser, &aux);
	if(INVALID_INPUT == status)
	{
		status = SkipSpaces(parser);
		if(status == END_LINE || status == NO_SPACES_WITH_END_LINE)
		{
			status = INVALID_INPUT_END_OF_LINE;
		}
		else if(END_FILE == status || NO_SPACES_WITH_END_FILE)
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
			if(status == END_LINE)
				status = INDEX_OUT_OF_BOUNDS;
			else status = INDEX_OUT_OF_BOUNDS;
			goto CleanUp;
		}
		vectorController->currentVector = nr;
		goto CleanUp;
	}
	else if (status != INVALID_PARSER_POINTER)
	{
		status = INVALID_INPUT;//need flush
	}
	vectorController->currentVector = nr;

CleanUp:
	return status;
}

/**
  *Returns STATUS_CODE: SUCCES
						INVALID_VECTOR_POINTER
						INVALID_PARSER_POINTER
						CURRENT_STRUCTURE_UNDEFINED
 */
STATUS_CODE VectorControllerPrint(PVECTOR_CONTROLLER vectorController, PPARSER parser,FILE *out)
{
	STATUS_CODE status;

	status = SUCCES;
	if (NULL == vectorController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}
	
	if (NULL == parser)
	{
		status = INVALID_PARSER_POINTER;
		goto CleanUp;
	}

	if (vectorController->currentVector >= vectorController->nrOfVectors || vectorController->nrOfVectors == 0)
	{
		status = CURRENT_STRUCTURE_UNDEFINED;
		goto CleanUp;
	}
	VectorPrint(*(vectorController->vectors[vectorController->currentVector]),out);

CleanUp:
	return status;
}


/**
 *Return STATUS_CODE: SUCCES
					  INVALID_CONTROLLER_POINTER
					  CURRENT_STRUCTURE_UNDEFINED - need flush
					  MALLOC_ERROR
					  NUMBER_READED_END_OF_LINE
					  NUMBER_READED_END_OF_FILE
					  INVALID_INPUT - not at end of line/file => need flush
					  PARSER_READ_FAIL
					  SUCCES
					  INVALID_INPUT_END_OF_LINE
					  INVALID_INPUT_END_OF_FILE
 */

STATUS_CODE VectorControllerAdd(PVECTOR_CONTROLLER vectorController, PPARSER parser)
{
	STATUS_CODE status;
	int nr;

	status = SUCCES;

	if (NULL == vectorController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}
	if (NULL == parser)
	{
		status = INVALID_PARSER_POINTER;
		goto CleanUp;
	}

	if (vectorController->currentVector >= vectorController->nrOfVectors || vectorController->nrOfVectors == 0)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}
	if(vectorController->vectors[vectorController->currentVector]->size == MAX_CAPACITY)
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
		VectorAdd(vectorController->vectors[vectorController->currentVector], nr);
		goto CleanUp;
	}
	if(INVALID_INPUT_END_OF_FILE == status || INVALID_INPUT_END_OF_LINE == status || INVALID_INPUT == status)
	{
		goto CleanUp;
	}
	if(END_LINE_NO_NUMBER == status || END_FILE == status)
	{
		goto CleanUp;
	}
	//status = ParseNextInt(parser, &aux);
	status = SkipSpaces(parser);
	if (status == END_LINE || status == END_FILE)//nu am avut sfarsit dupa numar deci verificam sa fie doar spatii
	{
		VectorAdd(vectorController->vectors[vectorController->currentVector], nr);
		goto CleanUp;
	}
	else if(status != INVALID_PARSER_POINTER)
	{
		status = INVALID_INPUT;//need flush
	}

CleanUp:
	if(END_LINE_NO_NUMBER == status)
	{
		status = INVALID_INPUT_END_OF_LINE;
	}
	if(END_FILE_NO_NUMBER == status)
	{
		status = INVALID_INPUT_END_OF_FILE;
	}
	return status;
}

STATUS_CODE VectorControllerLength(PVECTOR_CONTROLLER vectorController, FILE *out)
{
	STATUS_CODE status;
	int nr;
	status = SUCCES;
	if (NULL == vectorController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (vectorController->currentVector >= vectorController->nrOfVectors || 0 == vectorController->nrOfVectors)
	{
		status = CURRENT_STRUCTURE_UNDEFINED;
		goto CleanUp;
	}
	nr = VectorLength(*(vectorController->vectors[vectorController->currentVector]));
	fprintf(out, "%d\n", nr);
CleanUp:
	return status;
}

STATUS_CODE VectorControllerRemovePosition(PVECTOR_CONTROLLER vectorController,PPARSER parser)
{
	STATUS_CODE status;
	int nr;


	status = SUCCES;

	if (NULL == vectorController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (vectorController->currentVector >= vectorController->nrOfVectors || 0 == vectorController->nrOfVectors)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		// @TODO: ParserFlush(parser);
		//status = SUCCES;
		goto CleanUp;
	}
	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		status = VectorRemovePosition(vectorController->vectors[vectorController->currentVector], nr);
		goto CleanUp;
	}
	if (INVALID_INPUT_END_OF_FILE == status || INVALID_INPUT_END_OF_LINE == status|| INVALID_INPUT == status)
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
		status = VectorRemovePosition(vectorController->vectors[vectorController->currentVector], nr);
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

STATUS_CODE VectorControllerRemoveValue(PVECTOR_CONTROLLER vectorController, PPARSER parser)
{
	STATUS_CODE status;
	int nr;


	status = SUCCES;

	if (NULL == vectorController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (vectorController->currentVector >= vectorController->nrOfVectors || 0 == vectorController->nrOfVectors)
	{
		status = CURRENT_STRUCTURE_UNDEFINED; //need flush
		goto CleanUp;
	}

	status = ParseNextInt(parser, &nr);
	if (INVALID_INPUT == status)
	{
		// TODO:ParserFlush(parser);
		//status = SUCCES;
		goto CleanUp;
	}
	if (NUMBER_READED_END_OF_FILE == status || NUMBER_READED_END_OF_LINE == status)//dupa numar avem \n sau \0
	{
		status = VectorRemoveValue(vectorController->vectors[vectorController->currentVector], nr);
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
		status = VectorRemoveValue(vectorController->vectors[vectorController->currentVector], nr);
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

STATUS_CODE VectorControllerSearch(PVECTOR_CONTROLLER vectorController, PPARSER parser,FILE* out)
{
	STATUS_CODE status;
	int nr;
	int pos = -1;

	status = SUCCES;

	if (NULL == vectorController)
	{
		status = INVALID_CONTROLLER_POINTER;
		goto CleanUp;
	}

	if (vectorController->currentVector >= vectorController->nrOfVectors || 0 == vectorController->nrOfVectors)
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
		status = VectorSearch(vectorController->vectors[vectorController->currentVector], nr,&pos);
		if (status == SUCCES)
		{
			fprintf(out, "%d\n", pos);
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
		//@TODO: de ce?
		status = VectorSearch(vectorController->vectors[vectorController->currentVector], nr, &pos);
		if(status == SUCCES)
		{
			fprintf(out, "%d\n", pos);
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
