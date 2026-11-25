#include<sys/wait.h>
#include<netinet/in.h>
#include<unistd.h>
#include<fcntl.h>
#include <poll.h>
#include<errno.h>
#include<signal.h>
#include "sendAndRecvAll.h"
volatile sig_atomic_t childDied = 0;


void reapingChild(int *clientsCount){
	int status;
	while(true){
		pid_t retPid = waitpid(-1 , &status , WNOHANG);
		if(retPid < 0){
			if(errno == ECHILD)
				break;
			perror("waitpid() failed");
			break;
		}
		else if(retPid == 0)
			break;
		(*clientsCount)--;
		printf("child with pid = %d reaped\n",retPid);
		if(WIFSIGNALED(status)){
			printf("child killed by the signal = %d",WTERMSIG(status));
		}
	}
	printf("Total Connected Clients = %d\n",*clientsCount);
}

int handleClient(int client_fd){

	while(1){
		ssize_t n = cmnRecvFun(client_fd);
		if(n == 0){
			printf("Connection Closed");
			break;		
		}else if(n < 0)
			return cmnErrorFun(n);
		n = cmnSendFun(client_fd);
		if(n < 0)
			return cmnErrorFun(n);
		else if(n == 0)
			printf("Nothing was sent\n");
	}
	close(client_fd);
	return 0;
}
void handleSignal(int sign){
	childDied = 1;
}
int main(){
	int clientsCount = 0;
	int socket_fd , prcsId;
	socket_fd = socket(AF_INET , SOCK_STREAM , 0);
	if(socket_fd < 0){
		perror("Socket Creation Failed : ");
		return 1;
	}
	int opt = 1;
	printf("Socket created with socket_id = %d\n",socket_fd);
	if(setsockopt(socket_fd , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt)) < 0){
		perror("setsockopt() failed");
		return 7;
	}
	struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(socket_fd , (struct sockaddr *)&server_addr , sizeof(server_addr)) < 0){
		perror("Bind failed : ");
		return 2;
	}
	if(listen(socket_fd , 7) < 0){
		perror("Listen failed : ");
		return 3;
	}
	struct pollfd fds[1];
	fds[0].fd = socket_fd;
	fds[0].events = POLLIN;
	signal(SIGCHLD , handleSignal);
	while(true){
		int fdsReady = poll(fds , 1 , -1);
		if(fdsReady < 0){
			if(errno != EINTR){
				perror("poll() failed");
				return 8;
			}
			if(childDied == 1){
				reapingChild(&clientsCount);
				childDied = 0;
				continue;
			}
		}
		if(fds[0].revents & POLLIN){
			int client_fd = accept(socket_fd , NULL , NULL);
			if(client_fd < 0){
				perror("accept() failed");
				return 4;
			}
			prcsId = fork();
			if(prcsId < 0){
				perror("fork() failed");
				return 9;
			}
			else if(prcsId == 0){
				close(socket_fd);
				return handleClient(client_fd);
			}
			clientsCount++;
			close(client_fd);
			printf("Total Connected Clients = %d\n",clientsCount);
		}
	}
	if(prcsId != 0)
		close(socket_fd);
	return 0;
}
