#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include "common.h"

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
    {
	printf("user1\n");
    }

    if (signo == SIGUSR2)
    {
	printf("user2\n");
    }
}

void display(fb_info fb_inf)
{

    while (1)	
    {
	display_circle("2.jpg", fb_inf);
	display_string("暂停", 600, 100, fb_inf, string_color);
	sleep(1);

	display_rand("3.jpg", fb_inf);
	display_string("暂停", 600, 100, fb_inf , string_color);
	sleep(1);

	display_line("4.jpg", fb_inf);
	display_string("暂停", 600, 100, fb_inf, string_color);
	sleep(1);

	display_rand_line("5.jpg", fb_inf);
	display_string("暂停", 600, 100, fb_inf, string_color);
	sleep(1);
    }

}

#if 1
int main(void) 
{

    fb_info fb_inf;
    if (init_fb(&fb_inf) < 0)
    {
	fprintf(stderr, "Error initial framebuffer\n");
	return 1;
    }

    pid_t pid;

    struct sigaction sig_set;

    sigemptyset(&sig_set.sa_mask);
    sig_set.sa_handler = display;
    //sig_set.sa_handler = sig_handler;
    sig_set.sa_flags = 0;

    // while (1);
    menu();
    pid = fork();
    if (pid > 0)
    {
    	sigaction(SIGUSR1, &sig_set, NULL);
	//pause();
	//sigprocmask(SIG_BLOCK, &sig_set, NULL);
	//display(fb_inf);
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

    munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);

    return 0;
}
#endif
#if 0
int main(int argc, const char *argv[])
{   
    struct sigaction sig_set;

    sig_set.sa_handler = sig_handler;
    sigemptyset(&sig_set.sa_mask);
    sigaddset(&sig_set.sa_mask, SIGUSR1);
    sigaddset(&sig_set.sa_mask, SIGUSR2);

    sig_set.sa_flags = 0;

    //sigprocmask(SIG_BLOCK, &sig_set.sa_mask, NULL);
    sigaction(SIGUSR1, &sig_set, NULL);
    sigaction(SIGUSR2, &sig_set, NULL);

    while (1)
    {
	kill(getpid(), SIGUSR1);
	sleep(1);
	kill(getpid(), SIGUSR2);
	sleep(1);
    }

    return 0;
}
#endif
