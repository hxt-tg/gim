#include <ncurses.h>
#define CTRL(x)   ((x) & 0x1f)

static struct key {
    char *name;
    int code;
} Keys[] = {
    { "kLFT5", 0  },
    { "kEND5", 0  },
    { "kHOM5", 0  },
    { "kHOM3", 0  },
    { "kUP5", 0  },
    { "kDN5", 0  },
    { 0, 0  }
};

int main(void){
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    int ch = 0, i;
    printw("[%d]\n", KEY_ENTER);
    for (i = 0; Keys[i].name != 0; ++i) {
        int code;
        char *s = tigetstr(Keys[i].name);
        if (s && (long)(s) != -1) {
            printw("tigetstr for %s=%s\n", Keys[i].name, s);
            code = key_defined(s);
            if (code > 0) {
                Keys[i].code = code;
            }
        }
    }
    while(1){
        printw("You've press [%d]\"%s\"\n", ch, keyname(ch));
        printw("Ctrl j ? %s.\nEnter? %s.", ch == CTRL('j')? "Yes": "No", ch == KEY_ENTER? "Yes": "No");
        ch = getch();
        clear();
    }
    endwin();
    return 0;
}


