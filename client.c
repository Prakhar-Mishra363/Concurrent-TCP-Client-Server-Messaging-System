#include<netinet/in.h>
#include<unistd.h>
#include<fcntl.h>
#include "sendAndRecvAll.h"
int main(){
	int client_fd = socket(AF_INET , SOCK_STREAM , 0);
	if(client_fd < 0){
		perror("Socket failed : ");
		return 11;
	}
	printf("Created socket id = %d\n",client_fd);
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(client_fd , (struct sockaddr *)&server_addr , sizeof(server_addr)) < 0){
		perror("Connect failed\n");
		return 12;
	}
	while(true){
		ssize_t n = cmnSendFun(client_fd);
		if(n < 0)
			return cmnErrorFun(n);
		else if(n == 0){
			printf("Nothing was sent\n");
			continue;
		}
		n = cmnRecvFun(client_fd);
		if(n < 0)
			return cmnErrorFun(n);
		else if(n == 0){
			printf("Connection Closed\n");
			break;
		}
	}
	return 0;
}
