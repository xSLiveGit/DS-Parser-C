#include "Interface.h"
#include "../Controller/HashTableController/HashTableController.h"

STATUS_CODE CreateFileName(char** inputFile, char* file,char* path,char* extension)
{
	STATUS_CODE status;
	char* destination;

	status = SUCCES;
	destination = NULL;

	if (NULL == inputFile)
	{
		status = INVALID_POINTER;
		goto CleanUp;
	}

	status = StrCat(&destination, path);
	if (SUCCES != status)
	{
		goto CleanUp;
	}
	
	status = StrCat(&destination, file);
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	status = StrCat(&destination, extension);
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	*inputFile = destination;

CleanUp:
	if (SUCCES != status)
	{
		free(destination);
		destination = NULL;
	}
	return status;
}

STATUS_CODE InterfaceCreate(PINTERFACE * interface, char* file)
{
	STATUS_CODE status;
	char* inputFileName;
	char* outputFileName;
	FILE* outputFile;

	outputFile = NULL;
	inputFileName = NULL;
	outputFileName = NULL;
	status = SUCCES;
	int errCode;

	if (NULL == interface)
	{
		status = INVALID_POINTER;
	}
	(*interface) = (PINTERFACE)malloc(sizeof(INTERFACE));
	if (NULL == (*interface))
	{
		status = MALLOC_ERROR;
		goto CleanUp;
	}

	(*interface)->treeController = NULL;
	(*interface)->vectorController = NULL;
	(*interface)->heapController = NULL;
	(*interface)->listController = NULL;
	(*interface)->hashtableController = NULL;
	(*interface)->outputFile = NULL;
	(*interface)->parser = NULL;
	

	status = CreateFileName(&outputFileName, file, RES_FILES_PATH, ".res");
	if (SUCCES != status)
	{
		goto CleanUp;
	}
	
	status = CreateFileName(&inputFileName, file, IN_FILES_PATH, ".in");
	if (SUCCES != status)
	{
		goto CleanUp;
	}
	fprintf(stdout, "%s    %s\n", file, outputFileName);

	status = ParserCreate(&((*interface)->parser), inputFileName);
	if (SUCCES != status)
	{
		goto CleanUp;
	}


	errCode = fopen_s(&outputFile, outputFileName, "w");
	if (errCode != 0)
	{
		status = FILE_OPEN_ERROR;
		goto CleanUp;
	}

	status = VectorControllerCreate(&((*interface)->vectorController));
	if (SUCCES != status)
	{
		goto CleanUp;
	}
	status = ListControllerCreate(&((*interface)->listController));
	
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	status = HashTableControllerCreate(&((*interface)->hashtableController));
	if (SUCCES != status)
	{
		goto CleanUp;
	}
	status = HeapControllerCreate(&((*interface)->heapController));
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	status = TreeControllerCreate(&((*interface)->treeController));
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	(*interface)->outputFile = outputFile;
	

CleanUp:
	if (SUCCES !=status)
	{
		InterfaceDestroy(interface);
		*interface = NULL;
	}

	free(outputFileName);
	outputFileName = NULL;

	free(inputFileName);
	inputFileName = NULL;

	return status;
}

STATUS_CODE InterfaceDestroy(PINTERFACE *interface)
{
	STATUS_CODE status;

	status = SUCCES;

	if (NULL == interface)
	{
		status = INVALID_POINTER;
		goto EXIT;
	}
	if (NULL != (*interface)->parser)
	{
		status = status | ParserDestroy(&((*interface)->parser));
	}

	if (NULL != (*interface)->vectorController && NULL != (*interface)->vectorController)
	{
		status = status | VectorControllerDestroy(&((*interface)->vectorController));
	}
	
	if(NULL != (*interface)->listController)
	{
		status = status | ListControllerDestroy(&((*interface)->listController));
	}

	if (NULL != (*interface)->hashtableController)
	{
		status = status | HashTableControllerDestroy(&((*interface)->hashtableController));
	}

	if (NULL != (*interface)->heapController)
	{
		status = status | HeapControllerDestroy(&((*interface)->heapController));
	}

	if (NULL != (*interface)->treeController)
	{
		status = status | TreeControllerDestroy(&((*interface)->treeController));
	}

	if (NULL != (*interface)->outputFile)
	{
		fclose((*interface)->outputFile);
		(*interface)->outputFile = NULL;
	}

	free(*interface);
	*interface = NULL;
EXIT:
	return status;
}

STATUS_CODE GenerateName(char** outputFile, int nr)
{
	STATUS_CODE status;
	int len;
	char* nrString;
	*outputFile = NULL;
	status = SUCCES;
	IntToString(nr, &nrString);
	
	len = GetLength(nrString);
	if (len == 1)
	{
		status = StrCat(outputFile, "00");
	}
	else if (len == 2)
	{
		status = StrCat(outputFile, "0");
	}
	if (SUCCES != status)
	{
		goto CleanUp;
	}
	status = StrCat(outputFile, nrString);
CleanUp:
	return status;
}

BOOLEAN IsCommandWithoutInput(char* command)
{
	return (AreEquals(command, "VectorPrint")
		|| (AreEquals(command, "VectorLength"))
		|| (AreEquals(command, "ListPrint"))
		|| (AreEquals(command, "ListMerge"))
		|| (AreEquals(command, "ListSort"))
		|| (AreEquals(command, "HeapRemove"))
		);
}

void printMsgInternalErr(STATUS_CODE status, FILE* out)
{
	if(MALLOC_ERROR == status)
	{
		fprintf(stderr, "MALLOC_ERROR\n");
	}
	else if(PARSER_READ_FAIL == status)
	{
		fprintf(stderr, "PARSER_READ_FAIL\n");
	}
	else if (INVALID_POINTER == status)
	{
		fprintf(stderr, "INVALID_POINTER\n");
	}
	else if (INVALID_PPOINTER == status)
	{
		fprintf(stderr, "INVALID_PPOINTER\n");
	}
	else if(FILE_OPEN_ERROR == status)
	{
		fprintf(stderr, "FILE_OPEN_ERROR\n");
	}
	else if (FILE_OPEN_ERROR == status)
	{
		fprintf(stderr, "FILE_OPEN_ERROR\n");
	}
}

void printMsg(STATUS_CODE status,FILE* out)
{
	if (status == STRUCTS_LIMIT_REACHED)
	{
		fprintf(out, "STRUCTS_LIMIT_REACHED\n");
	}
	else if (CAPACITY_LIMIT_REACHED == status)
	{
		fprintf(out, "CAPACITY_LIMIT_REACHED\n");
	}
	else if (MALLOC_ERROR == status)
	{
		fprintf(stderr, "MALLOC_ERROR\n");
	}
	else if(END_LINE_NO_NUMBER == status || END_FILE_NO_NUMBER == status || INVALID_INPUT ==status|| INVALID_INPUT_END_OF_LINE == status || INVALID_INPUT_END_OF_FILE == status)
	{
		fprintf(out, "INVALID_INPUT\n");
	}
	else if(INDEX_OUT_OF_BOUNDS == status || INDEX_OUT_OF_BOUNDS_END_FILE == status)
	{
		fprintf(out, "INDEX_OUT_OF_BOUNDS\n");
	}
	else if(END_LINE_NO_STRING == status)
	{
		fprintf(out, "INVALID_COMMAND\n");
	}
	else if(CURRENT_STRUCTURE_UNDEFINED == status)
	{
		fprintf(out, "CURRENT_STRUCTURE_UNDEFINED\n");
	}
	else if(ELEMENT_NOT_FOUND == status)
	{
		fprintf(out, "NOT_FOUND\n");
	}
	else if(PREVIOUS_STRUCTURE_UNDEFINED == status)
	{
		fprintf(out, "PREVIOUS_STRUCTURE_UNDEFINED\n");
	}
	else if(STRUCTS_LIMIT_REACHED == status)
	{
		fprintf(out, "STRUCTS_LIMIT_REACHED\n");
	}
	else if(NUMBER_OF_STRUCTURES_EXCEEDED == status)
	{
		fprintf(out, "NUMBER_OF_STRUCTURES_EXCEEDED\n");
	}
}

STATUS_CODE TreatCommand(PINTERFACE interface, char* command,STATUS_CODE readStatus)
{
	STATUS_CODE status;

    status = SUCCES;
	if (AreEquals(command, "TreeRemove"))
	{
		status = TreeControllerRemove(interface->treeController, interface->parser,interface->outputFile);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
	}
	else if (AreEquals(command, "TreeSearch"))
	{
		status = TreeControllerSearch(interface->treeController, interface->parser, interface->outputFile);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
	} else if (AreEquals(command, "TreeInsert"))
	{
		status = TreeControllerInsert(interface->treeController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status || PARSER_READ_FAIL == status)
			{
				goto CleanUp;
			}
			if (INVALID_INPUT == status || CURRENT_STRUCTURE_UNDEFINED == status)
			{
				ParserFlush(interface->parser);
			}

		}
	}else if (AreEquals(command, "TreeRead"))
	{
		status = TreeControllerRead(interface->treeController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
			if (INVALID_INPUT == status)
			{
				ParserFlush(interface->parser);
			}
		}
	}
	else if (AreEquals(command, "TreeGoTo"))
	{
		status = TreeControllerGoTo(interface->treeController, interface->parser);
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
		printMsg(status, interface->outputFile);


		}
	else if (AreEquals(command, "HeapRemove"))
	{
		status = HeapControllerRemove(interface->heapController, interface->parser, interface->outputFile);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status || PARSER_READ_FAIL == status)
			{
				goto CleanUp;
			}
			if (INVALID_INPUT == status || CURRENT_STRUCTURE_UNDEFINED == status)
			{
				ParserFlush(interface->parser);
			}

		}
	}
	else if (AreEquals(command, "HeapInsert"))
	{
		status = HeapControllerInsert(interface->heapController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status || PARSER_READ_FAIL == status)
			{
				goto CleanUp;
			}
			if (INVALID_INPUT == status || CURRENT_STRUCTURE_UNDEFINED == status)
			{
				ParserFlush(interface->parser);
			}

		}
	}
	else if (AreEquals(command, "HeapRead"))
	{
		status = HeapControllerRead(interface->heapController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
			if (INVALID_INPUT == status)
			{
				ParserFlush(interface->parser);
			}
		}
	}
	else if (AreEquals(command, "HeapGoTo"))
	{
		status = HeapControllerGoTo(interface->heapController, interface->parser);
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
		printMsg(status, interface->outputFile);


	}
	else if (AreEquals(command, "HashTableSearch"))
	{
		status = HashTableControllerSearch(interface->hashtableController, interface->parser,interface->outputFile);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
	}
	else if (AreEquals(command, "HashTableGoTo"))
	{
		status = HashTableControllerGoTo(interface->hashtableController, interface->parser);
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
		printMsg(status, interface->outputFile);


	}
	else if (AreEquals(command, "HashTableRead"))
	{
		status = HashTableControllerRead(interface->hashtableController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
			if (INVALID_INPUT == status)
			{
				ParserFlush(interface->parser);
			}
		}
	} 
	else if (AreEquals(command, "ListSort"))
	{
		status = ListControllerMergeSort(interface->listController);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}
	}
	else if (AreEquals(command, "ListMerge"))
	{
		status = ListControllerMerge(interface->listController);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}
	}
	else if (AreEquals(command, "ListGoTo"))
	{
		status = ListControllerGoTo(interface->listController, interface->parser);
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
		printMsg(status, interface->outputFile);

	}
	else if (AreEquals(command, "ListAdd"))
	{
		status = ListControllerAdd(interface->listController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status || PARSER_READ_FAIL == status)
			{
				goto CleanUp;
			}
			if (INVALID_INPUT == status || CURRENT_STRUCTURE_UNDEFINED == status)
			{
				ParserFlush(interface->parser);
			}

		}
	}
	else if (AreEquals(command, "ListPrint"))
	{
		status = ListControllerPrint(interface->listController, interface->parser, interface->outputFile);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}

	}
	else if (AreEquals(command, "ListRead"))
	{
		status = ListControllerRead(interface->listController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
			if (INVALID_INPUT == status)
			{
				ParserFlush(interface->parser);
			}
		}
	}
	else if (AreEquals(command, "VectorSearch"))
	{
		status = VectorControllerSearch(interface->vectorController, interface->parser,interface->outputFile);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
	}
	else if (AreEquals(command, "VectorRemoveValue"))
	{
		status = VectorControllerRemoveValue(interface->vectorController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
	}
	else if (AreEquals(command, "VectorRemovePosition"))
	{
		status = VectorControllerRemovePosition(interface->vectorController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}
		if (INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
	}
	else if (AreEquals(command, "VectorLength"))
	{
		status = VectorControllerLength(interface->vectorController,  interface->outputFile);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}

	}


	else if (AreEquals(command, "VectorRead"))
	{
		status = VectorControllerRead(interface->vectorController, interface->parser);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
			if (INVALID_INPUT == status)
			{
				ParserFlush(interface->parser);
			}
		}	
	}
	else if (AreEquals(command, "VectorPrint"))
	{
		status = VectorControllerPrint(interface->vectorController, interface->parser,interface->outputFile);
		if (SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status)
			{
				goto CleanUp;
			}
		}

	}
	else if(AreEquals(command,"VectorAdd"))
	{
		status = VectorControllerAdd(interface->vectorController, interface->parser);
		if(SUCCES != status)
		{
			printMsg(status, interface->outputFile);
			if (MALLOC_ERROR == status || PARSER_READ_FAIL == status)
			{
				goto CleanUp;
			}
			if(INVALID_INPUT == status || CURRENT_STRUCTURE_UNDEFINED == status)
			{
				ParserFlush(interface->parser);
			}
			
		}
	}
	else if(AreEquals(command,"VectorGoTo"))
	{
		status = VectorControllerGoTo(interface->vectorController,interface->parser);
		if(INVALID_INPUT == status)
		{
			ParserFlush(interface->parser);
		}
		printMsg(status, interface->outputFile);

	}
	else if (STRING_READED_END_OF_FILE == readStatus && !(IsCommandWithoutInput(command)))
	{
		status = END_FILE;
		fprintf(interface->parser->fileDescriptor, "INVALID_INPUT\n");
		goto CleanUp;
	}
	else
	{
		fprintf(interface->outputFile, "INVALID_COMMAND\n");
		status = SUCCES;
	}

CleanUp:
	if (END_LINE == status || END_LINE_NO_NUMBER == status || NUMBER_READED_END_OF_LINE == status || INVALID_INPUT_END_OF_LINE == status)
	{
		status = SUCCES;
	}
	if(INVALID_INPUT == status)
	{
		status = SUCCES;
	}
	if (STRING_READED_END_OF_LINE == status)
	{
		status = SUCCES;
	}
	if(NUMBER_READED_END_OF_FILE == status)
	{
		status = END_FILE;
	}
	if(INVALID_INPUT_END_OF_LINE == status)
	{
		status = SUCCES;
	}
	if(INDEX_OUT_OF_BOUNDS == status)
	{
		status = SUCCES;
	}
	if(INDEX_OUT_OF_BOUNDS_END_FILE == status)
	{
		status = END_FILE;
	}
	if(CURRENT_STRUCTURE_UNDEFINED == status)
	{
		status = SUCCES;
	}
	if(ELEMENT_NOT_FOUND == status)
	{
		status = SUCCES;
	}
	if(INVALID_COMMAND == status)
	{
		status = SUCCES;
	}
	if(PREVIOUS_STRUCTURE_UNDEFINED == status)
	{
		status = SUCCES;
	}
	if(STRUCTS_LIMIT_REACHED == status)
	{
		status = SUCCES;
	}
	if(NUMBER_OF_STRUCTURES_EXCEEDED == status)
	{
		status = SUCCES;
		if(!IsCommandWithoutInput(command))
		{
			ParserFlush(interface->parser);
		}
	}
	return status;
}



STATUS_CODE InterfaceRunTest(int nr)
{
	STATUS_CODE status;
	PINTERFACE interface;
	char* command;
	char* file;

	status = SUCCES;
	interface = NULL;
	command = NULL;
	file = NULL;

	status = GenerateName(&file, nr);
	if (SUCCES != status)
	{
		goto CleanUp;
	}

	status = InterfaceCreate(&interface, file);
	if (SUCCES != status)
	{
		printf("Err at test : %s\n", file);
		goto CleanUp;
	}
	
	while (SUCCES == status)
	{
		status = ParseNextString(interface->parser, &command);

		if(END_LINE_NO_STRING == status)
		{
			fprintf(interface->outputFile, "IVALID_COMMAND\n");
			status = SUCCES;
			continue;
		}
		if (END_FILE_NO_STRING == status)
		{
			//fprintf(interface->outputFile, "INVALID_COMMAND\n");
			//printMsg(status, interface->outputFile);
			status = END_FILE;
			goto CleanUp;
		}
		if (END_FILE == status || INVALID_PCHAR_POINTER == status || PARSER_READ_FAIL == status || MALLOC_ERROR == status)
		{

			//printMsg(status, interface->outputFile);
			goto CleanUp;
		}
		else if (status == INVALID_INPUT)
		{
			fprintf(interface->outputFile,"INVALID_COMMAND\n");

			if (END_FILE == status)
			{

				status = SUCCES;

				goto CleanUp;
			}
			status = SUCCES;

			status = ParserFlush(interface->parser);
			if(END_LINE_NO_STRING == status || END_LINE == status)
			{
				status = SUCCES;
			}
			
		}
		else if(status == INVALID_INPUT_END_OF_LINE)
		{
			//printMsg(status, interface->outputFile);
			fprintf(interface->outputFile, "INVALID_COMMAND\n");
			status = SUCCES;
		}
		else if(INVALID_INPUT_END_OF_FILE == status)
		{
			//printMsg(status, interface->outputFile);
			//fprintf(interface->outputFile, "INVALID_INPUT\n");
			fprintf(interface->outputFile, "INVALID_COMMAND\n");
			status = SUCCES;

			goto CleanUp;
		}
		else
		{
			if (STRING_READED_END_OF_FILE == status )
			{
				status = TreatCommand(interface, command, status);
				goto CleanUp;
			}
 		    status = TreatCommand(interface, command, status);
			
		}

		free(command);
		command = NULL;
	
		if (STRING_READED_END_OF_FILE == status)
		{
			goto CleanUp;
		}
		if ( END_FILE == status)
		{
			status = SUCCES;
			break;
		}
	}

CleanUp:

	if (status != SUCCES)
	{
		printMsgInternalErr(status,stderr);
	}

	free(file);
	free(command);
	InterfaceDestroy(&interface);
	return status;
}

STATUS_CODE InterfaceRunMultipleTests(char* startTest, char* endTest)
{
	STATUS_CODE status;
	int first, last;
	char* test;

	status = SUCCES;
	StringToInt(startTest,&first);
	StringToInt(endTest, &last);
	test = NULL;

	while (first < last)
	{
		status = InterfaceRunTest(first);
		if (status != SUCCES)
		{
			printMsgInternalErr(status, stderr);
		}
		first++;
		free(test);
		test = NULL;
	}

	free(test);
	test = NULL;
	return status;
}


STATUS_CODE InterfaceRunAllTests()
{
	STATUS_CODE status;
	char* testFileName;
	int index;
	int errCode;
	FILE* testFile;
	errCode = -1;
	status = SUCCES;
	index = 1;
	char* file;
	do
	{
		GenerateName(&file, index);
		status = CreateFileName(&testFileName, file, IN_FILES_PATH, ".in");
		if (SUCCES != status)
		{
			goto CleanUp;
		}
		errCode = fopen_s(&testFile, testFileName, "r");
		if(errCode == 0)
		{
			fclose(testFile);
			InterfaceRunTest(index);
			index++;
		}
		free(file);
		free(testFileName);
		file = NULL;
		testFileName = NULL;
	} while (errCode == 0);
	
	
CleanUp:
	return status;
}