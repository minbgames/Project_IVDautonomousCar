#pragma once

#include <string>
#include <arpa/inet.h>
#include <sys/un.h>

class UDSClient
{
private:
	int cSock;
	socklen_t cSize;
	struct sockaddr_un sAddr;
	std::string fileName;

public:
	UDSClient(std::string fileName);
	~UDSClient();

	int start();
	int close();
	float recvFloat();
	int sendInt(int v);
};
