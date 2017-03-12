#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#include "DynamicVector.h"
#include "StringHelper\StringHelper.h"
#include "Parser\Parser.h"
#include "Interface\Interface.h"
int main(int argc, char* argv[])
{
	STATUS_CODE status;
	status = SUCCES;
	int nr;
	
	if (argc == 2)
	{
		if(AreEquals("runall",argv[1]))
		{
			status = InterfaceRunAllTests(); 
			goto CleanUp;
		}
		status = StringToInt(argv[1], &nr);
		if(SUCCES != status)
		{
			fprintf_s(stderr, "Invalid argument!\n");
			goto CleanUp;
		}
		status = InterfaceRunTest(nr);
	}
	else if(argc == 3)
	{
		status = InterfaceRunMultipleTests(argv[1], argv[2]);
	}
	else
	{
		fprintf_s(stderr, "Invalid argument!\n");
	}
	CleanUp:
	if(END_FILE_NO_STRING == status
		|| END_FILE_NO_NUMBER  == status
		|| END_FILE == status
		|| STRING_READED_END_OF_FILE == status
		|| INVALID_INPUT_END_OF_FILE == status
		|| END_FILE_NO_STRING == status)
	{
		status = SUCCES;
	}
	_CrtDumpMemoryLeaks();
	return status;
}