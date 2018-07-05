#ifndef GUI_MISC_H
#define GUI_MISC_H

#include <ncurses.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

#define MAXLINE 1024
#define CTRL(x) ((x) & 0x1f)

#define H LINES
#define W COLS

// Return
#define WILL_EXIT -2

// For record contact panel width and chat panel height
int xcont, ychat, STEP, lastw, lasth, n_contacts;
typedef struct {
    char username[40];
    char phone[15];
} User;

User self;
User *contacts;

typedef struct _PANEL_DATA {
    int x, y, w, h;
} PANEL_DATA;

PANEL *create_panel(PANEL_DATA *pd, int height, int width, int sy, int sx);
WINDOW *rearange_panel(PANEL *panel, int new_h, int new_w, int sy, int sx);
void destroy_win(WINDOW *local_win);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string);
void read_info();
void msg(const char *user_to, const char *m);
int hist(const char *user_from);
void upload(const char *user_to, const char *path);
void download(const char *filepath, const char *user_from);


#endif /* ifndef GUI_MISC_H */
