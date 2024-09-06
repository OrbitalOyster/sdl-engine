#ifndef GUI_H
#define GUI_H

#include "core.h"

enum GUI_Unit_Type {GUT_AUTO = 0, GUT_ABSOLUTE = 10, GUT_RELATIVE = 100};

struct GUI_Unit {
  enum GUI_Unit_Type type;
  union {
    int px;
    double f;
  };
};

struct GUI_Container {
  struct GUI_Unit top;
  struct GUI_Unit top_p;
  struct GUI_Unit right;
  struct GUI_Unit right_p;
  struct GUI_Unit bottom;
  struct GUI_Unit bottom_p;
  struct GUI_Unit left;
  struct GUI_Unit left_p;
  struct GUI_Unit width;
  struct GUI_Unit height;
};

struct GUI {
  struct Core *core;
  unsigned int number_of_containers;
  struct GUI_Container **containers;
};

struct GUI *create_gui(struct Core *core);
void add_gui_container(struct GUI *gui, struct GUI_Container *container);
void render_gui(struct GUI *gui);

#endif /* GUI_H */
