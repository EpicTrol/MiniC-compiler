#pragma once

#ifndef _SCAN_H_
#define _SCAN_H_

#include "globals.h"

/* token����󳤶� */
#define MAXTOKENLEN 40

/* �洢ÿ��token��lexeme */
extern char tokenString[MAXTOKENLEN + 1];

/* ����source����һ��token */
TokenType getToken(void);
//extern  int linepos; /* current position in LineBuf */
//extern  int bufsize; /* current size of buffer string */
extern  int EOF_flag;

#endif