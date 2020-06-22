#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"
#include "cgen.h"
#include <iostream>
using namespace std;
/*全局变量*/
int lineno = 0;
FILE* source;
FILE* listing;
FILE* code;//指向目标汇编代码文件的地址

int EchoSource = FALSE;// 是否显示输入的文件
int TraceScan = FALSE; // 是否打印token
int TraceParse = TRUE; // 是否显示语法树
int TraceAnalyze = TRUE;// 
int TraceCode = TRUE; // 
int Error = FALSE;

int main(int argc, char* argv[])
{
	TreeNode* syntaxTree;
	char file[200];

	cout << "请输入文件名(本文件夹下的文件,如a.txt/b.txt)：";
	cin >> file;
	source = fopen(file, "r");
	if (source == NULL) {
		fprintf(stderr, "文件 %s 没有找到\n", file);
		exit(1);
	}
	//strcpy(file, argv[1]);
	//if (strchr(file, '.') == NULL)
	//	strcat(file, ".c-");

	//source = fopen(file, "r");
	//if (source == NULL)
	//{
	//	fprintf(stderr, "文件 %s 没有找到\n", file);
	//	exit(1);
	//}

	listing = stdout;
	EOF_flag = FALSE;
	fprintf(listing, "*********************************************\n\n");
	fprintf(listing, "**************Minic- 编译器: %s**************\n\n", file);
	fprintf(listing, "*********************************************\n\n");
	//fprintf(listing, "C- 词法分析: %s\n",file);

	//语法分析
	syntaxTree = parse();
	if (TraceParse) {
		fprintf(listing, "\n*****************C- 语法树:*****************\n");
		printTree(syntaxTree);
	}

	//语义分析
	if (!Error)
	{
		fprintf(listing, "*** Building symbol table...\n");
		//buildSymbolTable(syntaxTree);
		analyze(syntaxTree); //analyse2
		fprintf(listing, "*** Performing type checking...\n");
		//typeCheck(syntaxTree);
	}

	//中间代码生成
	if (!Error)
	{
		char* codeFile;
		int fnlen = strcspn(file, ".");//取输入的文件名（.前面的字符）防止保存的文件被之前的覆盖掉
		codeFile = (char*)calloc(fnlen + 4, sizeof(char));
		strncpy(codeFile, file, fnlen);//将strcspn提取的文件名存入codeFile(输出文件)
		strcat(codeFile, ".tm");
		code = fopen(codeFile, "w");
		if (code == NULL) {
			printf("Unable to open %s\n", codeFile);
			exit(1);
		}
		codeGen(syntaxTree, codeFile);
		//if (!Error)
		//{
		//	fprintf(listing, "*** Output written \n");

		//	if (TraceCode)
		//		fprintf(listing, "*** CODE TRACING OPTION ENABLED; see output\n");
		//}
		fclose(code);
		//free(codeFile);
	}
	fclose(source);
	return 0;
}
