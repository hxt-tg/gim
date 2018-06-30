#include "gui_main.h"

extern int DIY_CU;

int main(void){
    initscr();
    noecho();
    cbreak();
    build_keys();
    init_settings_by_default();
    produce_ui();
    endwin();
    return 0;
}


