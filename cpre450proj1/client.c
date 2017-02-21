/*
*   Client program developed from example code provided by Professor Guan.
*   Program must be run after corresponding server code has been run to funtion
*   correctly. User may choose an action by entering the corresponding numerical
*   value from the menu provided. The server will provide the requested
*   information via a RPC connection.
*
*   Author: Ray Kinsella
*   Skeleton Code Provided By: Dr. Guan
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <rpc/rpc.h>
#include "date.h"

#define SIZE 100
long get_response(void);

/*
*   Gathers user input and returns number as long value.
*/
long get_response()
{
  long choice;

  printf("Menu: \n");
  printf("1. System Time\n");
  printf("2. CPU Usage\n");
  printf("3. Memory Usage\n");
  printf("4. Average Load Procs Per Min\n");
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

  //Verify hostname provided
  if(argc != 2){
	   fprintf(stderr, "usage:%s hostname\n", argv[0]);
	    exit(1);
  }

  //Assign server to provided hostname, and point lresult at response value.
  server = argv[1];
  lresult = (&response);

  //Initialize connection with server at provided hostname.
  if((cl=clnt_create(server,DATE_PROG,DATE_VERS,"udp")) == NULL){
	   clnt_pcreateerror(server);
	   exit(2);
  }

  //Get user respnse.
  response = get_response();

  //While exit command has not been sent
  while(response!=6){

    //If user chooses the exit and kill server option(7)
    if(response == 7){
      system("pkill server");
      break;
    }

    //Verify reception of message from server.
	  if((sresult = date_1(lresult,cl)) == NULL){
	     clnt_perror(cl,server);
	     exit(3);
    }

    //Print result and scan for next action.
 	  printf(" %s\n", *sresult);
	  response = get_response();
  }

  //Close connection on exit.
  clnt_destroy(cl);
  exit(0);
}
