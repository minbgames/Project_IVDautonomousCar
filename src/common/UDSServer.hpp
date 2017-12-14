#pragma once

#include <string>
#include <arpa/inet.h>
#include <sys/un.h>

class UDSServer
{
private:
	int sSock;
	int cSock;
	socklen_t cSize;
	struct sockaddr_un sAddr;
	struct sockaddr_un cAddr;
	std::string fileName;

public:
	UDSServer(std::string fileName);
	~UDSServer();

	int start();
	int close();
	int sendFloat(float data);
	int recvInt();
};
