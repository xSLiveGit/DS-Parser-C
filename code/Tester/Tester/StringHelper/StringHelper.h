#ifndef _STRING_HELPER_H_
#define _STRING_HELPER_H_
#include <stdlib.h>
#include "ExitCodeStatus.h"
#include "..\Parser\ParserExitCode.h"
int GetLength(char* string);
BOOLEAN AreEquals(char *s1, char* s2);
STATUS_CODE StringToInt(char* s1, int *number);
STATUS_CODE IntToString(int number, char ** result);
int CompareNumbersAsString(char* firstNumber, char* secondNumber);
BOOLEAN IsNumber(char* number);
STATUS_CODE StrCat(char** destination, char* source);
#endif // !_STRING_HELPER_H_
