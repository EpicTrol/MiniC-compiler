# Compile_MiniC

​       本项目为一个Mini C编译器，可以读入cpp/txt文件，进行单词扫描、词法分析、语法分析和语义分析，根据分析结果产生中间代码，并调用虚拟机解释器执行对应功能。注：本项目只含命令行界面的运行代码，具体的图形界面可采用MFC或qt等进行构建。

[TOC]

## 1 使用说明

### · 主菜单

<img src="\ScreenShot\1.png" alt="1" style="zoom:50%;" />

### · 单词扫描

<img src=\ScreenShot\2.png" alt="2" style="zoom:50%;" />

<img src="\ScreenShot\3.png" alt="3" style="zoom:50%;" />

### · 词法分析

<img src="\ScreenShot\5.png" alt="5" style="zoom:50%;" />

<img src="\ScreenShot\6.png" alt="6" style="zoom:50%;" />

<img src="\ScreenShot\7.png" alt="7" style="zoom:50%;" />

<img src="\ScreenShot\8.png" alt="8" style="zoom:50%;" />

### · 语法分析

<img src="\ScreenShot\9.png" alt="9" style="zoom:50%;" />

<img src="\ScreenShot\10.png" alt="10" style="zoom:50%;" />

### · 语义分析

<img src="\ScreenShot\11.png" alt="11" style="zoom:50%;" />

### · 中间代码生成

<img src="\ScreenShot\12.png" alt="12" style="zoom:50%;" />

<img src="\ScreenShot\16.png" alt="16" style="zoom:50%;" />

### · 虚拟机解释器

<img src="\ScreenShot\13.png" alt="13" style="zoom:50%;" />

<img src="\ScreenShot\14.png" alt="14" style="zoom:50%;" />

<img src="\ScreenShot\15.png" alt="15" style="zoom:50%;" />

## 2 测试用例

test1.cpp/t1.txt/t2.txt

```c
/* A program to perform Euclid's 
    Algorithm to comput gcd.    */

int gcd (int u, int v)
{  if (v == 0)  return u;
   else return gcd(v, u-u/v*v);
   /* u-u/v*v == u mod v */
}

void main(void)
{   int x; int y;
    x=input();
    y=input();
    output(gcd(x,y));
}
```

## 3 版本迭代

**Version-0.5** 2020/4/20 词法分析、状态集表示功能基本实现

**Version-0.6** 2020/4/21 修复了词法分析过程读取注释符不完整的BUG

```c
// /号的情况：/运算 或 /*多行注释
				case '/':
				{
					s = s + ch;
					i++;
					ch = Buffer[i];
					if (ch == '*')
					{
						s = s + ch;
						type = NOTE;
						i++;
						// 跳过注释内容
						while (true)
						{
							// 直到扫描到*/为止
							if ((ch = Buffer[i]) == '*' && Buffer[i + 1] == '/')
								break;
							i++;
						}
							
					}
					break;
				}
```

**Version-0.7** 2020/4/22 基于MFC的GUI界面设计

**Version-1.0** 2020/4/28 加入语法分析功能

**Version-1.2** 2020/4/29 美化界面、字体修改

**Version-1.5** 2020/5/15 语法分析功能重构

* 在语义分析部分处理时感觉之前的语法分析部分代码写的不是很好，主要是在结点类型划分上，结合语义分析部分设计结点后只需要划分4类（globals.h），同时优化了变量名代码更清晰易读，并且由于之前对于数组的变量（例如a[i]）只是通过空格缩进来体现，但和普通的变量标识符还是要区分开来，所以讲原先的IdK拆为VarEK, ArrayEK来表示普通变量和数组下标变量

```c++
typedef enum { StmtK, ExpK, DecK } NodeKind;//3大类结点
typedef enum { IfK, WhileK, ReturnK, CallK, CompoundK } StmtKind;
typedef enum { OpK, VarEK, ArrayEK, ConstK, AssignK } ExpKind;//此处将IdK拆开为VarEk和ArrayEk更好操作
typedef enum { ArrayK, VarK, FunK } DecKind;//声明类型：数组，变量，函数
```

* 同时加上类型检测器

```
typedef enum { Void, Integer, Boolean} ExpType;
```

**Version-1.6** 2020/5/20 语义分析功能实现，修改了语法分析模块的一些小BUG

* 由于前面的语法分析部分发现对一些变量解析的不是很全面，有些变量在分析语法树过程中并没有记录其变量类型，因此根据语义分析部分需要将修改util.cpp，重新成立各种变量结点，例如将形参，变量、函数返回值等用1个ExpType来统一规定其类型：Void, Integer, Boolean；同时将三种类型树节点（newStmtNode，newExpNode，newDeclearNode）的定义修改，添加对于的行号和函数入口标记：

```c++
t->lineno = lineno;
t->funcEntryLoc = 0;
```

**Version-1.7** 2020/5/22 优化语法分析模块`parse.cpp`

* 根据util和globals变量优化修改`parse.cpp`文件下TreeNode部分函数

包括：

```c++
static ExpType matchType()
static int isAType(TokenType tok)
```

在判断变量赋值和函数调用时有点小冲突 由于26条规则factor→(expression )| var | call | NUM中 var和call的token开头都为id，因此将这两条规则合并为TreeNode* identifier(void)，处理起来更方便：

```c++
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
		}
	}
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
```

**Version-1.9** 2020/6/2 加入中间代码生成的功能，生成汇编指令

**Version-2.0** 2020/6/12 加入虚拟机解释器

**Version-2.1** 2020/6/15 修复了一些小BUG

* 头文件互相包含，会导致编译的时候出现编译交叉导致编译出错，例如会出现语法错误：缺少“；”或者xx不是xxx的成员
* LNK2005 - 全局变量重复声明导致的

**Version-2.2** 2020/6/17 完善中间代码功能，测试虚拟机解释器

