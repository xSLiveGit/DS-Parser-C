#include "HelperController.h"
BOOLEAN IsExitClause(STATUS_CODE status)
{
	return((END_LINE_NO_NUMBER == status)
		|| (END_FILE_NO_NUMBER == status)
		|| (MALLOC_ERROR == status)
		|| (PARSER_READ_FAIL == status)
		|| (NUMBER_READED_END_OF_FILE == status)
		|| (INVALID_INPUT_END_OF_FILE == status)
		|| (INVALID_INPUT_END_OF_LINE == status)
		|| (INVALID_INPUT == status)
		);
}

BOOLEAN MustFreeResources(STATUS_CODE status)
{
	return ((MALLOC_ERROR == status) || (PARSER_READ_FAIL == status));
}