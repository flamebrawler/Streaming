#ifndef NETWORKING_H
#define NETWORKING_H

#include <winsock2.h>
#include <stdio.h>

int setupSock();

void receive(SOCKET s, char* buf, int bytes);

int receiveInt(SOCKET s, int bytes);

void sendInt(SOCKET s, int data, int bytes);

int bytestoint(const char* data, int pos, int bytes);

int inttobytes(char* buffer, int data, int pos, int bytes);

#endif