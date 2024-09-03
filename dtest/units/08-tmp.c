//
// 99

#include "utils/debug.h"

int main() {
  DTEST_UNIT_START("Debug system");
  INFO("Debug message");
  INFOF("Formatted debug message: %s", "hello");
  INFO2("Debug message (but green)");
  INFO2F("Formatted debug message: %i", 12345);
  WARN("Warning message");
  WARNF("Formatted warning message: %s", "hello");
  ERRF(99, "Error: %s", "99");
  // Should not get here
  DTEST_UNIT_END;
}
