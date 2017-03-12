#ifndef _TREE_CONTROLLER_H_
#define _TREE_CONTROLLER_H_


#include "../ControllerStatus.h"
#include "../../Parser/Parser.h"
#include "stdlib.h"
#include "../HelperController/HelperController.h"
#include "AVLTree.h"

typedef struct
{
	MY_TREE trees[MAX_STRUCTS];
	int currentTree;
	int nrOfTrees;
} TREE_CONTROLLER, *PTREE_CONTROLLER;


STATUS_CODE TreeControllerCreate(PTREE_CONTROLLER *treeController);
STATUS_CODE TreeControllerRead(PTREE_CONTROLLER treeController, PPARSER parser);
STATUS_CODE TreeControllerDestroy(PTREE_CONTROLLER *treeController);
STATUS_CODE TreeControllerGoTo(PTREE_CONTROLLER treeController, PPARSER parser);
STATUS_CODE TreeControllerInsert(PTREE_CONTROLLER treeController, PPARSER parser);
STATUS_CODE TreeControllerRemove(PTREE_CONTROLLER treeController, PPARSER parser,FILE *out);
STATUS_CODE TreeControllerSearch(PTREE_CONTROLLER treeController, PPARSER parser, FILE *out);

#endif //!_TREE_CONTROLLER_H_