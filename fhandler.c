#include "fhandler.h"

FILE *cfopen(char *path, char *m) { // NOTICE: "b" only for windows system.
  FILE *f = fopen(path, m);
  if (f == NULL) {
    fprintf(stderr, "\033[31merror\033[0m: %s\n", strerror(errno));
  }
  return f;
}

void parsingFile(FILE *f, Trie *r) {
  uchar *uc;
  while (!feof(f)) {
    size_t pos = 0, t = 0;
    uchar *line = cReadline(f);
    if (!line)
      break;
    int isutf8 = isUTF8(line);
    while ((uc = sep_term_input(line, &pos, isutf8)) != NULL) {
      trieUpdate(r, uc, pos - t);
      free(uc);
      t = pos;
    }
    free(line);
  }
  fclose(f); // this file will not be used anymore.
}

int cWrite2file(char *path, KV *head, size_t len) {
  FILE *f = cfopen(path, "w");
  if (f != NULL) {
    fprintf(f, "symbol,count\n");
    for (size_t i = 0; i < len; i++) {
      if (fprintf(f, "'%s',%ld\n", head[i].symbol, head[i].count) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        fclose(f);
        return 0;
      }
    }
    fclose(f);
  } else
    return 0;
  return 1;
}
