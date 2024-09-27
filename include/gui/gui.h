#ifndef GUI_H
#define GUI_H

#include "core.h"
#include "gui/container.h"

struct GUI;

struct GUI *create_gui(struct Core *core, char *skin_filename);
void add_gui_container(struct GUI *gui, struct GUI_Container *container);
void render_gui(struct GUI *gui);

#endif /* GUI_H */
