SOURCE := convert.c output.c utils.c fhandler.c trie.c
READLINE :=# -lreadline
CFLAGS := -Wall -Wextra -Werror -Wno-deprecated-declarations -Os

build:
	cc -o uc $(CFLAGS) $(READLINE) $(SOURCE) unicode.c

run: build
	@./uc

test:
	cc -o uc $(READLINE) $(SOURCE) test.c -g

debug:
	cc -o uc -g $(READLINE) $(SOURCE) unicode.c
	gdb ./uc -q

install: build
	cp ./uc ~/.local/bin

uninstall:
	rm -rf ~/.local/bin/uc
