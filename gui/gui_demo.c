#include <ncurses.h>
#include <string.h>

#define MAXLINE 1024
#define min(a, b) ((a) < (b) ? (a) : (b))
#define Swap(m, n) do{ \
    (m) ^= (n); \
    (n) ^= (m); \
    (m) ^= (n); \
} while(0)
#define draw_vline(sy, ey, x) \
    do {int i; for(i = sy; i < ey+1; i++) mvaddch(i, x, ACS_VLINE);} while(0)
#define draw_hline(sx, ex, y) \
    do {int i; for(i = sx; i < ex+1; i++) mvaddch(y, i, ACS_HLINE);} while(0)
#define H LINES
#define W COLS
#define CTRL(x) ((x) & 0x1f)

int DIY_CL = 0;
int DIY_CR = 0;
int DIY_CU = 0;
int DIY_CD = 0;


static struct key {
    const char *name;
    int code;
} Keys[] = {
    { "kLFT5", 0  },
    { "kUP5", 0  },
    { "kRIT5", 0  },
    { "kDN5", 0  },
    { "kEND5", 0  },
    { "kHOM5", 0  },
    { "kHOM3", 0  },
    { "kUP5", 0  },
    { "kDN5", 0  },
    { 0, 0  }
};

void draw_rect(int sy, int sx, int ey, int ex){
    if (sy > ey) Swap(sy, ey);
    if (sx > ex) Swap(sx, ex);
    mvaddch(sy, sx, ACS_ULCORNER);
    draw_vline(sy+1, ey-1, sx);
    mvaddch(ey, sx, ACS_LLCORNER);
    draw_hline(sx+1, ex-1, sy);
    mvaddch(ey, ex, ACS_LRCORNER);
    draw_vline(sy+1, ey-1, ex);
    mvaddch(sy, ex, ACS_URCORNER);
    draw_hline(sx+1, ex-1, ey);
}

void draw_panel(double p_contant, double p_typing){
    clear();
    int xc = COLS * p_contant, yt = (1-p_typing) * LINES;
    draw_rect(0, 0, H-1, W-1);
    draw_vline(1, H-2, xc);
    draw_hline(xc+1, W-2, yt);
    mvaddch(0, 0,     ACS_ULCORNER);
    mvaddch(0, xc,    ACS_TTEE);
    mvaddch(0, W-1,   ACS_URCORNER);
    mvaddch(H-1, 0,   ACS_LLCORNER);
    mvaddch(H-1, xc,  ACS_BTEE);
    mvaddch(H-1, W-1, ACS_LRCORNER);
    mvaddch(yt, xc,   ACS_LTEE);
    mvaddch(yt, W-1,  ACS_RTEE);
}

void prod_ui(){
    // Draw border
    // draw_rect(0, 0, LINES-1, COLS-1);
    double pc = 0.2, pt = 0.3;  // pc: contact%    pt: typing%
    int ch;
    draw_panel(pc, pt);
    while(1){
        ch = getch();
        if (ch == KEY_RESIZE){
            draw_panel(pc, pt);
        } else if (ch == DIY_CL) {
            pc -= 0.03;
            if (pc < 0.1) pc = 0.1;
            draw_panel(pc, pt);
        } else if (ch == DIY_CR) {
            pc += 0.03;
            if (pc > 0.5) pc = 0.5;
            draw_panel(pc, pt);
        } else if (ch == DIY_CU) {
            pt += 0.02;
            if (pt > 0.6) pt = 0.6;
            draw_panel(pc, pt);
        } else if (ch == DIY_CD) {
            pt -= 0.02;
            if (pt < 0.1) pt = 0.1;
            draw_panel(pc, pt);
        } else {
            ;
        }
    }
    return ;
    while(1){
        printw("Get: %d\n", getch());
        refresh();
    }
}

void build_keys(){
    use_extended_names(TRUE);
    nonl();
    keypad(stdscr,TRUE);
    int i;
    for (i = 0; Keys[i].name != 0; ++i) {
        char *s = tigetstr(Keys[i].name);
        if (s && (long)(s) != -1) {
            int code = key_defined(s);
            if (code > 0)
                Keys[i].code = code;
        }
    }
    for (i=0;Keys[i].name;++i)
        printf("key=%s, code=%d\n", Keys[i].name, Keys[i].code);

    DIY_CL = Keys[0].code;
    DIY_CU = Keys[1].code;
    DIY_CR = Keys[2].code;
    DIY_CD = Keys[3].code;
}

int main(void){
    initscr();
    noecho();
    cbreak();
    build_keys();

    prod_ui();
    draw_rect(0, 0, 5, 10);
    mvaddch(25, 25, ACS_LARROW);
    mvaddch(26, 25, ACS_S1);
    mvaddch(27, 25, ACS_S3);
    mvaddch(28, 25, ACS_S7);
    mvaddch(29, 25, ACS_S9);
    mvaddch(30, 25, ACS_LARROW);
    mvaddch(31, 25, ACS_LARROW);
    mvaddch(32, 25, ACS_LARROW);
    getch();
    endwin();
    return 0;
}


