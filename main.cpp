#include <syslog.h>
#include <unistd.h>
#include "daemon.h"

int main()
{
  daemonize();
  while(true)
    {
      syslog(LOG_NOTICE, "Daemon is now running ...");
      sleep(60);
    }
  closelog();
  return 0;
}
