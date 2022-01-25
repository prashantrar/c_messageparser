CC = gcc
CFLAGS =
MKDIR_P = mkdir -p

.PHONY: messageparser

all: messageparser tests

messageparser: c_messageparser.c c_messageparser.h c_messageparser_defines.h c_messageparser.o
	$(CC) -g -c -Wall -Werror -fpic c_messageparser.c
	$(CC) -g -shared -o libc_messageparser.so c_messageparser.o

tests: messageparser  libc_messageparser.so
	$(CC) -g -L./ -Wl,-rpath=./ -Wall -o tests c_messageparser_tests.c -lc_messageparser

clean:
	rm -rf ./~* *.o *.so tests