#include "analyze.h"
#include "symtab2.h"
#include "util.h"
#include "globals.h"

/* 变量的内存地址的计数器 */
// static int location = 0;

static int nestNum = 0;

static TreeNode* enclosingFunction = NULL;


static void typeError(TreeNode* t, char* message) {
	fprintf(listing, "Type error at line %d: %s\n", t->lineno, message);
	printf("%s\n", t->name);
	Error = TRUE;
}

static int isArrType(TreeNode* decl) {
	return decl->nodekind == DecK && decl->kind.dec == ArrayK;
}
static int isFunType(TreeNode* decl) {
	return decl->nodekind == DecK && decl->kind.dec == FunK;
}

static int typeEqual(ExpType target, ExpType cur) {
	return target == cur;
}

//函数调用的实参列表是否与其定义的形参列表一致
static int checkFunCall(TreeNode* FunKecl, TreeNode* funCall) {
	if (!FunKecl || !funCall) {
		//传入了空指针
		return 0;
	}
	TreeNode* declArgList = FunKecl->child[0];
	TreeNode* callArgList = funCall->child[0];
	while (declArgList && callArgList) {
		if (!typeEqual(declArgList->variableDataType, callArgList->expressionType)) {
			return 0;
		}
		declArgList = declArgList->sibling;
		callArgList = callArgList->sibling;
	}
	if (declArgList || callArgList) {
		return 0;
	}
	else {
		return 1;
	}
}

static void traverse(TreeNode* t, void (*preProc) (TreeNode*), void (*postProc) (TreeNode*)) {
	if (t) {
		preProc(t);
		//遍历所有子节点
		for (int i = 0; i < MAXCHILDREN; i++) {
			traverse(t->child[i], preProc, postProc);
		}
		postProc(t);
		traverse(t->sibling, preProc, postProc);
	}
}

//向符号表的当前scope插入一个标志符
static void insertNode(TreeNode* t) {
	switch (t->nodekind) {
	case DecK:	
		switch (t->kind.dec) //在当前scope中插入一个标志符
		{
		case VarK:
			if (st_lookup(t->name, 1)) {
				typeError(t, "Redeclaration");
				symbolTable->err = TRUE;
			}
			st_insert(t->name, t, t->lineno, symbolTable->location++);
			printf("%-14s   ", t->name);
			printf("%-8d   ", symbolTable->location);
			printf("%4d ", t->lineno);
		    printf("\n");
			break;
		case ArrayK:
			if (st_lookup(t->name, 1)) {
				typeError(t, "Redeclaration");
				symbolTable->err = TRUE;
			}
			st_insert(t->name, t, t->lineno, symbolTable->location);
			if (t->child[0]) {
				symbolTable->location += t->child[0]->val;
			}
			else {
				//数组参数，传入的是地址
				symbolTable->location++;
			}
			printf("%-14s   ", t->name);
			printf("%-8d   ", symbolTable->location);
			printf("%4d ", t->lineno);
			printf("\n");
			break;
		case FunK:
			if (st_lookup(t->name, 1)) {
				typeError(t, "Redeclaration");
				symbolTable->err = TRUE;
			}
			st_insert(t->name, t, t->lineno, symbolTable->location++);
			//printSymTab(listing);
			st_newscope();
			enclosingFunction = t;
			break;
		default:
			break;
		}
		break;
	case StmtK:
		switch (t->kind.stmt) {
		case CompoundK:
			//printf("嵌套所在的line: %d\n", t->lineno);
			//printSymTab(listing);
			if (nestNum != 0)//当前scope不是函数定义的复合语句中
			{
				st_newscope();
				//初始地址继承自外部的符号表
				symbolTable->location = symbolTable->next->location;
			}
			//symbolTable->location++;
			nestNum++;
			break;
		default:
			break;
		}
	default:
		break;
	}
} 

//type checking类型检查
static void checkNode(TreeNode* t) {
	BucketList* tmp;
	switch (t->nodekind) {
	case DecK:// 声明的变量数组等必须是整型
		switch (t->kind.dec) {
		case VarK:
			if (t->variableDataType != Integer) {
				typeError(t, "Var declared with non-integer type");
				symbolTable->err = TRUE;
			}
			break;
		case ArrayK:
			if (t->variableDataType != Integer) {
				typeError(t, "Array declared with non-integer type");
				symbolTable->err = TRUE;
			}
			break;
		case FunK:
			break;
		default:
			break;
		}
		break;
	case StmtK:
		switch (t->kind.stmt) {
		case CompoundK:
			t->scope = st_abrade(); //保存对应scope用于代码生成，否则将查找不到
			//最大偏移量（{}嵌套的最深层）
			if (t->scope->location > t->scope->maxOffset) {
				t->scope->maxOffset = t->scope->location;
			}
			if (symbolTable->next)//不是函数域
			{ 
				if (t->scope->location > symbolTable->maxOffset) {
					symbolTable->maxOffset = t->scope->location;
				}
			}
			nestNum--;
			if (nestNum == 0) {
				enclosingFunction = NULL;
			}
			break;
		case CallK://是否已定义/是否为函数/参数是否对应
			tmp = st_lookup(t->name, -1);
			if (!tmp)
			{
				typeError(t, "Call withoud declaration");
				t->expressionType = Void;
			}
			else if (!isFunType(tmp->declaration))
			{
				typeError(t, "Called object is not a function");
				t->expressionType = Void;
			}
			else if (!checkFunCall(tmp->declaration, t))
			{
				typeError(t, "Call with false arguments");
				t->expressionType = Void;
			}
			else {
				t->expressionType = Integer;
			}
			break;
		case IfK://if
			break;
		case WhileK://while
			break;
		case ReturnK:
			if (t->child[0])//有返回肯定为int
			{
				if (enclosingFunction->functionReturnType == Void) {
					typeError(t, "function with void return type returned non-void value");
					symbolTable->err = TRUE;
				}
			}
			else 
			{
				if (enclosingFunction->functionReturnType != Void) {
					typeError(t, "function with non-void return type returned void");
					symbolTable->err = TRUE;
				}
			}
			break;
		default:
			break;
		}
		break;
	case ExpK:
		switch (t->kind.exp) {
		case AssignK:
			if (t->child[0]->kind.exp != VarEK && t->child[0]->kind.exp != ArrayEK) {
				typeError(t, "l-value is not var or array");
				symbolTable->err = 1;
			}
			t->val = t->child[1]->val;
			break;
		case OpK:
			if ((t->child[0]->expressionType != Integer) ||
				(t->child[1]->expressionType != Integer)) {
				typeError(t, "Op applied to non-integer");
			}
			switch (t->op) {
			case LT: case NLT: case NGT:
			case GT: case EQU: case NEQU:
				t->expressionType = Boolean;
				break;
			default:
				t->expressionType = Integer;
				break;
			}
			break;
		case ConstK:
			t->expressionType = Integer;
			break;
		case VarEK:
			tmp = st_lookup(t->name, -1);
			if (!tmp) {
				typeError(t, "Var Used without declaration");
				t->expressionType = Void;
			}
			else {
				t->expressionType = Integer;
			}
			break;
		case ArrayEK:
			tmp = st_lookup(t->name, -1);
			if (!tmp) {
				typeError(t, "Array used without declaration");
				t->expressionType = Void;
			}
			else if (!isArrType(tmp->declaration)) {
				typeError(t, "Subscripted value is not array");
				t->expressionType = Void;
			}
			else {
				t->expressionType = Integer;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
} 

void initSymbolTable() {
	st_newscope();
	//预定义input
	TreeNode* inputFun = newDeclearNode(FunK);
	inputFun->name = copyString("input");
	inputFun->functionReturnType = Integer;
	inputFun->child[1] = newStmtNode(CompoundK);//{ }
	inputFun->child[1]->scope = (SymbolTable*)malloc(sizeof(SymbolTable));
	inputFun->child[1]->scope->location = inputFun->child[1]->scope->maxOffset = 0;
	st_insert("input", inputFun, -1, symbolTable->location++);
	//预定义output
	TreeNode* outputFun = newDeclearNode(FunK);
	outputFun->name = copyString("output");
	outputFun->functionReturnType = Void;
	outputFun->child[0] = newDeclearNode(VarK);
	outputFun->child[0]->variableDataType = Integer;
	outputFun->child[1] = newStmtNode(CompoundK); //{ }
	outputFun->child[1]->scope = (SymbolTable*)malloc(sizeof(SymbolTable));
	outputFun->child[1]->scope->location = outputFun->child[1]->scope->maxOffset = 1;
	st_insert("output", outputFun, -1, symbolTable->location++);
}

void analyze(TreeNode* syntaxTree) {
	fprintf(listing, "Variable Name   Location   Line Dec Numbers\n");
	fprintf(listing, "-------------   --------   ----------------\n");
	printf("函数内定义变量符号：\n");
	initSymbolTable();
	traverse(syntaxTree, insertNode, checkNode);//typeCheck
	if (TraceAnalyze) {
		fprintf(listing, "\nSymbol table:\n");
		printSymTab(listing);
	}
}
