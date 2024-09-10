#ifndef GUI_UNIT_H
#define GUI_UNIT_H

enum GUI_Unit_Type { GUT_NONE = 0, GUT_ABSOLUTE = 10, GUT_RELATIVE = 100 };

struct GUI_Unit {
  enum GUI_Unit_Type type;
  union {
    int px;
    double f;
  };
};

int unit_to_px(struct GUI_Unit unit, int reference);

#endif
