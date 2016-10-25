#include <math.h>
#include <time.h>
#include <stdio.h>

const int ten_to_fifteen = 50000;

int main(int argc, char **argv){

  float x = 0, y = 0, z = 0;

  struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = 5000;

  while (1) {
    if (x < 1 || y < 1 || z < 1) {
      x = 0xffffffff;
      y = 0xffffffff;
      z = 0xffffffff;
    }

    x = sqrt(x);
    y = sqrt(y);
    z = sqrt(z);

    if(nanosleep(&tim , &tim2) < 0 )
      return -1;
  }
  return 0;
}
