#pragma once

#include <unistd.h>
#include <termios.h>
#include <iostream>

enum class Newline
{
	GNCR, CRNL, NLCR
};

class Serial
{
private:
	int fd;
	char portPath[64];
	struct termios config;

public:

	Serial(const char *port, int baud, int bits, bool parity,
		Newline newLine = Newline::CRNL);
	~Serial();
	int open();
	int close();
	inline char readByte()
	{
		char buf;
		read(fd, &buf, 1);
		return buf;
	}
	int writeData(char *buf, int size);
};
