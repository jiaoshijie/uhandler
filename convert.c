#include "convert.h"
#include "gbk_uniTable.h"

static const int utf8_mask[3] = {192, 224, 240}; // 110-, 1110-, 11110-

uint utf8TOutf32(uchar *uc, size_t len) {
  if (len == 0)
    return 0;
  if (len == 1)
    len = 0;
  uint ui = 0;
  ui += uc[0] & (255 >> (len + 1));
  if (len)
    for (size_t i = 1; i < len; i++) {
      ui <<= 6;
      ui += uc[i] & (255 >> 2);
    }
  return ui;
}

ushort *utf32TOutf16(uint ui, size_t *len) {
  if (ui < 0xD800 || (ui > 0xDFFF && ui < 0x10000))
    *len = 1;
  else
    *len = 2;

  ushort *us = (ushort *)malloc(*len * sizeof(ushort));
  memset(us, 0, *len * sizeof(ushort));
  if (*len == 1) {
    *us = (ushort)(ui);
  } else {
    ui -= 0x010000;
    us[0] = (ushort)(((0x000ffc00 & ui) >> 10) +
                     0xD800); // 0b11111111110000000000 = 0x000ffc00
    us[1] = (ushort)(((0x000003ff & ui) >> 00) +
                     0xDC00); // 0b00000000001111111111 = 0x000003ff
  }
  return us;
}

ushort *utf8TOutf16(uchar *uc, size_t len, size_t *u16len) {
  uint ui = utf8TOutf32(uc, len);
  return utf32TOutf16(ui, u16len);
}

uchar *utf32TOgbk(uint ui, size_t *len) {
  uchar *uc = NULL;
  if (ui & 0xffffff00) {
    *len = 2;
    uc = (uchar *)malloc(*len);
    memset(uc, 0, *len);
    const Summary *summary = NULL;
    if (ui < 0x0460) // NOTICE: ui(unsigned int) is always >= 0
      summary = &gb2312_uni2indx_page00[(ui >> 4)];
    else if (ui >= 0x2000 && ui < 0x2650)
      summary = &gb2312_uni2indx_page20[(ui >> 4) - 0x200];
    else if (ui >= 0x3000 && ui < 0x3230)
      summary = &gb2312_uni2indx_page30[(ui >> 4) - 0x300];
    else if (ui >= 0x4e00 && ui < 0x9cf0)
      summary = &gb2312_uni2indx_page4e[(ui >> 4) - 0x4e0];
    else if (ui >= 0x9e00 && ui < 0x9fb0)
      summary = &gb2312_uni2indx_page9e[(ui >> 4) - 0x9e0];
    else if (ui >= 0xff00 && ui < 0xfff0)
      summary = &gb2312_uni2indx_pageff[(ui >> 4) - 0xff0];

    if (summary) {
      unsigned short used = summary->used;
      unsigned int i = ui & 0x0f;
      if (used & ((unsigned short)1 << i)) {
        /* Keep in 'used' only the bits 0..i-1. */
        used &= ((unsigned short)1 << i) - 1;
        /* Add 'summary->indx' and the number of bits set in 'used'. */
        used = (used & 0x5555) + ((used & 0xaaaa) >> 1);
        used = (used & 0x3333) + ((used & 0xcccc) >> 2);
        used = (used & 0x0f0f) + ((used & 0xf0f0) >> 4);
        used = (used & 0x00ff) + (used >> 8);
        ushort temp = gb2312_2charset[summary->indx + used];
        uc[0] = (uchar)((temp >> 8) | 0x80); // 0x80 = 0b10000000
        uc[1] = (uchar)(temp | 0x80);
      }
    }
    if (*uc == 0 || !summary) {
      free(uc);
      return NULL;
    }
  } else {
    *len = 1;
    uc = (uchar *)malloc(*len);
    memset(uc, 0, *len);
    *uc = (uchar)ui;
  }
  return uc;
}

uchar *uft8TOgbk(uchar *uc, size_t len, size_t *lengbk) {
  uint ui = utf8TOutf32(uc, len);
  return utf32TOgbk(ui, lengbk);
}

uint gbkTOutf32(uchar *uc, size_t len) {
  uint ui = 0;
  if (len == 2) {
    uchar c1 = (uchar)(uc[0] & 0x7f);
    if ((c1 >= 0x21 && c1 <= 0x29) || (c1 >= 0x30 && c1 <= 0x77)) {
      uchar c2 = (uchar)(uc[1] & 0x7f);
      if (c2 >= 0x21 && c2 < 0x7f) {
        uint i = 94 * (c1 - 0x21) + (c2 - 0x21);
        ushort wc = 0xfffd;
        if (i < 1410) {
          if (i < 831)
            wc = gb2312_2uni_page21[i];
        } else {
          if (i < 8178)
            wc = gb2312_2uni_page30[i - 1410];
        }
        if (wc != 0xfffd) {
          ui = (uint)wc;
        }
      }
    }
  } else { // just for len == 1
    ui += (uint)(*uc);
  }
  return ui;
}

uchar *utf32TOutf8(uint ui, size_t *len) {
  if (ui > 0x10ffff)
    return NULL;
  *len = 1;
  if (ui > 0x7f)
    (*len)++;
  if (ui > 0x7ff)
    (*len)++;
  if (ui > 0xffff)
    (*len)++;
  // if (ui > 0x1fffff) return NULL;
  uchar *uc = (uchar *)malloc(*len);
  memset(uc, 0, *len);
  if (1 == *len) {
    *uc = (uchar)ui;
  } else {
    for (size_t i = *len - 1; i > 0; i--)
      uc[i] = (uchar)(0x3f & (ui >> ((*len - i - 1) * 6))) + 0x80;
    uc[0] = (uchar)((0x1f >> (*len - 2)) & (ui >> ((*len - 1) * 6))) +
            utf8_mask[*len - 2];
  }
  return uc;
}

uchar *gbkTOutf8(uchar *uc, size_t len, size_t *len8) {
  uint ui = gbkTOutf32(uc, len);
  return utf32TOutf8(ui, len8);
}

ushort *gbkTOutf16(uchar *uc, size_t len, size_t *len16) {
  uint ui = gbkTOutf32(uc, len);
  return utf32TOutf16(ui, len16);
}

uint utf16TOutf32(ushort *us, size_t len) {
  // 0 for error
  uint ui = 0;
  if (len == 1) {
    ui = (uint)(*us);
  } else if (len == 2) {
    ui += (us[0] - 0xD800) & 0x03ff;
    ui <<= 10;
    ui += ((us[1] - 0xDC00) & 0x03ff) + 0x010000;
  }
  return ui;
}

uchar *utf16TOgbk(ushort *us, size_t len, size_t *lengbk) {
  uint ui = utf16TOutf32(us, len);
  return utf32TOgbk(ui, lengbk);
}

uchar *utf16TOutf8(ushort *us, size_t len, size_t *len8) {
  uint ui = utf16TOutf32(us, len);
  return utf32TOutf8(ui, len8);
}
