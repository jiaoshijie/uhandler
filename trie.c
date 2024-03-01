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
// void sortKV(KV *begin, size_t len,
//             int seq) { // seq: 1 for negative seq, 0 for positive seq
//   KV *end = begin + len, temp;
//   int flag = 1;
//   for (KV *i = begin; i != end - 1; i++) {
//     for (KV *j = begin; j != end - 1 - (i - begin); j++) {
//       if ((j->count > (j + 1)->count) ^ seq) {
//         temp = *j;
//         *j = *(j + 1);
//         *(j + 1) = temp;
//         flag = 0;
//       }
//     }
//     if (flag)
//       break;
//     flag = 1;
//   }
// }

// quick sort
void sortKV(KV *begin, size_t len, int seq) {
  if (len == 0) return;
  void quicksort(KV *begin, KV *end, int seq) {
    if (begin == end - 1) return;
    KV *b = begin, *e = end;
    KV kv = *b;
    while (b != e - 1) {
      while (b != e - 1 && ((e - 1)->count > kv.count) ^ seq) e--;
      if (b != e - 1) {
        *b = *(e - 1);
        b++;
      }
      while (b != e - 1 && (b->count < kv.count) ^ seq) b++;
      if (b != e - 1) {
        *(e - 1) = *b;
        e--;
      }
    }
    *b = kv;
    if (b != begin)
      quicksort(begin, b, seq);
    if (b + 1 != end)
      quicksort(b + 1, end, seq);
  }
  quicksort(begin, begin + len, seq);
}

// merge sort
// void sortKV(KV *begin, size_t len, int seq) {
//   if (len == 0 || len == 1) return;
//   void mergesort(KV *b1, KV *e1, KV *b2, KV* e2, int seq) {
//     if (b1 == e1 - 1) return;
//     KV* m1 = b1 + (e1 - b1) / 2, *mm = m1;
//     KV* m2 = b2 + (e2 - b2) / 2;
//     mergesort(b2, m2, b1, m1, seq);
//     mergesort(m2, e2, m1, e1, seq);

//     while (b2 != e2) {
//       if (b1 != mm && (m1 == e1 || (b1->count <= m1->count) ^ seq)) {
//         *(b2++) = *(b1++);
//       } else {
//         *(b2++) = *(m1++);
//       }
//     }
//   }
//   KV *work = (KV *)malloc(sizeof(KV) * len);
//   memcpy(work, begin, len * sizeof(KV));
//   mergesort(work, work + len, begin, begin + len, seq);
//   free(work);
// }

void freeKV(KV *head, size_t len) {
  for (size_t i = 0; i < len; i++) {
    free(head[i].symbol);
  }
  free(head);
}
