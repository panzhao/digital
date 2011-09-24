
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
	    
传出参数:    fb_inf :  被显示的图片信息存放在fb_info中
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

/********************************************************************
函    数:      display_rand  
功    能:      随机出现点的特效
传入参数:      filename :  要显示的图片的名字
               fb_inf  :  存放文件的缓存区
传出参数:     
返    回:
********************************************************************/
int display_rand(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
	
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

    int i, j, m = fb_inf.w*fb_inf.h*fb_inf.bpp/2;

    while (m--) 
    {
        i = rand()%fb_inf.w;
        j = rand()%fb_inf.h;
	fb_pixel(fb_inf, i, j, buf32[i + j * fb_inf.w]);
    }

    free(buf24);
    free(scale_buf);
    free(buf32);
	
    return 0;
}

/********************************************************************
函    数:      display_circle
功    能:      画圆
传入参数:      filename :  要显示的图片的名字
               fb_inf   :  存放文件的缓存区
传出参数:      fb_inf   :  写入被初始化的fb_inf中的图片
返    回:
********************************************************************/
int display_circle(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
	
    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

    int i ,j, r;
    long long a,b;
    int x = fb_inf.w/2;
    int y = fb_inf.h/2;

    for(r = 0;r < 5*fb_inf.w/8;r++)
    {
        for(i = 0;i<fb_inf.h;i++)
        {
            for (j = 0;j<fb_inf.w;j++)	
            {
                a = (j-x)*(j-x)+(i-y)*(i-y);
                b = r*r;
                if(abs(a-b)<4*r)
                {
		    fb_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);
                }
            }
        }
    }
    
    free(buf24);
    free(scale_buf);
    free(buf32);
    return 0;
}

/********************************************************************
函    数:      display_scale
功    能:      缩放图片
传入参数:      filename :  要显示的图片的名字
               jpeg_w   :   
传出参数:      fb_inf   :  写入被初始化的fb_inf中的图片
返    回:
********************************************************************/
int display_scale(char *filename,int jpeg_w,int jpeg_h,int jpeg_x,int jpeg_y,fb_info fb_inf)
{
    if(jpeg_w >= fb_inf.w || jpeg_h >= fb_inf.h||jpeg_w < 0|| jpeg_h < 0)
    {
        fprintf(stderr, "the jpeg_w or jpeg_h is error!\n");
        return -1;
    }
	
    if(jpeg_x >= fb_inf.w || jpeg_y >= fb_inf.h || jpeg_x < 0 || jpeg_y < 0)
    {
        fprintf(stderr, "the jpeg_x or jpeg_y is error!\n");
        return -1;
    }
	
    fb_info jpeg_inf;
    
    fb_inf.h = jpeg_h;
    fb_inf.w = jpeg_w;
    fb_inf.bpp = fb_inf.bpp;
    fb_inf.fbmem = fb_inf.fbmem;

    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
	
    int i ,j;

    for (i = 0; i < fb_inf.h; i++)
    {
	for (j = 0 ;j < fb_inf.w; j++)	
	{
            fb_pixel(fb_inf, j+jpeg_x, i+jpeg_y,buf32[j + i*fb_inf.w]);
        }
	usleep(200);
    }
   
    free(buf24);
    free(scale_buf);
    free(buf32);

    return 0;
}
