#include "globals.h"
#include "symtab2.h"
SymbolTable* symbolTable;

//hash function
static int hash(char* key) {
	int temp = 0;
	int i;
	for (i = 0; key[i] != '\0'; i++) {
		temp = ((temp << SHIFT) + key[i]) % SIZE;
	}
	return temp;
}

void deleteLineList(LineList * list) {
	LineList* next = NULL;
	while (list) {
		next = list->next;
		free(list);
		list = next;
	}
}
void deleteBucketList(BucketList * list) {
	BucketList* next = NULL;
	while (list) {
		next = list->next;
		free(list->name);
		deleteLineList(list->lines);
		list = next;
	}
}

//将行号lineno和内存地址loc插入符号表
void st_insert(char* name, TreeNode * dec, int lineno, int loc) {
	int h = hash(name);
	BucketList* l = (BucketList*)malloc(sizeof(BucketList));
	l->name = name;
	l->lines = (LineList*)malloc(sizeof(LineList));
	l->nestLevel = symbolTable->nestLevel; //符号表的每一项的嵌套层次
	l->declaration = dec;
	l->lines->lineno = lineno;
	l->memloc = loc;
	l->lines->next = NULL;

	//插入到表头 
	l->next = symbolTable->hashTable[h];
	symbolTable->hashTable[h] = l;
}

//找到则返回其地址，（depth-1表示搜索所有scope）
BucketList* st_lookup(char* name, int depth) {
	int h = hash(name);
	SymbolTable* currentScope = symbolTable;
	while (currentScope && depth--) {
		BucketList* l = currentScope->hashTable[h];
		while (l) {
			if (strcmp(name, l->name) == 0) {
				return l;
			}
			l = l->next;
		}
		currentScope = currentScope->next;
	}
	return NULL;
}

//在原symbolTable的基础上增加一个scope
void st_newscope()
{
	SymbolTable* newScope = (SymbolTable*)malloc(sizeof(SymbolTable));
	memset(newScope->hashTable, 0, sizeof(newScope->hashTable));
	newScope->location = newScope->maxOffset = 0;
	newScope->err = 0;
	newScope->next = symbolTable;
	symbolTable = newScope;
	if (symbolTable->next)//有下一层就加1
	{
		symbolTable->nestLevel = symbolTable->next->nestLevel + 1;
	}
	else {
		symbolTable->nestLevel = 0;
	}
}

//提取一个最近的scope并返回
SymbolTable* st_abrade() {
	SymbolTable* oldScope = symbolTable;
	symbolTable = oldScope->next;
	oldScope->next = NULL;
	return oldScope;
}

void printSymTab(FILE * listing) //fixing
{
	//fprintf(listing, "Variable Name   Location   Line Dec Numbers\n");
	//fprintf(listing, "-------------   --------   ----------------\n");
	for (SymbolTable* table = symbolTable; table; table = table->next) {
		//printf("nestLevel: %d\n", table->nestLevel);
		//fprintf(listing, "Variable Name   Location   Line Numbers\n");
		//fprintf(listing, "-------------   --------   ------------\n");
		for (int i = 0; i < SIZE; i++) {
			BucketList* l = table->hashTable[i];
			//打印符号表每一项
			while (l) {
				LineList* lineList = l->lines;
				fprintf(listing, "%-14s   ", l->name);
				fprintf(listing, "%-8d   ", l->memloc);
				while (lineList) {
					fprintf(listing, "%4d ", lineList->lineno);
					lineList = lineList->next;
				}
				fprintf(listing, "\n");
				l = l->next;
			}
		}
		//printf("\n");
	}
}