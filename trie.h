#pragma once
#include "utils.h"

typedef struct Trie {
  size_t count;
  struct Trie *children[256];  // unsigned char = 8 bit, 2^8 = 256
  uchar is_terminal;
}Trie;

typedef struct {
  uchar *symbol;
  size_t count;
}KV;

Trie* trieNewNode();
void trieUpdate(Trie*, uchar*, size_t);  // update count and insert new trie node
size_t trieFind(Trie*, uchar*, size_t);
void trieIterate(Trie*, size_t, KV*, size_t*, uchar*);
void trieFree(Trie*);


KV* genKV(Trie*);
void sortKV(KV*, size_t, int);
void printKV(KV*, size_t);
void freeKV(KV*, size_t);
