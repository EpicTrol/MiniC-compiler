#pragma once
#ifndef _CGEN_H
#define _CGEN_H

#define STACKMARKSIZE  8

//需要在其他位置可见的常量定义
#define WORDSIZE 4

#include "globals.h"

//extern int TraceCode;


//从语法树生成代码并保存到以"fileName"命名的文件中
//void codeGen(TreeNode* syntaxTree, char* fileName, char* moduleName);

//codeGen 遍历syntaxTree，生成目标代码到codeFile
void codeGen(TreeNode* syntaxTree, char* codeFile);//cgen2
#endif