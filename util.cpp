#pragma once
#include "globals.h"
#include "util.h"
#include <string.h>

void printToken(TokenType token, const char* tokenString)
{
	switch (token)
	{
	case ENDFILE:
		fprintf(listing, "EOF\n");
		break;
	case INT:
	case VOID:
	case WHILE:
	case IF:
	case RETURN:
	case ELSE:
		fprintf(listing, "保留字, name= %s\n", tokenString);
		break;
	case ERROR:
		fprintf(listing, "错误, val= %s\n", tokenString);
		break;
	case ID:
		fprintf(listing, "ID, name= %s\n", tokenString);
		break;
	case NUM:
		fprintf(listing, "NUM, val= %s\n", tokenString);
		break;

	case NEQU:
		fprintf(listing, "Special symbol: !=\n");
		break;
	case LSBR:
		fprintf(listing, "Special symbol: [\n");
		break;
	case RSBR:
		fprintf(listing, "Special symbol: ]\n");
		break;
	case LLBR:
		fprintf(listing, "Special symbol: {\n");
		break;
	case RLBR:
		fprintf(listing, "Special symbol: }\n");
		break;
	case ASSIGN:
		fprintf(listing, "Special symbol: =\n");
		break;
	case LT:
		fprintf(listing, "Special symbol: <\n");
		break;
	case GT:
		fprintf(listing, "Special symbol: >\n");
		break;
	case RBR:
		fprintf(listing, "Special symbol: )\n");
		break;
	case PLUS:
		fprintf(listing, "Special symbol: +\n");
		break;
	case MINUS:
		fprintf(listing, "Special symbol: -\n");
		break;
	case NLT:
		fprintf(listing, "Special symbol: >=\n");
		break;
	case NGT:
		fprintf(listing, "Special symbol: <=\n");
		break;
	case SEMI:
		fprintf(listing, "Special symbol: ;\n");
		break;
	case DOT:
		fprintf(listing, "Special symbol: ,\n");
		break;
	case TIMES:
		fprintf(listing, "Special symbol: *\n");
		break;
	case OVER:
		fprintf(listing, "Special symbol: /\n");
		break;
	case EQU:
		fprintf(listing, "Special symbol: ==\n");
		break;
	case LBR:
		fprintf(listing, "Special symbol: (\n");
		break;

	default: /* should never happen */
		fprintf(listing, "Unknown token: %d\n", token);

	}
}


TreeNode* newStmtNode(StmtKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;

		t->declaration = NULL;

		t->scope = NULL;
		t->funcEntryLoc = 0;
	}
	return t;
}

TreeNode* newExpNode(ExpKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->declaration = NULL;


		t->scope = NULL;
		t->funcEntryLoc = 0;
	}
	return t;
}

TreeNode* newDeclearNode(DecKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else
	{
		for (i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = DecK;
		t->kind.dec = kind;
		t->lineno = lineno;

		t->declaration = NULL;

		t->scope = NULL;
		t->funcEntryLoc = 0;
	}
	return t;
}

char* typeName(ExpType type)
{
	static char i[] = "integer";
	static char v[] = "void";
	static char invalid[] = "<<invalid type>>";

	switch (type)
	{
	case Integer: return i; break;
	case Void:    return v; break;
	default:      return invalid;
	}
}

char* copyString(char* s)
{
	int n;
	char* t;
	if (s == NULL) return NULL;
	n = (int)strlen(s) + 1;
	t = (char*)malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else strcpy(t, s);
	return t;
}

static int indentno = 0;//储存当前空间的缩进数

#define INDENT indentno+=4
#define UNINDENT indentno-=4

static void printSpaces(void)
{
	int i;
	for (i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 * 输出整个数，先根遍历输出，
 */
void printTree(TreeNode * tree)
{
	int i;
	INDENT;
	while (tree != NULL)
	{
		printSpaces();
		if (tree->nodekind == StmtK)
		{
			switch (tree->kind.stmt) {
			case WhileK:
				fprintf(listing, "While\n");
				break;
			case IfK:
				fprintf(listing, "If\n");
				break;
				//case ElseK://
				//	fprintf(listing, "Else\n");
				//	break;
			case CompoundK:
				fprintf(listing, "Compound\n");
				break;
			case ReturnK:
				fprintf(listing, "Return\n");
				break;
			case CallK:
				fprintf(listing, "Call \"%s()\" \n", tree->name);
				break;
			default:
				fprintf(listing, "Unknown ExpNode statement kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK)
		{
			switch (tree->kind.exp) {
			case OpK:
				fprintf(listing, "Op: ");
				printToken(tree->op, "\0");
				//fprintf(listing, "\n");
				break;
			case ConstK:
				fprintf(listing, "Const: %d\n", tree->val);
				break;
			//case IdK:
			//	fprintf(listing, "Id: %s", tree->name);
			//	if (tree->array == 0)//数组下标
			//	{
			//		fprintf(listing, "下标:");
			//		//fprintf(listing, "[%s]", tree->name);
			//	}
			//	fprintf(listing, "\n");
			//	break;
			case VarEK:
				fprintf(listing, "Var: %s\n", tree->name);
				break;
			case ArrayEK:
				fprintf(listing, "Array: %s\n", tree->name);
				break;
			case AssignK:
				fprintf(listing, "Assign: \n");
				//printToken(tree->op, "\n");
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == DecK)
		{
			switch (tree->kind.dec)
			{
			case FunK:
				fprintf(listing, "Fun_dec: ");
				fprintf(listing, " %s %s %s %s \n", "函数:", tree->name, "返回类型:", typeName(tree->functionReturnType));
				break;
			case VarK:
				fprintf(listing, "Var_dec: ");
				fprintf(listing, " %s %s %s %s\n", "变量名", tree->name, "变量类型", typeName(tree->variableDataType));
				break;
			case ArrayK:
				fprintf(listing, "[Array declaration \"%s\" of size %d"
					" and type \"%s\"]\n",
					tree->name, tree->val, typeName(tree->variableDataType));
				break;
			default:
				fprintf(listing, "Unknown declaration kind\n");
				break;
			}
		}
		else
			fprintf(listing, "Unknown node kind\n");
		for (i = 0; i < MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}