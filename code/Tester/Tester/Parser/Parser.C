#include "Parser.h"

inline BOOLEAN IsDelimitator(char c)
{
	return ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\0'));
}

inline BOOLEAN IsEnd(char c)
{
	return ((c == '\n') || (c == '\0'));
}

inline BOOLEAN IsSpace(char c)
{
	return ((c == '\t') || (c == ' '));
}

inline BOOLEAN IsCharacter(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

inline BOOLEAN MustDezalocate(STATUS_CODE status)
{
	return ((SPACE_DETECTED == status)
		|| (STRING_OVERFLOW == status)
		|| (END_LINE_NO_STRING == status)
		|| (END_FILE_NO_STRING == status)
		|| (PARSER_READ_FAIL == status)
		);
}

inline BOOLEAN IsEndParser(PPARSER parser)
{
	return (IsEnd(parser->buffer[parser->currentIndex]));
}

inline BOOLEAN IsEndLineParser(PPARSER parser)
{
	return ('\n' == parser->buffer[parser->currentIndex]);
}

inline BOOLEAN IsEndFileParser(PPARSER parser)
{
	return ('\0' == parser->buffer[parser->currentIndex]);
}

STATUS_CODE ParserCreate(PPARSER * parser, char * fileName)
{
	STATUS_CODE status;
	status = SUCCES;
	if (NULL == parser)
	{
		status = INVALID_PPARSER_POINTER;
		goto Exit;
	}
	if (NULL == fileName)
	{
		status = INVALID_FILE_POINTER;
		goto Exit;
	}
	(*parser) = (PPARSER)malloc(sizeof(PARSER));

	if (NULL == (*parser))
	{
		status = MALLOC_ERROR;
		goto Exit;
	}

	(*parser)->fileDescriptor = fopen(fileName, "r");
	if (NULL == (*parser)->fileDescriptor)
	{
		status = FILE_OPEN_ERROR;
		goto CleanUp;
	}
	(*parser)->currentIndex = 0;
	(*parser)->firstRead = 0;
	(*parser)->size = 0;
	goto Exit;

CleanUp :
	free(*parser);
	(*parser) = NULL;

Exit:
	return status;
}

STATUS_CODE ParserDestroy(PPARSER * parser)
{
	STATUS_CODE status;

	status = SUCCES;

	if (NULL == parser)
	{
		status = INVALID_PPARSER_POINTER;
		goto CleanUp;
	}

	if (NULL != (*parser)->fileDescriptor)
	{
		fclose((*parser)->fileDescriptor);
	}
	free((*parser)->buffer);

CleanUp:
	return status;
}

STATUS_CODE ParserRead(PARSER * parser)
{
	STATUS_CODE status;
//	int bytesReaded;
	status = SUCCES;

	if (NULL == parser)
	{
		status = INVALID_PARSER_POINTER;
		goto CleanUp;
	}

	parser->size = fread_s(parser->buffer, PARSER_DIMENSION, sizeof(char), PARSER_DIMENSION, parser->fileDescriptor);
	parser->currentIndex = 0;
	if (parser->size < PARSER_DIMENSION)
	{
		parser->buffer[parser->size] = '\0';
		goto CleanUp;
	}

	if (feof(parser->fileDescriptor))
	{
		status = END_FILE;
	}
CleanUp:
	return status;
}

STATUS_CODE SkipSpaces(PARSER *parser)
{
	STATUS_CODE status;
	
	if (NULL == parser)
	{
		status = INVALID_PARSER_POINTER;
		goto CleanUp;
	}

	if (IsEnd(parser->buffer[parser->currentIndex]))
	{
		if ('\n' == parser->buffer[parser->currentIndex])
		{
			status = NO_SPACES_WITH_END_LINE;
		}
		else
		{
			status = NO_SPACES_WITH_END_FILE;
		}
		goto CleanUp;
	}

	if (!IsSpace(parser->buffer[parser->currentIndex]))
	{
		status = NO_SPACES;
		goto CleanUp;
	}


	while (IsSpace(parser->buffer[parser->currentIndex]) && (!IsEnd(parser->buffer[parser->currentIndex])))
	{
		parser->currentIndex++;
		//@TODO:DUPA VECTOR FUNCTIONAL
		if (parser->size == parser->currentIndex)
		{
			ParserRead(parser);
		}
	}
	
	
	//@TODO:DUPA VECTOR FUNCTIONAL
	if (parser->size == parser->currentIndex)
	{
		ParserRead(parser);
	}
	if (IsEnd(parser->buffer[parser->currentIndex]))
	{
		if ('\n' == parser->buffer[parser->currentIndex])
		{
			status = END_LINE;
		}
		else
		{
			status = END_FILE;
		}
		goto CleanUp;
	}

	status = SUCCES;
CleanUp:
	return status;
}

/**
 * Returns STATUS_CODE: 
						INVALID_PCHAR_POINTER y
						END_FILE y
						INVALID_INPUT
						MALLOC_ERROR y
						PARSER_READ_FAIL y
						STRING_READED_END_OF_LINE
						STRING_READED_END_OF_FILE
 */
STATUS_CODE ParseNextString(PARSER *parser, char** buffer)
{
	STATUS_CODE status;
	int stringSize = 0;
	status = SUCCES;
	STATUS_CODE newStatus = SUCCES;
	if (NULL == buffer)
	{
		status = INVALID_PCHAR_POINTER;
		goto CleanUp;
	}
	if (parser->firstRead == 0)
	{
		status = ParserRead(parser);
		parser->firstRead = 1;
		if (parser->buffer[parser->currentIndex] == '\0')
		{
			goto CleanUp;
		}
	}

	if ('\n' == parser->buffer[parser->currentIndex])
	{
		if (parser->currentIndex < parser->size - 1)
		{
			parser->currentIndex++;
		}
		else 
		{
			status = ParserRead(parser);
		}
		
	}
	if (IsSpace(parser->buffer[parser->currentIndex]))
	{
		status = INVALID_INPUT;
		goto CleanUp;
	}
	*buffer = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
	if (NULL == *buffer)
	{
		status = MALLOC_ERROR;
		goto CleanUp;
	}

	//iteram buffer-ul 
	while (1)
	{
		// end of file buffer 
		if (parser->currentIndex == PARSER_DIMENSION)//invalid index
		{

			status = ParserRead(parser);
			if (status == PARSER_READ_FAIL)
			{
				goto CleanUp;
			}
		}

		// we got a delimiter
		if (0 == parser->size || IsDelimitator(parser->buffer[parser->currentIndex]) == TRUE)
		{
			if (IsSpace(parser->buffer[parser->currentIndex]))
			{
				status = SPACE_DETECTED;
				goto CleanUp;
			}
			if (stringSize > 0)
			{
				if ('\n' == parser->buffer[parser->currentIndex])
				{
					status = STRING_READED_END_OF_LINE;
				}
				else if ('\0' == parser->buffer[parser->currentIndex])
				{
					status = STRING_READED_END_OF_FILE;
				}
			}
			else 
			{
				if ('\n' == parser->buffer[parser->currentIndex])
				{
					status = END_LINE_NO_STRING;
				}
				else if ('\0' == parser->buffer[parser->currentIndex])
				{
					status = END_FILE_NO_STRING;
				}
			}
			break;
		}

		if (stringSize + 1== MAX_STRING_SIZE)
		{
			status = INVALID_COMMAND;
			goto CleanUp;
		}
		if (parser->buffer[parser->currentIndex] == '\0')
		{
			(*buffer)[stringSize] = '\0';
			return END_FILE;
		}
		if (stringSize + 1 == MAX_STRING_SIZE)
		{
			status = INVALID_INPUT;
			goto CleanUp;
		}
		(*buffer)[stringSize] = parser->buffer[parser->currentIndex];
		stringSize++;
		parser->currentIndex++;
	}

CleanUp:
	if (MustDezalocate(status))
	{
		free(*buffer);
		*buffer = NULL;
	}
	else 
	{
		if(NULL != *buffer)
		{
			(*buffer)[stringSize] = '\0';
		}
	}
	return status;
}

//ma duce pe\n sau \0
STATUS_CODE ParserFlush(PARSER * parser)
{
	STATUS_CODE status;
	status = SUCCES;
	
	if (parser->buffer[parser->currentIndex] == '\n' && parser->size >= parser->currentIndex)
	{
		parser->currentIndex++;
	}
	while (1)
	{
		if (parser->size == parser->currentIndex)
		{
			if (status == END_FILE || status == PARSER_READ_FAIL)
			{
				break;
			}
			status = ParserRead(parser);
		}
		
		if (!IsEnd(parser->buffer[parser->currentIndex])) {
			parser->currentIndex++;
		}
		else {
			break;
		}
	}
	return status;
}

/**
  *Returns STATUS_CODE: END_LINE_NO_NUMBER
						END_FILE_NO_NUMBER
						MALLOC_ERROR
						NUMBER_READED_END_OF_LINE
						NUMBER_READED_END_OF_FILE
						INVALID_INPUT
						PARSER_READ_FAIL
						SUCCES
						INVALID_INPUT_END_OF_LINE
						INVALID_INPUT_END_OF_FILE
						INVALID_INPUT- not at end of line/file => need flush
 */
STATUS_CODE ParseNextInt(PARSER *parser, int *nr)
{
	STATUS_CODE newStatus;
	STATUS_CODE status;
	int stringSize = 0;

	status = SUCCES;
	char* buffer = NULL;
	newStatus = SUCCES;
	if ('\n' == parser->buffer[parser->currentIndex])
	{
		if (parser->currentIndex < parser->size - 1)
		{
			parser->currentIndex++;
		}
		else
		{
			status = ParserRead(parser);
		}
	}

	if (IsSpace(parser->buffer[parser->currentIndex]))
	{
		status = SkipSpaces(parser);
		if (IsEndParser(parser))
		{
			if (IsEndLineParser(parser))
			{
				status = END_LINE_NO_NUMBER;
				goto CleanUp;
			}
			else
			{
				status = END_FILE_NO_NUMBER;
				goto CleanUp;
			}
		}
		
	}
	buffer = NULL;
	buffer = (char*)malloc(MAX_NUMBER_SIZE * sizeof(char));
	if (NULL == buffer)
	{
		status = MALLOC_ERROR;
		goto CleanUp;
	}

	//iteram buffer-ul 
	while (1)
	{
		// end of file buffer 
		if (parser->currentIndex == parser->size)//invalid index
		{
			status = ParserRead(parser);
			if (status == PARSER_READ_FAIL)
			{
				goto CleanUp;
			}
		}

		// we got a delimiter
		if (0 == parser->size || IsDelimitator(parser->buffer[parser->currentIndex]) == TRUE)
		{
			if (stringSize > 0)
			{
				if ('\n' == parser->buffer[parser->currentIndex])
				{
					status = NUMBER_READED_END_OF_LINE;
				}
				else if ('\0' == parser->buffer[parser->currentIndex])
				{
					status = NUMBER_READED_END_OF_FILE;
				}
			}
			else
			{
				if ('\n' == parser->buffer[parser->currentIndex])
				{
					status = END_LINE_NO_NUMBER;
				}
				else if ('\0' == parser->buffer[parser->currentIndex])
				{
					status = END_FILE_NO_NUMBER;
				}
			}
			break;
		}

			if (stringSize + 1== MAX_NUMBER_SIZE)
			{
				status = INVALID_INPUT;
				goto CleanUp;
			}

		buffer[stringSize] = parser->buffer[parser->currentIndex];
		stringSize++;
		if (stringSize + 1 == MAX_NUMBER_SIZE)
		{
			status = INVALID_INPUT;//need flush
			goto Overflow;
		}
		parser->currentIndex++;
	}

CleanUp:
	if (!MustDezalocate(status))
	{
		buffer[stringSize] = '\0';
		if (IsNumber(buffer) == TRUE)
		{
			newStatus = StringToInt(buffer, nr);
			if (newStatus != SUCCES)
			{
				if (status == NUMBER_READED_END_OF_FILE)
				{
					status = INVALID_INPUT_END_OF_FILE;
				}
				else if (status == NUMBER_READED_END_OF_LINE)
				{
					status = INVALID_INPUT_END_OF_LINE;
				}
			}
		}
		else
		{
			if(NUMBER_READED_END_OF_LINE == status)
			{
				status = INVALID_INPUT_END_OF_LINE;
			}
			else if(NUMBER_READED_END_OF_FILE == status)
			{
				status = INVALID_INPUT_END_OF_FILE;
			}
			else
			{
				status = INVALID_INPUT;
			}
		}
	}
	Overflow:
	free(buffer);
	return status;

}

