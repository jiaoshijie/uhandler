# unicode

## TODO

- [x] unicode to gbk
- [x] gbk to unicode
- [x] multi-convert(to 8, 16, 32, gbk)
- [x] implement `readline()`
- [x] check input(utf8 gbk)(modify `get_utf8_char` to `sep_term_input`)
- [x] free malloc memory
- [x] Is gbk multi-btyes(check) or wide-character?---gbk is multi-bytes encodings.
- [x] analyze file(only for utf8 and gbk)
- [x] what is wide character? (utf16 for win and utf32 for linux)
- [x] simple ui

## some info

### printf("\x..\x..");

使用printf和字节流(bytes string)来输出非ascii字符, `\x..\x..`根据系统编码来确定使用什么编码.

- GNU/Linux: utf-8(如果locale为utf-8的话), 不过也可以使用utf-32(`\U........`)来输出
- Win: gbk

### usigned char \*uch = "三";(字节流<->multi-bytes)

根据写该源代码时所使用的文件编码来决定"三"的编码.

- 如果文件为gbk编码, 则赋值到\*uch中的值为`\xC8\xFD`("三"的gbk编码)
- 如果文件为utf-8编码, 则赋值到\*uch中的值为`\xCE\xB8\x89`("三"的utf-8编码)

### wchar_t \*wch = L"三"; wide-character->宽字符(字符流<->character string)(?)

- sizeof():
  * GNU/Linux: 4 bytes -- utf-32 -- LE(小端(intel))
  * win: 2 bytes -- utf-16 -- LE(小端(intel))
- 仅支持`utf-8 -> wchar_t`, 如果文件编码为gbk则在编译时会报错

### input from terminal

- 同`printf("\x..\x..");`, 跟随系统编码
  * GNU/Linux: utf-8
  * win: gbk

## Reference

- [unicode.org](https://www.unicode.org/faq/utf_bom.html)
- [stackoverflow](https://stackoverflow.com/questions/496321/utf-8-utf-16-and-utf-32)
- [utf32to16](https://stackoverflow.com/questions/66679330/convert-unicode-codepoint-to-utf-16)
- [unicodebook](https://unicodebook.readthedocs.io/programming_languages.html)
- [cprogramming](https://www.cprogramming.com/tutorial/unicode.html)
- [天城文书](https://www.52unicode.com/devanagari-zifu)
- [severalCommonlyCharEncoding](https://dailc.github.io/2017/05/17/severalCommonlyCharEncoding.html)
- [cjk unicode character code point range](https://stackoverflow.com/questions/1366068/whats-the-complete-range-for-chinese-characters-in-unicode)
- [libiconv](https://www.gnu.org/software/libiconv/)
- [trie(prefix tree)](https://en.wikipedia.org/wiki/Trie)
- [detect OS](https://www.geeksforgeeks.org/how-to-detect-operating-system-through-a-c-program/)
