PREFIX ?= /usr/local

CC=g++

CFLAGS= -g -static -Werror -Wno-reorder
LDPATH= 
LIBPATH = -L /usr/local/lib
LIBS= 
LIBNAME=
#TLOPT=/C /P64

OBJS=\
        zCSV.o


all: libzetcsv.a $(OBJS)

clean:
	rm -rf $(OBJS) libzetcsv.a

install: libzetcsv.a
	cp -v ./zCSV.h $(PREFIX)/include/
	cp -v ./libzetcsv.a $(PREFIX)/lib/

libzetcsv.a : zCSV.o
	@rm -f ./libzetcsv.a
	ar rcs ./libzetcsv.a ./zCSV.o

zCSV.o: zCSV.cpp zCSV.h
	$(CC) $(CFLAGS) -c zCSV.cpp

