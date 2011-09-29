#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include "common.h"

/*主界面 函数*/
void menu(void)
{
	fb_info fb_inf;
	if (init_fb(&fb_inf) < 0)
	{
		fprintf(stderr, "Error initial framebuffer\n");
		return;
	}
	display_jpeg("1.jpg", fb_inf);
	if (init_ft("mao.ttf", 0) != 0){
		fprintf(stderr, "Error initial font\n")	;
		return;
	}
	display_string("播放", 100, 150, fb_inf, string_color);
	display_string("预览", 100, 250, fb_inf, string_color);
	display_string("音乐", 100, 350, fb_inf, string_color);
}
