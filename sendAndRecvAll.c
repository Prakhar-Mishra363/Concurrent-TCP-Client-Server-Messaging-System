#include "sendAndRecvAll.h"
ssize_t sendAll(int fd , const void * buffer , size_t totalLen){

	size_t bytesSendSoFar = 0;
	while(bytesSendSoFar < totalLen){
		ssize_t n = send(fd , (const char *)buffer + bytesSendSoFar , totalLen - bytesSendSoFar , 0);
		if(n < 0)
			return -1;
		if(n == 0)
			return 0;
		bytesSendSoFar += n; 
	}
	return bytesSendSoFar; 
	
}

ssize_t cmnErrorFun(ssize_t n){
	if(n == -1){
		perror("send()/recv() failed");
		return 14;
	}else if(n == -11){
		perror("malloc() failed");
		return 15;
	}
	return n;
}


ssize_t recvAll(int fd , void * buffer , size_t totalLen){

	size_t bytesRecvSoFar = 0;
	while(bytesRecvSoFar < totalLen){
		ssize_t n = recv(fd , (char *)buffer + bytesRecvSoFar , totalLen - bytesRecvSoFar , 0);
		if(n < 0)
			return -1;
		if(n == 0)
			return 0;
		bytesRecvSoFar += n; 
	}
	return bytesRecvSoFar; 
}

ssize_t cmnSendFun(int fd){
	char *sendData = malloc(501 * sizeof(char));
	if(sendData == NULL)
		return -11;
	printf("Enter the msg for receiver : ");
	fgets(sendData , 500 , stdin);
	size_t length = strlen(sendData) - 1;
	uint32_t networkMsg = htonl(length);
	ssize_t n = sendAll(fd , &networkMsg , sizeof(networkMsg));
	if(n < 0)
		return n;
	n = sendAll(fd , sendData , length);
	if(n < 0)
		return n;
	free(sendData);
	return n;	
}

ssize_t cmnRecvFun(int fd){
	uint32_t networkMsg;
	ssize_t n = recvAll(fd , &networkMsg , sizeof(networkMsg));
	if(n < 0)
		return n;
	else if(n == 0)
		return 0;	
	uint32_t msgLength = ntohl(networkMsg);
	char *recvData = malloc((msgLength + 1) * sizeof(char));
	if(recvData == NULL)
		return -11;
	n = recvAll(fd , recvData , msgLength);
	if(n < 0)
		return n;
	else if(n == 0)
		return 0;
	recvData[msgLength] = '\0';
	printf("Received msg = %s\n",recvData);
	free(recvData);
	return n;
}

