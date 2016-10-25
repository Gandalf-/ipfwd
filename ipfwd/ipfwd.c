#include <kvm.h>
#include <sys/param.h>
#include <sys/pcpu.h>
#include <sys/sysctl.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  struct kvm_t kd;
  struct pcpu *pcpu = calloc(1, sizeof(struct pcpu));

  if (! pcpu) {
    fprintf(stderr, "error: calloc\n");
    return -1;
  }

  pcpu = kvm_getpcpu(kd, 0);

  return 0;
}
