#include "StringHelper.h"
BOOLEAN AreEquals(char *s1, char* s2)
{
	int index;
	BOOLEAN equals;

	equals = TRUE;
	index = 0;

	if (NULL == s1 || NULL == s2)
	{
		return FALSE;
	}

	while (equals)
	{
		if (s2[index] == '\0' && s1[index] == '\0')//if both of given strings are endings on current positon -> they're equl
		{
			return TRUE;
		}
		if (s2[index] != s1[index])
		{
			return FALSE;
		}
		index++;
	}
	return TRUE;
}

STATUS_CODE StringToInt(char * s1, int *numar)
{

	int index;
	STATUS_CODE exitCode;

	exitCode = SUCCES;
	*numar = 0;
	index = 0;

	if (NULL == s1)
	{
		exitCode = INVALID_STRING_POINTER;
		goto CleanUp;
	}

	if (NULL == numar)
	{
		exitCode = INVALID_OUTPUT_PARAMETER_POINTER;
		goto CleanUp;
	}

	if ('-' == s1[0])
	{
		index++;
	}
	while ('\0' != s1[index])
	{
		*numar = *numar * 10 + (s1[index] - '0');
		index++;
	}
	if ('-' == s1[0])
	{
		*numar = *numar * (-1);
	}
CleanUp:
	return exitCode;
}

STATUS_CODE IntToString(int number, char ** result)
{
	int size;
	int miroredNumber;
	int i;
	STATUS_CODE exitCode;

	exitCode = SUCCES;
	size = 0;
	miroredNumber = 0;

	if (NULL == result)
	{
		exitCode = INVALID_OUTPUT_PARAMETER_POINTER;
		goto CleanUp;
	}

	do {
		size++;
		miroredNumber = miroredNumber * 10 + number % 10;
		number = number / 10;
	} while (0 != number);

	*result = (char*)malloc((size + 1)*sizeof(char));

	for (i = 0; i < size; i++)
	{
		(*result)[i] = '0' + (miroredNumber % 10);
		miroredNumber = miroredNumber / 10;
	}

	(*(result))[size] = '\0';
CleanUp:
	return exitCode;
}

int GetLength(char* string)
{
	int size;
	size = 0;
	if (NULL == string)
		return 0;
	while (string[size] != '\0')
	{
		size++;
	}
	return size;
}

BOOLEAN IsNumber(char* number)
{
	BOOLEAN result;
	static char minInt[] = "-2147483648";
	static char maxInt[] = "2147483647";
	int index;
	int compareResult;

	index = 0;
	result = TRUE;
	if (number[index] == '\0')//other character different by end of string
	{
		result = FALSE;
		goto CleanUp;
	}
	if (number[index] == '-')
	{
		index++;
	}

	while ((number[index] >= '0') && (number[index] <= '9'))
	{
		index++;
	}

	if (index < GetLength(number) )
	{
		return FALSE;
	}
	compareResult = CompareNumbersAsString(number, minInt);
	if (-1 == compareResult)
	{
		result = FALSE;
		goto CleanUp;
	}

	compareResult = CompareNumbersAsString(number, maxInt);
	if (1 == compareResult)
	{
		result = FALSE;
		goto CleanUp;
	}

CleanUp:
	return result;
}

int CompareNumbersAsString(char* firstNumber,char* secondNumber)
{
	int index;
	int result;

	index = 0;
	result = 0;

	int firstLen = GetLength(firstNumber);
	int secondLen = GetLength(secondNumber);

	if (firstNumber[0] != '-' && secondNumber[0] == '-')//1st >= 0 2nd <= 0
	{
		result = 1;
		goto CleanUp;
	}

	if (firstNumber[0] == '-' && secondNumber[0] != '-')//1st <=0 2nd >=0
	{
		result = -1;
		goto CleanUp;
	}

	if (firstLen > secondLen)
	{
		result = 1;
		goto CleanUp;
	}

	else if (secondLen > firstLen)
	{
		result = -1;
		goto CleanUp;
	}
	if (firstNumber[0] == '-')
	{
		index++;
	}
	//index is 0
	for (; index < firstLen;index++)
	{
		if (firstNumber[index] < secondNumber[index])
		{
			result = -1;
			goto CleanUp;
		}
		else if (firstNumber[index] > secondNumber[index])
		{
			result = 1;
			goto CleanUp;
		}
	}

CleanUp:
	if (('-' == firstNumber[0]) && ('-' == secondNumber[0]))
	{
		result = result * (-1);
	}
	return result;
}

STATUS_CODE StrCat(char** destination, char* source)
{
	STATUS_CODE status;
	char* temp;
	int size;
	int i;
	int destinationLength;

	i = 0;
	status = SUCCES;
	temp = NULL;
	size = 0;
	destinationLength = 0;

	if (NULL == destination)
	{
		status = INVALID_CHAR_POINTER;
		goto CleanUp;
	}
	
	destinationLength = GetLength(*destination);
	size = destinationLength + GetLength(source) + 1;

	if (NULL == *destination)//copy features
	{
		destinationLength = GetLength(*destination);
		*destination = (char*)malloc((size + 1)*sizeof(char));
		if (NULL == (*destination))
		{
			status = MALLOC_ERROR;
			goto CleanUp;
		}
		for (i = 0; i <= size; i++)
		{
			(*destination)[i] = source[i];
		}
		goto CleanUp;
	}

	

	temp = (char*)malloc(size * sizeof(char));
	if (NULL == temp)
	{
		status = MALLOC_ERROR;
		goto CleanUp;
	}

	for (i = 0; i < destinationLength; ++i)
	{
		temp[i] = (*destination)[i];
	}

	for (i = destinationLength; i < size; ++i)
	{
		temp[i] = source[i - destinationLength];
	}
	
	free(*destination);
	*destination = temp;

CleanUp:
	return status;
}

void BZero(char* buffer, int bufferSize)
{
	int i;
	
	i = 0;
	while (buffer[i] != '\0')
	{
		buffer[i] = 0;
	}
}

