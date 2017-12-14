#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


#include "Serial.hpp"

Serial::Serial(const char *port, int baud, int bits, bool parity, Newline newline)
{
	strncpy(portPath, port, 64);
	memset(&config, 0, sizeof(struct termios));

	int baudBit;
	int sizeBit;
	int parityBit;
	int lineBit;

	//전송속도 설정
	switch(baud) {
		case 9600:
			baudBit = B9600;
			break;
		case 19200:
			baudBit = B19200;
			break;
		case 38400:
			baudBit = B38400;
			break;
		case 57600:
			baudBit = B57600;
			break;
		case 115200:
			baudBit = B115200;
			break;
		default:
			perror("Unsupported baudrate");
			exit(1);
	}

	//전송비트 설정
	switch(bits) {
		case 5:
			sizeBit = CS5;
			break;
		case 6:
			sizeBit = CS6;
			break;
		case 7:
			sizeBit = CS7;
			break;
		case 8:
			sizeBit = CS8;
			break;
		default:
			perror("Unsupported bitsize");
			exit(1);
	}

	//패리티비트 설정
	if(parity) {
		parityBit = INPCK | PARMRK;
	}
	else {
		parityBit = IGNPAR;
	}

	//데이터구분 설정
	switch(newline) {
		case Newline::GNCR:
			lineBit = IGNCR;
			break;
		case Newline::CRNL:
			lineBit = ICRNL;
			break;
		case Newline::NLCR:
			lineBit = INLCR;
			break;
		default:
			perror("Unsupported newline");
			exit(1);
	}

	//시리얼 통신 설정
	config.c_cflag = baudBit | sizeBit | CLOCAL | CREAD;
	config.c_iflag = parityBit | lineBit;
	//not use output option
	config.c_oflag = 0;
	//not use signal bits (raw)
	config.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);
	config.c_cc[VTIME] = 0;
	//block until receive 1 byte
	config.c_cc[VMIN] = 1;

}

Serial::~Serial()
{
	this->close();
}

//시리얼포트 오픈
int Serial::open()
{
	fd = ::open(portPath, O_RDWR | O_NOCTTY | O_SYNC);
	if(fd == -1) {
		perror("Failed to open port.");
		exit(1);
	}
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &config);

	printf("serial ready\n");
	return 0;
}

//시리얼포트 닫기
int Serial::close()
{
	return ::close(fd);
}

//데이터쓰기
int Serial::writeData(char *buf, int size)
{
	return write(fd, buf, size);
}

//Serial::readByte() is lnline
