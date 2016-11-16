#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define check(ret, string) if (! ret) { fprintf(stderr, "error: %s\n", string); return -1;}

const int hist_size = 5;
const char *path = "/bin/echo";

static char *errormsg = "unknown error";
static char *get_rule_one[] = { "ipfw", "show 1", NULL};
static char *del_rule_one[] = { "ipfw", "delete 1", NULL};
static char *set_rule_one[] = {
  "ipfw", "add 1 prob 1.000 allow ip any to any", NULL};

double set_probability(double probability) {
  /*
    actually set the new first rule probability
  */
  pid_t pid;

  if ((pid = fork()) == 0) {
    execv(path, del_rule_one);
    exit(1); 
  }
  else {
    waitpid(pid, &status, 0);
    check(!status, "failed deleting old rule"); 
  }
}

double update_ipfw(double current, double probability) {
  /* 
    update the first rule depending on the current load
  */
  int status;
  double baseline = 40.0;

  /* do nothing if we're below 40% utilization */
  if (current - baseline < 0) 
    return probability;

  return 0;
}

int main(int argc, char **argv) {
  /* 
     monitors system load using uptime()
     normalizes results by number of cpus
     */

  int status, i;
  long num_cpus;
  double slope, previous, prediction, error, probability;
  double loads[1], history[hist_size];

  probability = 0.0;
  num_cpus = sysconf(_SC_NPROCESSORS_ONLN);

  while (1) {
    check(getloadavg(loads, 1), "could not get load average");

    for (i = hist_size; i > 0; i--)
      history[i] = history[i - 1];
    history[0] = loads[0] / num_cpus * 100;

    error = abs((history[0] - prediction) * 100);
    slope = history[0] - history[1];
    prediction = history[0] + slope;

    if (slope != 0) {
      probability = update_ipfw(history[0], probability);
      printf(
          "current: % 02.2f%%, " 
          "prediction error: % 02.1f%%, "
          "probability: % 02.1f%%\n",
          history[0],
          error, 
          probability);
    }
    sleep(1);
  }
  return 0;
}
