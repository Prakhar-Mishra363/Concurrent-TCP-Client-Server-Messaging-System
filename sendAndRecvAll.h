#ifndef SEND_RECV_ALL_H
#define SEND_RECV_ALL_H

#include<stddef.h>
#include<sys/types.h>
#include<sys/socket.h>  
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>

ssize_t sendAll(int , const void * , size_t);
ssize_t recvAll(int , void * , size_t);
ssize_t cmnRecvFun(int);
ssize_t cmnSendFun(int);
ssize_t cmnErrorFun(ssize_t);
#endif
