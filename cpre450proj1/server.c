/*
*   Server program developed from example code provided by Professor Guan.
*   Program must be run before corresponding client code has been run to funtion
*   correctly. Server listens for client input over RPC connection, and provides
*   the requested information, given the menu choice from client.
*
*   Author: Ray Kinsella
*   Skeleton Code Provided By: Dr. Guan
*/

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
  static char s[SIZE], t[SIZE], err[] = "\bInvalid Response\n \0";;
  struct sysinfo info;
  int day, hour, minute, second, i=0, cpu_found=0, tmp = 0, total=0, idle=0, x=0;
  unsigned long kb, mb, ramUsage = 0;
  FILE *proc;
  char c;

  //Gather current System Time
  clock = time(0);
  timeptr = localtime(&clock);

  //Switch based on user input in recieved from the client.
  switch(*option){
  //Case 1: Return System Time of the Server Machine.
	case 1:
    strcpy(t,"\bSystem Time: ");
    strftime(s,SIZE,"%A, %B %d, %Y - %T\n",timeptr);
	  strcat(t, s);
    ptr=t;
	  break;

  //Case 2: Return current CPU Usage as a percentage.
  //Code from https://github.com/hc0d3r/C/blob/master/linux-cpu-usage.c
	case 2:
    //Verify that program can access proc/stat file
    if( (proc = fopen("/proc/stat","r")) == NULL){
      sprintf(s, "Failed to open CPU stat file\n");
      ptr=s;
      break;
    }

    //Loop through stat file, recording the total and idle CPU times
    while((c = fgetc(proc)) != EOF ){
      if( (c == 'c' && i == 0) || (c == 'p' && i == 1) || (c == 'u' && i == 2)){
        cpu_found++;
      }else if( c >= '0' && c <= '9' && cpu_found == 3){
        tmp *= 10;
        tmp += c-'0';
      }else if( c == ' ' && cpu_found == 3 ){
        total += tmp;
        x++;
        if( x == 6 )
          idle = tmp;
        tmp = 0;
      }else if( c == '\n' && cpu_found == 3 ){
        i = 0;
        cpu_found = 0;
        total += tmp;
        tmp = 0;
        break;
      }
      i++;
    }

    sprintf(s, "\bCurrent CPU Usage:%6.2f%%\n", (float)(100*((total) - (idle)))/(float)total );
    fclose(proc);
    ptr=s;
	  break;

  //Case: 3 Gather the total and free ram values from the sysinfo object
	case 3:
    sysinfo(&info);
    ramUsage = info.totalram - info.freeram;
    if(ramUsage > 1048575){
        mb = ramUsage/1048576;
        kb = (ramUsage - mb*1048576)/1024.0;
        sprintf(t, "\bMemory Usage: %luMBs %luKBs\nTotal Memory: %5.1lfMBs\nPercentage In Use:%5.1lf%%\n", mb, kb, info.totalram/1048576.0, ((ramUsage*1.0)/(info.totalram*1.0))*100.0);
    }else{
        kb = ramUsage/1024.0;
        sprintf(t, "\bMemory Usage: %luKBs\nTotal Memory: %5.1lfMBs\n", kb, info.totalram/1048576.0);
    }
	  ptr=t;
	  break;

  //Case 4: Gather the Load averages at 1,5, and 15 minutes from the sysinfo object
  case 4:
    sysinfo(&info);
    sprintf(s, "\bLoad Average at 1 Minute: %lu\nLoad Average at 5 Minutes: %lu\nLoad Average at 15 Minutes: %lu\n", info.loads[0],info.loads[1],info.loads[2]);
    ptr=s;
    break;

  //Case 5: Gather the Uptime and parse to format from the sysinfo object.
  case 5:
    sysinfo(&info);

    if(info.uptime > 86399){
        day = info.uptime/86400;
        hour = (info.uptime - day*86400)/3600;
        minute = (info.uptime - day*86400 - hour*3600)/60;
        second = (info.uptime - day*86400 - hour*3600 - minute*60);
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

  //If server does not know command, return error.
	default: ptr=err;
	  break;
  }

  return(&ptr);
}
