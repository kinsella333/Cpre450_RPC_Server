#include <rpc/rpc.h>
#include <time.h>
#include <sys/types.h>
#include "date.h"
#include <sys/sysinfo.h>

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
  static char t[MAX];
  struct sysinfo info;
  int day, hour, minute, second;

  clock = time(0);
  timeptr = localtime(&clock);

  switch(*option){
	case 1:
    strcpy(t,"\bSystem Time: ");
    strftime(s,MAX,"%A, %B %d, %Y - %T\n",timeptr);
	  strcat(t, s);
    ptr=t;
	  break;

	case 2:strftime(s,MAX,"%T",timeptr);
	  ptr=s;
	  break;

	case 3:strftime(s,MAX,"%A, %B %d, %Y - %T",timeptr);
	  ptr=s;
	  break;

  case 5:
  sysinfo(&info);

  if(info.uptime > 86399){
      day = info.uptime/86400;
      hour = (info.uptime - day*86400)/3600;
      minute = (info.uptime - day*3600)/60;
      second = (info.uptime - day*3600 - hour*3600 - minute*60);
      sprintf(t, "\bUptime: %d Days %d Hours %d Minutes %d Seconds\n", day, hour, minute ,second);
  }else if(info.uptime > 3599){
      hour = info.uptime/3600;
      minute = (info.uptime - hour*3600)/60;
      second = (info.uptime - hour*3600 - minute*60);
      sprintf(t, "\bUptime: %d Hours %d Minutes %d Seconds\n", hour, minute, second);
  }else if(info.uptime > 59){
      minute = info.uptime/60;
      second = (info.uptime - minute*60);
      sprintf(t, "\bUptime: %d Minutes %d Seconds\n", minute, second);
  }else{
      second = info.uptime;
      sprintf(t, "\bUptime: %d Seconds\n", second);
  }
  ptr=t;
  break;

	default: ptr=err;
	  break;
  }

  return(&ptr);
}
