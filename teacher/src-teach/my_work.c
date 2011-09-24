/********************************************************************
文    件:     my_work.c
功    能:     实现文件鼠标和图片的同步操作
函数列表:
日    期:
*********************************************************************/
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "common.h"

/*全局的用来记录鼠标事件的结构体*/
mouse_event_t mevent;

/********************************************************************
函    数:        syn_disp_mouse
功    能:        实现鼠标和图片显示的同步
传入参数:        
传出参数:
返    回:
********************************************************************/
int syn_disp_mouse(fb_info fb_inf)
{   
    /*存放线程的id*/
    pthread_t tid;  
    
    /*存放创建线程的返回值*/
    int err = 0;

    display_jpeg("test.jpg", fb_inf);
    
    /*创建用来鼠标工作的线程*/
    err = pthread_create(&tid, NULL, operate_mouse, (void *)&fb_inf);
    if (err != 0)
    {
        fprintf(stderr, "can't create the pthread\n");
	exit(-1);
    }
   
    /*用来显示图片的函数*/
    operate_display(fb_inf);

    return 0;
}

/********************************************************************
函    数:        operation_mouse
功    能:        使鼠标工作起来
传入参数:
传出参数:
返    回:
********************************************************************/
void *operate_mouse(void *fb_inf)
{
    test_mouse(*(fb_info *)fb_inf);

    pthread_exit((void *)0);
}

/********************************************************************
函    数:        display_photo
功    能:        用来显示图片的函数
传入参数:        
传出参数:
返    回:
********************************************************************/
int operate_display(fb_info fb_inf)
{    
    while (1)
    {
        switch(mevent.button)
	{
	    case 1:       /*显示图片*/
	    {
	        display_photo(fb_inf); 
	        break;
	    }

	    case 2:       /*显示主界面*/
	    {
                display_jpeg("test.jpg", fb_inf);
		break;
	    }

	    case 3:
	    {
	        exit(1);
	    }

	    default :
	    {
                display_jpeg("test.jpg", fb_inf);
		break;
	    }
	}
    }	 

    return 0;
}


/********************************************************************
函    数:        display_photo
功    能:        显示图片的函数
传入参数:        fb_inf  :  缓存区的信息
传出参数:
返    回:
********************************************************************/
int display_photo(fb_info fb_inf)
{
    display_jpeg("0.jpg", fb_inf);
    sleep(1);
           
    display_rand("1.jpg", fb_inf);
    sleep(1);
	
    display_circle("2.jpg", fb_inf);
    sleep(1);
	
    display_jpeg_l_r("3.jpg", fb_inf);
    sleep(1);

    return 0;
}

#if 0
/*备用*/
    /*测试特效*/
#if 0
    while (1)
    {
        display_jpeg("0.jpg", fb_inf);
        sleep(1);
           
        display_rand("1.jpg", fb_inf);
        sleep(1);
	
        display_circle("2.jpg", fb_inf);
        sleep(1);
	
        display_jpeg_l_r("3.jpg", fb_inf);
       sleep(1);   
     }
     
     //system("./mp3 2.mp3");
#endif
    /*结束特效测试*/
    

    /* Test mouse */
    //test_mouse(fb_inf);

#if 1
    /*Test font*/
    while (1)
    {
        if (init_ft("mao.ttf", 0) != 0)
        {
            fprintf(stderr, "Error initial font\n")	;
            return 1;
        }
    }	
    
    display_string("显示文字", 32, 400, fb_inf, 0xFF0000);
#endif
/*备用*/
#endif
