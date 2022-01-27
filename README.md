# C Message Passing Library
## Posix based thread safe library for embedded systems

c_messageparser library is a posix based message passing library

- Thread safe
- Memory managed on stack

## Features

- Send and receive messages based on message structure.
- Max 255 messages support at any time.
- Messages are passed asynchronously.
- Library size under 8KB good for embedded system.
- C++ Library also provided for comparison.

## Assumptions
 - Assuming that library is initialized from a parent thread that spawns child threads or in an embedded system initialized upon startup.
 - Send order is not preserved in C Library.
 - Send order is preserved in the C++ Library.(FIFO)
 - Asynchrnous in nature.
 - Underlying logic uses pthreads but also supports c++11 threading.
 - Max number of messages on message heap is 255 can be changed by user.
 - Multiple messages can be sent using send.
 - If a thread wishes to receive multiple messages, recv needs to be called with thread ID until NULL pointer is returned in msg.

## Design Decisions
 - The C API uses simple array like structure to store and retrieve messages.
 - The C++ Library is included for comparison to show how complex data structutres increase memory consumed by library.
 - The actual message structure and the message heap are static variables inside the C file to make them thread safe and prevent actual threads from modifying or seeing that logic.
 - The memory allocated for the library comes from a static variable on the stack instead of heap to ensure that message passing library works even when other tasks/threads might possibly hog the heap.
 - Heap could also lead to fragmentation and hence stack has been used. In a real embedded system multiple calls to malloc/free equivalent have extra processor overheads.
 - The C-API also has a built in security feature of verifying sender thread ID in the send function to ensure spurious threads dont end up sending data.

## Tradeoffs
 -  A data structure like a hash map would be more apt in this case but to keep the implementation small(<~8KB>) and simple and most importantly suitable for embedded systems.
 -  The repo also has a c++ library that is built with hashmap and though faster it utilizes about 31KB of memory as opposed to the 8KB C Library.

## Pre-Requisites
1. OS/Emulator with posix API support or porting layer
2. GCC/G++ 5.5.0+ or equivalent
3. GNU Make or equivalent
   
## Build and Run

Project uses a simple GNU makefile to build and generate shared object and tests.

| Make target      | Output                      |
| ------           | ------                      |
| Default          | release libraries and tests |
| debug            | debug libraries and tests   |
| messageparser    | C library shared object     |
| messageparsercxx | C++ library shared object   |
| tests            | C library tests             |
| testscxx         | C++ library tests           |
| clean            | remove all artifacts        |


run make from folder. (Default release version)

```sh
make
gcc -Os  -c -Wall -Werror -fpic c_messageparser.c -lpthread -lrt
gcc -Os  -shared -o libc_messageparser.so c_messageparser.o
gcc -Os  -L./ -Wl,-rpath=./ -Wall -o tests c_messageparser_tests.c -lc_messageparser -lpthread -lrt
g++ -std=c++11 -Os -c -Wall -Werror -fpic c_messageparser.cpp -o c_messageparsercxx.o -lpthread -lrt
g++ -std=c++11 -Os -shared -o libc_messageparsercxx.so c_messageparsercxx.o
g++ -std=c++11 -Os -L./ -Wl,-rpath=./ -Wall -o testscxx c_messageparser_tests.c -lc_messageparsercxx -lpthread -lrt

```

For debug environments...

```sh
make debug
gcc -Os  -g -c -Wall -Werror -fpic c_messageparser.c -lpthread -lrt
gcc -Os  -g -shared -o libc_messageparser.so c_messageparser.o
gcc -Os  -g -L./ -Wl,-rpath=./ -Wall -o tests c_messageparser_tests.c -lc_messageparser -lpthread -lrt
g++ -std=c++11 -Os -g -c -Wall -Werror -fpic c_messageparser.cpp -o c_messageparsercxx.o -lpthread -lrt
g++ -std=c++11 -Os -g -shared -o libc_messageparsercxx.so c_messageparsercxx.o
g++ -std=c++11 -Os -g -L./ -Wl,-rpath=./ -Wall -o testscxx c_messageparser_tests.c -lc_messageparsercxx -lpthread -lrt

```
A successful make generates the following artifacts

```sh
-rw-r--r-- 1 user user  2901 Jan 27 00:16 c_messageparser.c
-rw-r--r-- 1 user user  2901 Jan 27 00:16 c_messageparser.cpp
-rw-r--r-- 1 user user  8744 Jan 27 01:06 c_messageparsercxx.o
-rw-r--r-- 1 user user   721 Jan 26 23:39 c_messageparser_defines.h
-rw-r--r-- 1 user user  2145 Jan 26 23:33 c_messageparser.h
-rw-r--r-- 1 user user  8744 Jan 27 01:06 c_messageparser.o
-rw-r--r-- 1 user user  5914 Jan 27 00:46 c_messageparser_tests.c
-rw-r--r-- 1 user user   252 Jan 27 00:17 c_messageparser_tests.h
-rwxr-xr-x 1 user user 12168 Jan 27 01:06 libc_messageparser.so
-rwxr-xr-x 1 user user 12168 Jan 27 01:06 libc_messageparsercxx.so
-rw-r--r-- 1 user user   549 Jan 26 22:47 Makefile
-rw-r--r-- 1 user user  5641 Jan 27 01:12 README.md
-rwxr-xr-x 1 user user 22048 Jan 27 01:06 tests
-rwxr-xr-x 1 user user 22048 Jan 27 01:06 testscxx
```


In order to run tests complete one of the above make targets and run
```sh
./tests
SUCCESS: testSingleThreadTxRx()
SUCCESS: testMultipleThreadTxRx()
SUCCESS: testSendMaxLimit()
SUCCESS: testRecvNone()
SUCCESS: testRepeatTxRx()
```

## Debugging

With debug builds the program and all libraries can be debugged using any GDB based tool. The debugging tool used for this program is vscode with GDB.

## Statistics

| Type           | C          | C++             |
| ------         | ------     |  ------         |
| Library Size   | 8K         | 32K             |
| Execution Time | 40ms       | 35ms            |
