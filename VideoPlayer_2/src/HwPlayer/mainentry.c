#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

#ifdef CHIP_IS_SS268
#include "ss268_panel.h"
#elif defined CHIP_IS_SS22X
#include "ss22x_panel.h"
#else
#include "sd20xpanel.h"
#endif
#include "player.h"
#include "interface.h"

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

/*******************************************************************************************/
static int width, height;
static bool b_exit = false;
static bool player_working = false;

void signal_handler_fun(int signum) {
    printf("catch signal [%d]\n", signum);
    b_exit = true;
}

static void * mm_player_thread(void *args)
{
    int ret;
    char *filename = (char *)args;

    while (!b_exit)
    {
        if (!player_working)
        {
            sleep(1);
            continue;
        }

        ret = mm_player_get_status();
        if (ret < 0)
        {
            printf("mmplayer has been closed!\n");
            player_working = false;
            continue;
        }

        if (ret & AV_PLAY_ERROR)
        {
            mm_player_close();
            av_log(NULL, AV_LOG_ERROR, "ret is AV_PLAY_ERROR so close!!!\n");
            b_exit = true;
        }
        else if (ret & AV_PLAY_LOOP)
        {
            mm_player_close();
            av_log(NULL, AV_LOG_ERROR, "ret is AV_PLAY_LOOP so close!!!\n");
            b_exit = true;
        }
        else if ((ret & AV_PLAY_COMPLETE) == AV_PLAY_COMPLETE)
        {
            player_working = false;
            mm_player_close();
            av_log(NULL, AV_LOG_ERROR, "ret is AV_PLAY_COMPLETE so close!!!\n");
#if 0
            ret = mm_player_open(filename, 0, 0, width, height);
            if (ret < 0)
            {
                b_exit = true;
            }
#else
            b_exit = true;
#endif
            player_working = true;
        }
        av_usleep(50 * 1000);
    }

    av_log(NULL, AV_LOG_ERROR, "mm_player_thread end!!!\n");
    return NULL;
}


int play_video_hw(char *my_file_name,int my_x, int my_y, int my_width,int my_height)
{
    int ret, index = 1;
    int volumn = 0;
    bool mute = false, win_down = false;
    char cmd;
    pthread_t mm_thread = NULL;
    double duration, position;
    char url[128];
    bool disp_flag = false;
    b_exit = false;
    if (!my_file_name) {
        printf("invalid input file, please retey!\n");
        return -1;
    }

    //signal(SIGINT, signal_handler_fun);


    sd20x_sys_init();

    sd20x_panel_init(E_MI_DISP_INTF_LCD, 0);
    mm_player_set_opts("audio_device", "", 0);

    ssd20x_getpanel_wh(&width, &height);

    printf("try playing %s ...\n", my_file_name);

    mm_player_set_opts("video_rotate", "", AV_ROTATE_NONE);
    mm_player_set_opts("video_only", "", 0);
   // mm_player_set_opts("video_ratio", "", AV_SCREEN_MODE);
    mm_player_set_opts("enable_scaler", "", 1);
    mm_player_set_opts("resolution", "8294400", 0);
    mm_player_set_opts("play_mode", "", AV_ONCE);

    ret = mm_player_open(my_file_name,my_x, my_y, my_width, my_height);
    if (ret < 0) {
        goto exit;
    }
    mm_player_getduration(&duration);
    player_working = true;

    memset(url, '\0', sizeof(url));
    strncpy(url, my_file_name, strlen(my_file_name));
    ret = pthread_create(&mm_thread, NULL, mm_player_thread, (void *)url);
    if (ret != 0) {
        goto exit;
    }

    if (mm_thread)
        pthread_join(mm_thread, NULL);


exit:
    mm_player_close(); 
    sd20x_panel_deinit(E_MI_DISP_INTF_LCD);
    sd20x_sys_deinit();
    printf("play end ==> %s ...\n", my_file_name);
    return 0;
}




void play_video_init_hw()
{
    b_exit = false;

    //signal(SIGINT, signal_handler_fun);

    sd20x_sys_init();

    sd20x_panel_init(E_MI_DISP_INTF_LCD, 0);
    mm_player_set_opts("audio_device", "", 0);

    ssd20x_getpanel_wh(&width, &height);


    mm_player_set_opts("video_rotate", "", AV_ROTATE_NONE);
    mm_player_set_opts("video_only", "", 0);
    // mm_player_set_opts("video_ratio", "", AV_SCREEN_MODE);
    mm_player_set_opts("enable_scaler", "", 1);
    mm_player_set_opts("resolution", "8294400", 0);
    mm_player_set_opts("play_mode", "", AV_ONCE);
}



void play_video_uninit_hw()
{
    mm_player_close();
    sd20x_panel_deinit(E_MI_DISP_INTF_LCD);
    sd20x_sys_deinit();
}


int play_video_play_hw(char *my_file_name,int my_x, int my_y, int my_width,int my_height)
{
    int ret = 1;
    char url[128];
    b_exit = false;
    pthread_t mm_thread = NULL;
    if (!my_file_name) {
        printf("invalid input file, please retey!\n");
        return -1;
    }

    ret = mm_player_open(my_file_name,my_x, my_y, my_width, my_height);
    if (ret < 0) {
        return -1;
    }
    player_working = true;

    memset(url, '\0', sizeof(url));
    strncpy(url, my_file_name, strlen(my_file_name));
    ret = pthread_create(&mm_thread, NULL, mm_player_thread, (void *)url);
    if (ret != 0) {
        return -1;
    }

    if (mm_thread)
        pthread_join(mm_thread, NULL);
}



