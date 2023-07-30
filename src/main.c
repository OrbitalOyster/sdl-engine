#include <stdarg.h>

#include "utils/debug.h"

int main() {
  INFOF("Info: %s", "foo");
  WARNF("Warning: %s", "bar");
  ERRF(1, "Error: %s", "baz");
}
