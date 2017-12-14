#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <cstdio>
#include <iostream>

#include "UDSServer.hpp"

UDSServer::UDSServer(std::string filename)
{
    this->fileName = fileName;
}

UDSServer::~UDSServer()
{
    this->close();
}

int UDSServer::start()
{
    if(access(fileName.c_str(), F_OK) == 0) {
        unlink(fileName.c_str());
    }

    sSock = socket(PF_FILE, SOCK_STREAM, 0);
    if(sSock == -1) {
        perror("socket() error");
        return 1;
    }

    memset(&sAddr, 0, sizeof(sAddr));
    sAddr.sun_family = AF_UNIX;
    strcpy(sAddr.sun_path, fileName.c_str());

    if(bind(sSock, (struct sockaddr*)&sAddr, sizeof(sAddr)) == -1) {
        perror("bind() error");
        return 1;
    }

    if(listen(sSock, 1) == -1) {
        perror("listen() error");
        return 1;
    }

    std::cout << "listen..." << std::endl;

    cSize = sizeof(cAddr);
    cSock = accept(sSock, (struct sockaddr*)&cAddr, &cSize);
    if(cSock == -1) {
        perror("accept() error");
        return 1;
    }


    return 0;
}

int UDSServer::close()
{
    ::close(cSock);
    ::close(sSock);

    return 0;
}

int UDSServer::sendFloat(float data)
{
    float buf = data;
    write(cSock, &buf, sizeof(float));

    return 0;
}

int UDSServer::recvInt()
{
	int buf;
	read(cSock, &buf, sizeof(int));
	return buf;
}
