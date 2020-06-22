#include "globals.h"
#include "code.h"
#include "symtab2.h"
#include "cgen.h"

static int tmpOffset = 0;// 数据区临时变量堆栈偏移量 将临时变量保存在寄存器中，而不是经常操作内存读出和写入

static int frameOffset = 0;//当前函数的地址

static int ofpFO = 0;//函数调用时偏移，调用时加上调用函数的地址，结束后恢复
static int retFO = -1;//函数返回地址（-1）
static int initFO = -2;//加载后面的参数&变量

static void cGen(TreeNode* tree, int isAddress);

static void getDec(TreeNode* tree)
{
	BucketList* l = NULL;
	int loc;

	switch (tree->kind.dec)
	{
	//VarK和ArrayK的在自己的scope的相对地址在typecheck已经算过了
	//case VarK:
	//	if (TraceCode) { emitComment("-> VarD"); }
	//	// l = st_lookup(tree->attr.name, 1);
	//	if (symbolTable->nestLevel == 0) //0层
	//	{
	//		//修改寄存器fp
	//		emitRM("LDA", fp, -1, fp, "decrease the fp");
	//	}
	//	if (TraceCode) { emitComment("<- VarD"); }
	//	break;
	//case ArrayK:
	//	if (TraceCode) { emitComment("-> ArrayD"); }
	//	if (symbolTable->nestLevel == 0) {
	//		if (tree->child[0]) {
	//			//全局变量
	//			emitRM("LDA", fp, -tree->child[0]->val, fp, "decrease the fp");
	//		}
	//		//函数的数组参数的声明好像不会被生成代码
	//	}
	//	if (TraceCode) { emitComment("<- ArrayD"); }
	//	break;
	case FunK:
		if (TraceCode){ emitComment("-> Fun"); }
		loc = emitSkip(0);//获得当前位置 即函数的入口地址
		l = st_lookup(tree->name,-1);
		l->declaration->funcEntryLoc = loc;
		//在该函数中调用其他函数时frameOffset计算新的初始地址
		frameOffset = initFO - l->declaration->child[1]->scope->maxOffset;
		// printf("name:%s offset:%d\n", l->decl->attr.name, l->decl->child[1]->scope->maxOffset);
		//函数的起始处，保存返回地址到retFO处
		emitRM("ST", ac, retFO, fp, "FunK: store return address to retFO");
		//函数主体 {}内的复合语句
		cGen(tree->child[1], FALSE);
		// 当前frame的retFO处保存了该次调用的地址
		emitRM("LD", pc, retFO, fp, "FunK: return");//装入内存
		if (TraceCode) { emitComment("<- Fun"); }
		break;
	default:
		break;
	}
}

static void getStmt(TreeNode* tree)
{
	TreeNode* p1, * p2, * p3, * sibling;
	BucketList* l = NULL;
	int savedLoc1, savedLoc2, currentLoc;

	switch (tree->kind.stmt) 
	{
	case CallK://fixing
		if (TraceCode) { emitComment("-> Call"); }
		//计算函数参数
		emitComment("callE: cal every arguments");
		l = st_lookup(tree->name,-1); //调用的函数声明节点
		p1 = tree->child[0];//对应函数参数列表
		while (p1) {
			//cGen是整一块的连着，要先断开sibling分开生成
			sibling = p1->sibling;
			p1->sibling = NULL;
			cGen(p1, FALSE);
			emitRM("ST", ac, initFO + frameOffset , fp, "CallK: store argument");
			
			p1->sibling = sibling;
			p1 = p1->sibling;
		}
		//返回地址保存到fp中入栈
		emitRM("ST", fp, ofpFO + frameOffset, fp, "store current fp");
		emitRM("LDA", fp, ofpFO + frameOffset, fp, "push new frame");
		//将被调函数的返回地址保存到ac，运行到return后跳转
		emitRM("LDA", ac, 1, pc, "save return in ac");//在定义时已经返回地址保存到pc中
		
		//获取函数入口地址并跳转
		emitRM("LDC", fp, l->declaration->funcEntryLoc, 0, "jump to function");//ldc忽略s
		emitRM("LD", fp, ofpFO, fp, "pop current frame");//退出当前状态，重新指向当前
		if (TraceCode) { emitComment("<- Call"); }
		break;
	case ReturnK:
		if (TraceCode) { emitComment("-> Return"); }
		//return的值, 将保存到ac 
		cGen(tree->child[0], FALSE);
		//当前frame的retFO处保存了该次调用的地址，然后LD装入内存
		emitRM("LD", pc, retFO, fp, "retS: return");
		if (TraceCode) { emitComment("<- Return"); }
		break;
	case CompoundK://fixing
		if (TraceCode) { emitComment("-> Compound"); }
		// 将{}补回符号表 不然查找不到
		tree->scope->next = symbolTable;
		symbolTable = tree->scope;
		//dec 定义内部的变量
		cGen(tree->child[0], FALSE);
		//stmt 内部的各种语句
		cGen(tree->child[1], FALSE);
		//st_abrade(); // 撤回符号表
		if (TraceCode) { emitComment("<- Compound"); }
		break;
	case IfK://和tiny的一样 ok
		if (TraceCode) { emitComment("-> If"); }
		p1 = tree->child[0];
		p2 = tree->child[1];
		p3 = tree->child[2];
		/* gen code for test-part */
		cGen(p1, FALSE);
		savedLoc1 = emitSkip(1);
		emitComment("if: jump to else belongs here");
		/* recurse on then part */
		cGen(p2, FALSE);
		savedLoc2 = emitSkip(1);
		emitComment("if: jump to end belongs here");
		currentLoc = emitSkip(0);
		emitBackup(savedLoc1);
		emitRM_Abs("JEQ", ac, currentLoc, "If: jmp to else");
		emitRestore();
		/* recurse on else part */
		cGen(p3, FALSE);
		currentLoc = emitSkip(0);
		emitBackup(savedLoc2);
		emitRM_Abs("LDA", pc, currentLoc, "jf: jmp to end");
		emitRestore();
		if (TraceCode) { emitComment("<- If"); }
		break;
	case WhileK:
		if (TraceCode) { emitComment("-> While"); }
		p1 = tree->child[0];
		p2 = tree->child[1];
		//保存while语句的初始位置
		savedLoc1 = emitSkip(0);
		/* gen code for test-part */
		cGen(p1, FALSE);
		//计算括号内值是否 == 0，跳到while语句末尾
		savedLoc2 = emitSkip(1);
		emitComment("While: jump to end belongs here");
		//stmt
		cGen(p2, FALSE);
		//跳到while语句起始处
		emitRM_Abs("LDA", pc, savedLoc1, "jmp to begin");
		emitComment("While: jump to begin belongs here");
		//补全savedLoc2处的jump语句
		currentLoc = emitSkip(0);
		emitBackup(savedLoc2);

		emitRM_Abs("JEQ", ac, currentLoc, "jmp to end");//不是pc
		emitRestore();
		if (TraceCode) { emitComment("<- While"); }
		break;
	default:
		break;
	}
}

static void getExp(TreeNode* tree, int isAddress) 
{
	TreeNode* p1, * p2, * sibling;
	BucketList* l = NULL;
	int currentTmpOffset;
	switch (tree->kind.exp)
	{
	case VarEK://简单的(整型)变量
		if (TraceCode) { emitComment("-> VarEK"); }
		emitComment("VarEK: address of arr");
		//计算变量的地址
		//printf("%s\n",tree->name);
		l = st_lookup(tree->name, -1);
		//全局变量（函数外面的）对应声明的嵌套层次为0，局部变量(函数体内的声明）的大于0
		if (l->nestLevel == 0) {
			emitRM("LDA", ac, -l->memloc, gp, "VarEK: cal address in global");
		}
		else //局部变量
		{
			emitRM("LDA", ac, initFO - l->memloc, fp, "VarEK: cal address in local");
		}
		//isAddress为FALSE则不是在赋值表达式的左边
		if (!isAddress) {
			emitComment("VarEK: if ! isAddress load the value");
			emitRM("LD", ac, 0, ac, "VarEK: load value to ac");
		}
		if (TraceCode) { emitComment("<- VarEK"); }
		break;
	case ArrayEK://下标数组变量
		if (TraceCode) { emitComment("-> ArrayEK"); }
		emitComment("ArrayEK: cal address of arr(base + index)");
		//计算数组表达式的下标（保存到reg[ac]）
		cGen(tree->child[0], FALSE);
		//计算数组的基址
		l = st_lookup(tree->name, -1);
		//全局变量对应声明的嵌套层次为0，局部变量(函数体内的声明）的大于0
		if (l->nestLevel == 0) {
			emitRM("LDA", ac1, -l->memloc, gp, "ArrayEK: cal  base address in global");
		}
		else {
			emitRM("LDA", ac1, initFO - l->memloc, fp, "ArrayEK: cal  base address in local");
		}
		//若引用的是函数的数组参数 即[]，后面没加东西
		if (!l->declaration->child[0]) {
			// printf("引用数组参数name:%s line:%d\n", l->decl->attr.name, l->decl->lineno);
			emitRM("LD", ac1, 0, ac1, "ArrayEK: redirect");
		}
		//计算绝对地址，保存到reg[ac]
		emitRO("SUB", ac, ac1, ac, "ArrayEK: cal the absolute address");//自顶向下要用减
		if (!isAddress) {
			emitComment("ArrayEK: if ! isAddress load the value");
			emitRM("LD", ac, 0, ac, "ArrayEK: put value in ac");
		}
		if (TraceCode) { emitComment("<- ArrayEK"); }
		break;
	case AssignK://赋值
		if (TraceCode) { emitComment("-> Assign"); }
		//左值可能是相对地址被保存到reg[ac]
		cGen(tree->child[0], TRUE);
		currentTmpOffset = tmpOffset++;
		emitRM("ST", ac, currentTmpOffset, tp, "AssignK: push left");//从顶部push
		
		cGen(tree->child[1], FALSE);// 右值 
		// 将左值保存到 ac1 
		emitRM("LD", ac1, currentTmpOffset, tp, "AssignK: load left");
		// 右值保存到ac
		emitRM("ST", ac, 0, ac1, "AssignK: store right to left");
		if (TraceCode) { emitComment("<- Assign"); }
		break;
	case OpK:
		if (TraceCode) { emitComment("-> Op"); }
		cGen(tree->child[0], FALSE);
		//push 左操作数
		currentTmpOffset = tmpOffset++;
		emitRM("ST", ac, currentTmpOffset, tp, "OpK: push left");
		//ac = 右操作数
		cGen(tree->child[1], FALSE);
		//load左操作数
		emitRM("LD", ac1, currentTmpOffset, tp, "OpK: load left");
		switch (tree->op) {
		case PLUS:
			emitRO("ADD", ac, ac1, ac, "OpK +");
			break;
		case MINUS:
			emitRO("SUB", ac, ac1, ac, "OpK -");
			break;
		case TIMES:
			emitRO("MUL", ac, ac1, ac, "OpK *");
			break;
		case OVER:
			emitRO("DIV", ac, ac1, ac, "OpK /");
			break;
		case LT:
			emitRO("SUB", ac, ac1, ac, "op <");
			emitRM("JLT", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case NGT:
			emitRO("SUB", ac, ac1, ac, "op <=");
			emitRM("JLE", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case GT:
			emitRO("SUB", ac, ac1, ac, "op >");
			emitRM("JGT", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case NLT:
			emitRO("SUB", ac, ac1, ac, "op >=");
			emitRM("JGE", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case EQU:
			emitRO("SUB", ac, ac1, ac, "op ==");
			emitRM("JEQ", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case NEQU:
			emitRO("SUB", ac, ac1, ac, "op !=");
			emitRM("JNE", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		default:
			emitComment("BUG: Unknown operator");
			break;
		}
		if (TraceCode) { emitComment("<- Op"); }
		break;
	case ConstK:
		if (TraceCode) { emitComment("-> Const"); }
		//用LDC 装入常数
		emitRM("LDC", ac, tree->val, 0, "const: load const");
		if (TraceCode) { emitComment("<- Const"); }
		break;
	default:
		break;
	}
}

//递归遍历语法树，生成代码 默认isAddress为false
static void cGen(TreeNode* tree, int isAddress)
{
	if (tree) {
		switch (tree->nodekind) {
		case DecK:
			getDec(tree);
			break;
		case StmtK:
			getStmt(tree);
			break;
		case ExpK:
			getExp(tree, isAddress);
			break;
		default:
			break;
		}
		cGen(tree->sibling, FALSE);
	}
}

void getPreprocess()
{
	BucketList* l = NULL;
	int loc;

	if (TraceCode){emitComment("-> Input");}
	//存储函数的入口地址
	loc = emitSkip(0);
	l = st_lookup("input",-1);
	l->declaration->funcEntryLoc = loc;
	/* 函数的起始处，保存返回地址到retFO */
	emitRM("ST", ac, retFO, fp, "input: store return address to retFO");
	//函数主体
	emitRO("IN", ac, 0, 0, "input: read integer value");
	emitRM("LD", pc, retFO, fp, "input: return");
	if (TraceCode) { emitComment("<- Input"); }
	//printf("\nGenerating Input Success\n");
	if (TraceCode) { emitComment("-> Output"); }
	loc = emitSkip(0);//函数入口地址
	l = st_lookup("output",-1);
	l->declaration->funcEntryLoc = loc;
	//保存返回地址到retFO
	emitRM("ST", ac, retFO, fp, "output: store return address to retFO");
	//将参数加载到ac并输入
	emitRM("LD", ac, initFO, fp, "output: load first arg to ac");
	emitRO("OUT", ac, 0, 0, "output: print");
	emitRM("LD", pc, retFO, fp, "output: return");
	if (TraceCode) { emitComment("<- Output"); }
	//printf("\nGenerating Output Success\n");
}

void codeGen(TreeNode* syntaxTree, char* codeFile)
{
	int savedLoc, endLoc;
	BucketList* l = NULL;

	printf("\nGenerating Code...\n");
	char* s = (char*)malloc(strlen(codeFile) + 7);
	strcpy(s, "File: ");
	strcat(s, codeFile);
	emitComment("MiniC Compilation to TM Code");
	emitComment(s);

	emitComment("Standard prelude");
	emitRM("LD", gp, 0, ac, "load gp with maxaddress");// mp
	emitRM("LDA", fp, -symbolTable->location, gp, "copy offset to fp");
	emitRM("ST", ac, 0, ac, "clear location 0");
	savedLoc = emitSkip(3);
	emitComment("End of standard prelude");
	getPreprocess();
	cGen(syntaxTree, FALSE);

	//call main
	endLoc = emitSkip(0);
	emitBackup(savedLoc);
	//函数的起始处，保存返回地址到retFO处
	emitRM("LDC", ac, endLoc, 0, "main: load return address to ac");
	emitRM("ST", ac, retFO, fp, "main: store return address to retFO");
	l = st_lookup("main",-1);//和TINY相比要添加函数
	emitRM("LDC", pc, l->declaration->funcEntryLoc, 0, "jump to main");
	emitRestore();
	printf("\n end\n");
	emitComment("End of execution.");
	emitRO("HALT", 0, 0, 0, "");
	free(s);
	printf("\nGenerate Code Finished\n");
}