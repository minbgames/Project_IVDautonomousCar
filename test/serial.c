#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

double estimate(clock_t, clock_t);

int receiveData(int, char*);


int main(void)
{
	char portname[32] = "/dev/ttyUSB0";

	int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
	assert(fd > -1);

	struct termios serial;
	memset(&serial, 0, sizeof(struct termios));

	//115200Baud, 8Bit
	serial.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	//ignore parity, map CR to NL
	serial.c_iflag = IGNPAR | ICRNL;
	//unuse output option
	serial.c_oflag = 0;
	//raw input (unuse signal bits)
	serial.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);

	serial.c_cc[VTIME] = 0;
	//block until receive 1 byte
	serial.c_cc[VMIN] = 1;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &serial);


	char buf[32];
	int recvSize = 15; //18byte - magic number

	int n, i;


	int cnt = 0;

	clock_t setTime;
	clock_t stopTime;

	int temp;

	char sendData[14] = {'S', 'T', 'X', 0x01, 0x00, 0x01, 0x00, 0x00,
						0xFE, 0xA0, 0x01, 0, 0x0D, 0x0A};


	for (;;) {

		setTime = clock();

		receiveData(fd, buf);

		stopTime = clock();


		system("clear");
		printf("%d ", cnt++);

		for(i = 0; i<13; i++) {
			printf("%X ", buf[i]);
		}
		putchar('\n');

		if (buf[0] == 0)
			puts("AorM : manual");
		else
			puts("AorM : auto");

		if (buf[1] == 0)
			puts("E-STOP : off");
		else
			puts("E-STOP : on");

		if (buf[2] == 0)
			puts("E-STOP : forward drive");
		else if (buf[2] == 1)
			puts("E-STOP : neutral");
		else
			puts("E-STOP : backward drive");

		printf("SPEED : 0x%X 0x%X\n", buf[3], buf[4]);
		printf("STEER : %hd\n", buf[5] | (buf[6]<<8&0xFF00));
		printf("BRAKE : %d\n", buf[7]);
		printf("ENC : 0x%X 0x%X 0x%X 0x%X\n", buf[8], buf[9], buf[10], buf[11]);

		printf("ALIVE : %hhu\n", buf[12]);

		putchar('\n');


		printf("%.2lf per second\n", 1/estimate(setTime, stopTime));


		write(fd, sendData, 14);
		sendData[11] += 1;
	}


	close(fd);

	return 0;


}



double estimate(clock_t setTime, clock_t stopTime)
{
	return ((double)stopTime - (double)setTime) / (CLOCKS_PER_SEC/1000);
}


int receiveData(int fd, char *buf)
{
	char magic;
	char temp[13];
	int flag = 1;


	for(;flag;) {
		read(fd, &magic, 1);
		if (magic != 'S')
			continue;

		read(fd, &magic, 1);
		if (magic != 'T')
			continue;

		read(fd, &magic, 1);
		if (magic != 'X')
			continue;

		for(int i=0; i<13; i++) {
			read(fd, &temp[i], 1);
		}

		read(fd, &magic, 1);
		if (magic != 0x0D)
			continue;

		read(fd, &magic, 1);
		if (magic != 0x0A)
			continue;

		flag = 0;
	}

	memcpy(buf, temp, 13);

	return 0;

}
