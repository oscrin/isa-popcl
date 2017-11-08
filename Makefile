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
#   - pack to *.tgz:          pack

# Compiler
CC=g++

# Compile arguments (-pedantic -Wall -W)
CFLAGS=-std=c++11

# Filenames and extensions
FILE=popcl
EXT=cpp
DOC=-doc.pdf
LOGIN=xkubic39

#Compiling
make: popcl.cpp
	$(CC) $(CFLAGS) -o $(FILE) $(FILE).$(EXT) src/args.$(EXT) src/connection.$(EXT) src/pop3man.$(EXT) src/fileman.$(EXT)

# Removing binaries
clean:
	rm $(FILE)

# Packing
pack:
	tar -cf $(LOGIN).tar Makefile $(FILE).$(EXT) auth_file.txt src/
	gzip $(LOGIN).tar
	mv $(LOGIN).tar.gz $(LOGIN).tgz

