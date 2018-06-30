#ifndef GUI_MISC_H
#define GUI_MISC_H

#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <error.h>

#define MAXLINE 1024
#define CTRL(x) ((x) & 0x1f)

#define H LINES
#define W COLS

// Return
#define WILL_EXIT -2

// For record contact panel width and chat panel height
int xcont, ychat, STEP, lastw, lasth;

typedef struct _PANEL_DATA {
    int x, y, w, h;
} PANEL_DATA;

PANEL *create_panel(PANEL_DATA *pd, int height, int width, int sy, int sx);
void rearange_panel(PANEL *panel, int new_h, int new_w, int sy, int sx);
void destroy_win(WINDOW *local_win);

#endif /* ifndef GUI_MISC_H */
