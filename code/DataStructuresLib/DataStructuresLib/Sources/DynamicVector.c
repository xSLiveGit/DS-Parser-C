
#include "../Headers/DynamicVector.h"
#include <stdio.h> 
#include <stdlib.h>
#define INIT_VECTOR_CAPACITY 1

STATUS_CODE
VectorCreate(PDYNAMIC_VECTOR *dynamicVector)
{
	DYNAMIC_VECTOR* dv;
	STATUS_CODE statusCode;

	statusCode = SUCCES;
	dv = NULL;
	if(NULL == dynamicVector)
	{
		statusCode = INVALID_PVECTOR_POINTER;
		goto CleanUp;
	}
	dv = (DYNAMIC_VECTOR*)malloc(sizeof(DYNAMIC_VECTOR));

	if (NULL == dv)
	{
		statusCode = MALLOC_ERROR;//malloc error
		goto CleanUp;
	}

	(*dv).capacity = INIT_VECTOR_CAPACITY;
	(*dv).size = 0;
	(*dv).v = (int*)malloc(INIT_VECTOR_CAPACITY * sizeof(int));
	//dv->v = NULL;
	if(NULL == (*dv).v)
	{
		statusCode = MALLOC_ERROR;
	}
	*dynamicVector = dv;
CleanUp:
	if (MALLOC_ERROR == statusCode)
	{
		free(dv);
	}
	return statusCode;
}

int
VectorDestroy(PDYNAMIC_VECTOR *v)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;
	DYNAMIC_VECTOR* wp;

	if (NULL == v)
	{
		return INVALID_PVECTOR_POINTER;
	}

	wp = *v;
	//if (NULL == *v)
	//{
	//	return INVALID_VECTOR_POINTER;
	

	if(NULL != wp && wp->v != NULL)
		free(wp->v);
	free(wp);
	*v = NULL;
	return SUCCES;//succes
}

int Resize(DYNAMIC_VECTOR *v) {
	int* nv;
	STATUS_CODE statusCode;
	int i;

	statusCode = SUCCES;
	nv = NULL;

	if (NULL == v)
	{
		statusCode = INVALID_VECTOR_POINTER;//invalid vector pointer
		goto CleanUp;
	}

	nv = (int*)malloc((v->capacity*2)*sizeof(int));
	if(NULL == nv)
	{
		statusCode = MALLOC_ERROR;
		goto CleanUp;
	}

	for (i = 0; i < v->size; i++)
	{
		nv[i] = v->v[i];
	}
	free(v->v);
	v->v = NULL;
	v->v = nv;
	v->capacity *= 2;
CleanUp:
	
	return statusCode;
}

STATUS_CODE VectorAdd(PDYNAMIC_VECTOR v, int el)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;

	if (NULL == v) 
	{
		statusCode = INVALID_VECTOR_POINTER;
		goto CleanUp;
	}
	
	if (v->size == v->capacity) 
	{
		Resize(v);
	}
	v->v[v->size] = el;
	v->size++;
CleanUp:
	return statusCode;
}

STATUS_CODE VectorLength(DYNAMIC_VECTOR v)
{
	return v.size;
}

STATUS_CODE VectorRemovePosition(PDYNAMIC_VECTOR v, int pos)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;

	int i;

	if (NULL == v)
	{
		statusCode = INVALID_VECTOR_POINTER;
		goto CleanUp;
	}

	if (pos < 0 || pos >= v->size)
	{
		statusCode = INDEX_OUT_OF_BOUNDS;
		goto CleanUp;
	}

	else
	{
		for (i = pos; i < v->size - 1; i++)
		{
			v->v[i] = v->v[i + 1];
		}
		v->size--;
	}

CleanUp:
	return statusCode;
}

STATUS_CODE VectorRemoveValue(PDYNAMIC_VECTOR v, int value)
{
	int i, j;
	STATUS_CODE statusCode;

	statusCode = ELEMENT_NOT_FOUND;

	if (NULL == v)
	{
		statusCode = INVALID_VECTOR_POINTER;
		goto CleanUp;
	}

	for (i = 0; i < v->size;)
	{
		if (v->v[i] == value)
		{
			statusCode = SUCCES;
			for (j = i; j < v->size - 1; j++)
			{
				v->v[j] = v->v[j + 1];
			}
			v->size--;
		}
		else
		{
			i++;
		}
	}
CleanUp:
	return statusCode;
}

int VectorGet(DYNAMIC_VECTOR v, int position, int *returned) {
	STATUS_CODE statusCode;

	statusCode = SUCCES;

	if (position < 0 || position > v.size) 
	{
		statusCode =  INDEX_OUT_OF_BOUNDS;
		goto CleanUp;
	}
	*returned = v.v[position];
CleanUp:
	return statusCode;
}


STATUS_CODE VectorSearch(PDYNAMIC_VECTOR v,int value,int *position)
{
	STATUS_CODE status;
	int i;

	status = ELEMENT_NOT_FOUND;
	
	if(NULL == v)
	{
		status = INVALID_VECTOR_POINTER;
		goto CleanUp;
	}
	
	for (i = 0; i < v->size;i++)
	{
		if(v->v[i] == value)
		{
			*position = i;
			status = SUCCES;
			goto CleanUp;
		}
	}

	CleanUp:
	return status;
}
int VectorPrint(DYNAMIC_VECTOR v,FILE* fileDescriptor)
{
	STATUS_CODE statusCode;
	int i;

	statusCode = SUCCES;

	if (NULL == fileDescriptor)
	{
		statusCode = FILE_OPEN_ERROR;
		goto CleanUp;
	}

	for (i = 0; i < v.size; i++)
	{
		fprintf(fileDescriptor, "%d ", v.v[i]);
		
	}
	fprintf(fileDescriptor, "\n");

	

	return SUCCES;
CleanUp:
	return statusCode;
}
