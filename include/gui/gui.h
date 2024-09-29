#ifndef GUI_H
#define GUI_H

#include "core.h"
#include "gui/container.h"

struct GUI;

struct GUI *create_gui(struct Core *core, char *skin_filename);
void add_gui_window(struct GUI *gui, struct GUI_Container *parent,
                    struct GUI_Window *window);
void add_gui_button(struct GUI *gui, struct GUI_Container *parent,
                    struct GUI_Button *button);
void render_gui(struct GUI *gui);

#endif /* GUI_H */
