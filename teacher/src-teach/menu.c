#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include "common.h"

int display_menu(char *, fb_info);

/*主界面 函数*/
void menu(fb_info fb_inf)
{
	display_menu("1.jpg", fb_inf);
	
	if (init_ft("mao.ttf", 0) != 0)
	{
	    fprintf(stderr, "Error initial font\n");
	    return;
	}
	
	display_string("播放", 100, 100, fb_inf, 0xFF0000);
	display_string("预览", 100, 200, fb_inf, 0xFF0000);
	display_string("音乐", 100, 300, fb_inf, 0xFF0000);
	display_string("推出", 100, 400, fb_inf, 0xFF0000);
}
