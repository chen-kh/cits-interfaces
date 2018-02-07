#!/bin/bash
# note that this is a temp file for compiling, finally there should be a makefile

# first compile all .c in folder TCSAE
# cd ../TCSAE; make -f makefile.am.sample

# then compile your interfaces
gcc -I../TCSAE -o encode_decode.o -c encode_decode.c
gcc -I../TCSAE -o citssocket.o -c citssocket.c
gcc -I../TCSAE -o parser.o -c parser.c
gcc -I../TCSAE -o creator.o -c creator.c
gcc -I../TCSAE -o test-sample.o -c test-sample.c
gcc -I../TCSAE -o test-sample.exe ../TCSAE/*.o citssocket.o parser.o creator.o encode_decode.o test-sample.o
if [ "$?" -eq "0" ]; then
	echo "now you can run test-sample.exe"
fi