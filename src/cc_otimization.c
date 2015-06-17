#include <stdio.h>
#include <stdlib.h>
#include "cc_otimization.h"
#include "cc_ILOCreader.h"
#include <string.h>

void searchLeaders(cc_ILOCinstLst_t **instlst){
	cc_ILOCinstLst_t* auxInstList;
	auxInstList= *instlst;
	while(auxInstList!=NULL)
	{
		if(strstr(auxInstList->code,":")!=NULL)
			auxInstList->isLeader=1;
		if(strstr(auxInstList->code,"jump")!=NULL || strstr(auxInstList->code,"cbr")!=NULL)
			auxInstList->nextInst->isLeader=1;
		auxInstList=auxInstList->nextInst;
	}
}

cc_ILOCinstLst_t* getVertexlistInstrunctions(cc_ILOCinstLst_t **instlst)
{
	cc_ILOCinstLst_t* auxInstList,*retInstLst;
	auxInstList= *instlst;
	auxInstList=auxInstList->nextInst;
	while(auxInstList!=NULL && auxInstList->isLeader==0)
	{
		if(auxInstList->nextInst!= NULL && auxInstList->nextInst->isLeader==1)
		{
			retInstLst=auxInstList->nextInst;
			auxInstList->nextInst=NULL;
			return retInstLst;
		}
		auxInstList=auxInstList->nextInst;
	}
	return NULL;
}

void otimizateIlocInst(cc_ILOCinstLst_t **instlst)
{
	cc_ILOCinstLst_t* auxInstList,*auxInstList2;
	int isUsed;
	char *reg,*regaux;
	auxInstList=*instlst;
	int i=1;
	while(auxInstList !=NULL)
	{
		isUsed=0;
		if(strstr(auxInstList->code,":")==NULL && strstr(auxInstList->code,"jump")==NULL && strstr(auxInstList->code,"cbr")==NULL)
		{
			auxInstList2=*instlst;
			while(auxInstList2!=NULL)
			{
				reg=getSubstring(auxInstList->code);
				if((strcmp(auxInstList->code,auxInstList2->code)!=0 && strstr(auxInstList2->code,reg)!=NULL )|| strstr(reg,"rb")!=NULL) 
				{
					isUsed=1;
				}
				auxInstList2=auxInstList2->nextInst;
			}
		}

		if(isUsed==0 && strstr(auxInstList->code,"load")!=NULL)
		{
			auxInstList2=auxInstList->nextInst;
			listRemoveInst(instlst,auxInstList->code);
			auxInstList=auxInstList2;
		}
		else if (auxInstList->nextInst!=NULL &&   strcmp(auxInstList->code,auxInstList->nextInst->code)==0)
		{
			auxInstList2=auxInstList->nextInst->nextInst;
			listRemoveInst(instlst,auxInstList->nextInst->code);
			auxInstList=auxInstList2;
		}
		else
			auxInstList=auxInstList->nextInst;
	}
	auxInstList=*instlst;
	while(auxInstList !=NULL)
	{
		auxInstList->lineNumber=i;
		i++;
		auxInstList=auxInstList->nextInst;
	}
}

cc_otmGraph_t* createVertex(cc_ILOCinstLst_t *instrunctions)
{
	int i;

	cc_otmGraph_t* newVertex = (cc_otmGraph_t*)malloc(sizeof(cc_otmGraph_t));
	newVertex->vertex_lst_dst = (cc_graphVertexList_t*)malloc(sizeof(cc_graphVertexList_t));
	newVertex->vertex_lst_src = (cc_graphVertexList_t*)malloc(sizeof(cc_graphVertexList_t));
	newVertex->firstLineNumber= instrunctions->lineNumber;
	newVertex->vertex_lst_dst->vertexInfo = NULL;
	newVertex->vertex_lst_src->vertexInfo = NULL;
	newVertex->vertex_lst_src->nextVertex = NULL;
	newVertex->vertex_lst_dst->nextVertex = NULL;
	if(instrunctions != NULL)
	{
		newVertex->instrunctions = instrunctions;
	}
	
	if(strstr(instrunctions->code,":")!=NULL)
	{	
		strcpy(newVertex->label,instrunctions->code);
		i=0;
		while (newVertex->label[i] != '\0')
		{
			if(newVertex->label[i] == ':')
				newVertex->label[i] = '\0';
			else
				i++;
		}
	}
	else
		strcpy(newVertex->label,"MAIN");
	return newVertex;
}

void *appendVertex(cc_otmGraph_t* srcVertex, cc_otmGraph_t* dstVertex)
{
	cc_graphVertexList_t *auxVertexList;
	cc_graphVertexList_t *auxVertexList2;
	auxVertexList = srcVertex->vertex_lst_dst;
	auxVertexList2 = dstVertex->vertex_lst_src;

	if(dstVertex->vertex_lst_src->vertexInfo==NULL)
	{
		dstVertex->vertex_lst_src->vertexInfo = srcVertex;
		dstVertex->vertex_lst_src->nextVertex = NULL;
	}
	else
	{
		while (auxVertexList2->nextVertex != NULL)
		{
			auxVertexList2 = auxVertexList2->nextVertex;
		}
		auxVertexList2->nextVertex = (cc_graphVertexList_t*)malloc(sizeof(cc_graphVertexList_t));
		auxVertexList2->nextVertex->vertexInfo = srcVertex;
		auxVertexList2->nextVertex->nextVertex = NULL;
	}

	if(auxVertexList->vertexInfo == NULL)
	{
		auxVertexList->vertexInfo = dstVertex;
	}
	else
	{
		while (auxVertexList->nextVertex != NULL)
		{
			auxVertexList = auxVertexList->nextVertex;
		}
		auxVertexList->nextVertex = (cc_graphVertexList_t*)malloc(sizeof(cc_graphVertexList_t));
		auxVertexList->nextVertex->vertexInfo = dstVertex;
		auxVertexList->nextVertex->nextVertex = NULL;
	}
} 


void removeVertex(cc_otmGraph_t* Vertex)
{
	if(Vertex != NULL)
	{
		cc_graphVertexList_t* auxVertexList;
		cc_graphVertexList_t* auxVertexList2;
		auxVertexList = Vertex->vertex_lst_src;

		while(auxVertexList->nextVertex != NULL)
		{
			removeVertex(auxVertexList->vertexInfo);
			free(auxVertexList->vertexInfo);
			auxVertexList2 = auxVertexList;
			auxVertexList = auxVertexList->nextVertex;
			free(auxVertexList2);
		}
	}
}


void printVertexvDstList(cc_otmGraph_t *vertex)
{
	cc_graphVertexList_t * auxDstLst;
	auxDstLst= vertex->vertex_lst_dst;
	if(auxDstLst!=NULL && auxDstLst->vertexInfo !=NULL)
	{	
		printf("%s  father of", vertex->label);
		while(auxDstLst!=NULL && auxDstLst->vertexInfo !=NULL)
		{	
			printf(", %s", auxDstLst->vertexInfo->label);
			auxDstLst= auxDstLst->nextVertex;
		}
		printf("\n");
	}
} 

void createGraphOtm(cc_graphVertexList_t **graph,cc_ILOCinstLst_t *instrunctions)
{
	*graph=(cc_graphVertexList_t*)malloc(sizeof(cc_graphVertexList_t));
	cc_graphVertexList_t *auxGraph, *auxGraph2;;
	cc_ILOCinstLst_t* auxInst1,*auxInst2;
	auxInst1=auxInst2=instrunctions;
	auxGraph= * graph;
	int haveEdge=0;
	while(auxInst2!=NULL)
	{
		auxInst2=getVertexlistInstrunctions(&auxInst1);
		auxGraph->vertexInfo=createVertex(auxInst1);
		auxGraph->nextVertex=NULL;
		auxInst1=auxInst2;
		if(auxInst2!=NULL)
			auxGraph->nextVertex = (cc_graphVertexList_t*)malloc(sizeof(cc_graphVertexList_t));
		else
			auxGraph->nextVertex = NULL;
		auxGraph=auxGraph->nextVertex;
	}
	auxGraph=NULL;
	auxGraph=*graph;
	auxGraph2 = (cc_graphVertexList_t*)malloc(sizeof(cc_graphVertexList_t));
	while(auxGraph!=NULL && auxGraph->vertexInfo!=NULL)
	{
		auxGraph2=*graph;
		//if(strcmp(auxGraph2->vertexInfo->label,auxGraph->vertexInfo->label)==0)
			//auxGraph2=auxGraph2->nextVertex;
		while(auxGraph2!= NULL && auxGraph2->vertexInfo!= NULL)
		{
			//printf("%s tests %s\n",auxGraph->vertexInfo->label,auxGraph2->vertexInfo->label);
			if(strcmp(auxGraph2->vertexInfo->label,auxGraph->vertexInfo->label)==0)
				auxInst1 =auxGraph->vertexInfo->instrunctions->nextInst;
			else
				auxInst1=auxGraph->vertexInfo->instrunctions;	
			while(auxInst1 != NULL)
			{
				if(strstr(auxInst1->code,auxGraph2->vertexInfo->label)!=NULL)
				{
					//printf("%s father of %s\n",auxGraph->vertexInfo->label,auxGraph2->vertexInfo->label);
					haveEdge=1;
					appendVertex(auxGraph->vertexInfo,auxGraph2->vertexInfo);
				}
				auxInst1=auxInst1->nextInst;
			}
			/*if(auxGraph2->nextVertex!=NULL &&strcmp(auxGraph2->nextVertex->vertexInfo->label,auxGraph->vertexInfo->label)==0)
				auxGraph2=auxGraph2->nextVertex->nextVertex;	
			elseg*/
				auxGraph2=auxGraph2->nextVertex;
		}
		
		if(strcmp(auxGraph->vertexInfo->label,"MAIN")!=0 && haveEdge!=1 && auxGraph->nextVertex!=NULL )
		{
			appendVertex(auxGraph->vertexInfo,auxGraph->nextVertex->vertexInfo);
			//printf("%s father of %s\n",auxGraph->vertexInfo->label,auxGraph->nextVertex->vertexInfo->label);
		}
		haveEdge=0;
		//printVertexvDstList(auxGraph->vertexInfo);
		auxGraph=auxGraph->nextVertex;
	}
}

cc_dominationTree_t* createDominationNode(cc_otmGraph_t* vertex)
{
	cc_dominationTree_t* newNode = (cc_dominationTree_t*)malloc(sizeof(cc_dominationTree_t));
	strcpy(newNode->label,vertex->label);
	newNode->firstLineNumber= vertex->firstLineNumber;
	newNode->childList=(cc_childDominationLst_t*)malloc(sizeof(cc_childDominationLst_t));
	newNode->childList->childInfo=NULL;
	return newNode;
}

void *appendDominationNode(cc_dominationTree_t* father, cc_dominationTree_t* newChild)
{
	if (newChild != NULL)
	{	
		cc_childDominationLst_t *auxNodeList;
		auxNodeList = father->childList;
		
		if(auxNodeList->childInfo == NULL)
		{
			auxNodeList->childInfo = newChild;
		}
		else
		{
			while (auxNodeList->nextChild != NULL)
			{
				auxNodeList = auxNodeList->nextChild;
			}
			auxNodeList->nextChild = (cc_childDominationLst_t*)malloc(sizeof(cc_childDominationLst_t));
			auxNodeList->nextChild->childInfo = newChild;
			auxNodeList->nextChild->nextChild = NULL;
		}
	}
}

void removeDominationNode(cc_dominationTree_t* node)
{
	if(node != NULL)
	{
		cc_childDominationLst_t* auxNodeList;
		cc_childDominationLst_t* auxNodeList2;
		auxNodeList = node->childList;

		while(auxNodeList->nextChild != NULL)
		{
			removeDominationNode(auxNodeList->childInfo);
			free(auxNodeList->childInfo);
			auxNodeList2 = auxNodeList;
			auxNodeList = auxNodeList->nextChild;
			free(auxNodeList2);
		}
	}
}

cc_dominationTree_t *createDominationTree(cc_otmGraph_t **Vertex)
{	
	cc_graphVertexList_t* auxVertexList;
	cc_dominationTree_t* auxNode,*childNode;
	cc_otmGraph_t* auxVertex;
	auxVertex= *Vertex;
	auxVertexList=auxVertex->vertex_lst_dst;
	auxNode=NULL;
	//printf("eta %p  %p\n ",Vertex,auxVertex->vertex_lst_dst);
	if(auxVertex!=NULL)
		auxNode= createDominationNode(auxVertex);
	while(auxVertexList!=NULL && auxVertexList->vertexInfo!=NULL && auxVertexList->vertexInfo->firstLineNumber >= auxVertex->firstLineNumber)
	{ 	if(strcmp(auxVertexList->vertexInfo->label,auxVertex->label)!=0)
		{
			childNode=createDominationTree(&auxVertexList->vertexInfo);
			if(childNode!=NULL)
			{
				//printf("%s father of %s  \n", auxNode->label,childNode->label);
				appendDominationNode(auxNode,childNode);
			}
		}
		auxVertexList=auxVertexList->nextVertex;
	}
	return auxNode;
}

void printTree(cc_dominationTree_t* tree)
{
	cc_childDominationLst_t* treeLstAux;
	treeLstAux=tree->childList;
	if(tree!=NULL)
	{
		printf("%s\n", tree->label);
		while(treeLstAux!=NULL && treeLstAux->childInfo)
		{
			printTree(treeLstAux->childInfo);
			treeLstAux=treeLstAux->nextChild;
		}
		
	}
}

cc_dominationTree_t* findNode(char *label, cc_dominationTree_t* tree)
{
	cc_childDominationLst_t * childLstAux;
	childLstAux= tree->childList;
	cc_dominationTree_t* auxNode;
	auxNode=NULL;
	if(strcmp(tree->label,label)==0)
	{
		return tree;
	}
	while(childLstAux!=NULL && childLstAux->childInfo!=NULL && auxNode==NULL)
	{
		auxNode=findNode(label,childLstAux->childInfo);
		childLstAux=childLstAux->nextChild;
	}
	return auxNode;
}

void detectLoopinGraph(cc_graphVertexList_t **graph,cc_dominationTree_t* tree)
{
	cc_graphVertexList_t* auxGraph,*auxGraph2;
	auxGraph= *graph;
	cc_dominationTree_t *auxNode, *auxChild;
	while(auxGraph!=NULL && auxGraph->vertexInfo!=NULL)
	{
		auxGraph2= auxGraph->vertexInfo->vertex_lst_dst;
		while(auxGraph2!=NULL && auxGraph2->vertexInfo!=NULL && auxGraph2->vertexInfo->firstLineNumber <= auxGraph->vertexInfo->firstLineNumber)
		{
			auxNode=findNode(auxGraph2->vertexInfo->label,tree);
			if(auxNode!=NULL)
			{
				auxChild=findNode(auxGraph->vertexInfo->label,auxNode);
				if(auxChild!=NULL || strcmp(auxGraph2->vertexInfo->label,auxGraph2->vertexInfo->label)==0)
				{
					auxGraph2->haveLoop=1;
					//printf("%s tem ligacao com %s mas e dominado por ele, logo existe loop \n",auxGraph->vertexInfo->label, auxGraph2->vertexInfo->label);
				}
			}
			auxGraph2=auxGraph2->nextVertex;	
		}
		auxGraph=auxGraph->nextVertex;
	}
}


char* getSubstring(char key[100])
{
	char *substring,newString[100]="";
	int i,k=0,flagCopia=0;
	int size=(strlen(key)-1);
	//printf("%s \n",key);
	for(i=0;i<size ;i++)
	{
		if(flagCopia && key[i]!=' '){
			newString[k]=key[i];
			k++;
		}
		else if(key[i]=='>' && key[i-1]=='=')
		{
			flagCopia=1;
		}
	}
	//if(newString!=NULL)
		//printf("%s\n", newString);
	substring=newString;
	return substring;
}

int checkInstruction(cc_otmGraph_t** vertexSrc, char*varName, char* lastVertexLabel,int isUnvariant,char instS[100])
{
	cc_ILOCinstLst_t* instrunctionsAux;
	instrunctionsAux=(*vertexSrc)->instrunctions;
	cc_graphVertexList_t* auxVertexList;
	auxVertexList= (*vertexSrc)->vertex_lst_dst;
	while(instrunctionsAux!=NULL && isUnvariant!=0)
	{ 	
		
		if(strstr(instrunctionsAux->code,varName)!=NULL && strcmp(instrunctionsAux->code,instS)!=0) 
		{
			isUnvariant=0;
		}
		instrunctionsAux= instrunctionsAux->nextInst;
	}
	if(strcmp((*vertexSrc)->label,lastVertexLabel)!=0 && isUnvariant==1)
	{	
		while(auxVertexList!=NULL && auxVertexList->vertexInfo !=NULL && isUnvariant==1)
		{
			isUnvariant=checkInstruction(&auxVertexList->vertexInfo,varName,lastVertexLabel,isUnvariant,instS);
			auxVertexList= auxVertexList->nextVertex;
		}
	}
	return isUnvariant;
}

void checkUnvariantInstrucition(cc_otmGraph_t** vertexSrc, cc_otmGraph_t** startPoint, char *LabelDst)
{
	cc_ILOCinstLst_t *instrunctionsAux,*instrunctionsAux2;
	char* varName;
	instrunctionsAux= (*vertexSrc)->instrunctions;
	int isUnvariant=1;
	cc_graphVertexList_t* auxVertexList;
	auxVertexList=(*vertexSrc)->vertex_lst_dst;
	cc_otmGraph_t* vertexStartAux;
	while(instrunctionsAux!=NULL)
	{
		instrunctionsAux2=NULL;
		if(strstr(instrunctionsAux->code,"cbr")==NULL && strstr(instrunctionsAux->code,"jump")==NULL && strstr(instrunctionsAux->code,":")==NULL && strstr(instrunctionsAux->code,"store")==NULL)
		{
			varName=getSubstring(instrunctionsAux->code);
			isUnvariant=checkInstruction(vertexSrc,varName,LabelDst,isUnvariant,instrunctionsAux->code);
			if(isUnvariant==1 || strstr(instrunctionsAux->code,"loadI")!=NULL)
			{
				instrunctionsAux->isUnvariant=1;
				if((*vertexSrc)->vertex_lst_src->nextVertex==NULL || strcmp((*vertexSrc)->vertex_lst_src->nextVertex->vertexInfo->label,LabelDst)==0)
				{
					vertexStartAux=(*startPoint)->vertex_lst_src->vertexInfo;
					lstInsertInstructionLastPostion(&vertexStartAux->instrunctions,instrunctionsAux->code);
					instrunctionsAux2=instrunctionsAux->nextInst;
					vertexStartAux=(*vertexSrc);
					listRemoveInst(&vertexStartAux->instrunctions,instrunctionsAux->code);
					instrunctionsAux=instrunctionsAux2;
				}
				else if(strcmp((*vertexSrc)->vertex_lst_src->vertexInfo->label,LabelDst)==0 && (*vertexSrc)->vertex_lst_src->nextVertex!=NULL)
				{
					vertexStartAux=(*startPoint)->vertex_lst_src->nextVertex->vertexInfo;
					lstInsertInstructionLastPostion(&vertexStartAux->instrunctions,instrunctionsAux->code);
					instrunctionsAux2=instrunctionsAux->nextInst;
					vertexStartAux=(*vertexSrc);
					listRemoveInst(&vertexStartAux->instrunctions,instrunctionsAux->code);
					instrunctionsAux=instrunctionsAux2;
				}

			}
		}
		if(instrunctionsAux2==NULL)
			instrunctionsAux=instrunctionsAux->nextInst;
	}
	if(strcmp((*vertexSrc)->label,LabelDst)!=0)
	{
		while(auxVertexList!=NULL && auxVertexList->vertexInfo !=NULL)
		{
			checkUnvariantInstrucition(&auxVertexList->vertexInfo,startPoint,LabelDst);
			auxVertexList= auxVertexList->nextVertex;
		}
	}
}



void findUnvariantComand(cc_graphVertexList_t **graph)
{
	cc_graphVertexList_t* auxGraph,*auxGraph2;
	auxGraph=*graph;
	while(auxGraph!=NULL && auxGraph->vertexInfo!=NULL)
	{
		auxGraph2= auxGraph->vertexInfo->vertex_lst_dst;
		while(auxGraph2!=NULL && auxGraph2->vertexInfo!=NULL && auxGraph2->vertexInfo->firstLineNumber <= auxGraph->vertexInfo->firstLineNumber)
		{
			if(auxGraph2->haveLoop==1)
			{
				checkUnvariantInstrucition(&auxGraph2->vertexInfo,&auxGraph2->vertexInfo,auxGraph->vertexInfo->label);
			}
			auxGraph2=auxGraph2->nextVertex;	
		}
		auxGraph=auxGraph->nextVertex;
	}
}

char* newCode(cc_graphVertexList_t *graph)
{
	cc_graphVertexList_t* auxGraphList;
	cc_ILOCinstLst_t * auxInstList;
	char code[2048],*nCode;
	strcpy(code,"");
	auxGraphList=graph;
	while(auxGraphList!=NULL)
	{

		auxInstList=auxGraphList->vertexInfo->instrunctions;
		while(auxInstList!=NULL)
		{
			strcat(code,auxInstList->code);
			auxInstList=auxInstList->nextInst;
		}
		auxGraphList=auxGraphList->nextVertex;
	}
	nCode=code;
	printf("%s\n", nCode );
	return nCode;
}