#ifndef __CC_OTIMIZATION_H__
#define __CC_OTIMIZATION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cc_ILOCreader.h"
//#include "iloc.h"



typedef struct cc_graphVertexList
{
	struct cc_otmGraph* vertexInfo;
	struct cc_graphVertexList* nextVertex;
	int haveLoop;	
}cc_graphVertexList_t;

typedef struct cc_otmGraph
{
	char label[50];
	int firstLineNumber;
	cc_ILOCinstLst_t *instrunctions;
	cc_graphVertexList_t  *vertex_lst_dst;
	cc_graphVertexList_t  *vertex_lst_src;

}cc_otmGraph_t;
 

typedef struct cc_dominationTree
{
	char label[50];
	int firstLineNumber;
	struct cc_childDominationLst* childList;

}cc_dominationTree_t;

typedef struct cc_childDominationLst
{
	cc_dominationTree_t* childInfo;
	struct cc_childDominationLst *nextChild;
}cc_childDominationLst_t;


cc_otmGraph_t* createVertex(cc_ILOCinstLst_t *instrunctions);
void *appendVertex(cc_otmGraph_t* srcVertex, cc_otmGraph_t* dstVertex);
void removeVertex(cc_otmGraph_t* Vertex);

cc_dominationTree_t* createDominationNode(cc_otmGraph_t* vertex);
void *appendDominationNode(cc_dominationTree_t* father, cc_dominationTree_t* newChild);
void removeDominationNode(cc_dominationTree_t* node);
char* getSubstring(char key[100]);
cc_dominationTree_t *createDominationTree(cc_otmGraph_t **Vertex);
char* newCode(cc_graphVertexList_t *graph);

void searchLeaders(cc_ILOCinstLst_t **instlst);

#endif