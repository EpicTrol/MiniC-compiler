#pragma once
#ifndef _SYMTAB_H
#define _SYMTAB_H

#include <stdio.h>
#include "globals.h"

#define SIZE 211//hash table的大小
#define SHIFT 4//hash function中两项间的乘数


//LineList 记录变量被引用的行号
typedef struct LineListRec {
	int lineno;
	struct LineListRec* next;
} LineList;

/* BucketList 记录每个变量
 * 包括其id、所处嵌套层次、声明的位置、出现行号、内存位置
 */
typedef struct BucketListRec {
	char* name;
	LineList* lines;
	int nestLevel;
	TreeNode* declaration;
	int memloc;
	struct BucketListRec* next;
} BucketList;

//代表每个声明scope
typedef struct SymbolTable {
	BucketList* hashTable[SIZE];
	int location;//在对应的scope中分配的相对地址
	int maxOffset; //复合语句的嵌套，最多的变量偏移
	int nestLevel; //嵌套的层次，从0开始
	int err;
	struct SymbolTable* next;
} SymbolTable;


extern SymbolTable* symbolTable;//总的符号表 含多个scope

//将行号和内存地址插入符号表
void st_insert(char* name, TreeNode* dec, int lineno, int loc);

//返回变量对应的BucketList 找到则返回其指针
BucketList* st_lookup(char* name, int depth);

//增加一个scope
void st_newscope();

//提取一个最近的scope并返回
SymbolTable* st_abrade();

//打印当前scope的符号表
void printSymTab(FILE* listing);

#endif // _SYMTAB_H