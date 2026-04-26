SRC = $(wildcard engine/*.c)

linux: 
	gcc -g -I . test.c $(SRC) -o test -lm

windows:
	x86_64-w64-mingw32-gcc -g -I . test.c $(SRC) -o ~/VM\ Shared/text.exe -lm
	