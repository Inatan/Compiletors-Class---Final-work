#ifndef __CC_ILOCREADER_H__
#define __CC_ILOCREADER_H__

typedef struct cc_ILOCinstLst{
	char code[255];
	int isLeader;
	int isUnvariant;
	int lineNumber;
	struct cc_ILOCinstLst* nextInst ;
}cc_ILOCinstLst_t;


void createILOCinstLst(cc_ILOCinstLst_t **instlst);
void lstInsertInstruction(cc_ILOCinstLst_t **instlst, char code[255]);
cc_ILOCinstLst_t* listRemoveLastInst(cc_ILOCinstLst_t *instlst);
void deleteListInst(cc_ILOCinstLst_t **instlst);
void readIloc(char *fileName,cc_ILOCinstLst_t** instLstin);
cc_ILOCinstLst_t* getVertexlistInstrunctions(cc_ILOCinstLst_t **instlst);

#endif