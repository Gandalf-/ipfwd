#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

  int loads[100] = {0};
  int counter, load, i;
  double elapsed, duration;
  float x, y, z;
  time_t start, end;
  struct timespec tim, tim2;

  /* supported loads */
  loads[15] = 30000;
  loads[25] = 20000;
  loads[50] = 10000;
  loads[75] = 8500;

  if (argc != 3) {
    printf("usage: %s [cpu load %%] [duration]\n", argv[0]);
    return -1;
  }

  /* get and verify parameters */
  load = atoi(argv[1]);
  duration = (double)atoi(argv[2]);

  if(load < 0 || load > 100 || duration < 1 ) {
    printf("usage: %s [cpu load %%] [duration]\n", argv[0]);
    return -1;
  }

  if (loads[load] == 0) {
    printf("supported percentages: ");
    for (i = 0; i < 100; i++)
      if (loads[i] != 0)
        printf("%d, ", i);
    printf("\n");
    return -1;
  }

  /* pre loop setup */
  counter = 0;
  tim.tv_sec = 0;
  tim.tv_nsec = loads[load];
  time(&start);
  printf("starting %d (%d) percent load for %d seconds\n", 
      load, loads[load], (int)duration);

  while (elapsed < duration) {

    /* work */
    if (x < 1 || y < 1 || z < 1)
      x = y = z = 0xffffffff;

    for (int i = 0; i < 10; i++) {
      x = sqrt(x); x = sqrt(x); x = sqrt(x);
      y = sqrt(y); y = sqrt(y); y = sqrt(y);
      z = sqrt(z); z = sqrt(z); z = sqrt(z);
    }

    /* check times less frequently so we focus on the math */
    if (counter == 50) {
      time(&end);
      elapsed = difftime(end, start);
      counter = 0;
    }
    else {
      counter += 1;
    }

    if(nanosleep(&tim , &tim2) < 0 )
      return -1;
  }
  return 0;
}
