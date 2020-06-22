#include "globals.h"
#include "util.h"
#include "scan.h"
/* states in scanner DFA */
typedef enum
{
	START, INNUM, INID, INCMP, INASSIGN, INLT, INGT, INNOEQ, INCMT1, INCMT2, INCMT3, DONE
}
StateType;

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN + 1];

/* BUFLEN = length of the input buffer for
   source code lines */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line保持在当前行 */
int linepos = 0; /* current position in LineBuf LineBuf的当前位置 */
int bufsize = 0; /* current size of buffer string */
int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* 
   getNextChar从lineBuf中提取下一个非空字符,如果lineBuf耗尽的话就读取下一行
   */
static int getNextChar(void)//void可去掉
{
	if (!(linepos < bufsize))
	{
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source))
		{
			if (EchoSource) fprintf(listing, "%4d: %s", lineno, lineBuf);
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else
		{
			EOF_flag = TRUE;
			return EOF;
		}
	}
	else
		return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character
   in lineBuf */
static void ungetNextChar(void)
{
	if (!EOF_flag)
		linepos--;
}

/* 保留字的查找表格 */
static struct
{
	const char* str;
	TokenType tok;
} reservedWords[MAXRESERVED] = {
	{"if",IF},{"else",ELSE},
	{"int",INT},{"return",RETURN},{"void",VOID},
   {"while",WHILE},
};

/* lookup an identifier to see if it is a reserved word */
/* uses linear search
通过临近搜索查找一个字符看它是不是保留字*/
static TokenType reservedLookup(char* s)
{
	int i;
	for (i = 0; i < MAXRESERVED; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;//是保留字
	return ID;
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void)
{
	int tokenStringIndex = 0;/*插入tokenString的索引*/
	TokenType currentToken;/*保持当前的token以返回*/
	StateType state = START;/*当前状态：通常在START处开始*/
	int save;/*表示保存到tokenString的标志*/
	char pre_c; /*记录最后一个字符*/

	while (state != DONE)
	{
		int c = getNextChar();
		save = TRUE;
		switch (state)
		{
		case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c))
				state = INID;
			//else if ((c == '<') || (c == '>') || (c == '=') || (c == '!'))
			//	state = INCMP;
			else if (c == '=')
				state = INASSIGN;
			else if (c == '<')
				state = INLT;
			else if (c == '>')
				state = INGT;
			else if ((c == ' ') || (c == '\t') || (c == '\n'))
				save = FALSE;//空格换行和tab
			else if (c == '!')
				state = INNOEQ;
			else if (c == '/')
			{
				save = FALSE;
				state = INCMT1;
			}
			else
			{
				state = DONE;
				switch (c)
				{
				case EOF:
					save = FALSE;
					currentToken = ENDFILE;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case '/':
					currentToken = OVER;
					break;
				case '(':
					currentToken = LBR;
					break;
				case ')':
					currentToken = RBR;
					break;
				case '[':
					currentToken = LSBR;
					break;
				case ']':
					currentToken = RSBR;
					break;
				case '{':
					currentToken = LLBR;
					break;
				case '}':
					currentToken = RLBR;
					break;
				case ';':
					currentToken = SEMI;
					break;
				case ',':
					currentToken = DOT;
					break;
				default:
					currentToken = ERROR;
					break;
				}
			}
			break;
			//case INCMP:
			//	state = DONE;
			//	if (c == '=')//下一个为=
			//	{
			//		if (pre_c == '<')
			//			currentToken = NGT;
			//		else if (pre_c == '>')
			//			currentToken = NLT;
			//		else if (pre_c == '=')
			//			currentToken = EQU;
			//		else if (pre_c == '!')
			//			currentToken = NEQU;
			//		else
			//			currentToken = ASSIGN;
			//	}
			//	else//下一个不为等于，即上一个只会是< > = !
			//	{
			//		if (pre_c == '<')
			//			currentToken = LT;
			//		else if (pre_c == '>')
			//			currentToken = GT;
			//		else if (pre_c == '=')
			//			currentToken = ASSIGN;
			//		else
			//		{ /*backup in the input*/
			//			save = FALSE;
			//			currentToken = ERROR;
			//		}
			//		ungetNextChar();
			//	}
			//	break;
		case INASSIGN:
			state = DONE;
			if (c == '=')
				currentToken = EQU;
			else
			{
				currentToken = ASSIGN;
				ungetNextChar();
				save = FALSE;
			}
			break;
		case INLT:
			state = DONE;
			if (c == '=')
				currentToken = NGT;
			else
			{
				currentToken = LT;
				ungetNextChar();
				save = FALSE;
			}
			break;
		case INGT:
			state = DONE;
			if (c == '=')
				currentToken = NLT;
			else
			{
				currentToken = GT;
				ungetNextChar();
				save = FALSE;
			}
			break;
		case INNOEQ:
			state = DONE;
			if (c == '=')
				currentToken = NEQU;
			else
			{
				ungetNextChar();
				save = FALSE;
				currentToken = ERROR;//!后面不能跟其他字母
			}
			break;
			//注释
		case INCMT1:
			save = FALSE;
			if (c == '*')
				state = INCMT2;
			else
			{
				ungetNextChar();
				save = TRUE;
				state = DONE;
				currentToken = OVER;
			}
			break;
		case INCMT2:
			save = FALSE;
			if (c == '*')
				state = INCMT3;
			else
				state = INCMT2;
			break;
		case INCMT3:
			save = FALSE;
			if (c == '/')
				state = START;
			else if (c == '*')
				state = INCMT3;
			else
				state = INCMT2;
			break;
		case INNUM:
			if (!isdigit(c))
			{ /* backup in the input */
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INID://
			if (!isalpha(c))
			{ /* backup in the input*/
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = ID;
			}
			break;
		case DONE:
		default: /* should never happen */
			fprintf(listing, "词法 Bug: state= %d\n", state);
			state = DONE;
			currentToken = ERROR;
			break;
		}


		/*检测是否token的长度超越界限*/
		if ((save) && (tokenStringIndex <= MAXTOKENLEN))
			tokenString[tokenStringIndex++] = (char)c;
		if (state == DONE)
		{
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID)//查找保留字并判断（如果该标示符为保留字）
				currentToken = reservedLookup(tokenString);
		}

		pre_c = c;//记录currentToken
	}
	if (TraceScan) {
		fprintf(listing, "\t%d: ", lineno);
		printToken(currentToken, tokenString);
	}
	return currentToken;
} 