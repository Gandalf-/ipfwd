#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  double loads[3];
  int ret = getloadavg(loads, 3);

  for (int i = 0; i < 3; i++) {
    printf("%d : %f\n", i, loads[i]);
  }

  return 0;
}
