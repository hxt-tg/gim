#include "gui_main.h"
#include <string.h>

extern int DIY_CL, DIY_CR, DIY_CU, DIY_CD, DIY_ST;
extern int xcont, ychat, STEP, lastw, lasth, focus_panel, n_contacts;
extern User self;
extern User *contacts;

PANEL *pcontact, *pchat, *ptyping;
WINDOW *whelp;
int cur_user = 0;

#define min(a, b) \
    ((a) < (b) ? (a) : (b))
#define max(a, b) \
    ((a) > (b) ? (a) : (b))
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
    {"kLFT5",    0},
    {"kUP5",     0},
    {"kRIT5",    0},
    {"kDN5",     0},
    {"kEY_BTAB", 0},
    {"kHOM5",    0},
    {"kHOM3",    0},
    {"kUP5",     0},
    {"kDN5",     0},
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
    DIY_ST = Keys[4].code;
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

void inflate_contact(){
    char sn[128] = {0}, sp[128] = {0}, *s = sn;
    sprintf(sn, "User: %s", self.username);
    sprintf(sp, "Phone: %s", self.phone);
    print_in_middle(panel_window(pcontact), 1, 0, xcont, (char *)"GIM");
    print_in_middle(panel_window(pcontact), 3, 0, xcont, sn);
    print_in_middle(panel_window(pcontact), 4, 0, xcont, sp);
    wmove(panel_window(pcontact), 5, 0);
    int i;
    for (i = 0; i < xcont-1; i++)
        waddch(panel_window(pcontact), '_');

    for (i = 0; i < n_contacts; i++) {
        sprintf(sn, "%s", contacts[i].username);
        sprintf(sp, " (%s)", contacts[i].phone);
        for (s = sn + strlen(sn); s-sn < xcont-3; s++) *s = ' ';
        for (s = sp + strlen(sp); s-sp < xcont-3; s++) *s = ' ';

        if (i == cur_user) wattron(panel_window(pcontact), A_REVERSE);
        mvwprintw(panel_window(pcontact), 7+3*i, 1, "%s", sn);
        mvwprintw(panel_window(pcontact), 8+3*i, 1, "%s", sp);
        if (i == cur_user) wattroff(panel_window(pcontact), A_REVERSE);
    }
}

void load_msg(const char *p){
    char filename[256];
    int ch;
    sprintf(filename, "/home/ubuntu/.gim/%s", p);
    FILE *fp = fopen(filename, "r");
    while ((ch = fgetc(fp)) != EOF)
        waddch(panel_window(pchat), ch);
    fclose(fp);
}

void inflate_chat(){
    print_in_middle(stdscr, 1, xcont+1, W-xcont, contacts[cur_user].username);
    print_in_middle(stdscr, 2, xcont+1, W-xcont, contacts[cur_user].phone);
    wmove(stdscr, 3, xcont+1);
    int i;
    for (i = xcont+1; i < W-1; i++)
        waddch(stdscr, '_');
    load_msg(contacts[cur_user].phone);
}

void draw_panel(){
    clear();
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
    rearange_panel(pchat,    ychat-4, W-2-xcont, 4, xcont+1);
    scrollok(panel_window(pchat), TRUE);
    rearange_panel(ptyping,  H-2-ychat, W-2-xcont, ychat+1, xcont+1);
    inflate_contact();
    inflate_chat();
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
    } else if (ch == DIY_CL || ch == KEY_F(7)) {
        if (xcont-STEP < MIN_CONT_P*W) return ERR;
        xcont -= STEP;
        rearange_panel(pcontact, xcont-1, H-2, 1, 1);
        draw_panel();
    } else if (ch == DIY_CR || ch == KEY_F(8)) {
        if (xcont+STEP > MAX_CONT_P*W) return ERR;
        xcont += STEP;
        draw_panel();
    } else if (ch == DIY_CU || ch == KEY_F(9)) {
        if (ychat-STEP < MIN_CHAT_P*H) return ERR;
        ychat -= STEP;
        draw_panel();
    } else if (ch == DIY_CD || ch == KEY_F(10)) {
        if (ychat+STEP > MAX_CHAT_P*H) return ERR;
        ychat += STEP;
        draw_panel();
    } else if (ch == CTRL('w')) {
        return WILL_EXIT;
    } else if (ch == 'i') {
        char m[MAXLINE] = {0};
        echo();
        mvwgetnstr(panel_window(ptyping), 0, 0, m, MAXLINE);
        msg(contacts[cur_user].phone, m);
        noecho();
        wclear(panel_window(ptyping));
        wrefresh(panel_window(ptyping));
    } else if (ch == 'h') {
        return OK;
        touchwin(whelp);
        wrefresh(whelp);
        getch();
        touchwin(stdscr);
        wrefresh(stdscr);
    } else if (ch == KEY_NPAGE) {
        cur_user = (cur_user+1)%n_contacts;
        draw_panel();
    } else if (ch == KEY_PPAGE) {
        cur_user = (cur_user+n_contacts-1)%n_contacts;
        draw_panel();
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == 0) {
    } else if (ch == ERR) {
        hist(contacts[cur_user].phone);
        draw_panel();
        /* wprintw(panel_window(pchat), "Lone msg.\n"); */
        /* refresh(); */
        /* update_panels(); */
        /* doupdate(); */
    } else {
        return OK;
        waddch(panel_window(ptyping), ch);
        wrefresh(panel_window(ptyping));
    }
    return OK;
}


void init_settings_by_default(){
    lastw = W;
    lasth = H;
    STEP = 1;
    xcont = W*DFT_XCONT_P;
    ychat = H*DFT_YCHAT_P;
    cur_user = 0;
}

void destroy_ui(){
    del_panel(pchat);
    del_panel(ptyping);
    del_panel(pcontact);
}

void make_help(){
    int h = max(0.5*H, 10), w = max(0.5*W, 40);
    whelp = newwin(h, w, 2, 2);
    box(whelp, 0, 0);
    mvwaddstr(whelp, 2, 6, "Help");
    wstandend(whelp);
}

void read_info(){
    FILE *fp = fopen("/home/ubuntu/.gim/info", "r");
    fscanf(fp, "%s", self.username);
    fscanf(fp, "%s", self.phone);
    fclose(fp);
    fp = fopen("/home/ubuntu/.gim/contacts", "r");
    int i;
    fscanf(fp, "%d", &n_contacts);
    contacts = (User *) malloc(sizeof(User)*n_contacts);
    for (i = 0; i < n_contacts; i++){
        fscanf(fp, "%s", contacts[i].username);
        fscanf(fp, "%s", contacts[i].phone);
    }
    fclose(fp);
}

void start_ui(){
    start_color();
    timeout(2000);
    pcontact = create_panel(NULL, H-2, xcont-1, 1, 1);
    pchat    = create_panel(NULL, ychat-1, W-2-xcont, 1, xcont+1);
    ptyping  = create_panel(NULL, H-2-ychat, W-2-xcont, ychat+1, xcont+1);
    scrollok(panel_window(pchat), TRUE);
    draw_panel();
    while(WILL_EXIT != process_event(getch()));
    destroy_ui();
}


