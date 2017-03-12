#ifndef _DYNAMICVECTOR_H_
#define _DYNAMICVECTOR_H_
#include "ExitCodeStatus.h"
#include <stdio.h>
typedef struct {
	int *v;
	int size;
	int capacity;
} DYNAMIC_VECTOR, *PDYNAMIC_VECTOR;

/**
  *Returns STATUS_CODE: MALLOC_ERROR
						SUCCES
 */
int VectorCreate(PDYNAMIC_VECTOR *dynamicVector);

/**
  *Returns STATUS_CODE: INVALID_VECTOR_POINTER
						INVALID_PVECTOR_POINTER
						SUCCES
 */
int VectorDestroy(PDYNAMIC_VECTOR *);

/**
 *Returns STATUS_CODE : SUCCES 
						INVALID_VECTOR_POINTER
*/
int VectorAdd(PDYNAMIC_VECTOR, int);

/**
	*Returns INT: size of vector
*/
int VectorLength(DYNAMIC_VECTOR);

/**
  * Returns STATUS_CODE: INDEX_OUT_OF_BOUNDS
						 SUCCES
						 INVALID_VECTOR_POINTER
 */
int VectorRemovePosition(PDYNAMIC_VECTOR, int);

/**
  * Return STATUS_CODE: ELEMENT_NOT_FOUND
						INVALID_VECTOR_POINTER
						SUCCES
  */
int VectorRemoveValue(PDYNAMIC_VECTOR, int);

int VectorGet(DYNAMIC_VECTOR,int,int*);


int VectorPrint(DYNAMIC_VECTOR v, FILE* fileDescriptor);

/**
 * @Returns: STATUS_VALUE: SUCCES - find element
 *						   NOT_FOUND
 *						   INVALID_VECTOR_POINTER
 */
STATUS_CODE VectorSearch(PDYNAMIC_VECTOR v, int value, int *position);
#endif // !_HEADER_H_
