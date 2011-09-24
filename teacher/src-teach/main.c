#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"

/* main function */
int main(int argc, char *argv[]) 
{
    /*use to store the information of framebuffer*/
    fb_info fb_inf;
    
    /*initialing framebuffer*/
    if (init_fb(&fb_inf) < 0)
    {
        fprintf(stderr, "Error initial framebuffer\n");
	return 1;
    }
#if 0    
    while (1)
    {
        if (init_ft("mao.ttf", 0) != 0)
        {
            fprintf(stderr, "Error initial font\n")	;
 //           return 1;
        }
        
	display_string("显示文字", 32, 400, fb_inf, 0xFF0000);
    }	
#endif
    

    
    /*同步鼠标和图片显示函数*/
    syn_disp_mouse(fb_inf);

    munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);

    return 0;
}

