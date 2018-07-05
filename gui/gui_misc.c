#include "gui_misc.h"

PANEL *create_panel(PANEL_DATA *pd, int height, int width, int sy, int sx){
    WINDOW *win = newwin(height, width, sy, sx);
    PANEL *panel = new_panel(win);
    set_panel_userptr(panel, pd);
    return panel;
}

WINDOW *rearange_panel(PANEL *panel, int new_h, int new_w, int sy, int sx){
    WINDOW *old = panel_window(panel);
    WINDOW *temp = newwin(new_h, new_w, sy, sx);
    replace_panel(panel, temp);
    // TODO: Update view.
    delwin(old);
    return temp;
}

void destroy_PANEL(PANEL *panel){
    delwin(panel_window(panel));
    del_panel(panel);
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string){
    int length, x, y;
    float temp;

    if(win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if(startx != 0)
        x = startx;
    if(starty != 0)
        y = starty;
    if(width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length)/ 2;
    x = startx + (int)temp;
    mvwprintw(win, y, x, "%s", string);
    refresh();
}

size_t write_data(void* buffer,size_t size,size_t nmemb,void *stream){
    FILE *fptr = (FILE*)stream;
    fwrite(buffer,size,nmemb,fptr);
    return size*nmemb;
}

void msg(const char *user_to, const char *m){
    CURL *curl;
    /* CURLcode res; */
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    char msg[MAXLINE];
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://119.29.148.127:5000/msg");
        sprintf(msg, "u1=%s&u2=%s&content=%s", self.phone, user_to, m);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msg);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

int hist(const char *user_from){
    CURL *curl;
    /* CURLcode res; */
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    char msg[MAXLINE];
    char tmp_path[256], origin_path[256];
    sprintf(tmp_path, "/home/ubuntu/.gim/%s.tmp", user_from);
    sprintf(origin_path, "/home/ubuntu/.gim/%s", user_from);
    printf("[[[[[[[%s]]]]]]]\n", user_from);
    FILE *tmpfp = fopen(tmp_path, "w");
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://119.29.148.127:5000/hist");
        sprintf(msg, "u1=%s&u2=%s", self.phone, user_from);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msg);
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,tmpfp);
        curl_easy_setopt(curl,CURLOPT_POST,1);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    fclose(tmpfp);
    curl_global_cleanup();
    char cmd[256];
    sprintf(cmd, "mv %s %s", tmp_path, origin_path);
    system(cmd);
    return OK;
}

void upload(const char *user_to, const char *path){
    CURL *curl;
    /* CURLcode res; */
    struct stat file_info;
    FILE *fd;
    char msg[MAXLINE];
    fd = fopen(path, "rb"); /* open file to upload */
    if(!fd) return; /* can't continue */

    /* to get the file size */
    if(fstat(fileno(fd), &file_info) != 0) return; /* can't continue */

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL,
                "http://119.29.148.127:5000/upload");
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_READDATA, fd);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                (curl_off_t)file_info.st_size);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        sprintf(msg, "u1=%s&u2=%s", self.phone, user_to);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msg);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

    }
    fclose(fd);
}

void download(const char *filepath, const char *user_from){

}

