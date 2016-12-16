/* ipfwd.h
 * author: Austin Voecks
 *
 * Monitors system load using uptime() to set probability of IPFW's first rule
 * allowing any packet. This rule increases performance by sacrificing complete
 * security. The probability is dynamic based on the current load.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysctl.h>

#define check(ret, string) \
  if (! ret) { \
    fprintf(stderr, "error: %s\n", string); exit(1);}

enum command {ENABLE, DISABLE};

const int sensitivity = 10;
const int hist_size = 5;
const char *sysctl_ipfw_v4 = "net.inet.ip.fw.enable";
const char *sysctl_ipfw_v6 = "net.inet6.ip6.fw.enable";

#if DEBUG
const char *ipfw_path = "/bin/echo";
const char *service_path = "/bin/echo";
#else
const char *ipfw_path = "/sbin/ipfw";
const char *service_path = "/usr/sbin/service";
#endif
