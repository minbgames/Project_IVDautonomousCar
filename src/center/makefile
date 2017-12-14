all: main

#CC = gcc
CC = clang++

#debug
CFLAGS = -Wall -O1 -g -std=gnu++11

#release
#CFLAGS = -03 -std=gnu++11

#libraries
LIBS = -L ./$(BINDIR) -lcenter -ludsclient -lpthread

#dir
BINDIR = bin

_OBJS = Serial.o Control.o Receiver.o PID.o LidarManager.o LMS1xx.o
OBJS = $(patsubst %,$(BINDIR)/%,$(_OBJS))

_OBJS_UDS = ../../common/UDSClient.o
OBJS_UDS = $(patsubst %,$(BINDIR)/%,$(_OBJS_UDS))

main: center.cpp $(BINDIR)/libcenter.a $(BINDIR)/libudsclient.a
	$(CC) $(CFLAGS) -o $(BINDIR)/center.elf center.cpp $(LIBS)

$(BINDIR)/%.o: %.cpp %.hpp
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/libcenter.a: $(OBJS)
	ar crv $@ $^

$(BINDIR)/libudsclient.a: $(OBJS_UDS)
	ar crv $@ $^

clean:
	rm $(BINDIR)/*
