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
/*斜线*/
int display_dia_line(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j, m, n;

	for (i=0,j=0,m=fb_inf.w-1,n=fb_inf.h-1;i<fb_inf.w||j<fb_inf.h||m>0||n>0; i++,j++,m--,n--)
	{
		fb_line(filename, fb_inf, i, 0, fb_inf.w-1, n);
		fb_line(filename, fb_inf, 0, j, m, fb_inf.h-1);
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
#if 0
/*实现 中间 两根线分别 拉伸 则出现四块*/
int display_four(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j, m, n;
	for(i = fb_inf.h/2; i > 0; i--)
	{
		for (j = 0; j < fb_inf.w; j++)
		{
			fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
			fb_pixel(fb_inf,j,fb_inf.h-i-1,buf32[j+(fb_inf.h-i-1)*(fb_inf.w)]);
		}
		usleep(900);
	}
	free(buf24);
	free(scale_buf);
	free(buf32);
	return 0;
}
#endif
int display_line(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;
#if 1
	for(i = 0; i < fb_inf.h/2; ++i)
	{
		for (j = 0; j < fb_inf.w; ++j)
		{
			fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
			fb_pixel(fb_inf,j,fb_inf.h-i-1,buf32[j+(fb_inf.h-i-1)*(fb_inf.w)]);
		}
		usleep(900);
	}
#endif
#if 0
	for(i =fb_inf.h-10 ; i >0; --i){
		usleep(500);
		for (j = fb_inf.w-10; j > 0; --j)
		{
			fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
	}
#endif
	free(buf24);
	free(scale_buf);
	free(buf32);
	return 0;
}
/*老师 给的 画圆*/
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
			for(j = 0;j<fb_inf.w;j++)	
			{
				a = (j-x)*(j-x)+(i-y)*(i-y);
				b = r*r;
				if(abs(a-b)<4*r)
					fb_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);

			}
		}
	}
	free(buf24);
	free(scale_buf);
	free(buf32);
	return 0;
}
/*书上抄的缩放 没搞懂*/
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

	fb_info fb_inff;
	fb_inff.h = jpeg_h;
	fb_inff.w = jpeg_w;
	fb_inff.bpp = fb_inf.bpp;
	fb_inf.fbmem = fb_inf.fbmem;

	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
	
	int i ,j;

	for (i = 0; i < fb_inf.h; i++)
	{
		for (j = 0 ;j < fb_inf.w; j++)	
		{
			fb_pixel(fb_inf, j+jpeg_x, i+jpeg_y,buf32[j + i*fb_inff.w]);
		}
		usleep(200);
	}
	free(buf24);
	free(scale_buf);
	free(buf32);

	return 0;
}
/*想实现 从图片的一边抽出来的效果 可能思路有问题 pass*/
int display_jpeg(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j, m, n;
	for(i = fb_inf.h-1, m = fb_inf.h-1; m > 0; m--, i--)
	{
		for (j = 0, n = fb_inf.w-1; j < fb_inf.w || n > 0; n--, j++)
		{
			fb_pixel(fb_inf, j, i, buf32[n + m * fb_inf.w]);
		}
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
/*今天刚研究的 随机出现点 哈哈哈*/
int display_rand(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
	int i, j, m = fb_inf.w*(fb_inf.h-1)*fb_inf.bpp;

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
/*随机 线条*/
int display_rand_line(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int j, i, m = fb_inf.w*8;

	while (m--)
	{
		i = rand()%fb_inf.h;
		for (j = 0; j < fb_inf.w; j++)
		{
			fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
