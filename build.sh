#!/usr/bin/env bash

set -xe

SOURCE="convert.c output.c utils.c fhandler.c trie.c unicode.c"
CFLAGS="-Wall -Wextra -Wno-deprecated-declarations -Os"

# NOTE: if using gcc this line and above is only needed thing to do.
/usr/local/musl/bin/musl-gcc -o uc_aarch64 ${CFLAGS} ${SOURCE}
# /usr/local/musl/bin/musl-clang -o uc_aarch64 ${CFLAGS} ${SOURCE} -v   # TODO: not work
