#include "../Headers/AVLTree.h"

int Height(PTNOD nod)
{
	if (NULL == nod)
	{
		return 0;
	}
	return nod->height;
}

inline int maxim(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	return b;
}

STATUS_CODE CreateNod(PTNOD *nod,int value)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;
	
	if (NULL == nod)
	{
		statusCode = INVALID_PNOD_POINTER;
		goto CleanUp;
	}
	
	(*nod) = (PTNOD)malloc(sizeof(TNOD));
	
	if (NULL == *nod)
	{
		statusCode = MALLOC_ERROR;
		goto CleanUp;
	}
	
	(*nod)->height = 1;
	(*nod)->value = value;
	(*nod)->left = NULL;
	(*nod)->right = NULL;

CleanUp:
	return statusCode;
}

PTNOD RightRotate(PTNOD x)
{
	PTNOD y;
	PTNOD TreeAux;

	y = x->left;
	TreeAux = y->right;

	y->right = x;
	x->left = TreeAux;

	x->height = maxim(Height(x->left), Height(x->right)) + 1;
	y->height = maxim(Height(y->left), Height(y->right)) + 1;

	return y;
}

PTNOD LeftRotate(PTNOD x)
{
	PTNOD y;
	PTNOD TreeAux;

	y = x->right;
	TreeAux = y->left;

	y->left = x;
	x->right = TreeAux;

	x->height = maxim(Height(x->left), Height(x->right)) + 1;
	y->height = maxim(Height(y->left), Height(y->right)) + 1;
	
	return y;
}

int BalanceFactor(PTNOD nod)
{
	if (NULL == nod)
	{
		return 0;
	}
	return (Height(nod->left) - Height(nod->right));
}

PTNOD InsertAux(PTNOD nod, int value,STATUS_CODE *statusCode)
{
	int balanceLevel;

	*statusCode = SUCCES;

	if (NULL == nod)
	{
		*statusCode = CreateNod(&nod,value);
		if (SUCCES == *statusCode)
		{
			return nod;
		}
		else 
		{
			return NULL;//malloc error
		}
	}

	if (value < nod->value)
	{
		nod->left = InsertAux(nod->left, value,statusCode);
	}
	else if (value > nod->value)
	{
		nod->right = InsertAux(nod->right, value,statusCode);
	}
	else
	{
		return nod;//Duplicate case
	}

	nod->height = 1 + maxim(Height(nod->left), Height(nod->right));
	
	balanceLevel = BalanceFactor(nod);

	//left - left
	if (balanceLevel > 1 && value < nod->left->value)
	{
		return RightRotate(nod);
	}

	//right right
	if (balanceLevel < -1 && value > nod->right->value)
	{
		return LeftRotate(nod);
	}

	// Left Right Case
	if (balanceLevel > 1 && value > nod->left->value)
	{
		nod->left = LeftRotate(nod->left);
		return RightRotate(nod);
	}

	// Right Left Case
	if (balanceLevel < -1 && value < nod->right->value)
	{
		nod->right = RightRotate(nod->right);
		return LeftRotate(nod);
	}
		
	return nod;
}

PTNOD MinFromTree(PTNOD nod)
{
	PTNOD current;
	
	current = nod;
	if (NULL == current)
	{
		return NULL;
	}
	while (current->left != NULL)
	{
		current = current->left;
	}

	return current;
}

STATUS_CODE TreeCreate(MY_TREE * tree)
{
	tree->size = 0;
	tree->root = NULL;
	return SUCCES;
}


PTNOD DeleteAux(PTNOD nod, int value, STATUS_CODE *statusCode)
{
	PTNOD temp;
	int balance;

	temp = NULL;
	*statusCode = ELEMENT_NOT_FOUND;

	if (NULL == nod)
	{
		return NULL;
	}

	if (value < nod->value)
	{
		nod->left = DeleteAux(nod->left, value, statusCode);
	}
	else if (value > nod->value)
	{
		nod->right = DeleteAux(nod->right, value, statusCode);
	}

	else
	{
		*statusCode = SUCCES;
		if ((NULL == nod->left) || (NULL == nod->right))//0 or 1 child
		{
			if (NULL != nod->left)
			{
				temp = nod->left;
			}
			else
			{
				temp = nod->right;
			}
			if (NULL == temp)//no child
			{
				temp = nod;
				nod = NULL;
				free(temp);
			}
			else//1 child
			{
				*nod = *temp;//copy info from non null child
				free(temp);
			}
		}
		else//2 child
		{
			temp = MinFromTree(nod->right);
			nod->value = temp->value;
			nod->right = DeleteAux(nod->right, temp->value, statusCode);
		}
	}
	if (NULL == nod)
	{
		return nod;
	}

	nod->height = 1 + maxim(Height(nod->left), Height(nod->right));

	balance = BalanceFactor(nod);

	// Left Left Case
	if (balance > 1 && BalanceFactor(nod->left) >= 0)
		return RightRotate(nod);

	// Left Right Case
	if (balance > 1 && BalanceFactor(nod->left) < 0)
	{
		nod->left = LeftRotate(nod->left);
		return RightRotate(nod);
	}

	// Right Right Case
	if (balance < -1 && BalanceFactor(nod->right) <= 0)
		return LeftRotate(nod);

	// Right Left Case
	if (balance < -1 && BalanceFactor(nod->right) > 0)
	{
		nod->right = RightRotate(nod->right);
		return LeftRotate(nod);
	}

	return nod;
}

STATUS_CODE TreeDelete(MY_TREE * tree,int value)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;

	tree->root = DeleteAux(tree->root, value, &statusCode);

	return statusCode;
}

STATUS_CODE TreeFind(MY_TREE tree, int value,BOOLEAN *exist)
{
	STATUS_CODE statusCode;
	PTNOD current;
	statusCode = SUCCES;
	*exist = FALSE;

	current = tree.root;

	while ((NULL != current) && (*exist == FALSE))
	{
		if (value == current->value)
		{
			*exist = TRUE;
		}
		else if (value < current->value)
		{
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	return statusCode;
}

STATUS_CODE TreeDestroyAux(PTNOD *nod)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;

	if (NULL == nod)
	{
		statusCode = INVALID_PNOD_POINTER;
		goto CleanUp;
	}

	if (NULL == *nod)
	{
		goto CleanUp;
	}
	if ((*nod)->left != NULL)
	{
		statusCode = statusCode | TreeDestroyAux(&((*nod)->left));
	}
	if ((*nod)->right != NULL)
	{
		statusCode = statusCode | TreeDestroyAux(&((*nod)->right));
	}
	free(*nod);
CleanUp:
	return statusCode;
}

STATUS_CODE TreeDestroy(MY_TREE * tree)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;

	if (NULL == tree)
	{
		statusCode = INVALID_TREE_POINTER;
		goto CleanUp;
	}

	statusCode = TreeDestroyAux(&(tree->root));
	tree->root = NULL;
	
CleanUp:
	return statusCode;
}



STATUS_CODE TreeInsert(MY_TREE * tree,int value)
{
	STATUS_CODE statusCode;

	statusCode = SUCCES;
	if (NULL == tree) {
		return SUCCES;
	}
	tree->root = InsertAux(tree->root, value,&statusCode);
	tree->size++;
	return statusCode;
}


