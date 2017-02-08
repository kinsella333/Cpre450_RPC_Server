#include <rpc/rpc.h>
#include <time.h>
#include <sys/types.h>
#include "date.h"

#define MAX 100


char **
date_1(option)
long *option;
{
  struct tm *timeptr;
  time_t clock;
  static char *ptr;
  static char err[] = "Invalid Response \0";
  static char s[MAX];

  clock = time(0);
  timeptr = localtime(&clock);

  switch(*option)
  {
	case 1:strftime(s,MAX,"%A, %B %d, %Y",timeptr);
	  ptr=s;
	  break;

	case 2:strftime(s,MAX,"%T",timeptr);
	  ptr=s;
	  break;
	
	case 3:strftime(s,MAX,"%A, %B %d, %Y - %T",timeptr);
	  ptr=s;
	  break;

	default: ptr=err;
	  break;
  }

  return(&ptr);
}
