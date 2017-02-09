#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <rpc/rpc.h>
#include "date.h"

#define SIZE 100
long get_response(void);


long get_response()
{
  long choice;

  printf("Menu: \n");
  printf("1. System Time\n");
  printf("2. CPU Usage\n");
  printf("3. Memory Usage\n");
  printf("4. Load Procs per min\n");
  printf("5. Uptime\n");
  printf("6. Exit\n");
  printf("7. Kill Server and Exit\n");
  printf("Enter Choice (1-7):");
  scanf("%ld", &choice);

  system("clear");

  return(choice);
}


int main(int argc, char **argv){
  CLIENT *cl;
  char *server;
  char **sresult;
  char s[SIZE];
  long response;
  long *lresult;

  if(argc != 2){
	   fprintf(stderr, "usage:%s hostname\n", argv[0]);
	    exit(1);
  }

  server = argv[1];
  lresult = (&response);

  if((cl=clnt_create(server,DATE_PROG,DATE_VERS,"udp")) == NULL){
	   clnt_pcreateerror(server);
	   exit(2);
  }

  response = get_response();

  while(response!=6){

    if(response == 7){
      system("pkill server");
      break;
    }

	   if((sresult = date_1(lresult,cl)) == NULL){
	      clnt_perror(cl,server);
	      exit(3);
	   }

 	   printf(" %s\n", *sresult);
	   response = get_response();
  }

  clnt_destroy(cl);
  exit(0);
}
