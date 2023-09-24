#ifndef _U_FHANDLER_H_
#define _U_FHANDLER_H_

#include "trie.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *cfopen(char *path, char *);
void parsingFile(FILE *, Trie *);
int cWrite2file(char *path, KV *, size_t);

#endif  // _U_FHANDLER_H_
