SOURCE := convert.c output.c utils.c fhandler.c trie.c
READLINE :=# -lreadline

run: build
	@./uc

build:
	cc -o uc $(READLINE) $(SOURCE) unicode.c

test:
	cc -o uc $(READLINE) $(SOURCE) test.c

debug:
	cc -o uc -g $(READLINE) $(SOURCE)
	gdb ./uc -q
