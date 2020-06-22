#pragma once

#ifndef _SCAN_H_
#define _SCAN_H_

#include "globals.h"

/* token的最大长度 */
#define MAXTOKENLEN 40

/* 存储每个token的lexeme */
extern char tokenString[MAXTOKENLEN + 1];

/* 返回source中下一个token */
TokenType getToken(void);
//extern  int linepos; /* current position in LineBuf */
//extern  int bufsize; /* current size of buffer string */
extern  int EOF_flag;

#endif