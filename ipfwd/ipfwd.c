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
  pid_t pid;
  int status, enabled, on = 1, off = 0;
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

    case RESTART:
      if ((pid = fork()) == 0) {
        dup2(open("/dev/null", O_WRONLY), 1);
        execl(service_path, "service", "ipfw", "restart", NULL);
        exit(1); 
      }
      else {
        waitpid(pid, &status, 0);
        check(!status, "failed to restart ipfw during ipfwd startup"); 
      }
      break;
  }
}

void set_probability(int rule_number, double probability) {
  /* run the ipfw commands to set the new first rule probability
  */
  int status;
  pid_t pid;
  char *rule_body     = "-q add       prob 0.000 allow ip from any to any ";
  char *rule_body_fmt = "-q add %d prob %0.3f allow ip from any to any";
  char *dele_body     = "-q delete      ";
  char *dele_body_fmt = "-q delete %d";
  char *set_rule, *del_rule;

  /* delete old rule */
  if ((pid = fork()) == 0) {
    del_rule = malloc(sizeof(dele_body));
    snprintf(
        del_rule, strlen(dele_body), dele_body_fmt, 
        rule_number);

    dup2(open("/dev/null", O_WRONLY), 1);
    dup2(open("/dev/null", O_WRONLY), 2);
    execl(ipfw_path, "ipfw", del_rule, NULL);
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

      set_rule = malloc(sizeof(rule_body));
      snprintf(
          set_rule, strlen(rule_body), rule_body_fmt, 
          rule_number, probability);

      dup2(open("/dev/null", O_WRONLY), 1);
      execl(ipfw_path, "ifpw", set_rule, NULL);
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
   *   static_prob : double, always use this probability for accept all rule,
   *                 ignore current system load. 0 denotes no static_prob,
   *                 defaults to 0
   */

  int status, i, num_cpus, rule_number;
  double slope, previous, prediction, error, probability, static_prob;
  double loads[1], history[hist_size];

  rule_number = 1;
  static_prob = -1.0;
  probability = 0.0;
  previous    = 100;
  num_cpus    = sysconf(_SC_NPROCESSORS_ONLN);

  if (argc > 1) {
    rule_number = atoi(argv[1]);
    assert(rule_number > 0 && rule_number < 65535);
  }
  if (argc > 2) {
    static_prob = atof(argv[2]);
    assert(static_prob >= 0.0 && static_prob <= 1.0);
  }

  printf("[ipfwd] starting\n");
  control_ipfw(ENABLE);

  while (1) {
    check(getloadavg(loads, 1), "could not get load average");

    history[0] = loads[0] * 100.0 / (double)num_cpus;
    error      = fabs((history[0] - prediction));
    slope      = history[0] - history[1];
    prediction = history[0] + slope;

    for (i = hist_size; i > 0; i--) {
      history[i] = history[i - 1];
    }

    /* only take action when this load is different than the previous one */
    if (slope) {

      /* update probability if load has changed enough */
      if (fabs(previous - history[0]) > sensitivity) {

        if (static_prob != -1.0) {
          probability = static_prob;
        }
        else {
          probability = sigmoid(history[0] / 100);
        }

        previous = history[0];
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
