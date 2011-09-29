#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include "common.h"

int dis_flag = 0;
int restar_flag = 0;
int start = 0;

void init_restar()
{
     dis_flag = 0;
     restar_flag = 0;
     start = 0;
}

void display(fb_info fb_inf)
{
		display_circle("2.jpg", fb_inf);
		display_string("暂停", 500, 100, fb_inf, 0xFF0000);
		
		if (restar_flag == 1)
		{   
		    init_restar();
		    return;
		}
		
		sleep(1);
         
		display_rand("3.jpg", fb_inf);
		display_string("暂停", 500, 100, fb_inf, 0xFF0000);
		
		if (restar_flag == 1)
		{
		    init_restar();
		    return;
		}
		
		sleep(1);
	        
		display_line("4.jpg", fb_inf);
		display_string("暂停", 500, 100, fb_inf, 0xFF0000);
		
		if (restar_flag == 1)
		{
		    init_restar();
		    return;
		}
		
		sleep(1);
	        
		display_rand_line("5.jpg", fb_inf);
		display_string("暂停", 500, 100, fb_inf, 0xFF0000);

		if (restar_flag == 1)
		{
		    init_restar();
		    return;
		}
		
		sleep(1);
}

int main(void) 
{
	fb_info fb_inf;
	if (init_fb(&fb_inf) < 0)
	{
		fprintf(stderr, "Error initial framebuffer\n");
		return 1;
	}

        menu(fb_inf);
	pid_t pid;
	
	struct sigaction sig_set;
        sigemptyset(&sig_set.sa_mask);
#if 0	
	sigdelset(&sig_set.sa_mask, 10);
        sigdelset(&sig_set.sa_mask, 12);
        sigdelset(&sig_set.sa_mask, SIGQUIT);
        sigdelset(&sig_set.sa_mask, SIGTSTP);
#endif
	sig_set.sa_handler = sig;
	sig_set.sa_flags = 0;
        
        sigprocmask(SIG_BLOCK, &sig_set.sa_mask, NULL);

	sigaction(SIGUSR1, &sig_set, NULL);
	sigaction(SIGUSR2, &sig_set, NULL);
   
      //test_mouse(fb_inf);
#if 1 
	pid = fork();
	if (pid > 0)
	{       
		while (1)
		{   
		    if (start == 0)
		    {   
		        dis_flag = 0;
			restar_flag = 0;
		        continue;
		    }

		    display(fb_inf);
		}
	}
	else if (pid == 0)
	{
		test_mouse(fb_inf);
	}
	else
	{
		perror("fork failed");	
		exit(-1);
	}
#endif
	munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);
	return 0;
}

void sig(int signo)
{
    if (signo == SIGUSR2)
    {    
	dis_flag++;
	start = 1;
    }
    else if(signo == SIGUSR1)
    { 
        restar_flag = 1;
    }
    else
    {
        ;
    }
}    
