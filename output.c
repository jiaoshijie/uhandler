#include "output.h"

void printMultiBytes(uchar* uc, size_t len, char* prompt) {
  if (!prompt) prompt = "";
  printf("%s\033[36m", prompt);
  for (size_t i = 0; i < len; i++)
    // printf("%s\\x%02x\\x%02x\n", prompt, (uchar)((us & 0xff00) >> 8), (uchar)(us & 0x00ff));
    printf("\\x%02x", uc[i]);
  printf("\033[0m\n");
}

void printUtf16Bytes(ushort* us, size_t len, char* prompt) {
  if (!prompt) prompt = "";
  printf("%s\033[36m", prompt);
  for (size_t i = 0; i < len; i++) {
    printf("\\u%04x", us[i]);
  }
  printf("\033[0m\n");
}

void printUtf32Bytes(uint ui, char* prompt) {
  if (!prompt) prompt = "";
  printf("%s\033[36m\\U%08x\033[0m\n", prompt, ui);
}
