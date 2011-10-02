#include<stdio.h>
#include"common.h"
#include<string.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<unistd.h>
#include<stdlib.h>
char *bu[] = {"1.jpg","2.jpg","3.jpg","4.jpg","5.jpg","6.jpg","7.jpg","9.jpg","10.jpg","11.jpg","12.jpg","13.jpg","14.jpg","15.jpg"};
int previewpicture(fb_info fb_inf)
{
			
	display_jpeg_2("15.jpg",fb_inf);

	display_scale(bu[0],fb_inf.w/4,fb_inf.h/4,fb_inf.w/16,fb_inf.h/16,fb_inf);
	display_scale(bu[1],fb_inf.w/4,fb_inf.h/4,3*fb_inf.w/8,fb_inf.h/16,fb_inf);
	display_scale(bu[2],fb_inf.w/4,fb_inf.h/4,11*fb_inf.w/16,fb_inf.h/16,fb_inf);
	display_scale(bu[3],fb_inf.w/4,fb_inf.h/4,fb_inf.w/16,3*fb_inf.h/8,fb_inf);
	display_scale(bu[4],fb_inf.w/4,fb_inf.h/4,3*fb_inf.w/8,3*fb_inf.h/8,fb_inf);
	display_scale(bu[5],fb_inf.w/4,fb_inf.h/4,11*fb_inf.w/16,3*fb_inf.h/8,fb_inf);
	display_scale(bu[6],fb_inf.w/4,fb_inf.h/4,1*fb_inf.w/16,11*fb_inf.h/16,fb_inf);
	display_scale(bu[7],fb_inf.w/4,fb_inf.h/4,3*fb_inf.w/8,11*fb_inf.h/16,fb_inf);
	display_scale(bu[8],fb_inf.w/4,fb_inf.h/4,11*fb_inf.w/16,11*fb_inf.h/16,fb_inf);


	if(init_ft("mao.ttf",0) != 0)
	{
		fprintf(stderr, "Error inital font\n");
		return 1;
	}
	display_string("下一页",3*fb_inf.w/4,98*fb_inf.h/100,fb_inf, 0x00ff0000);
	pause();

	display_jpeg_2("15.jpg",fb_inf);
	display_scale(bu[9],fb_inf.w/4,fb_inf.h/4,fb_inf.w/16,fb_inf.h/16,fb_inf);
	display_scale(bu[10],fb_inf.w/4,fb_inf.h/4,3*fb_inf.w/8,fb_inf.h/16,fb_inf);
	display_scale(bu[11],fb_inf.w/4,fb_inf.h/4,11*fb_inf.w/16,fb_inf.h/16,fb_inf);
	display_scale(bu[12],fb_inf.w/4,fb_inf.h/4,fb_inf.w/16,3*fb_inf.h/8,fb_inf);
	display_scale(bu[13],fb_inf.w/4,fb_inf.h/4,3*fb_inf.w/8,3*fb_inf.h/8,fb_inf);
	display_scale(bu[5],fb_inf.w/4,fb_inf.h/4,11*fb_inf.w/16,3*fb_inf.h/8,fb_inf);
	display_scale(bu[6],fb_inf.w/4,fb_inf.h/4,1*fb_inf.w/16,11*fb_inf.h/16,fb_inf);
	display_scale(bu[7],fb_inf.w/4,fb_inf.h/4,3*fb_inf.w/8,11*fb_inf.h/16,fb_inf);
	display_scale(bu[8],fb_inf.w/4,fb_inf.h/4,11*fb_inf.w/16,11*fb_inf.h/16,fb_inf);

	if(init_ft("mao.ttf",0) != 0)
	{
		fprintf(stderr, "Error inital font\n");
		return 1;
	}
	display_string("下一页",3*fb_inf.w/4,98*fb_inf.h/100,fb_inf, 0x00ff0000);
	return 0;
}
int previewmusic(fb_info fb_inf)
{
	display_jpeg_2(bu[3],fb_inf);
	if(init_ft("mao.ttf",0) != 0)
	{
		fprintf(stderr, "Error inital font\n");
		return 1;
	}
	display_string("那年的夏天",fb_inf.w/2,fb_inf.h/2,fb_inf, 0x00ff0000);
	return 0;
}
