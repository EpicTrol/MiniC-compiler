#pragma once

#ifndef _UTIL_H_
#define _UTIL_H_

#include "globals.h"

void printToken(TokenType, const char*);

TreeNode* newStmtNode(StmtKind);

TreeNode* newExpNode(ExpKind);

TreeNode* newDeclearNode(DecKind kind);

char* copyString(char*);

char* typeName(ExpType type);

void printTree(TreeNode*);

#endif