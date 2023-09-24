#ifndef _U_CONVERT_H_
#define _U_CONVERT_H_

#include "utils.h"
#include <stdlib.h>
#include <string.h>

// TO utf-8
uchar *utf32TOutf8(uint, size_t *);
uchar *utf16TOutf8(ushort *, size_t, size_t *);
uchar *gbkTOutf8(uchar *, size_t, size_t *);

// TO utf-32
uint utf8TOutf32(uchar *, size_t);
uint utf16TOutf32(ushort *, size_t);
uint gbkTOutf32(uchar *, size_t);

// TO utf-16
ushort *utf32TOutf16(uint, size_t *);
ushort *utf8TOutf16(uchar *, size_t, size_t *);
ushort *gbkTOutf16(uchar *, size_t, size_t *);

// TO gbk(gb2312)
// NOTICE: gbk actually is multibyte encodings, (single byte for english, double
// bytes for chinese)
uchar *utf32TOgbk(uint, size_t *);
uchar *uft8TOgbk(uchar *, size_t, size_t *);
uchar *utf16TOgbk(ushort *, size_t, size_t *);

#endif // _U_CONVERT_H_
