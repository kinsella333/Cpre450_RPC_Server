#include <rpc/rpc.h>
#include <time.h>
#include <sys/types.h>
#include "date.h"
#include <sys/sysinfo.h>
#include <stdio.h>
#include <unistd.h>


#define SIZE 100

char **
date_1(option)
long *option;
{
  struct tm *timeptr;
  time_t clock;
  static char *ptr;
  static char err[] = "Invalid Response \0";
  static char s[SIZE];
  static char t[SIZE];
  struct sysinfo info;
  int day, hour, minute, second;
  FILE *proc;
  char c;
  int i=0, cpu_found=0, tmp = 0, total=0, idle=0, x=0;


  clock = time(0);
  timeptr = localtime(&clock);

  switch(*option){
	case 1:
    strcpy(t,"\bSystem Time: ");
    strftime(s,SIZE,"%A, %B %d, %Y - %T\n",timeptr);
	  strcat(t, s);
    ptr=t;
	  break;

	case 2:
    if( (proc = fopen("/proc/stat","r")) == NULL){
      sprintf(s, "Failed to open CPU stat file\n");
      ptr=s;
      break;
    }


    while((c = fgetc(proc)) != EOF ){
      if( (c == 'c' && i == 0) || (c == 'p' && i == 1) || (c == 'u' && i == 2)){
        cpu_found++;
      }

      else if( c >= '0' && c <= '9' && cpu_found == 3){
        tmp *= 10;
        tmp += c-'0';
      }

      else if( c == ' ' && cpu_found == 3 ){
        total += tmp;
        x++;
        if( x == 6 )
          idle = tmp;
        tmp = 0;
      }

      else if( c == '\n' && cpu_found == 3 ){
        i = 0;
        cpu_found = 0;
        total += tmp;
        tmp = 0;
        break;
      }
      i++;
    }

    sprintf(s, "\bCurrent CPU Usage: %6.2f%%\n", (float)(100*((total) - (idle)))/(float)total );
    fclose(proc);
    ptr=s;
	  break;

	case 3:
    
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
