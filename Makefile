CC = gcc
cc = g++
CFLAGS = -Os 
CXXFLAGS = -Os

.PHONY: clean

all: messageparser tests

debug: CXXFLAGS += -g
debug: CFLAGS += -g
debug: all

messageparser: c_messageparser.c c_messageparser.h c_messageparser_defines.h
	$(CC) $(CFLAGS) -c -Wall -Werror -fpic c_messageparser.c -lpthread -lrt
	$(CC) $(CFLAGS) -shared -o libc_messageparser.so c_messageparser.o

tests: messageparser  libc_messageparser.so
	$(CC) $(CFLAGS) -L./ -Wl,-rpath=./ -Wall -o tests c_messageparser_tests.c -lc_messageparser -lpthread -lrt

clean:
	rm -rf ./~* *.o *.so tests