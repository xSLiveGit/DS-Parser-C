#include "../Headers/Heap.h"

int MyHeapCreate(PMY_HEAP * heap)
{
	int statusCode;
	statusCode = SUCCES;
	
	if (NULL == heap)
	{
		statusCode = INVALID_PHEAP_POINTER;
		goto CleanUp;
	}

	*heap = (PMY_HEAP)malloc(sizeof(MY_HEAP));
	if(NULL == *heap)
	{
		statusCode = MALLOC_ERROR;
		goto CleanUp;
	}
	statusCode = VectorCreate(&((*heap)->vector));
	if(SUCCES != statusCode)
	{
		goto CleanUp;
	}
	(*heap)->size = 0;

CleanUp:
	if(SUCCES != statusCode)
	{
		if(*heap != NULL)
		{
			free((*heap)->vector);
			(*heap)->vector = NULL;
		}
		free(*heap);
		*heap = NULL;
	}
	return statusCode;
}

int MyHeapDestroy(PMY_HEAP * heap)
{
	int statusCode;

	statusCode = SUCCES;
	if(NULL == heap)
	{
		statusCode = INVALID_PVECTOR_POINTER;
		goto CleanUp;
	}
	if(NULL != (*heap)->vector)
	{
		statusCode = VectorDestroy(&(*heap)->vector);
		(*heap)->vector = NULL;
	}
	CleanUp:
	return statusCode;
}

inline int RightSon(int poz)
{
	return (2 * poz + 2);
}

inline int LeftSon(int poz)
{
	return (2 * poz + 1);
}

inline int Father(int poz)
{
	return ((poz -1) / 2);
}

STATUS_CODE Up(MY_HEAP *heap,int poz)
{
	int key;
	int father;
	int fatherEl;

	fatherEl = -1;
	STATUS_CODE statusCode = SUCCES;
	

	if (NULL == heap)
	{
		statusCode = INVALID_HEAP_POINTER;
		goto CleanUp;
	}
	if (poz < 0 || poz >= heap->vector->size)
	{
		statusCode = INDEX_OUT_OF_BOUNDS;
		goto CleanUp;
	}

	father = Father(poz);
	VectorGet(*heap->vector, poz, &key);
	VectorGet(*heap->vector, father, &fatherEl);

	while ((poz > 0) && fatherEl > key) {
		heap->vector->v[poz] = fatherEl;
		poz = father;
		father = Father(poz);
		VectorGet(*(heap->vector), father, &fatherEl);
	}

	heap->vector->v[poz] = key;

CleanUp:
	return statusCode;
}

STATUS_CODE Down(MY_HEAP *heap, int k)
{
	STATUS_CODE statusCode;
	int key;
	int size;
	int leftSonPosition;
	int rightSonPosition;
	int minSonPosition;
	int temp;
	int leftSonElement;
	int rightSonElement;
	int minSonElement;
	int currentElement;
	statusCode = SUCCES;
	
	if (NULL == heap)
	{
		statusCode = INVALID_HEAP_POINTER;
		goto CleanUp;
	}

	size = VectorLength(*(heap->vector));
	VectorGet(*heap->vector, k, &key);

	leftSonPosition = LeftSon(k);
	rightSonPosition = RightSon(k);
	minSonPosition = leftSonPosition;
	VectorGet(*heap->vector,leftSonPosition,&leftSonElement);
	VectorGet(*heap->vector, rightSonPosition, &rightSonElement);
	
	do {
		minSonPosition = -1;
		leftSonPosition = LeftSon(k);
		rightSonPosition = RightSon(k);
		VectorGet(*(heap->vector), leftSonPosition, &leftSonElement);
		VectorGet(*(heap->vector), rightSonPosition, &rightSonElement);
		// Alege un fiu mai mic ca tatal.
		if (leftSonPosition < heap->vector->size) {
			if (rightSonPosition <= heap->vector->size && rightSonElement < leftSonElement) 
			{
				minSonPosition = rightSonPosition;
			}
			else
			{
				minSonPosition = leftSonPosition;
			}
			VectorGet(*(heap->vector), minSonPosition, &minSonElement);
			VectorGet(*(heap->vector), k, &currentElement);
			if (minSonElement > currentElement) {
				minSonPosition = -1;
			}
		}
		if (minSonPosition != -1) {
			temp = heap->vector->v[minSonPosition];
			heap->vector->v[minSonPosition] = heap->vector->v[k];
			heap->vector->v[k] = temp;
			k = minSonPosition;
		}
	} while (minSonPosition != -1);

CleanUp:
	return statusCode;
}

STATUS_CODE MyHeapAdd(MY_HEAP *heap,int value)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;

	statusCode = VectorAdd(heap->vector,value);
	
	if (SUCCES != statusCode)
	{
		goto CleanUp;
	}

	statusCode = Up(heap, heap->vector->size - 1);
	heap->size++;
CleanUp:
	return statusCode;
}

STATUS_CODE MyHeapDelete(MY_HEAP * heap,int* returned)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;

	if (NULL == heap)
	{
		statusCode = INVALID_HEAP_POINTER;
		goto CleanUp;
	}

	if (0 == heap->size)
	{
		statusCode = ELEMENT_NOT_FOUND;
		goto CleanUp;
	}

	if (heap->size >= 1)
	{
		VectorGet(*(heap->vector), 0, returned);
		heap->vector->v[0] = heap->vector->v[heap->vector->size - 1];
		heap->vector->size--;
		Down(heap, 0);
		
		heap->size--;
	}
	else {//
		statusCode = INDEX_OUT_OF_BOUNDS;
	}
CleanUp:
	return statusCode;
}



