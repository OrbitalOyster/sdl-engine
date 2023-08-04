#include <math.h>

#include "utils/dmath.h"
#include "utils/debug.h"

int main() {
  double inf = 1 / .0;
  double nan = .0 / .0;
  INFO2F("0.1 + 0.2 == %0.25lf", .1 + .2);
  INFO2F("compare(0.1 + 0.2, 0.3) == %i", compare(.1 + .2, .3));
  INFO2F("compare(%0.25lf, %0.25lf) == %i", 1000 - DMATH_EPSILON / 2, 1000.0, compare(1000.0 - DMATH_EPSILON / 2, 1000.0));
  INFO2F("NAN + 0.1 == %0.25lf", nan + .1);
  INFO2F("isfinite(NAN) == %i", isfinite(nan));
  INFO2F("isinf(NAN) == %i", isinf(nan));
  INFO2F("isinf(INF) == %i", isinf(inf));
  INFO2F("compare(0.1, NAN) == %i", compare(.1, nan));
  INFO2F("compare(INF, NAN) == %i", compare(inf, nan));
  INFO2F("compare(NAN, NAN) == %i", compare(nan, nan));
  INFO2F("compare(INF, INF) == %i", compare(inf, inf));
  INFO2F("compare(-INF, -INF) == %i", compare(-inf, -inf));
  INFO2F("compare(INF, -INF) == %i", compare(inf, -inf));
  INFO2F("compare(1000000000, 0.000001) == %i", compare(1000000000, 0.000001));

  INFO("Info");
  INFOF("Info: %s", "foo");
  INFO2("Info");
  INFO2F("Info: %s", "foo");
  WARN("Warning");
  WARNF("Warning: %s", "bar");
  ERRF(1, "Error: %s", "baz");
}
