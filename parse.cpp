#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

/*MiniC-BNF语法*/
static TokenType token; /* holds current token */

static TreeNode* declar_list(void);//2. declaration-list→ declaration-list declaration | declaration
/* 3.declaration -> var_declaration | fun_declaration
 * 4.var_declaration -> type_specifier ID ; | type_specifier ID [ NUM ];
 * 5.type_specifier -> int | void
 * 6.fun_declaration -> type_specifier ID (params) comp_stmt
 */
static TreeNode* declaration(void);
static TreeNode* var_declaration(void);
//static TreeNode* params(void);//7. params→params-list | void
static TreeNode* param_list(void);//8.param-list→param-list, param | param
static TreeNode* param(void);//9.param→ type-specifier ID | type-specifier ID[ ]
static TreeNode* comp_stmt(void);//10.compound-stmt→ { local-declarations statement-list }
static TreeNode* local_declarations(void);//11.local-declarations→ local-declarations var-declaration |empty
static TreeNode* var_declaration(void);
static TreeNode* stmt_list(void); //12.statement - list→ statement - list statement | empty
static TreeNode* statement(void);/*13.statement→expression-stmt | compound-stmt
								  | selection-stmt | iteration - stmt | return-stmt*/
static TreeNode* expr_stmt(void);/*14.expression-stmt→ expression ； | ；*/

static TreeNode* select_stmt(void); /*15.(if) selection - stmt→ if (expression) statement
											| if（expression）statement else statement*/
static TreeNode* iter_stmt(void);//16.(while) iteration-stmt→while( expression) statement
static TreeNode* return_stmt(void);//17.return-stmt→return ；| return expression ；
static TreeNode* express(void);//18.expression→ var=expression | simple-expression
//static TreeNode* var(void);
static TreeNode* simple_express(TreeNode* temp); /*20.simple - expression→additive - expression relop additive - expression
										| additive - expression
										21.relop→<=|< | >|>= |= = |!=  */
static TreeNode* add_express(TreeNode* temp);//22 additive-expression→additive-expression addop term | term
									//23.addop→ + | -
static TreeNode* term(TreeNode* temp); //24.term→term mulop factor | factor
							//25.mulop→* |/
static TreeNode* factor(TreeNode* temp);//26 factor→(expression )| var | call | NUM
//static TreeNode* call(void); 
static TreeNode* args(void); //28.args→arg - list | empty
static TreeNode* arg_list(void); //29.arg - list→arg - list, expression | expression

static TreeNode* identifier(void);/*//19.var→ID | ID[expression]和27.call→ID(args)
由于第26条factor开头的token为id的对应的规则有两条（19和27，故将其合并在一起 */

static void syntaxError(char* message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = TRUE;
}

static void match(TokenType expected)
{
	if (token == expected) token = getToken();
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		fprintf(listing, "      ");
	}
}

static ExpType matchType()
{
	ExpType t_type = Void;
	switch (token)
	{
	case INT:  t_type = Integer; token = getToken(); break;
	case VOID: t_type = Void; token = getToken(); break;
	default: {
		syntaxError("expected a type id but got a ");
		printToken(token, tokenString);
		fprintf(listing, "\n");
		break;
	}
	}
	return t_type;
}

static int isAType(TokenType tok)
{
	if ((tok == INT) || (tok == VOID))
		return TRUE;
	else
		return FALSE;
}

//逐步对每个BNF文法递归下降分析
TreeNode * declar_list(void)
{
	TreeNode* t = NULL;
	/*
	if (token == INT || token == VOID)
	{
		t = declaration();
		TreeNode* p = t;
		while ((token != ENDFILE) && (token == INT || token == VOID))
		{
			TreeNode* q = declaration();
			if (q != NULL)
			{
				if (t == NULL)    t = p = q;
				else
				{
					p->sibling = q;
					p = q;
				}
			}
		}
	}*/

	t = declaration();
	TreeNode* p = t;
	while (token != ENDFILE)
	{
		TreeNode* q = declaration();
		if (q != NULL)
		{
			if (t == NULL)    t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

TreeNode* declaration(void)
{
	TreeNode* t = NULL;
	ExpType decType;//节点对应的类型
	char* id;

	decType = matchType();

	id = copyString(tokenString);
	match(ID); // int/void abcd ()函数， 

	switch (token) {
	case SEMI:
		t = newDeclearNode(VarK);
		if (t != NULL) {
			t->variableDataType = decType;
			t->name = id;
		}
		match(SEMI);//;变量声明
		break;
	case LSBR://'[' 数组声明
		t = newDeclearNode(ArrayK);
		if (t != NULL) {//记录数组类型
			t->variableDataType = decType;
			t->name = id;
		}
		match(LSBR);// [

		if (t != NULL)
		{
			t->val = atoi(tokenString);//val为int型
			//t->array = 0;
		}

		match(NUM);//[]中间的数值
		match(RSBR);// ]
		match(SEMI);
		break;
	case LBR://'(' 函数声明
		t = newDeclearNode(FunK);
		if (t != NULL) 
		{
			t->functionReturnType = decType;
			t->name = id;
		}
		match(LBR);
		//t->kind.dec = FunK;//函数
		if (t != NULL)
			t->child[0] = param_list();//用逗号分开的参数列表
		match(RBR);
		if (t != NULL)
			t->child[1] = comp_stmt();//复合语句(函数代码)
		break;
	default:
		syntaxError("声明上的意外token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

/*
TreeNode* params(void)//参数列表 params_list | void
{
	TreeNode* t = newParamNode(Null);
	if (token == VOID)
	{
		t->kind.param = Var;
		t->attr.type = copyString(tokenString);
		match(VOID);
	}
	else if (token == INT) {
		//t->kind.param = Var;
		//t->attr.type = copyString(tokenString);
		t->child[0] = param_list();
		//match(INT);
	}
	else
	{
		syntaxError("界限上的意外token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}*/

TreeNode* param_list(void)//函数参数(形参): params_list参数列表/void没有参数
{
	TreeNode* t;
	TreeNode* p;
	if (token == VOID) //即没有参数
	{
		match(VOID);
		return NULL;
	}
	t = param();
	p = t;
	while ((t != NULL) && (token == DOT))
	{
		match(DOT);//逗号后接着下一个参数
		TreeNode* q = param();
		if (q != NULL)
		{
			p->sibling = q;
			p = q;
		}
	}
	return t;
}

TreeNode* param(void)//单个的函数形参
{
	TreeNode* t;
	ExpType parmType;
	char* id;
	parmType = matchType();  //形参类型
	id = copyString(tokenString);
	match(ID);
	if (token == LSBR)//参数后面跟着方括号[为数组参数
	{
		match(LSBR);
		match(RSBR);
		t = newDeclearNode(ArrayK);
	}
	else
		t = newDeclearNode(VarK);//普通的变量参数

	if (t != NULL) {
		t->name = id;
		t->val = 0;
		t->variableDataType = parmType;
	}
	return t;
}

TreeNode* comp_stmt()//复合语句不用修改
{
	//TreeNode* t = newStmtNode(CompoundK);
	//match(LLBR);
	//if ((t != NULL) && (token == INT || token == VOID))
	//	t->child[0] = local_declarations();
	//if (t != NULL)
	//	t->child[1] = stmt_list();
	//match(RLBR);
	TreeNode* t = NULL;
	//fprintf(listing, "*** Entered compound_statement()\n");
	match(LLBR);
	if ((token != RLBR) && (t = newStmtNode(CompoundK)))
	{
		if (isAType(token))
			t->child[0] = local_declarations();
		if (token != RLBR)
			t->child[1] = stmt_list();
	}
	match(RLBR);
	//fprintf(listing, "*** Exiting compound_statement()\n");
	return t;
}

TreeNode* local_declarations(void)
{
	TreeNode* t = NULL;// declaration();
	TreeNode* p;

	if (isAType(token))//(token == INT) || (token == VOID)
		t = var_declaration();
	if (t != NULL)
	{
		p = t;
		while (isAType(token))
		{
			TreeNode* q = var_declaration();
			if (q != NULL)
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

TreeNode* var_declaration(void)
{
	TreeNode* t = NULL;
	ExpType   decType;
	char* id;

	decType = matchType();

	id = copyString(tokenString);
	match(ID);

	switch (token)
	{
	case SEMI:
		t = newDeclearNode(VarK);
		if (t != NULL)
		{
			t->variableDataType = decType;
			t->name = id;
		}
		match(SEMI);
		break;
	case LSBR:  // [ 数组
		t = newDeclearNode(ArrayK);
		if (t != NULL)
		{
			t->variableDataType = decType;
			t->name = id;
		}
		match(LSBR);
		if (t != NULL)
		{
			t->val = atoi(tokenString);
			//t->array = 0;//下标数组变量
		}
		match(NUM);
		match(RSBR);// ]
		match(SEMI);
		break;
	default:
		syntaxError("unexpected token ");
		printToken(token, tokenString);
		fprintf(listing, "\n");
		token = getToken();
		break;
	}
	return t;
}

TreeNode* stmt_list(void)
{
	/*
	TreeNode* t = statement();
	TreeNode* q = t;
	while ((token != RLBR) && (token != ENDFILE))
	{
		TreeNode* p = statement();
		if (p != NULL)
		{
			if (q == NULL)t = q = p;
			else
			{
				q->sibling = p;
				q = p;
			}
		}
	}*/
	TreeNode* t = NULL;
	TreeNode* ptr;
	TreeNode* newNode;
	//fprintf(listing, "*** Entered statement_list()\n");
	if (token != RLBR)
	{
		t = statement();
		ptr = t;

		while (token != RLBR)
		{
			newNode = statement();
			if ((ptr != NULL) && (newNode != NULL))
			{
				ptr->sibling = newNode;
				ptr = newNode;
			}
		}
	}
	return t;
}

TreeNode* statement(void)
{
	TreeNode* t = NULL;
	switch (token) {
	case IF: t = select_stmt(); break;
	case WHILE: t = iter_stmt(); break;
	case RETURN:    t = return_stmt(); break;
	case LLBR:    t = comp_stmt(); break;
	case ID:
	case NUM:
	case LBR:
	case SEMI:
		t = expr_stmt(); break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode* expr_stmt(void)
{
	TreeNode* t = NULL;//newStmtNode(ExpressionK);
	if (token == SEMI)
		match(SEMI);
	else if (token != RLBR)
	{
		t = express();
		match(SEMI);
	}
	return t;
}

//if
TreeNode* select_stmt(void)
{
	/*
	TreeNode * t = newStmtNode(IfK);
	match(IF);
	if (t != NULL) t->child[0] = exp();
	match(THEN);
	if (t != NULL) t->child[1] = stmt_sequence();
	if (token == ELSE) {
		match(ELSE);
		if (t != NULL) t->child[2] = stmt_sequence();
	}
	match(END);
	return t;*/
	TreeNode* t;// = newStmtNode(Ifk);
	TreeNode* expr;
	TreeNode* ifStmt;
	TreeNode* elseStmt = NULL;

	match(IF);
	match(LBR);
	expr = express();
	match(RBR);
	ifStmt = statement();

	if (token == ELSE) {

		match(ELSE);
		elseStmt = statement();//另一个语句列表
	}
	t = newStmtNode(IfK);
	if (t != NULL)
	{
		t->child[0] = expr;
		t->child[1] = ifStmt;
		t->child[2] = elseStmt;
	}
	return t;
}

//While
TreeNode* iter_stmt(void)
{
	/*
	TreeNode* t = newStmtNode(WhileK);
	match(WHILE);
	match(LBR);
	if (t != NULL) t->child[0] = express();
	match(RBR);
	if (t != NULL) t->child[1] = statement();
	*/
	TreeNode* t;
	TreeNode* expr;
	TreeNode* stmt;
	match(WHILE);
	match(LBR);
	expr = express();
	match(RBR);
	stmt = statement();
	t = newStmtNode(WhileK);
	if (t != NULL)
	{
		t->child[0] = expr;
		t->child[1] = stmt;
	}
	return t;
}

//return
TreeNode* return_stmt(void)
{
	/*
	TreeNode* t = newStmtNode(ReturnK);
	match(RETURN);
	if (token == SEMI)
		match(SEMI);
	else
	{
		t->child[0] = express();
		match(SEMI);
	}
	return t;
	*/
	TreeNode* t;
	TreeNode* expr = NULL;

	match(RETURN);

	t = newStmtNode(ReturnK);
	if (token != SEMI)
		expr = express();
	if (t != NULL)
		t->child[0] = expr;
	match(SEMI);
	return t;
}

//表达式 赋值表达式/简单表达式
TreeNode* express(void)
{
	TreeNode* t = NULL;
	TreeNode* lvalue = NULL;//在minic中唯一左值
	TreeNode* rvalue = NULL;
	int gotLvalue = FALSE;  /* boolean */

	if (token == ID)//赋值
	{
		lvalue = identifier();
		gotLvalue = TRUE;
	}
	if ((gotLvalue == TRUE) && (token == ASSIGN))
	{
		if ((lvalue != NULL) && (lvalue->nodekind == ExpK) && 
			((lvalue->kind.exp == VarEK) || (lvalue->kind.exp == ArrayEK)))
		{
			//fprintf(listing, ">>>   Generating node for ASSIGN\n");
			match(ASSIGN);
			rvalue = express();

			t = newExpNode(AssignK);
			if (t != NULL)
			{
				t->child[0] = lvalue;
				t->child[1] = rvalue;
				//t->val = 0;//添加 防止干扰
			}
		}
		else
		{
			syntaxError("attempt to assign to something not an lvalue\n");
			token = getToken();
		}
	}
	else //简单表达式
		t = simple_express(lvalue);
	return t;
}

//var&call 赋值普通变量/数组下表变量/函数名 （ident_statement）
TreeNode* identifier(void)
{
	TreeNode* t;
	TreeNode* expr = NULL;
	TreeNode* arguments = NULL;
	char* id ;

	if (token == ID)
		id = copyString(tokenString);
	match(ID);

	if (token == LBR)// ( 对应第27条规则call
	{
		match(LBR);
		arguments = args();
		match(RBR);// )

		t = newStmtNode(CallK);
		if (t != NULL)
		{
			t->child[0] = arguments;
			t->name = id;
			//t->val = 0;//添加 防止干扰
		}
	}
	/*
	else if(token == LSBR) // 对应第19条规则赋值var
	{
		if (token == LSBR)// [
		{
			match(LSBR);
			expr = express();
			match(RSBR);
			//t->child[0] = expr;
			//t->name = id;
			//t->val = 1;
		}
		t = newExpNode(IdK);//ArrE
		if(t!=NULL)
		{
			t->child[0] = expr;
			t->name = id;
			//t->array = 0;//添加 防止干扰
		}
		//if (t != NULL)
		//{
		//	t->child[0] = expr;
		//	t->name = id;
		//	t->val = 1;
		//}
	}*/
	else if (token == LSBR) // 下标数组变量
	{
		t = newExpNode(ArrayEK);
		t->name = id;
		match(LSBR);
		t->child[0] = express();
		match(RSBR);
	}
	else //普通的标志符
	{
		t = newExpNode(VarEK);
		t->name = id;
	}
	return t;
}

/* var
TreeNode* var(void)
{
	TreeNode* t = newExpNode(IdK);
	t->name = copyString(tokenString);
	match(ID);
	if (token == LBR)
	{
		match(LSBR);
		t->child[0] = express();
		match(RSBR);
	}
	return t;
}*/

//simple-express 关系操作符组成或单个关系操作符
TreeNode* simple_express(TreeNode * temp)
{
	TreeNode* t;// = add_express(temp);
	TreeNode* le = NULL;
	TreeNode* re = NULL;
	TokenType opera;
	//fprintf(listing, "*** Entered simple_expression()\n");
	le = add_express(temp);//此处修改 漏了
	if ((token == LT) || (token == NGT) || (token == GT) 
		|| (token == NLT) || (token == EQU) || (token == NEQU))
	{
		opera = token;
		match(token);
		re = add_express(NULL);

		t = newExpNode(OpK);
		if (t != NULL)
		{
			t->child[0] = le;
			t->child[1] = re;
			t->op = opera;
		}
	}
	else //仅有1个关系操作符
		t = le;
	return t;
}

//+ - 
TreeNode* add_express(TreeNode * temp)
{
	TreeNode* t = term(temp);
	while ((token == PLUS) || (token == MINUS))
	{
		TreeNode* p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = t;
			p->op = token;
			t = p;
			match(token);
			t->child[1] = term(NULL);
		}
	}
	return t;
}

// * /
TreeNode* term(TreeNode * temp)
{
	//fprintf(listing, "*** Entered term()\n");
	TreeNode* t = factor(temp);
	while ((token == TIMES) || (token == OVER))
	{
		TreeNode* p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->op = token;
			t = p;
			match(token);
			p->child[1] = factor(NULL);
		}
	}
	return t;
}

TreeNode* factor(TreeNode * temp)
{
	TreeNode* t = NULL;
	//fprintf(listing, "*** Entered factor()\n");
	if (temp != NULL)
	{
		//fprintf(listing, ">>>   Returning temp subtree\n");
		return temp;
	}
	/*
	switch (token) {
	case NUM:
		t = newExpNode(ConstK);
		if ((t != NULL) && (token == NUM)) {
			t->attr.val = atoi(tokenString);//TreeNode的val为int 要用atoi将str转为int
			t->variableDataType = Integer;//修改此处
			match(NUM);
		}
		break;
	case LBR:// ( 表达式
		match(LBR);
		t = express();
		match(RBR);
		break;
	case ID:
	{
		t = identifier();//var/call
	}
	break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}*/
	if (token == ID)
	{
		t = identifier();
	}
	else if (token == LBR)
	{
		match(LBR);
		t = express();
		match(RBR);
	}
	else if (token == NUM)
	{
		t = newExpNode(ConstK);
		if (t != NULL)
		{
			t->val = atoi(tokenString);
			t->variableDataType = Integer;
		}
		match(NUM);
	}
	else
	{
		syntaxError("unexpected token ");
		printToken(token, tokenString);
		fprintf(listing, "\n");
		token = getToken();
	}
	return t;
}

TreeNode* args(void)
{
	TreeNode* t = NULL;
	if (token != RBR)
		t = arg_list();
	return t;
}

TreeNode* arg_list(void)
{
	TreeNode* t = express();
	TreeNode* p = t;
	//while (token != RBR) // )
	//{
	//	match(DOT);
	//	TreeNode* q = express();
	//	if (q != NULL)
	//	{
	//		if (t == NULL)    t = p = q;
	//		else
	//		{
	//			p->sibling = q;
	//			p = q;
	//		}
	//	}
	//}
	while (token == DOT)
	{
		match(DOT);
		TreeNode* q = express();
		if ((p != NULL) && (t != NULL))
		{
			p->sibling = q;
			p = q;
		}
	}
	return t;
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode* parse(void)
{
	TreeNode* t;
	token = getToken();
	t = declar_list();

	if (token != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}
