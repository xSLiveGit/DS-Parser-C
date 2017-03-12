#include "../Headers/List.h"

BOOLEAN IsEmptyList(LIST l )
{
	if (NULL == l.first)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/*
int CreateNode(PNod *nod)
{
	if (NULL == nod)
	{
		return INVALID_PNOD_POINTER;
	}
	*nod = (PNod)malloc(sizeof(Nod));//HERE
	if (NULL == (*nod))
	{
		return MALLOC_ERROR;
	}
	(*nod)->next = NULL;
	(*nod)->prev = NULL;
	(*nod)->value = 0;
	return SUCCES;
}*/

int CreateNode(PNOD * nod,int value)
{
	int statusCode;

	statusCode = SUCCES;

	if (NULL == nod)
	{
		statusCode = INVALID_PNOD_POINTER;
		goto CleanUp;
	}
	(*nod) = (PNOD)malloc(sizeof(NOD));
	if (NULL == (*nod))
	{
		
		statusCode = MALLOC_ERROR;
		goto CleanUp;
	}
	(*nod)->next = NULL;
	(*nod)->prev = NULL;
	(*nod)->value = value;
	

CleanUp:
	return statusCode;
}


int CreateList(LIST *l)
{
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	return SUCCES;
}

int ListAdd(LIST *l , int el)
{
	PNOD nod;
	int statusCode;

	statusCode = SUCCES;
	nod = NULL;

	if (NULL == l)
	{
		statusCode = INVALID_LIST_POINTER;
		goto CleanUp;
	}


	statusCode = CreateNode(&nod,el);

	if (0 == statusCode)
	{
		if (IsEmptyList(*l))
		{
			l->first = nod;
			l->last = nod;
			l->size = 1;
			
		}
		else
		{
			if (l->first != NULL)
			{
				l->last->next = nod;
				nod->prev = l->last;
				l->last = nod;
				l->size = (l->size) + 1;
			}
		}
	}

CleanUp:
	return statusCode;
}


/*
*	Function will destroy l1 & l2 configuration	
*/
int ListMerge(LIST *l1, LIST *l2, LIST *outputList)
{
	PNOD nod1, nod2,nodAux;
	int statusCode;

	statusCode = SUCCES;

	if (NULL == l1 || NULL == l2)
	{
		statusCode = INVALID_LIST_POINTER;
		goto CleanUp;
	}

	nod1 = (*l1).first;
	nod2 = (*l2).first;

	if ((NULL != nod1) && (nod1->value < nod2->value))
	{
		CreateNode(&nodAux, nod1->value);
		outputList->first = nodAux;
		outputList->last = nodAux;
		nod1 = nod1->next;
	}
	else if((NULL != nod2) && (nod2->value <= nod1->value))
	{
		CreateNode(&nodAux, nod2->value);
		outputList->first = nodAux;
		outputList->last = nodAux;
		nod2 = nod2->next;
	}
	else
	{
		outputList->first = NULL;
		outputList->last = NULL;
		outputList->size = 0;
		goto CleanUp;
	}

	while ((NULL != nod1) && (NULL != nod2))
	{
		if (nod1->value < nod2->value)
		{
			//nodAux = nod1;
			CreateNode(&nodAux, nod1->value);
			nod1 = nod1->next;
		}
		else
		{
			//nodAux = nod2;
			CreateNode(&nodAux,nod2->value);
			nod2 = nod2->next;
		}
		nodAux->prev = outputList->last;
		outputList->last->next = nodAux;
		outputList->last = nodAux;
	}
	
	while (NULL != nod1)//daca au mai ramas elemente in prima lista
	{
		//nodAux = nod1;
		CreateNode(&nodAux, nod1->value);
		nod1 = nod1->next;
		nodAux->prev = outputList->last;
		outputList->last->next = nodAux;
		outputList->last = nodAux;
	}

	while (NULL != nod2)//daca au mai ramas elemente in a doua lista
	{
		//nodAux = nod2;
		CreateNode(&nodAux,nod2->value);
		nod2 = nod2->next;
		nodAux->prev = outputList->last;
		outputList->last->next = nodAux;
		outputList->last = nodAux;
	}
CleanUp:
	return statusCode;
}

int ListPrint(LIST l,FILE* fileDescriptor)
{
	
	NOD *nod;

	if (NULL == fileDescriptor)
	{
		return FILE_OPEN_ERROR;
	}

	nod = l.first;
	while (NULL != nod)
	{
		fprintf(fileDescriptor, "%d ", nod->value);
		nod = nod->next;
	}
	fprintf(fileDescriptor, "\n");
	

	return SUCCES;
}

int ListMergeSort(LIST *l)
{
	LIST l1,l2;
	PNOD midNod;
	int statusCode;
	int aux;

	statusCode = CreateList(&l1);
	statusCode = CreateList(&l2);

	aux = 0;
	statusCode = SUCCES;
	
	if (NULL == l)
	{
		statusCode = INVALID_LIST_POINTER;
		goto CleanUp;
	}

	if (NULL == l->first || NULL == l->first->next)
	{
		goto CleanUp;//null list and 1 element list are both sorted
	}

	if (l->first->next == l->last)//2 elements in list
	{
		if (l->first->value > l->last->value)
		{
			aux = l->first->value;
			l->first->value = l->last->value;
			l->last->value = aux;
		}
		goto CleanUp;
	}

	l1.first = l->first;
	l2.last = l->last;
	statusCode = GetMiddle(l->first, &midNod);
	
	if(statusCode != 0)
	{	
		goto CleanUp;
	}

	l1.last = midNod;
	l2.first = midNod->next;
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	l1.last->next = NULL;
	l2.first->prev = NULL;


	statusCode = ListMergeSort(&l1);
	statusCode = ListMergeSort(&l2);
	
	statusCode = ListMerge(&l1, &l2,l);

	if (0 != statusCode)
	{
		goto CleanUp;
	}


CleanUp:
	if(SUCCES == statusCode)
	{
		ListDestroy(&l1);
		ListDestroy(&l2);
	}
	else
	{
		l->first = l1.first;
		l1.last->next = l2.first;
		l2.first->prev = l1.last;
		l->last = l2.last;
	}
	return statusCode;
}

int GetMiddle(PNOD head,PNOD* result) {
	int statusCode;

	statusCode = SUCCES;
	PNOD slow, fast;

	if (NULL == head) 
	{ 
		statusCode = INVALID_NOD_POINTER;
		goto CleanUp;
	}

	if (NULL == result)
	{
		statusCode = INVALID_PNOD_POINTER;
		goto CleanUp;
	}

	slow = head;
	fast = head;
	while (fast->next != NULL && fast->next->next != NULL) {
		slow = slow->next; 
		fast = fast->next->next;
	}
	*result = slow;

CleanUp:
	return statusCode;
}

int ListDestroy(PLIST list)
{
	int statusCode;
	PNOD nod;

	statusCode = SUCCES;
	nod = NULL;

	if (NULL == list)
	{
		statusCode = INVALID_LIST_POINTER;
		goto CleanUp;
	}

	nod = list->first;
	while (NULL != nod )
	{
		list->first = list->first->next;
		free(nod);
		nod = list->first;
	}


CleanUp:
	list->size = 0;
	list->first = NULL;
	list->last = NULL;
	return statusCode;
}

int ListFind(PLIST list,int value, BOOLEAN * exist)
{
	int status_code;
	PNOD nod;

	status_code = SUCCES;
	nod = NULL;
	*exist = FALSE;

	if (NULL == list)
	{
		status_code = INVALID_LIST_POINTER;
		goto CleanUp;
	}

	if (NULL == exist)
	{
		status_code = INVALID_RESULT_PARAMETER_POINTER;
		goto CleanUp;
	}

	nod = list->first;
	while (NULL != nod)
	{
		if (nod->value == value)
		{
			*exist = TRUE;
			goto CleanUp;
		}
		nod = nod->next;
	}
CleanUp:
	return status_code;
}


