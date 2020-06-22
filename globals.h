#pragma once
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/*MAXRESERVED = the number of reserved words保留字的数量*/
#define MAXRESERVED 6

typedef enum
{ /*所有token*/
	ENDFILE, ERROR,
	/*保留字*/
	IF, ELSE, INT, RETURN, VOID, WHILE,
	/*多字符tokens*/
	ID, NUM,
	/*特殊符号*/
	PLUS, MINUS, TIMES, OVER, LT, GT, NLT, NGT, EQU, NEQU, ASSIGN, SEMI, DOT,
	RBR, LBR, RSBR, LSBR, RLBR, LLBR
}TokenType;

extern FILE* source;  /*source code text file源代码文本文件*/
extern FILE* listing;  /*listing output text file列表输出文本文件*/
extern FILE* code; /*code text file for TM simulator TM模拟器的代码文本文件*/

extern int lineno;  /*source line number for listing需要列出的资源行数*/

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum { StmtK, ExpK, DecK } NodeKind;//3大类结点
typedef enum { IfK, WhileK, ReturnK, CallK, CompoundK } StmtKind;
typedef enum { OpK, VarEK, ArrayEK, ConstK, AssignK } ExpKind;//此处将IdK拆开为VarEk和ArrayEk更好操作
typedef enum { ArrayK, VarK, FunK } DecKind;//声明类型：数组，变量，函数


/* Used in the type checker语义分析 类型检测器中要处理的类型 */
typedef enum { Void, Integer, Boolean, Array, Function } ExpType;//这里的void应该是空的意思


#define MAXCHILDREN 3

typedef struct treeNode
{
	struct treeNode* child[MAXCHILDREN];
	struct treeNode* sibling;

	int lineno;//根出现位置

	NodeKind nodekind;//节点类型
	union { StmtKind stmt; ExpKind exp; DecKind dec; } kind;

	/* 声明或表达式的属性 */
	//union {
	TokenType op;
	int val;/* 常量表达式、数组表达式（或声明）的下标 */
	char* name;
	//} attr;

	/* 在每个新层次的起始处（子树根）保存对应的scope */
	struct SymbolTable* scope;
	int funcEntryLoc;//函数入口地址

	ExpType functionReturnType;
	ExpType variableDataType;
	ExpType expressionType;


	/*
	 * 语义分析type checking记录，记录一个变量对应的声明节点
	 *  允许代码生成器查找给定类型的信息标识符。
	 */
	struct treeNode* declaration;

} TreeNode;

/*****    追踪标志    *****/

extern int EchoSource;
extern int TraceScan;
extern int TraceParse;
extern int TraceAnalyze;
extern int TraceCode;
extern int Error;
#endif