#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define check(ret, string) if (! ret) {errormsg = string; goto ERROR;}

const int hist_size = 5;

int main(int argc, char **argv) {
  /* 
    monitors system load using uptime()
    normalizes results by number of cpus
  */

  int i;
  long num_cpus;
  double slope, previous, prediction, error;
  double loads[1], history[hist_size];
  char *errormsg = "unknown error";

  num_cpus = sysconf(_SC_NPROCESSORS_ONLN);

  while (1) {
    check(getloadavg(loads, 1), "could not get load average");

    for (i = hist_size; i > 0; i--)
      history[i] = history[i - 1];
    history[0] = loads[0] / num_cpus;

    error = abs((history[0] - prediction) * 100);
    slope = history[0] - history[1];
    prediction = history[0] + slope;

    if (slope != 0)
      printf(
          "current: % 02.2f%%, prediction error: % 02.1f%%\n", 
          history[0] * 100, error);
    sleep(1);
  }

ERROR:
  printf("error: %s\n", errormsg);
  return 0;
}
