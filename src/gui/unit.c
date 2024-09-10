#include "gui/unit.h"

#include <math.h>

#include "utils/debug.h"

int unit_to_px(struct GUI_Unit unit, int reference) {
  switch (unit.type) {
  case GUT_NONE:
    return 0;
  case GUT_ABSOLUTE:
    return unit.px;
  case GUT_RELATIVE:
    return (int)round(unit.f * reference);
  default:
    ERRF(1, "Invalid GUI unit type: %u", unit.type);
    return 0;
  }
}
