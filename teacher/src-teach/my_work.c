/********************************************************************
  文    件:    my_work.c
  功    能:    实现线程和鼠标的同步
  函数列表:    
  日    期:
 *********************************************************************/
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include "common.h"

int dis_flag = 0;             /*显示的标志*/
int restar_flag = 0;          /*重启的标志*/
int start = 0;                /*第一次开始的标志*/
int pre_flag = 0;             /*开始预览的标志*/

/********************************************************************
  函    数:        init_restar
  功    能:        在点击停止回到主界面的时候把标志位都清零
  传入参数:        无
  传出参数:        无
  返    回:
  特殊说明:	
 ********************************************************************/
void init_restar()
{
    dis_flag = 0;
    restar_flag = 0;
    start = 0;
    pre_flag = 0;
}

/********************************************************************
  函    数:        display
  功    能:        显示图片
  传入参数:        fb_inf : 图片显示的缓冲区
  传出参数:        fb_inf : 需片信息
  返    回:
  特殊说明:	
 ********************************************************************/
void display(fb_info fb_inf)
{
    display_four("2.jpg", fb_inf);

    if (restar_flag == 1)
    {   
        init_restar();
        return;
    }


    display_rand("3.jpg", fb_inf);

    if (restar_flag == 1)
    {
        init_restar();
        return;
    }

    display_line("4.jpg", fb_inf);

    if (restar_flag == 1)
    {
        init_restar();
        return;
    }

    display_rand_line("5.jpg", fb_inf);

    if (restar_flag == 1)
    {
        init_restar();
        return;
    }

    sleep(1);
}

/********************************************************************
  函    数:        syn_mouse_disp
  功    能:        同步鼠标和显示
  传入参数:        无
  传出参数:        无
  返    回:        无
  特殊说明:	
 *******************************************************************/
int syn_mouse_disp(fb_info fb_inf) 
{
    pid_t pid;
    struct sigaction sig_set;

    sigemptyset(&sig_set.sa_mask);
    sig_set.sa_handler = sig_handler;
    sig_set.sa_flags = 0;

    sigprocmask(SIG_BLOCK, &sig_set.sa_mask, NULL);

    sigaction(SIGUSR1, &sig_set, NULL);
    sigaction(SIGUSR2, &sig_set, NULL);
    sigaction(SIGALRM, &sig_set, NULL);

    menu(fb_inf);

    pid = fork();
    if (pid > 0)
    {       
        pid = fork();
        if (pid > 0)
        {
            while (1)
            {
                while (1)
                {
                    if (start == 1)
                        break;
                    if (pre_flag == 0)
                        continue;

                    previewpicture(fb_inf);
                }
                while (1)
                {   
                    if (pre_flag == 1)
                        break;
                    if (start == 0)
                        continue;

                    display(fb_inf);
                }
            }
        }
        if (pid == 0)
        {
            music();
        }
        else
        {
            perror("fork 2 failed");
            exit(-1);
        }
    }
    else if (pid == 0)
    {
        test_mouse(fb_inf);
    }
    else
    {
        perror("fork 1 failed");	
        exit(-1);
    }

    return 0;
}

/********************************************************************
  函    数:        sig_hander
  功    能:        信号处理函数        
  传入参数:        捕捉到的信号
  传出参数:        无
  返    回:  
  特殊说明:	
 ********************************************************************/
void sig_handler(int signo)
{
    if (signo == SIGALRM)
    {    
        dis_flag++;
        start = 1;
    }

    if (signo == SIGUSR1)
    { 
        restar_flag = 1;
    }

    if (signo == SIGUSR2) 
    {
        pre_flag = 1;
    }
}    
