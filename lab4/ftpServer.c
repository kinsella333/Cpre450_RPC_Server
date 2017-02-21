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
    struct stat fileStat;
    char command[32], buf[32], filename[32], out[1024], error[] = "Error not accepted command";
    DIR *d;
    struct dirent *dir;

    //Create socket
    connect_fd = socket(AF_INET , SOCK_STREAM , 0);
    if (connect_fd == -1){
        printf("Could not create connect socket");
    }

    //Prepare the server object
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    //Bind server to socket
    if(bind(connect_fd,(struct sockaddr *)&server , sizeof(server)) < 0){
        perror("bind failed. Error");
        return 1;
    }

    //listen over connection socket for client.
    listen(connect_fd , 3);
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_fd = accept(connect_fd, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_fd < 0){
        perror("accept failed");
        return 1;
    }

    do{
      //clear recieve buf and command array
      memset(buf, 0, sizeof(buf));
      memset(command, 0, sizeof(command));

      //Listen for client query
			read_size = recv(client_fd , buf, sizeof(buf), 0);

      //Parse command type
      sscanf(buf, "%s", command);

      //If ls or dir, return current directory contents.
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
            //Get filename
            sscanf(buf, "%s %s", command, filename);
            //Get file stats on size to allocate
            stat(filename, &fileStat);

            //Open and read file contents
            filehandle = open(filename, O_RDONLY);

            //Verify file exists and is a file not a directory
            size = fileStat.st_size;
            if(filehandle == -1){
                size = 0;
            }
            if(S_ISDIR(fileStat.st_mode)){
                size = 0;
            }
            //Send size to client so that it can allocate memory
            send(client_fd, &size, sizeof(int), 0);

            //If non zero size, then send the username and file to client.
            if(size){
                send(client_fd, filename, sizeof(filename), 0);
                sendfile(client_fd, filehandle, NULL, size);
            }

      }else if(strcmp(command, "quit") == 0){
            //If client quits, send goodbye before closing
            write(client_fd , "Goodbye" , sizeof("Goodbye"));
      }else{
            //If unknown command send error
        		write(client_fd, error, sizeof(error));
      }

		}while(read_size > 0);

    return 0;
}
