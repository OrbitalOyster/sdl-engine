#ifndef GUI_H
#define GUI_H

#include "core.h"

enum GUI_Unit_Type {GUT_AUTO = 0, GUT_ABSOLUTE = 10, GUT_RELATIVE = 100};

struct GUI_Unit {
  enum GUI_Unit_Type type;
  union {
    int px;
    double pct;
  };
};

struct GUI_Distance {
  enum GUI_Unit_Type type;
  union {
    int px;
    double pct;
  };
  enum GUI_Unit_Type ref_type;
  union {
    int ref_px;
    double ref_pct;
  };
};


struct GUI_Container {
  struct GUI_Distance top_distance;
  struct GUI_Unit top_anchor;

  struct GUI_Distance right_distance;
  struct GUI_Unit right_anchor;

  struct GUI_Distance bottom_distance;
  struct GUI_Unit bottom_anchor;

  struct GUI_Distance left_distance;
  struct GUI_Unit left_anchor;

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
