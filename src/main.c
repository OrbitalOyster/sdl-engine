#include <stdarg.h>

#include "utils/debug.h"

int main() {
  INFO("Info");
  INFOF("Info: %s", "foo");
  INFO2("Info");
  INFO2F("Info: %s", "foo");
  WARN("Warning");
  WARNF("Warning: %s", "bar");
  ERRF(1, "Error: %s", "baz");
}
