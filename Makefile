# Faculty of Information Technology, Brno University of Technology
# Subject: ISA (Network Applications and Network Administration)
# Description: POP3 Client
# Date created: October 2017
# Author: Jan Kubica
# Login: xkubic39
# Email: xkubic39@stud.fit.vutbr.cz
# File: Makefile - code compilation
#
# Usage:
#   - compile program:        make
#   - clean binaries:         clean
#   - pack to *.tar:          pack

# Compiler
CC=g++

# Compile arguments (-pedantic -Wall -W)
CFLAGS=-std=c++11
LDFLAGS=-L/usr/local/ssl/lib
LDLIBS=-lssl -lcrypto

# Filenames and extensions
FILE=popcl
EXT=cpp
DOC=manual.pdf README
LOGIN=xkubic39

#Compiling
make: popcl.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(FILE) $(FILE).$(EXT) src/args.$(EXT) src/connection.$(EXT) src/pop3man.$(EXT) src/fileman.$(EXT)

# Removing binaries
clean:
	rm $(FILE)

# Packing
pack:
	tar -cf $(LOGIN).tar Makefile src/ $(FILE).$(EXT) $(DOC)

