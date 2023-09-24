#include <locale.h>
#include <signal.h>
#include <stdio.h>
// #include <readline/readline.h>  // only for linux
// #include <readline/history.h>  // only for linux

#include "convert.h"
#include "fhandler.h"
#include "output.h"
#include "trie.h"
#include "utils.h"

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

static int quit = 0;

static void ui();
static void menu();
static void submenu();
static void menu_1();
static int menu_2();
static void submenu_2(Trie *);
static void submenu_3(KV *, size_t);
static void submenu_sort(KV *, size_t, int); // for 4 and 5

static void clear();
static void clear_n();
static void pause();
static void utf8Tother(uchar *, size_t);
static void gbkTother(uchar *, size_t);

static void INThanler();

int main() {
  // char *locale = setlocale(LC_ALL, "en_US.utf8");  // en_US.utf8
  // char *locale = setlocale(LC_ALL, "zh_CN.gbk");   // zh_CN.gbk--do nothing
  setlocale(LC_ALL, "en_US.utf8"); // en_US.utf8
  signal(SIGINT, INThanler);
  ui();
  clear();
  return 0;
}

void ui() {
  while (!quit) {
    clear();
    menu();
    char ch = getchar();
    clear_n();
    if (ch == '1') {
      menu_1();
    } else if (ch == '2') {
      while (menu_2() == 2)
        ;
    } else if (ch == '3' || ch == 'q' || ch == EOF) {
      quit = 1;
    }
  }
}

void menu() {
  puts(
      "**********************************************************************");
  puts(
      "******************************** Menu ********************************");
  puts(
      "**********************************************************************");
  puts("");
  puts("1: Enter a character and output its unicode and gbk(if has) encoding");
  puts("2: Analyze the number of occurrences of characters in a file");
  puts("3: Quit(q)");
  puts("");
  printf("> ");
}

void submenu() {
  puts("**************************************************************");
  puts("************************** Sub Menu **************************");
  puts("**************************************************************");
  puts("");
  puts("1: Print results to standard output");
  puts("2: Find the number of times a character appears in the file");
  puts("3: Write the results to a specified file");
  puts("4: Sort the results (positive order)");
  puts("5: Sort the results (reverse order)");
  puts("6: Parsing another file");
  puts("7: Return to the previous menu(-)");
  puts("8: Quit(q)");
  puts("");
  printf("> ");
}

void menu_1() {
  printf("Enter a character> ");
  uchar *uc = NULL, *line = cReadline(stdin);
  if (line == NULL) {
    quit = 1;
    return;
  }
  size_t len = 0;
  int isutf8 = isUTF8(line);
  uc = sep_term_input(line, &len, isutf8);
  free(line);
  printf("\nEncoding: \033[33m%s\033[0m\n\n", isutf8 ? "utf-8" : "gbk");
  printf("source symbol: \033[32m%s\033[0m\n", uc);
  if (isutf8)
    utf8Tother(uc, len);
  else
    gbkTother(uc, len);
  free(uc);
  pause();
}

int menu_2() {
  int sub_quit = 0;
  printf("Enter the file path to parse> ");
  uchar *path = cReadline(stdin);
  FILE *f = cfopen((char *)path, "r"); // maybe add "b" for windows
  if (path)
    free(path);
  if (f != NULL) {
    Trie *root = trieNewNode();
    parsingFile(f, root);
    KV *head = genKV(root);
    while (!sub_quit) {
      clear();
      submenu();
      char ch = getchar();
      clear_n();
      if (ch == '1') {
        printKV(head, root->count);
        pause();
      } else if (ch == '2') {
        submenu_2(root);
      } else if (ch == '3') {
        submenu_3(head, root->count);
      } else if (ch == '4') {
        submenu_sort(head, root->count, 0);
      } else if (ch == '5') {
        submenu_sort(head, root->count, 1);
      } else if (ch == '6') {
        sub_quit = 2;
      } else if (ch == '7' || ch == '-') {
        sub_quit = 1;
      } else if (ch == '8' || ch == 'q' || ch == EOF) {
        sub_quit = 1;
        quit = 1;
      }
    }
    freeKV(head, root->count);
    trieFree(root);
  } else {
    pause();
  }
  return sub_quit;
}

void submenu_2(Trie *r) {
  printf("Enter the character you want to check> ");
  uchar *line = cReadline(stdin), *uc;
  int isutf8 = isUTF8(line);
  size_t len = 0;
  uc = sep_term_input(line, &len, isutf8);
  free(line);
  printf("\nThe character '\033[33m%s\033[0m' appears \033[36m%ld\033[0m "
         "time(s) in this file.\n",
         uc, trieFind(r, uc, len));
  free(uc);
  pause();
}

void submenu_3(KV *head, size_t len) {
  printf("Enter the file path to write to> ");
  uchar *path = cReadline(stdin);
  puts("\nWriting...\n");
  if (cWrite2file((char *)path, head, len))
    puts("Writing finished!");
  free(path);
  pause();
}

void submenu_sort(KV *head, size_t len, int seq) {
  // for 4 and 5
  puts("\nSorting...\n");
  sortKV(head, len, seq);
  puts("Sort finished!");
  pause();
}

void clear() {
  puts(""); // to clear "> "
  system(CLEAR);
}

void clear_n() {
  char ch = getchar();
  while (ch != '\n' && ch != EOF)
    ch = getchar();
}

void pause() {
  printf("\nPress \033[31menter\033[0m key to continue...");
  clear_n();
}

void utf8Tother(uchar *utf8, size_t len) {
  size_t len16 = 0, lengbk = 0;
  uint utf32 = utf8TOutf32(utf8, len);
  ushort *utf16 = utf32TOutf16(utf32, &len16);
  uchar *gbk = utf32TOgbk(utf32, &lengbk);
  printMultiBytes(utf8, len, "utf-8: ");
  printUtf16Bytes(utf16, len16, "utf-16: ");
  printUtf32Bytes(utf32, "utf-32: ");
  if (gbk) {
    printMultiBytes(gbk, lengbk, "gbk: ");
    free(gbk);
  }
  free(utf16);
}

void gbkTother(uchar *gbk, size_t len) {
  size_t len16 = 0, len8 = 0;
  uint utf32 = gbkTOutf32(gbk, len);
  uchar *utf8 = utf32TOutf8(utf32, &len8);
  ushort *utf16 = utf32TOutf16(utf32, &len16);
  printMultiBytes(gbk, len, "gbk: ");
  printMultiBytes(utf8, len8, "utf-8: ");
  printUtf16Bytes(utf16, len16, "utf-16: ");
  printUtf32Bytes(utf32, "utf-32: ");
  free(utf8);
  free(utf16);
}

void INThanler() {
  clear();
  exit(0);
}
