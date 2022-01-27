# C Message Passing Library
## Posix based thread safe library for embedded systems

c_messageparser library is a posix based message passing library

- Thread safe
- Memory managed on stack

## Features

- Send and receive messages based on message structure.
- Max 255 messages support at any time.
- Messages are passed asynchronously.
- Library size under 8KB good for embedded system

## Assumptions
 - Send order is not preserved.
 - Asynchrnous in nature.
 - Underlying logic uses pthreads but also supports c++11 threading.
 - Max number of messages on message heap is 255.

## Design Decisions
 - The C API uses simple array like structure to store and retrieve messages.
 - The actual message structure and the message heap are static variables inside the C file to make them thread safe and prevent actual threads from modifying or seeing that logic.

## Tradeoffs
 -  A data structure like a hash map would be more apt in this case but to keep the implementation small(<~8KB>) and simple and most importantly suitable for embedded systems.

## Pre-Requisites
1. OS/Emulator with posix API support or porting layer
2. GCC/G++ 5.5.0+ or equivalent
3. GNU Make or equivalent
   
## Build and Run

Project uses a simple GNU makefile to build and generate shared object and tests.

| Make target | Output |
| ------ | ------ |
| Default | release library and tests |
| debug | debug library and targets |
| messageparser| library shared object |
| tests | library tests |
| clean | remove all artifacts |


run make from folder. (Default release version)

```sh
make
gcc -Os  -c -Wall -Werror -fpic c_messageparser.c -lpthread -lrt
gcc -Os  -shared -o libc_messageparser.so c_messageparser.o
gcc -Os  -L./ -Wl,-rpath=./ -Wall -o tests c_messageparser_tests.c -lc_messageparser -lpthread -lrt
```

For debug environments...

```sh
make debug
gcc -Os  -g -c -Wall -Werror -fpic c_messageparser.c -lpthread -lrt
gcc -Os  -g -shared -o libc_messageparser.so c_messageparser.o
gcc -Os  -g -L./ -Wl,-rpath=./ -Wall -o tests c_messageparser_tests.c -lc_messageparser -lpthread -lrt
```
A successful make generates the following artifacts

```sh
-rw-r--r-- 1 user user  2901 Jan 27 00:16 c_messageparser.c
-rw-r--r-- 1 user user   721 Jan 26 23:39 c_messageparser_defines.h
-rw-r--r-- 1 user user  2145 Jan 26 23:33 c_messageparser.h
-rw-r--r-- 1 user user  8744 Jan 27 01:06 c_messageparser.o
-rw-r--r-- 1 user user  5914 Jan 27 00:46 c_messageparser_tests.c
-rw-r--r-- 1 user user   252 Jan 27 00:17 c_messageparser_tests.h
-rwxr-xr-x 1 user user 12168 Jan 27 01:06 libc_messageparser.so
-rw-r--r-- 1 user user   549 Jan 26 22:47 Makefile
-rw-r--r-- 1 user user  5641 Jan 27 01:12 README.md
-rwxr-xr-x 1 user user 22048 Jan 27 01:06 tests
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
## Plugins

Dillinger is currently extended with the following plugins.
Instructions on how to use them in your own application are linked below.

| Plugin | README |
| ------ | ------ |
| Dropbox | [plugins/dropbox/README.md][PlDb] |
| GitHub | [plugins/github/README.md][PlGh] |
| Google Drive | [plugins/googledrive/README.md][PlGd] |
| OneDrive | [plugins/onedrive/README.md][PlOd] |
| Medium | [plugins/medium/README.md][PlMe] |
| Google Analytics | [plugins/googleanalytics/README.md][PlGa] |