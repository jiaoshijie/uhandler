#pragma once
#include <stdio.h>
#include <string.h>
#include "utils.h"

void printMultiBytes(uchar*, size_t, char*);
void printUtf16Bytes(ushort*, size_t, char*);
void printUtf32Bytes(uint, char*);
