#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(int argc , char *argv[])
{
    int connect_fd , client_fd , c , read_size;
    struct sockaddr_in server , client;
    char command[1024], buf[1024];

    //Create socket
    connect_fd = socket(AF_INET , SOCK_STREAM , 0);
    if (connect_fd == -1){
        printf("Could not create connect socket");
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    if(bind(connect_fd,(struct sockaddr *)&server , sizeof(server)) < 0){
        //print the error message
        perror("bind failed. Error");
        return 1;
    }

    listen(connect_fd , 3);

    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_fd = accept(connect_fd, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_fd < 0){
        perror("accept failed");
        return 1;
    }

    do{
      memset(buf, 0, sizeof(buf));
      memset(command, 0, sizeof(command));
			read_size = recv(client_fd , buf, sizeof(buf), 0);

      sscanf(buf, "%s", command);
      printf("%s\n", command);

			if(strcmp(command, "ls") == 0){
						write(client_fd , command , sizeof(command));
			}else{
        		write(client_fd, buf, sizeof(buf));
      }

		}while(read_size > 0);

    return 0;
}
