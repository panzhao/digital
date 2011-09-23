
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "common.h"

#define DEBUG 0
#if DEBUG
#endif

/********************************************************************
函    数:     display_jpeg   
功    能:     上下同时显示图片
传入参数:     filename :    指向要显示的图片的名字
              fb_info  :    
传出参数:
返    回:
********************************************************************/
int display_jpeg(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
	
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
	
    int i = 0;
    int j = 0;
    int k = 480;

    int m = fb_inf.h - 1;
        
    /*上下同时显示*/
    for(i = 0; i < fb_inf.h / 2; ++i)
    {   
	for (j = 0; j < fb_inf.w ; ++j)
	{
            fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
	}

	for (k = 0; k < fb_inf.w; k++)
	{
	    fb_pixel(fb_inf, k, m, buf32[k + m * fb_inf.w]);
	}
	   
	usleep(10000);
        m--;
    }

    free(buf24);
    free(scale_buf);
    free(buf32);
	
    return 0;
}

/********************************************************************
函    数:    display_jpeg_l_r        
功    能:    从左右同时显示图片
传入参数:    filename :  要显示的图片的名字
             fb_info  :  存放文件的缓存区
传出参数:    fb_inf   :  被显示的图片信息存放在fb_info中
返    回:
********************************************************************/
int display_jpeg_l_r(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;                /*用来存放提取出来的图片的信息*/
	
    /*提取图片信息并存放在jpeg_inf中*/
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	
    /*将图片信息存放在*/
    u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
	
    int i = 0;
    int j = 0;
    int k = 480;

    int m = fb_inf.w - 1;
        
    /*从左往右显示*/
    for( i = 0; i < fb_inf.w; ++i)
    {   
        /*循环显示左半部分的图片*/
	for (j = 0; j < fb_inf.h; ++j)
	{
	    fb_pixel(fb_inf, i, j, buf32[i + j * fb_inf.w]);
	}
        
	/*循环显示右半部分的图片*/
	for (k = 0; k < fb_inf.h; k++)
	{
	   fb_pixel(fb_inf, m, k, buf32[m + k * fb_inf.w]);
	}

        usleep(10000);
	m--;
    }

    free(buf24);
    free(scale_buf);
    free(buf32);
	
    return 0;
}
