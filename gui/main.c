#include "gui_main.h"

int main(void){
    read_info();
    initscr();
    noecho();
    cbreak();
    init_settings_by_default();
    build_keys();
    start_ui();
    endwin();
    return 0;
}


