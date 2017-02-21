#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc , char *argv[]){
    int sock, size, filehandle;
    struct sockaddr_in server;
    char message[32] , server_reply[1024], quit[5], getCheck[4], *file, filename[32];

    //Verify IP was provided
    if(argc != 2){
      printf("Usage: Server IP address");
      return 1;
    }

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1){
        perror("Could not create socket");
    }

    //Set up server object
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);

    //Connect to server
    if(connect(sock ,(struct sockaddr *)&server , sizeof(server)) < 0){
        perror("connect failed. Error");
        return 1;
    }

    do{
        //clear message and reply arrays
        memset(server_reply, 0, sizeof(server_reply));
        memset(message, 0, sizeof(message));

        //Accept user choice and parse accordingly
        printf("ftp> ");
        fgets(message, 32, stdin);
        strncpy(quit, message, 4);
        quit[4] = '\0';

        //Send command to server
        if(send(sock , message , strlen(message) , 0) < 0){
            perror("Send failed");
            return 1;
        }

        //Seperate pathing if user uses get command
        strncpy(getCheck, message, 3);
        getCheck[3] = '\0';
        if(strcmp(getCheck, "get") == 0){

            //Get file size to allocate memory
            if(recv(sock, &size, sizeof(int), 0) < 0){
                perror("recv failed");
                break;
            }

            //If size is zero, either file does not exist or is directory
            if(size == 0){
                printf("No such file in the directory\n");
                continue;
            }

            //Get filename for creation after download, and allocate file memory
            recv(sock, filename, sizeof(filename), 0);
            file = malloc(size);
            recv(sock, file, size, 0);
            filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);

            //write data from server into file and close file.
            write(filehandle, file, size);
            close(filehandle);
            printf("File: %s recieved\n", filename);
        }else{
            //Receive information per request from the server
            if(recv(sock , server_reply , sizeof(server_reply) , 0) < 0){
                perror("recv failed\n");
                break;
            }
            printf("%s\n",server_reply);
        }

    }while(strcmp(quit,"quit") != 0);

    close(sock);
    return 0;
}
