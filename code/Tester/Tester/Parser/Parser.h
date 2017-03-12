#ifndef _PARSER_H_
#define _PARSER_H_
#include <stdio.h>
#include <ExitCodeStatus.h>
#include "ParserExitCode.h"
#include "..\StringHelper\StringHelper.h"
typedef struct {
	char buffer[PARSER_DIMENSION];
	FILE* fileDescriptor;
	int currentIndex;
	int size;
	int firstRead;
} PARSER, *PPARSER;

STATUS_CODE ParserCreate(PPARSER *parser, char *fileName);

STATUS_CODE ParserDestroy(PPARSER *parser);
STATUS_CODE ParserRead(PARSER *parser);

STATUS_CODE ParseNextInt(PARSER *parser, int *nr);

STATUS_CODE SkipSpaces(PARSER *parser);

STATUS_CODE ParseNextString(PARSER *parser, char** buffer);

STATUS_CODE ParserFlush(PARSER *parser);

BOOLEAN IsDelimitator(char c);

BOOLEAN IsEnd(char c);

BOOLEAN IsSpace(char c);

#endif //!_PARSER_H_
