#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include "common.h"


int main(int argc, const char *argv[])
{
    fb_info fb_inf;

    if (init_fb(&fb_inf) < 0)
    {
        fprintf(stderr, "ERRor initial framebuf\n");
    	return 1;
    }

    syn_mouse_disp(fb_inf);
    
    munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);
    
    return 0;
}
