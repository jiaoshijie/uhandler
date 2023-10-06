#include <stdio.h>
// #include <readline/readline.h>
// #include <readline/history.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "utils.h"
#include "convert.h"
#include "output.h"
#include "fhandler.h"
#include "trie.h"

// unicode character: 😈 😘
void test();
void filetest();
void testsort();

int main() {
  char *locale = setlocale(LC_ALL, "en_US.utf8");  // en_US.utf8
  // char *locale = setlocale(LC_ALL, "zh_CN.gbk");   // zh_CN.gbk--do nothing
  testsort();
  return 0;
}

void testsort() {
  FILE *f = cfopen("./test.c", "r");
  Trie* root = trieNewNode();
  parsingFile(f, root);
  KV* head = genKV(root);
  printKV(head, root->count);
  sortKV(head, root->count, 1);
  printf("---------------------------------------------------\n");
  printKV(head, root->count);
}

void filetest() {
  FILE* f = cfopen("./main.txt", "r");
  Trie* root = trieNewNode();
  parsingFile(f, root);
  KV* head = genKV(root);
  sortKV(head, root->count, 1);
  printKV(head, root->count);
  cWrite2file("./main.csv", head, root->count);
  freeKV(head, root->count);
  trieFree(root);
}

void test() {
  // uchar *line = (uchar*)readline(NULL);  // only for linux
  uchar *line = cReadline(stdin);
  size_t pos = 0, t = 0, len16 = 0, len = 0;  // len for utf8 or gbk.
  uchar *utf8, *gbk;
  ushort *utf16;
  uint utf32;
  int isutf8 = isUTF8(line);
  printf("\nEncoding: %s\n\n", isutf8 ? "utf-8" : "gbk");
  if (isutf8) {
    while((utf8 = sep_term_input(line, &pos, isutf8)) != NULL) {
      printf("-----------------------------------------\n");
      printf("character symbol: %s\n", utf8);
      utf32 = utf8TOutf32(utf8, pos - t);
      utf16 = utf32TOutf16(utf32, &len16);
      gbk = utf32TOgbk(utf32, &len);
      printMultiBytes(utf8, pos - t, "utf-8: ");
      printUtf16Bytes(utf16, len16, "utf-16: ");
      printUtf32Bytes(utf32, "utf-32: ");
      if (gbk) {
        printMultiBytes(gbk, len, "gbk: ");
        free(gbk);
      }
      free(utf8);
      free(utf16);
      t = pos;
    }
  } else {
    while((gbk = sep_term_input(line, &pos, isutf8)) != NULL) {
      printf("-----------------------------------------\n");
      printf("character symbol: %s\n", gbk);
      utf32 = gbkTOutf32(gbk, pos - t);
      utf8 = utf32TOutf8(utf32, &len);
      utf16 = utf32TOutf16(utf32, &len16);
      printMultiBytes(gbk, pos - t, "gbk: ");
      printMultiBytes(utf8, len, "utf-8: ");
      printUtf16Bytes(utf16, len16, "utf-16: ");
      printUtf32Bytes(utf32, "utf-32: ");
      free(gbk);
      free(utf8);
      free(utf16);
      t = pos;
    }
  }
  free(line);
}

#if 0
void testTrie() {
  Trie* root = trieNewNode();
  uchar *uc = "\xe6\x9d\xb0";
  trieInsert(root, uc, strlen(uc));
}

void testfhandler() {
  const char* path = "./main.txt";
  FILE* f = cfopen(path);
  uchar* line = cReadline(f);
  printf("%s\n", line);
  line = cReadline(f);
  printf("%s\n", line);
  fclose(f);
}

void bad() {
  for (unsigned int i = 0x4e00; i == 0x4e00; i++) {
    unsigned char ch[16];
    wchar_t wc[16];
    memset(ch, 0, 16);
    memset(wc, 0, 16 * sizeof(wchar_t));
    snprintf(ch, 16, "\\U%08x", i);
    swprintf(wc, 16, L"%s", ch);
    wprintf(L"%ls\n", wc);
  }
}

void testTreadline() {
  // printf("%d\n", ((unsigned short) 1 << 15));
  // unsigned char* line = (unsigned char*)readline(NULL);
  // if (line[0] == 0) printf("is empty\n");
  // else printf("%s\n", line);
  // fgets();
  uchar* line = cReadline();
  printf("%ld: %s\n", strlen(line), line);
  free(line);
}
#endif
