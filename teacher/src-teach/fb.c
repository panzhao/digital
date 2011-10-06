/********************************************************************
文    件:    fb.c
功    能:    初始化framebuffer
函数列表:
日    期:
*********************************************************************/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <stdlib.h>
#include "common.h"

#define FB_DEVICE "/dev/fb0"
#define DEBUG 0
#if DEBUG
#endif

/********************************************************************
函    数:        init_fb
功    能:        初始化framebuffer
传入参数:
传出参数:
返    回:
特殊说明:	
********************************************************************/
int init_fb(fb_info *fb_inf)
{
	int fd;

	if ((fd = open(FB_DEVICE, O_RDWR)) < 0)
	{
            fprintf(stderr, "Open %s failed:%s\n", FB_DEVICE, strerror(errno));
	    return -1;
	}

	struct fb_var_screeninfo fb_var;

	if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0)
	{
	    fprintf(stderr, "fb ioctl failed:%s\n", strerror(errno));
	    return -1;
	}

	fb_inf->w = fb_var.xres;
	fb_inf->h = fb_var.yres;
	fb_inf->bpp = fb_var.bits_per_pixel;
#if 0
	printf("width:%d\thign:%d\tbpp:%d\n",
			fb_inf->w, fb_inf->h, fb_inf->bpp);
#endif	
	fb_inf->fbmem = mmap(0, fb_inf->w * fb_inf->h * fb_inf->bpp /8,
			PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if (fb_inf->fbmem == MAP_FAILED)
	{
	    fprintf(stderr, "fb mmap failed:%s\n", strerror(errno));
	    return -1;
	}

	close(fd);

	return 0;
}

int fb_pixel(fb_info fb_inf, int x, int y, u32_t color)
{
	u8_t *pos = (u8_t*)fb_inf.fbmem + (fb_inf.w * y + x) * fb_inf.bpp/8;

	switch (fb_inf.bpp)
	{
        case 32:				  /* 32 bits */
			*(pos + 3) = color >> 24;
		case 24:				  /* 24 bits */
			*(pos + 2) = color >> 16;
		case 16:	  		          /* 16 bits */
			*(pos + 1) = color >> 8;
		case 8:			                  /* 8 bits */
			*pos = color;
			return 0;
		default:
			return -1;
	}

	return -1;

}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
#if 0 

/********************************
  函数：fb_line()
  传入参数：起始和终点坐标 及颜色
  使用算法：p = 2*dy-dx
  功能：在屏幕中打印出一条斜线（斜率非1/2）
  返回值：0      
 ********************************/
int fb_line(char *filename, fb_info fb_inf, int x1, int y1, int x2, int y2)     
{
	int dx = x2 - x1;                                      
	int dy = y2 - y1;
	int inc = ((dx*dy) > 0 ? 1 : -1);                  
	int p = 0;
	fb_info jpeg_inf;
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	if(abs(dx) > abs(dy))             /*判断dx和dy的绝对值 即 随x不断增进 根据算法判断y是否递加*/
	{
		if (dx < 0)                /* 确保x在x轴使用递加 即 将始末两坐标中x值小的作为起点坐标*/       
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
			dx = -dx;
			dy = -dy;
		}
		
		dy = abs(dy);
		p = 2*dy - dx;                /*画斜线的算法*/
		
		while (x1 <= x2)
		{
			fb_pixel(fb_inf, x1, y1, buf32[x1, y2*fb_inf.w]);
			x1++;
			if (p < 0)
			{
				p += 2*dy;
			}
			else
			{
				y1 += inc;
				p += 2*(dy - dx);
			}
		}
	}
	else                             /*随y不断增进 根据算法判断x是否需要递加*/
	{
		if (dy < 0)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
			dx = -dx;
			dy = -dy;
		}
		dx = abs(dx);
		p = 2*dx - dy;
		while (y1 <= y2)
		{
			fb_pixel(fb_inf, x1, y1, buf32[x1, y1*fb_inf.w]);
			//fb_pixel(fb_inf, x1, y1, color);
			y1++;
			if (p < 0)
			{
				p += 2*dx;
			}
			else
			{
				x1 += inc;
				p += 2*(dx - dy);
			}
		}
	}

	free(buf24);
	free(scale_buf);
	free(buf32);

	return 0;
}
#endif
/* painting horizontal */
int fb_pixel_y(fb_info fb_inf, int x, int y, int len, u32_t color)
{
	int i;
	for(i = y; i < len; i++)
	{
		fb_pixel(fb_inf,x, y+i, color);
	}

	return 0;
}
int fb_pixel_row(fb_info fb_inf, int x, int y, int len, u32_t color)
{
	int i;
#if 1
	for(i = x; i < len; i++){
		fb_pixel(fb_inf,x+i, y, color);
	}
#else
	/* FIXME: not */
	//	memset(fb_inf.fbmem + (y * fb_inf.w + x) * fb_inf.bpp/8, color, len * 
#endif

	return 0;
}

int fb_test(void)
{
	printf("Framebuffer Test\n");

	fb_info fb_inf;
	if(init_fb(&fb_inf) < 0)
	{
	    return -1;
	}

	printf("Framebuffer  Test fb_pixel\n");

	fb_pixel(fb_inf, 640, 400, 0xFFFFFFFF);

	fb_pixel_row(fb_inf, 0, 200, 1280, 0xFF0000);

	munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);
	return 0;
}

