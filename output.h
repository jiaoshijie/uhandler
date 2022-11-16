#pragma once
#include "utils.h"
#include <stdio.h>
#include <string.h>

void printMultiBytes(uchar *, size_t, char *);
void printUtf16Bytes(ushort *, size_t, char *);
void printUtf32Bytes(uint, char *);
