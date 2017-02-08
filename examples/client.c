#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <rpc/rpc.h>
#include "date.h"

#define MAX 100
long get_response(void);


long get_response()
{
  long choice;

  printf("Menu: \n");
  printf("1. Date\n");
  printf("2. Time\n");
  printf("3. Both\n");
  printf("4. Exit\n");
  printf("Make a choice (1-4):");
  scanf("%ld", &choice);

  return(choice);
}


main(int argc, char **argv)
{
  CLIENT *cl;
  char *server;
  char **sresult;
  char s[MAX];
  long response;
  long *lresult;

  if(argc != 2)
  {
	fprintf(stderr, "usage:%s hostname\n", argv[0]);
	exit(1);
  }

  server = argv[1];
  lresult = (&response);

  if((cl=clnt_create(server,DATE_PROG,DATE_VERS,"udp")) == NULL)
  {
	clnt_pcreateerror(server);
	exit(2);
  }
  
  response = get_response();
  
  while(response!=4)
  {
	if((sresult = date_1(lresult,cl)) == NULL)
	{
	  clnt_perror(cl,server);
	  exit(3);
	}

 	printf(" %s\n", *sresult);
	response = get_response();
  }

  clnt_destroy(cl);
  exit(0);
}
