#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned int uint;  // unsigned_int_32 4bytes
typedef unsigned short ushort;  // unsigned_int_16 2bytes

#define BUFSIZE 1024

uchar* cReadline(FILE*);

int isUTF8(uchar*);
uchar* sep_term_input(uchar*, size_t*, int);  // maybe it's gbk or utf8.
