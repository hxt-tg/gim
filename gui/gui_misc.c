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
    struct stat tmp_info, src_info;
    sprintf(tmp_path, "/home/ubuntu/.gim/%s.tmp", user_from);
    sprintf(origin_path, "/home/ubuntu/.gim/%s", user_from);
    FILE *tmpfp = fopen(tmp_path, "w"), *srcfp;
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
    curl_global_cleanup();
    fclose(tmpfp);
    srcfp = fopen(origin_path, "rb");
    tmpfp = fopen(tmp_path, "rb");
    if(fstat(fileno(tmpfp), &tmp_info) != 0) return ERR; /* can't continue */
    if(fstat(fileno(srcfp), &src_info) != 0) return ERR; /* can't continue */
    fclose(srcfp);
    fclose(tmpfp);
    if (tmp_info.st_size == src_info.st_size) return ERR;
    char cmd[512];
    sprintf(cmd, "cp %s %s", tmp_path, origin_path);
    refresh();
    system(cmd);
    return OK;
}

int upload(const char *user_to, const char *path){
    CURL *curl;

    CURLM *multi_handle;
    int still_running;

    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    static const char buf[] = "Expect:";

    /* Fill in the file upload field. This makes libcurl load data from
     *      the given file name when curl_easy_perform() is called. */
    curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "file",
            CURLFORM_FILE, path,
            CURLFORM_END);

    /* Fill in the filename field */
    curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "u1",
            CURLFORM_COPYCONTENTS, self.phone,
            CURLFORM_END);
    curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "u2",
            CURLFORM_COPYCONTENTS, user_to,
            CURLFORM_END);

    /* Fill in the submit field too, even if this is rarely needed */
    curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "submit",
            CURLFORM_COPYCONTENTS, "send",
            CURLFORM_END);

    curl = curl_easy_init();
    multi_handle = curl_multi_init();

    /* initialize custom header list (stating that Expect: 100-continue is not
     *      wanted */
    headerlist = curl_slist_append(headerlist, buf);
    if(curl && multi_handle) {

        /* what URL that receives this POST */
        curl_easy_setopt(curl, CURLOPT_URL, "http://119.29.148.127:5000/upload");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        curl_multi_add_handle(multi_handle, curl);

        curl_multi_perform(multi_handle, &still_running);

        do {
            struct timeval timeout;
            int rc; /* select() return code */
            CURLMcode mc; /* curl_multi_fdset() return code */

            fd_set fdread;
            fd_set fdwrite;
            fd_set fdexcep;
            int maxfd = -1;

            long curl_timeo = -1;

            FD_ZERO(&fdread);
            FD_ZERO(&fdwrite);
            FD_ZERO(&fdexcep);

            /* set a suitable timeout to play around with */
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            curl_multi_timeout(multi_handle, &curl_timeo);
            if(curl_timeo >= 0) {
                timeout.tv_sec = curl_timeo / 1000;
                if(timeout.tv_sec > 1)
                    timeout.tv_sec = 1;
                else
                    timeout.tv_usec = (curl_timeo % 1000) * 1000;

            }

            /* get file descriptors from the transfers */
            mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

            if(mc != CURLM_OK)
            {
                break;

            }

            /* On success the value of maxfd is guaranteed to be >= -1. We call
             *          select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
             *                   no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
             *                            to sleep 100ms, which is the minimum suggested value in the
             *                                     curl_multi_fdset() doc. */

            if(maxfd == -1) {
#ifdef _WIN32
                Sleep(100);
                rc = 0;
#else
                /* Portable sleep for platforms other than Windows. */
                struct timeval wait = { 0, 100 * 1000  }; /* 100ms */
                rc = select(0, NULL, NULL, NULL, &wait);
#endif

            }
            else {
                /* Note that on some platforms 'timeout' may be modified by select().
                 *            If you need access to the original value save a copy beforehand. */
                rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);

            }

            switch(rc) {
                case -1:
                    /* select error */
                    break;
                case 0:
                default:
                    /* timeout or readable/writable sockets */
                    curl_multi_perform(multi_handle, &still_running);
                    break;

            }

        } while(still_running);

        curl_multi_cleanup(multi_handle);

        /* always cleanup */
        curl_easy_cleanup(curl);

        /* then cleanup the formpost chain */
        curl_formfree(formpost);

        /* free slist */
        curl_slist_free_all (headerlist);

    }
    return 0;


}

int upload2(const char *user_to, const char *path){
    CURL *curl;
    CURLcode res;
    struct stat file_info;
    /* curl_off_t speed_upload, total_time; */
    FILE *fd = fopen(path, "rb"); /* open file to upload */
    if(!fd)
        return 1; /* can't continue */

    /* to get the file size */
    if(fstat(fileno(fd), &file_info) != 0)
        return 1; /* can't continue */

    curl = curl_easy_init();
    if(curl) {
        /* upload to this place */
        curl_easy_setopt(curl, CURLOPT_URL,
                "http://119.29.148.127:5000/upload");
        char msg[256];
        sprintf(msg, "u1=%s&u2=%s", self.phone, user_to);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msg);
        /* tell it to "upload" to the URL */
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* set where to read from (on Windows you need to use READFUNCTION too) */
        curl_easy_setopt(curl, CURLOPT_READDATA, fd);

        /* and give the size of the upload (optional) */
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                (curl_off_t)file_info.st_size);

        /* enable verbose for easier tracing */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        /* /1* Check for errors *1/ */
        /* if(res != CURLE_OK) { */
        /*     fprintf(stderr, "curl_easy_perform() failed: %s\n", */
        /*             curl_easy_strerror(res)); */


        /* } */
        /* else { */
        /*     /1* now extract transfer info *1/ */
        /*     curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD_T, &speed_upload); */
        /*     curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &total_time); */

        /*     fprintf(stderr, "Speed: %" CURL_FORMAT_CURL_OFF_T " bytes/sec during %" */
        /*             CURL_FORMAT_CURL_OFF_T ".%06ld seconds\n", */
        /*             speed_upload, */
        /*             (total_time / 1000000), (long)(total_time % 1000000)); */


        /* } */
        /* /1* always cleanup *1/ */
        curl_easy_cleanup(curl);

    }
    fclose(fd);
}

void download(const char *user_from, const char *save_path){
    CURL *curl;
    /* CURLcode res; */
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    char msg[MAXLINE];
    FILE *tmpfp = fopen(save_path, "w");
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://119.29.148.127:5000/download");
        sprintf(msg, "u1=%s&u2=%s", user_from, self.phone);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msg);
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,tmpfp);
        curl_easy_setopt(curl,CURLOPT_POST,1);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    fclose(tmpfp);
}

