#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>

int main(int argc , char *argv[])
{
    int connect_fd , client_fd , c , read_size, filehandle, size;
    struct sockaddr_in server , client;
    struct stat fstat;
    char command[32], buf[32], filename[32], out[1024], error[] = "Error not accepted command";
    DIR *d;
    struct dirent *dir;

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

			if(strcmp(command, "ls") == 0 || strcmp(command, "dir") == 0){
            d = opendir(".");
            sprintf(out, "\nContents of Directory:\n");

            if(d){
              while ((dir = readdir(d)) != NULL){
                strcat(out, dir->d_name);
                strcat(out, "\n");
              }
              closedir(d);
            }
						write(client_fd , out , sizeof(out));

			}else if(strcmp(command, "get") == 0){
            sscanf(buf, "%s %s", command, filename);
            stat(filename, &fstat);
            filehandle = open(filename, O_RDONLY);

            size = fstat.st_size;
            if(filehandle == -1){
                size = 0;
            }
            send(client_fd, &size, sizeof(int), 0);

            if(size){
                send(client_fd, filename, sizeof(filename), 0);
                sendfile(client_fd, filehandle, NULL, size);
            }

      }else if(strcmp(command, "quit") == 0){
            write(client_fd , "Goodbye" , sizeof("Goodbye"));
      }else{
        		write(client_fd, error, sizeof(error));
      }

		}while(read_size > 0);

    return 0;
}
