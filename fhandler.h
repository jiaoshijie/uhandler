#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "utils.h"
#include "trie.h"

FILE* cfopen(char* path, char*);
void parsingFile(FILE*, Trie*);
int cWrite2file(char* path, KV*, size_t);
