#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define check(ret, string) if (! ret) {error = string; goto ERROR;}
const int hist_size = 5;

int main(int argc, char **argv) {

  int i;
  long num_cpus;
  double slope, previous;
  double loads[1];
  double history[hist_size];
  char *error = "unknown error";

  num_cpus = sysconf(_SC_NPROCESSORS_ONLN);

  while (1) {
    check(getloadavg(loads, 1), "could not get load average");

    for (i = hist_size; i > 0; i--)
      history[i] = history[i - 1];
    history[0] = loads[0] / num_cpus;

    slope = history[0] - history[1];

    if (slope != 0)
      printf("current: %f, slope: %f\n", 
          history[0], slope);
    sleep(1);
  }

ERROR:
  printf("error: %s\n", error);
  return 0;
}
