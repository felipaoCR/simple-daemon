#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "daemon.h"
#include <signal.h>

/*Simple daemon script */

/*Signal handler in case of pausing or terminating the daemon*/
void signal_handler(int signal)
{
  openlog("Signal caught ...", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
  switch (signal)
    {
    case SIGTERM:
      syslog(LOG_INFO,"Daemon terminated");
    case SIGQUIT:
      syslog(LOG_INFO, "Daemon has quit. Performing core dump");
      exit(0);
    case SIGINT:
      syslog(LOG_INFO, "Ctrl-c ... daemon interrupted");
      exit(0);
    case SIGTSTP:
      syslog(LOG_INFO,"Ctrl-z ... daemon stopped temporarily");
    case SIGABRT:
      syslog(LOG_INFO, "Daemon aborted");
    case SIGKILL:
      syslog(LOG_INFO, "Daemon killed");
    case SIGSEGV:
      syslog(LOG_INFO, "Segmentation fault");
    case SIGCONT:
      syslog(LOG_INFO, "Continue if stopped ...");
    default:
      syslog(LOG_INFO, "Some signal received, daemon will now close ...");
      exit(0);
    }
}


void daemonize()
{
  pid_t p_id, s_id;
  
  openlog("my daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);

  /*pass parent process id to child process*/
  p_id = fork();   
  
  if(p_id < 0)
  { 
    exit(EXIT_FAILURE);
    syslog(LOG_INFO,"Couldn't fork parent process");
  }
  if(p_id > 0)
  { 
    exit(EXIT_SUCCESS);
    syslog(LOG_INFO,"Child process created succesfully");
  }
  if (p_id == 0)
  {
    syslog(LOG_INFO, "Process is already the child process ...");
  }
  /*Set umask to 0 in case the daemon needs its own mask's rwe priviligies*/
  umask(0);
  
  syslog(LOG_INFO, "RWE priviliges conceived");
  /*Set process group id*/
  s_id = setsid();

  if(s_id < 0)
    {
      syslog(LOG_NOTICE, "Couldn't set group process ID");
      exit(EXIT_FAILURE);
    }

  if(chdir("/") < 0)
    {
      syslog(LOG_NOTICE, "Couldn't change to root directory");
      exit(EXIT_FAILURE);
    }

  /*close standard file descriptors. This won´t let users interact with the daemon */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  
  /* Make some signal handling*/
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGSEGV, signal_handler);
  signal(SIGABRT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGTSTP, signal_handler);
  signal(SIGKILL, signal_handler);
  signal(SIGCONT, signal_handler);
  

  /*End of daemonizer*/
  
  
}
