#ifndef _U_OUTPUT_H_
#define _U_OUTPUT_H_

#include "utils.h"
#include <stdio.h>
#include <string.h>

void printMultiBytes(uchar *, size_t, char *);
void printUtf16Bytes(ushort *, size_t, char *);
void printUtf32Bytes(uint, char *);

#endif  // _U_OUTPUT_H_
