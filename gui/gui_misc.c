#include "gui_misc.h"

PANEL *create_panel(PANEL_DATA *pd, int height, int width, int sy, int sx){
    WINDOW *win = newwin(height, width, sy, sx);
    PANEL *panel = new_panel(win);
    set_panel_userptr(panel, pd);
    return panel;
}

void rearange_panel(PANEL *panel, int new_h, int new_w, int sy, int sx){
    WINDOW *old = panel_window(panel);
    WINDOW *temp = newwin(new_h, new_w, sy, sx);
    replace_panel(panel, temp);
    // TODO: Update view.
    delwin(old);
}

void destroy_PANEL(PANEL *panel){
    delwin(panel_window(panel));
    del_panel(panel);
}
