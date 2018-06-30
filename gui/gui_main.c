#include "gui_main.h"
#include <string.h>

extern int DIY_CL, DIY_CR, DIY_CU, DIY_CD;
extern int xcont, ychat, STEP, lastw, lasth;
PANEL *pcontact, *pchat, *ptyping;

#define min(a, b) \
    ((a) < (b) ? (a) : (b))
#define Swap(m, n) \
    do {(m) ^= (n); (n) ^= (m); (m) ^= (n);} while(0)
#define draw_vline(sy, ey, x) \
    do {int i; for(i = sy; i < ey+1; i++) mvaddch(i, x, ACS_VLINE);} while(0)
#define draw_hline(sx, ex, y) \
    do {int i; for(i = sx; i < ex+1; i++) mvaddch(y, i, ACS_HLINE);} while(0)

static struct key {
    const char *name;
    int code;
} Keys[] = {
    {"kLFT5", 0},
    {"kUP5",  0},
    {"kRIT5", 0},
    {"kDN5",  0},
    {"kEND5", 0},
    {"kHOM5", 0},
    {"kHOM3", 0},
    {"kUP5",  0},
    {"kDN5",  0},
    {0, 0}
};

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

void draw_panel(){
    // wclear(wcontact);
    // Build borders
    draw_rect(0, 0, H-1, W-1);
    draw_vline(1, H-2, xcont);
    draw_hline(xcont+1, W-2, ychat);
    mvaddch(0,     0,     ACS_ULCORNER);
    mvaddch(0,     xcont, ACS_TTEE);
    mvaddch(0,     W-1,   ACS_URCORNER);
    mvaddch(H-1,   0,     ACS_LLCORNER);
    mvaddch(H-1,   xcont, ACS_BTEE);
    mvaddch(H-1,   W-1,   ACS_LRCORNER);
    mvaddch(ychat, xcont, ACS_LTEE);
    mvaddch(ychat, W-1,   ACS_RTEE);
    // refresh windows
    rearange_panel(pcontact, H-2, xcont-1, 1, 1);
    rearange_panel(pchat,    ychat-1, W-2-xcont, 1, xcont+1);
    rearange_panel(ptyping,  H-2-ychat, W-2-xcont, ychat+1, xcont+1);
    //box(panel_window(pcontact), 0, 0);
    //box(panel_window(pchat), 0, 0);
    //box(panel_window(ptyping), 0, 0);
    refresh();
    update_panels();
    doupdate();
}

int process_event(int ch){
    if (ch == KEY_RESIZE){
        xcont *= W/(double)lastw;
        ychat *= H/(double)lasth;
        lasth = H;
        lastw = W;
        draw_panel();
    } else if (ch == DIY_CL) {
        if (xcont-STEP < MIN_CONT_P*W) return ERR;
        xcont -= STEP;
        rearange_panel(pcontact, xcont-1, H-2, 1, 1);
        draw_panel();
    } else if (ch == DIY_CR) {
        if (xcont+STEP > MAX_CONT_P*W) return ERR;
        xcont += STEP;
        draw_panel();
    } else if (ch == DIY_CU) {
        if (ychat-STEP < MIN_CHAT_P*H) return ERR;
        ychat -= STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == CTRL('w')) {
        return WILL_EXIT;
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == DIY_CD) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else {
        waddch(panel_window(ptyping), ch);
        wrefresh(panel_window(ptyping));
        ;
    }
    return OK;
}

void init_settings_by_default(){
    lastw = W;
    lasth = H;
    STEP = 1;
    xcont = W*DFT_XCONT_P;
    ychat = H*DFT_YCHAT_P;
    pcontact = create_panel(NULL, H-2, xcont-1, 1, 1);
    pchat    = create_panel(NULL, ychat-1, W-2-xcont, 1, xcont+1);
    ptyping  = create_panel(NULL, H-2-ychat, W-2-xcont, ychat+1, xcont+1);
}

void produce_ui(){
    int ch;
    draw_panel();
    while(WILL_EXIT != process_event(ch = getch()));
}


