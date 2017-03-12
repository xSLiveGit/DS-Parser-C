#ifndef _HELPER_CONTROLLER_H_
#define _HELPER_CONTROLLER_H_

#include <ExitCodeStatus.h>
#include "../../Parser/Parser.h"

BOOLEAN IsExitClause(STATUS_CODE status);
BOOLEAN MustFreeResources(STATUS_CODE status);

#endif //!_HELPER_CONTROLLER_H_