#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>

#include "UDSClient.hpp"

UDSClient::UDSClient(std::string filename)
{
    this->fileName = fileName;
}

UDSClient::~UDSClient()
{
    this->close();
}

int UDSClient::start()
{

    cSock = socket(PF_FILE, SOCK_STREAM, 0);
    if(cSock == -1) {
        perror("socket() error");
        return 1;
    }

    memset(&sAddr, 0, sizeof(sAddr));
    sAddr.sun_family = AF_UNIX;
    strcpy(sAddr.sun_path, fileName.c_str());

    if(connect(cSock, (struct sockaddr*)&sAddr, sizeof(sAddr)) == -1) {
        perror("connect() error");
        return 1;
    }

    return 0;
}

int UDSClient::close()
{
    ::close(cSock);
}

float UDSClient::recvFloat()
{
    float buf;
    read(cSock, &buf, sizeof(float));
    return buf;
}

int UDSClient::sendInt(int v)
{
	int buf = v;
	write(cSock, &buf, sizeof(int));
	return 0;
}
