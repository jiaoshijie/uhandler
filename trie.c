#include "trie.h"

Trie *trieNewNode() {
  Trie *newN = (Trie *)malloc(sizeof(Trie));
  newN->count = 0;
  newN->is_terminal = 0;
  memset(newN->children, 0, 256 * sizeof(Trie *));
  return newN;
}

void trieUpdate(Trie *r, uchar *uc, size_t len) {
  Trie *tr = r;
  for (size_t i = 0; i < len; i++) {
    if (tr->children[uc[i]] == NULL) {
      tr->children[uc[i]] = trieNewNode();
    } else {
      if (tr->is_terminal)
        tr->is_terminal = 0;
    }
    tr = tr->children[uc[i]];
  }
  tr->is_terminal = 1;
  if (tr->count == 0)
    r->count++; // root->count records the the amount of node
  tr->count++;
}

size_t trieFind(Trie *r, uchar *uc, size_t len) {
  Trie *tr = r;
  for (size_t i = 0; i < len; i++) {
    if (tr->children[uc[i]] == NULL)
      return 0;
    tr = tr->children[uc[i]];
  }
  return tr->count;
}

void trieFree(Trie *r) {
  if (!r->is_terminal)
    for (size_t i = 0; i < 256; i++) {
      if (r->children[i]) {
        trieFree(r->children[i]);
      }
    }
  free(r);
}

void trieIterate(Trie *r, size_t d, KV *head, size_t *index, uchar *uc) {
  if (r->is_terminal) {
    head[*index].symbol = (uchar *)malloc(d + 1);
    memset(head[*index].symbol, 0, d + 1);
    memcpy(head[*index].symbol, uc, d);
    head[*index].count = r->count;
    (*index)++;
    return;
  }
  for (size_t i = 0; i < 256; i++) {
    if (r->children[i]) {
      uc[d] = (uchar)i;
      trieIterate(r->children[i], d + 1, head, index, uc);
      uc[d] = 0;
    }
  }
}

KV *genKV(Trie *r) {
  KV *head = (KV *)malloc(sizeof(KV) * r->count);
  memset(head, 0, sizeof(KV) * r->count);
  size_t index = 0;
  uchar uc[4];
  memset(uc, 0, 4);
  trieIterate(r, 0, head, &index, uc);
  return head;
}

void printKV(KV *head, size_t len) {
  for (size_t i = 0; i < len; i++) {
    printf("'%s': \033[36m%ld\033[0m\n", head[i].symbol, head[i].count);
  }
}

// Bubble sort
void sortKV(KV *begin, size_t len,
            int seq) { // seq: 1 for negative seq, 0 for positive seq
  KV *end = begin + len, temp;
  int flag = 1;
  for (KV *i = begin; i != end - 1; i++) {
    for (KV *j = begin; j != end - 1 - (i - begin); j++) {
      if ((j->count > (j + 1)->count) ^ seq) {
        temp = *j;
        *j = *(j + 1);
        *(j + 1) = temp;
        flag = 0;
      }
    }
    if (flag)
      break;
    flag = 1;
  }
}

void freeKV(KV *head, size_t len) {
  for (size_t i = 0; i < len; i++) {
    free(head[i].symbol);
  }
  free(head);
}
