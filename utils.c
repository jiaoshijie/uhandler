#include "utils.h"

static const int utf8_mask[3] = {192, 224, 240}; // 110-, 1110-, 11110-

uchar *cReadline(FILE *stream) {
  uchar *line = NULL;
  size_t size = 0, flag = 0;
  uchar *buf = (uchar *)malloc(BUFSIZE);
  memset(buf, 0, BUFSIZE);
  while (fgets((char *)buf, BUFSIZE, stream)) {
    size_t buflen = strlen((char *)buf);
    if (buf[buflen - 1] == '\n') {
      buf[--buflen] = '\0';
      flag = 1;
    }
    // NOTE: use realloc function
    line = (uchar *)realloc(line, size + buflen + 1);
    memcpy(line + size, buf, buflen + 1);
    if (flag)
      break;
    size += buflen;
    memset(buf, 0, BUFSIZE);
  }
  free(buf);
  return line;
}

uchar *sep_term_input(uchar *line, size_t *pos, int utf8) {
  if (!line[*pos])
    return NULL;
  uchar *uc = NULL;
  size_t len = 1;
  if (utf8) {
    if ((line[*pos] & utf8_mask[0]) == utf8_mask[0])
      len++;
    if ((line[*pos] & utf8_mask[1]) == utf8_mask[1])
      len++;
    if ((line[*pos] & utf8_mask[2]) == utf8_mask[2])
      len++;
  } else { // gbk(gb2312)
    if (line[*pos] & 0x80) {
      len++;
    }
  }
  uc = (uchar *)malloc(len + 1);
  memset(uc, 0, len + 1);
  memcpy(uc, &line[*pos], len);
  *pos += len;
  return uc;
}

int isUTF8(uchar *line) {
  if (line == NULL)
    return 1;
  size_t pos = 0;
  while (line[pos]) {
    if ((line[pos] & 0x80) == 0)
      pos++;
    else {
      size_t len = 0;
      if ((line[pos] & utf8_mask[0]) == utf8_mask[0])
        len++;
      if ((line[pos] & utf8_mask[1]) == utf8_mask[1])
        len++;
      if ((line[pos] & utf8_mask[2]) == utf8_mask[2])
        len++;
      if (!len)
        return 0;
      pos++;
      while (len--) {
        if ((line[pos++] & 0xC0) != 0x80)
          return 0;
      }
    }
  }
  return 1;
}
