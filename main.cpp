#include <syslog.h>
#include <unistd.h>
#include "daemon.h"

int main()
{
  daemonize();
  while(true)
    {
      syslog(LOG_NOTICE, "Application has successfully been daemonized");
      sleep(60);
    }
  closelog();
  return 0;
}
