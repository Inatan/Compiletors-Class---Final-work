#include <stdio.h>
#include <stdlib.h>
#include "cc_ILOCreader.h"
#include <string.h>

void createILOCinstLst(cc_ILOCinstLst_t **instlst)
{
	*instlst = NULL;
}

void lstInsertInstruction(cc_ILOCinstLst_t **instlst, char newCode[255])
{
	cc_ILOCinstLst_t* auxInstList;
	auxInstList = *instlst;
	cc_ILOCinstLst_t* newListElem;
	newListElem = (cc_ILOCinstLst_t*)malloc(sizeof(cc_ILOCinstLst_t));
	strcpy(newListElem->code,newCode);
	newListElem->nextInst = NULL;
	newListElem->isLeader=0;
	if(*instlst == NULL)
	{
		newListElem->isLeader=1;
		newListElem->lineNumber=1;
		*instlst = newListElem;
		return;
	}
	while(auxInstList->nextInst != NULL)
	{
		auxInstList = auxInstList->nextInst;
	}
	newListElem->lineNumber=auxInstList->lineNumber+1;
	auxInstList->nextInst = newListElem;
}

void listRemoveInst(cc_ILOCinstLst_t **instlst,char* code)
{
	cc_ILOCinstLst_t * auxInstList,*deletedElement;
	auxInstList=*instlst;
	if(strcmp(auxInstList->code,code)==0)
	{
		
		deletedElement=auxInstList;
		auxInstList=auxInstList->nextInst;
		free(deletedElement);
		deletedElement = NULL;
	}
	while(auxInstList!=NULL)
	{
		if(auxInstList->nextInst!=NULL && strcmp(auxInstList->nextInst->code,code)==0)
		{
			deletedElement=auxInstList->nextInst;
			auxInstList->nextInst=auxInstList->nextInst->nextInst;
			free(deletedElement);
			deletedElement=NULL;

		}
		if(auxInstList!=NULL)
			auxInstList = auxInstList->nextInst;
	}
}

cc_ILOCinstLst_t* listRemoveLastInst(cc_ILOCinstLst_t *instlst)
{
	if(instlst->nextInst == NULL)
	{
		free(instlst);
		instlst = NULL;
		return instlst;
	}
	cc_ILOCinstLst_t* auxInstList;
	auxInstList = instlst;
	while(auxInstList->nextInst != NULL)
	{
		auxInstList = auxInstList->nextInst;
	}
	free(auxInstList);
	auxInstList = NULL;
	return instlst;
}

void deleteListInst(cc_ILOCinstLst_t **instlst)
{
	cc_ILOCinstLst_t* auxInstList;
	cc_ILOCinstLst_t* next;
	auxInstList = *instlst;
	next = auxInstList;
	while (next != NULL)
	{
		next = auxInstList->nextInst;
		free(auxInstList);
		auxInstList = next;
	}
	*instlst = NULL;
}

void lstInsertInstructionLastPostion(cc_ILOCinstLst_t **instlst, char* newCode)
{
	cc_ILOCinstLst_t* auxInstList;
	auxInstList = *instlst;
	cc_ILOCinstLst_t* newListElem;
	newListElem = (cc_ILOCinstLst_t*)malloc(sizeof(cc_ILOCinstLst_t));
	strcpy(newListElem->code,newCode);
	newListElem->nextInst = NULL;
	newListElem->isLeader=0;
	if(*instlst == NULL)
	{
		newListElem->isLeader=1;
		newListElem->lineNumber=1;
		*instlst = newListElem;
		return;
	}
	while(auxInstList->nextInst != NULL && strstr(auxInstList->nextInst->code,"jump")==NULL && strstr(auxInstList->nextInst->code,"cbr")==NULL)
	{

		auxInstList = auxInstList->nextInst;
	}


	if(auxInstList->nextInst != NULL)
	{
		newListElem->nextInst= auxInstList->nextInst;
	}
	newListElem->lineNumber=auxInstList->lineNumber+1;
	auxInstList->nextInst = newListElem;
}

void readIloc(char *fileName,cc_ILOCinstLst_t** instLstin)
{
	FILE *fp;
	char code[2048];
	char buff[255];
	int count=1;
	strcpy(code,"");
	createILOCinstLst(instLstin);
	fp = fopen(fileName, "r");

	while(!feof(fp))
	{
	  fgets(buff, 255, (FILE*)fp);
	  strcat(code,buff);
	  lstInsertInstruction(instLstin,buff);
	}
}

void printILOClst(cc_ILOCinstLst_t* list)
{
	cc_ILOCinstLst_t* auxInstList;
	auxInstList = list;
	while(auxInstList != NULL)
	{
		printf("%d: %s \n",auxInstList->lineNumber,auxInstList->code);
		auxInstList = auxInstList->nextInst;
	}

}
