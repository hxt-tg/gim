#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include "gui_misc.h"

#define set_step(s) STEP=(s)

// Constants
#define MAX_CONT_P 0.4
#define MAX_CHAT_P 0.9
#define MIN_CONT_P 0.1
#define MIN_CHAT_P 0.4
#define DFT_XCONT_P 0.25
#define DFT_YCHAT_P 0.65

// Functions
void build_keys(); // Call at the first of main
void draw_rect(int sy, int sx, int ey, int ex);
void draw_panel();
int  process_event(int ch);
void init_settings_by_default();
void produce_ui();

// For keyboard
int DIY_CL, DIY_CR, DIY_CU, DIY_CD, DIY_ST;

#endif /* ifndef GUI_MAIN_H */

