SOURCE := convert.c output.c utils.c fhandler.c trie.c
READLINE :=# -lreadline
CFLAGS := -Wall -Wno-deprecated-declarations -Os -pedantic

run: build
	@./uc

build:
	cc -o uc $(CFLAGS) $(READLINE) $(SOURCE) unicode.c

test:
	cc -o uc $(READLINE) $(SOURCE) test.c

debug:
	cc -o uc -g $(READLINE) $(SOURCE)
	gdb ./uc -q

install: build
	cp ./uc ~/.local/bin

uninstall:
	rm -rf ~/.local/bin
