/********************************************************************
文    件:    mouse.c
功    能:    鼠标功能
函数列表:
日    期:
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

#define C_WIDTH  10
#define C_HEIGHT 17
#define T___     0XFFFFFFFF
#define BORD     0x0
#define X___     0xFFFF

static unsigned long cursor_pixel[C_WIDTH * C_HEIGHT] = 
{
    BORD, T___, T___, T___, T___, T___, T___, T___, T___, T___,
    BORD, BORD, T___, T___, T___, T___, T___, T___, T___, T___,
    BORD, X___, BORD, T___, T___, T___, T___, T___, T___, T___,
    BORD, X___, X___, BORD, T___, T___, T___, T___, T___, T___,
    BORD, X___, X___, X___, BORD, T___, T___, T___, T___, T___,
    BORD, X___, X___, X___, X___, BORD, T___, T___, T___, T___,
    BORD, X___, X___, X___, X___, X___, BORD, T___, T___, T___,
    BORD, X___, X___, X___, X___, X___, X___, BORD, T___, T___,
    BORD, X___, X___, X___, X___, X___, X___, X___, BORD, T___,
    BORD, X___, X___, X___, X___, X___, X___, X___, X___, BORD,
    BORD, X___, X___, X___, X___, X___, BORD, BORD, BORD, BORD,
    BORD, X___, X___, BORD, X___, X___, BORD, T___, T___, T___,
    BORD, X___, BORD, T___, BORD, X___, X___, BORD, T___, T___,
    BORD, BORD, T___, T___, BORD, X___, X___, BORD, T___, T___,
    T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
    T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
    T___, T___, T___, T___, T___, T___, BORD, BORD, T___, T___
};

static unsigned long save_cursor[C_WIDTH * C_HEIGHT];

/********************************************************************
函    数:        test_mouse
功    能:        测试鼠标的函数
传入参数:        fb_inf : framebuf的信息
传出参数:        fb_inf : 写在framebuf中的信息
返    回:
特殊说明:	
********************************************************************/
int test_mouse(fb_info fb_inf)
{
    int mfd;
    mouse_open(NULL, &mfd);

    int m_x = fb_inf.w / 2;
    int m_y = fb_inf.h / 2;
    int mouse = 0;
    
    fb_drawcursor(fb_inf, m_x, m_y);

    u8_t buf[8];
    mouse_event_t mevent;
    memset(&mevent, 0, sizeof(mevent));

    while (1)
    {
        int n = read(mfd, buf, 8);
	if (n != -1)
        {
            mouse_parse(buf,&mevent);
            fb_restorecursor(fb_inf, m_x, m_y);
            
	    m_x += mevent.x;
            m_y += mevent.y;
	   
	    judge_mouse(&m_x, &m_y, fb_inf);

            switch (mevent.button)
	    {
               case 0:
	       {
	           mouse_but(m_x, m_y, &mouse, fb_inf);
	           break;
	       }	   
	       
	       case 1:
	       {
	           mouse = 1;
		   break;
	       }

	       case 2:
	       {    
	           mouse = 2;
		   break;
	       }
	       
	       case 3:
	       {
	           mouse = 3;
		   break;
	       }	   
	       
	       default:
	       {
	           break;
               }
	    }
	
	fb_drawcursor(fb_inf, m_x, m_y);
       }
    }

	return 0;
}

int mouse_open(char *device_name, int *fd)
{
	if (NULL == device_name)
		device_name = "/dev/input/mice";

	*fd = open(device_name, O_RDONLY | O_NONBLOCK);
	if(*fd < 0){
		fprintf(stderr, "Open %s:%s\n", device_name, strerror(errno));
		exit(1);
	}

	return 0;
}

int mouse_parse(const u8_t *buf, mouse_event_t* mevent)
{
	switch(buf[0] & 0x7){
		case 1:						/* left */
			mevent->button = 1;
			break;
		case 2:
			mevent->button = 2;		/* right */
			break;
		case 4:
			mevent->button = 3;		/* middle */
			break;
		default:
			mevent->button = 0;
			break;
	}
#if 0
	mevent->x = buf[1];
	mevent->y = buf[2];
#else
	mevent->x =  buf[1] - ((buf[0] & 0x10) ? 256 : 0);
	mevent->y = -buf[2] + ((buf[0] & 0x20) ? 256 : 0); 
#endif
	return 0;
}

/* cursor */

static int fb_pixel_m(fb_info fb_inf, int x, int y, u32_t color)
{
	/* 16 bits */
	u8_t *pos = (u8_t*)fb_inf.fbmem + (fb_inf.w * y + x) * fb_inf.bpp/8;


	switch (fb_inf.bpp){
		case 32:				  /* 32 bits */
			*(pos + 3) = color >> 24;
		case 24:				  /* 24 bits */
			*(pos + 2) = color >> 16;
		case 16:	  		       /* 16 bits */
			*(pos + 1) = color >> 8;
		case 8:			      /* 8 bits */
			*pos = color;
			return 0;
		default:
			return -1;
	}
	return -1;
}

int fb_restorecursor(fb_info fb_inf, int x, int y)
{
	int i, j;
	u32_t color;
	for (j = 0;j<C_HEIGHT;j++)
	{
		for (i = 0;i<C_WIDTH;i++)
		{
			color = save_cursor[i + j * C_WIDTH];
			fb_pixel_m(fb_inf,x+i,y+j,color);
		}
	}

	return 0;
}
static int  fb_savecursor (fb_info fb_inf,int x,int y)
{
	int i,j;
	u32_t color;

	for (j = 0;j<C_HEIGHT;j++)
	{
		for (i = 0;i<C_WIDTH;i++)
		{
			color = *(u32_t*)(fb_inf.fbmem + ((y + j) * fb_inf.w + x + i) * fb_inf.bpp/8);
			save_cursor[i + j * C_WIDTH] = color;
		}
	}
	return 0;
}

int  fb_drawcursor(fb_info fb_inf, int x, int y)
{
	int i,j;
	unsigned int color;

	fb_savecursor (fb_inf,x,y);

	for (j = 0;j<C_HEIGHT;j++)
	{
		for (i = 0;i<C_WIDTH;i++)
		{
			color = cursor_pixel[i + j * C_WIDTH];
			if (color != T___)
				fb_pixel_m(fb_inf,x+i,y+j,color);
		}
	}
	return 0;
}
/*************************************************
函    数：    mouse_but()
功    能：    鼠标各个按键锁定区域及动作
传入参数：    鼠标的坐标，状态和屏幕信息
传出参数：
返    回:
特殊说明:
*************************************************/
void mouse_but(int m_x, int m_y, int *mouse, fb_info fb_inf)
{
   /*点击鼠标左键 并 点主界面的 播放区域 */
   if ((*mouse == 1) && (m_x > 100) && (m_x < 164) && (m_y > 68) && (m_y < 100))
   {   
       kill(getppid(), SIGUSR2);
   }   

   /*点击鼠标左键 并 点主界面的 预览 区域*/
   if ((*mouse == 1) && (m_x > 100) && (m_x < 164) && (m_y > 168) && (m_y < 200))
   {
       kill(getppid(), SIGALRM); 
   }

   /*点击鼠标左键 并 点主界面的 音乐 区域*/
   if ((*mouse == 1) && (m_x > 100) && (m_x < 164) && (m_y > 268) && (m_y < 300))
   {
       kill(getppid(), SIGQUIT); 
   }

   /*点击鼠标左键 并 点主界面的 退出 区域*/	
   if ((*mouse == 1) && (m_x > 100) && (m_x < 164) && (m_y > 368) && (m_y < 400))
   {
       memset(fb_inf.fbmem, 0, fb_inf.w*fb_inf.h*fb_inf.bpp/8); 
       kill(0, SIGQUIT); 
   }

   /*点击鼠标右键 实现:w
   功能：回到主界面*/ 
   if (*mouse == 2)
   {   
	kill(getppid(), SIGUSR1);
   }   

   /*点击鼠标中键 实现功能：结束所有进程*/ 
   if (*mouse == 3)
   {   
       kill(0,SIGQUIT);
   }   
    
   *mouse = 0;
}

/*********************************************************
函    名：
功    能：
传入参数：
传出参数：
返    回:
特殊说明:
*********************************************************/
void judge_mouse(int *m_x, int *m_y, fb_info fb_inf)
{
    if (*m_x >= (fb_inf.w-C_WIDTH))
    {
	*m_x = fb_inf.w-C_WIDTH;
    }
    
    if (*m_y >= (fb_inf.h-C_HEIGHT))
    {
	*m_y = fb_inf.h-C_HEIGHT;	
    }
    
    if (*m_x < 0)
    {
	*m_x = 0;	
    }
    
    if (*m_y < 0)
    {
	*m_y = 0;	
    }
	
}
