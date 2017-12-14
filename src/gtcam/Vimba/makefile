all: main
.PHONY: clean

SHELL:=/bin/bash -O extglob

#CC = gcc
CC = clang++

#debug
CFLAGS = -Wall -O1 -g -std=gnu++11

#release
#CFLAGS = -03 -std=gnu++11

#libraries
LIBS = `pkg-config opencv --libs` \
		-L. -lVimbaC \
		-L. -lVimbaCPP \
		-L. -lVimbaImageTransform

#includes
INCL = -I./ `pkg-config opencv --cflags`

#dir
BINDIR = bin

_OBJS = FrameObserver.o \
		ApiController.o \
		FrameEvent.o \
		VimbaCam.o

OBJS = $(patsubst %,$(BINDIR)/%,$(_OBJS))

main: $(OBJS)
	#$(CC) $(CFLAGS) -o $(BINDIR)/program.elf $(OBJS) $(LIBS)
	ar crv $(BINDIR)/libvimba.a $^

$(BINDIR)/%.o: %.cpp
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCL)


clean:
	rm $(BINDIR)/*
