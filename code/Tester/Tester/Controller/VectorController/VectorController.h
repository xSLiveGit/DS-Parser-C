#ifndef _VECTOR_CONTROLLER_H_
#define _VECTOR_CONTROLLER_H_
#include "../ControllerStatus.h"
#include "DynamicVector.h"
#include "../../Parser/Parser.h"
#include "stdlib.h"
#include "../HelperController/HelperController.h"

typedef struct
{
	PDYNAMIC_VECTOR vectors[MAX_STRUCTS];
	int currentVector;
	int nrOfVectors;
} VECTOR_CONTROLLER, *PVECTOR_CONTROLLER;

STATUS_CODE VectorControllerCreate(PVECTOR_CONTROLLER *vectorController);
STATUS_CODE VectorControllerDestroy(PVECTOR_CONTROLLER *vectorController);
STATUS_CODE VectorControllerRead(PVECTOR_CONTROLLER vectorController, PPARSER parser);
STATUS_CODE VectorControllerGoTo(PVECTOR_CONTROLLER vectorController, PPARSER parser);
STATUS_CODE VectorControllerPrint(PVECTOR_CONTROLLER vectorController, PPARSER parser,FILE* out);
STATUS_CODE VectorControllerAdd(PVECTOR_CONTROLLER vectorController, PPARSER parser);
STATUS_CODE VectorControllerLength(PVECTOR_CONTROLLER vectorController, FILE *out);
STATUS_CODE VectorControllerRemovePosition(PVECTOR_CONTROLLER vectorController,PPARSER parser);
STATUS_CODE VectorControllerRemoveValue(PVECTOR_CONTROLLER vectorController, PPARSER parser);
STATUS_CODE VectorControllerSearch(PVECTOR_CONTROLLER vectorController, PPARSER parser, FILE* out);
#endif // !_VECTOR_CONTROLLER_H_
