#include <stdio.h>

#include "core.h"
#include "oops.h"

int main() {
  printf("Starting engine...\n");
  Core* core = initCore();
  WARN("Oops test 1\n");
  OOPS("Oops test 2\n", 1);
  destroyCore(core);
  printf("Engine closed\n");
  return 0;
}

