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

/*Simple daemon script */

void daemonize()
{
  pid_t p_id, s_id;


  /*pass parent process id to child process*/
  p_id = fork();   
  
  if(p_id < 0) exit(EXIT_FAILURE);
  if(p_id > 0) exit(EXIT_SUCCESS);
  /*Make a call to umask in case you need rwe priviligies*/
  umask(0);

  openlog("my daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);

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

  /*End of daemonizer*/
  
  
}
