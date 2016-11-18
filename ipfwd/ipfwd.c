#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define check(ret, string) \
  if (! ret) { \
    fprintf(stderr, "error: %s\n", string); exit(1);}

const int sensitivity = 10;
const int hist_size = 5;
const char *path = "/bin/echo";

void set_probability(double probability) {
  /*
    run the ipfw commands to set the new first
    rule probability
  */
  int status;
  pid_t pid;

  /* delete old rule */
  if ((pid = fork()) == 0) {
    execl(path, "ipfw", "delete 1", NULL);
    exit(1); 
  }
  else {
    waitpid(pid, &status, 0);
    check(!status, "failed deleting old rule"); 
  }

  /* add new rule */
  if ((pid = fork()) == 0) {
    char *rule_body = "add 1 prob 0.000 allow ip any to any";
    char *set_rule_one = malloc(sizeof(rule_body));
    snprintf(
        set_rule_one, strlen(rule_body),
        "add 1 prob %0.3f allow ip any to any", probability);

    execl(path, "ifpw", set_rule_one, NULL);
    exit(1); 
  }
  else {
    waitpid(pid, &status, 0);
    check(!status, "failed setting new rule"); 
  }
}

int main(int argc, char **argv) {
  /* 
     monitors system load using uptime()
     normalizes results by number of cpus
     */

  int status, i, num_cpus;
  double slope, previous, prediction, error;
  double probability;
  double loads[1], history[hist_size];

  probability = 0.0;
  previous = 100;
  num_cpus = sysconf(_SC_NPROCESSORS_ONLN);

  while (1) {
    check(getloadavg(loads, 1), "could not get load average");

    for (i = hist_size; i > 0; i--)
      history[i] = history[i - 1];
    history[0] = loads[0] / num_cpus * 100;

    error = abs((history[0] - prediction));
    slope = history[0] - history[1];
    prediction = history[0] + slope;

    /* only take action when this load is different than the previous one */
    if (slope) {

      /* update probability if load has changed enough */
      if (abs(previous - history[0]) > sensitivity) {
        probability =  history[0] / 100;
        previous = history[0];
        set_probability(probability);
      }

      /* debug */
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
