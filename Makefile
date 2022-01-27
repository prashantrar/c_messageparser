CC = gcc
cc = g++
CFLAGS = -Os 
CXXFLAGS = -std=c++11 -Os

.PHONY: clean

all: messageparser tests messageparsercxx testscxx

debug: CXXFLAGS += -g
debug: CFLAGS += -g
debug: all

messageparser: c_messageparser.c c_messageparser.h c_messageparser_defines.h
	$(CC) $(CFLAGS) -c -Wall -Werror -fpic c_messageparser.c -lpthread -lrt
	$(CC) $(CFLAGS) -shared -o libc_messageparser.so c_messageparser.o

messageparsercxx: c_messageparser.cpp c_messageparser.h c_messageparser_defines.h
	$(cc) $(CXXFLAGS) -c -Wall -Werror -fpic c_messageparser.cpp -o c_messageparsercxx.o -lpthread -lrt
	$(cc) $(CXXFLAGS) -shared -o libc_messageparsercxx.so c_messageparsercxx.o

testscxx: messageparsercxx libc_messageparsercxx.so
	$(cc) $(CXXFLAGS) -L./ -Wl,-rpath=./ -Wall -o testscxx c_messageparser_tests.c -lc_messageparsercxx -lpthread -lrt

tests: messageparser  libc_messageparser.so
	$(CC) $(CFLAGS) -L./ -Wl,-rpath=./ -Wall -o tests c_messageparser_tests.c -lc_messageparser -lpthread -lrt

clean:
	rm -rf ./~* *.o *.so tests*