/* ipfwd.c 
 * author: Austin Voecks
 *
 * Monitors system load using uptime() to set probability of IPFW's first rule
 * allowing any packet. This rule increases performance by sacrificing complete
 * security. The probability is dynamic based on the current load.
 */

#include "ipfwd.h"

double sigmoid(double x) {
  /* push low values lower and high values higher
   */
  if (x < 0.1) return 0;
  if (x > 0.9) return 1;
  return 1 / (1 + exp((x - 0.5) * -8));
}

void control_ipfw(enum command cmd) {
  /* enables or disables ipfw
   */
  int enabled, on = 1, off = 0;
  size_t size = sizeof(enabled);

  sysctlbyname(sysctl_ipfw_v4, &enabled, &size, NULL, 0);

  switch(cmd) {
    case ENABLE:
      if (! enabled) {
        sysctlbyname(sysctl_ipfw_v4, NULL, NULL, &on, size);
        sysctlbyname(sysctl_ipfw_v6, NULL, NULL, &on, size);
      }
      /* do nothing if already on */
      break;

    case DISABLE:
      if (enabled) {
        sysctlbyname(sysctl_ipfw_v4, NULL, NULL, &off, size);
        sysctlbyname(sysctl_ipfw_v6, NULL, NULL, &off, size);
      }
      /* do nothing if already off */
      break;
  }
}

void set_probability(int rule_number, double probability) {
  /* run the ipfw commands to set the new first rule probability
   */
  int status;
  pid_t pid;
  char *rule_body = "-q add 00000 prob 0.000 allow ip from any to any ";
  char *set_rule_one;

  /* delete old rule */
  if ((pid = fork()) == 0) {
    execl(ipfw_path, "ipfw", "-q delete 1", NULL);
    exit(1); 
  }
  else {
    waitpid(pid, &status, 0);
    /* don't care if this fails */
  }

  if ((pid = fork()) == 0) {
    /* prob in [0.1, 0.9] -> add new rule */
    if (probability > 0) {
      control_ipfw(ENABLE);

      set_rule_one = malloc(sizeof(rule_body));
      snprintf(
          set_rule_one, strlen(rule_body),
          "-q add %d prob %0.3f allow ip from any to any", 
          rule_number, probability);

      execl(ipfw_path, "ifpw", set_rule_one, NULL);
      exit(1); 
    }
    /* prob > 0.9 -> disable ipfw until load decreases */
    else if (probability == 1) {
      control_ipfw(DISABLE);
      exit(0); 
    }
    /* prob < 0.1 -> leave the rule deleted so we check every packet */
    else {
      control_ipfw(ENABLE);
      exit(0);
    }
  }
  else {
    waitpid(pid, &status, 0);
    check(!status, "failed to update ipfw"); 
  }
}

int main(int argc, char **argv) {
  /* monitors system load using uptime() to set probability of IPFW's first
   * rule allowing any packet. This rule increases performance by sacrificing
   * complete security. The probability is dynamic based on the current load.
   *
   * ipfwd [rule_number] [static_prob]
   *   rule_number : int, rule number to use for accept all rule. rules before
   *                 this will always be run. defaults to 1
   *
   *   static_prob : int, always use this probability for accept all rule,
   *                 ignore current system load. 0 denotes no static_prob,
   *                 defaults to 0
   */

  int status, i, num_cpus, rule_number, static_prob;
  double slope, previous, prediction, error, probability;
  double loads[1], history[hist_size];

  static_prob = 0;
  rule_number = 1;
  probability = 0.0;
  previous    = 100;
  num_cpus    = sysconf(_SC_NPROCESSORS_ONLN);

  if (argc > 1)
    rule_number = atoi(argv[1]);
  if (argc > 2)
    static_prob = atoi(argv[2]);

  control_ipfw(ENABLE);

  while (1) {
    check(getloadavg(loads, 1), "could not get load average");

    for (i = hist_size; i > 0; i--)
      history[i] = history[i - 1];
    history[0] = loads[0] / num_cpus * 100;

    error      = fabs((history[0] - prediction));
    slope      = history[0] - history[1];
    prediction = history[0] + slope;

    /* only take action when this load is different than the previous one */
    if (slope) {

      /* update probability if load has changed enough */
      if (fabs(previous - history[0]) > sensitivity) {
        probability = sigmoid(history[0] / 100);
        previous    = history[0];

        if (static_prob)
          set_probability(rule_number, static_prob);
        else
          set_probability(rule_number, probability);
      }

      printf(
          "current: % 02.3f%%, error: % 02.1f%%, probability: % 02.3f%%\n",
          history[0], error, probability);
    }
    sleep(1);
  }
  return 0;
}
