#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include<stdio.h>
#include"common.h"
#include<string.h>
#include<sys/mman.h>
#include<unistd.h>
#include<stdlib.h>
#define PICTURE_MAX 1024
#define MUSIC_MAX 1024

int sig_number = 0;
#if 0
void sig_usr(int signo)
{
    fb_info fb_inf;

	if (signo == SIGABRT)
	{
        printf("wwwwwwwwwwwwwwwwwww\n");
		sig_number = sig_number-18;
	}
	if (signo == SIGCONT)
	{
        printf("qqqqqqqqqqqqqqqqqqqq\n");
		usleep(100);
	}
/*    if (signo == SIGUSR1)
    {
        init_restar();
        return;
    }
*/}
#endif

int previewpicture(fb_info fb_inf)
{
	struct  dirent *mydr;
	DIR *fpdir;
	char buf[PICTURE_MAX][PICTURE_MAX];
	int picture_number = 0;
	int i = 0;
 /* struct sigaction sig_set;

    sigemptyset(&sig_set.sa_mask);
    sig_set.sa_handler = sig_usr;
    sig_set.sa_flags = 0;

    sigprocmask(SIG_BLOCK, &sig_set.sa_mask, NULL);


    sigaction(SIGABRT, &sig_set, NULL);
    sigaction(SIGCONT, &sig_set, NULL);
*/
	fpdir = opendir("../image");
	while(((mydr = readdir(fpdir) ))!= NULL )
	{
		if((strcmp(mydr->d_name, "..")==0) || (strcmp(mydr->d_name, ".")==0))
			continue;
		strcpy(buf[picture_number],mydr->d_name);
		picture_number++;
	}
	closedir(fpdir);

	while(i <= picture_number )
	{
		display_jpeg("1.jpg",fb_inf);
		if(sig_number < 0)
			sig_number = 0;
		if(sig_number > picture_number)
			sig_number = picture_number;
		i = sig_number;
		
		if(i < picture_number )
			display_scale(buf[i++],fb_inf.w/4,fb_inf.h/4,fb_inf.w/16,fb_inf.h/16,fb_inf);
		else break;
		if(i < picture_number )
			display_scale(buf[i++],fb_inf.w/4,fb_inf.h/4,3*fb_inf.w/8,fb_inf.h/16,fb_inf);
		else break;
		if(i < picture_number )
			display_scale(buf[i++],fb_inf.w/4,fb_inf.h/4,11*fb_inf.w/16,fb_inf.h/16,fb_inf);
		else break;
		if(i < picture_number )
			display_scale(buf[i++],fb_inf.w/4,fb_inf.h/4,fb_inf.w/16,3*fb_inf.h/8,fb_inf);
		else break;
		if(i < picture_number )
			display_scale(buf[i++],fb_inf.w/4,fb_inf.h/4,3*fb_inf.w/8,3*fb_inf.h/8,fb_inf);
		else break;
		if(i < picture_number )
			display_scale(buf[i++],fb_inf.w/4,fb_inf.h/4,11*fb_inf.w/16,3*fb_inf.h/8,fb_inf);
		else break;
		if(i < picture_number )
			display_scale(buf[i++],fb_inf.w/4,fb_inf.h/4,1*fb_inf.w/16,11*fb_inf.h/16,fb_inf);
		else break;
		if(i < picture_number )
			display_scale(buf[i++],fb_inf.w/4,fb_inf.h/4,3*fb_inf.w/8,11*fb_inf.h/16,fb_inf);
		else break;
		if(i < picture_number )
			display_scale(buf[i++],fb_inf.w/4,fb_inf.h/4,11*fb_inf.w/16,11*fb_inf.h/16,fb_inf);
		else break;
		sig_number = i;

		if(init_ft("mao.ttf",0) != 0)
		{
			fprintf(stderr, "Error inital font\n");
			return 1;
		}
		display_string("上一页",fb_inf.w/2,98*fb_inf.h/100,fb_inf, 0x00ff0000);
		display_string("下一页",3*fb_inf.w/4,98*fb_inf.h/100,fb_inf, 0x00ff0000);

        if (restar_flag == 1)
        {
        printf("ddddddddddd\n");
            init_restar();
            return 1;
        }
        pause();
	}
	display_string("已到最后一页",fb_inf.w,98*fb_inf.h/100,fb_inf, 0x00ff0000);

	return 0;
}

#if 0
int previewmusic(fb_info fb_inf)//音乐名字预览未完成
{
	struct  dirent *musicdr;
	DIR *musicdir;
	char bufmusic[MUSIC_MAX][MUSIC_MAX];
	int music_number = 0;
	int j = 0,i = 1;
	display_jpeg_2("1.jpg",fb_inf);
	musicdir = opendir("/home/akaedu/929/teacher/music");
	while((musicdr = readdir(musicdir) )!= NULL )
	{
		if((strcmp(musicdr->d_name, "..")==0) || (strcmp(musicdr->d_name, ".")==0))
			continue;
		strcpy(bufmusic[music_number],musicdr->d_name);
		music_number++;
		//      printf("%s\n",buf);
	}

	closedir(musicdir);
	if(init_ft("mao.ttf",0) != 0)
	{
		fprintf(stderr, "Error inital font\n");
		return 1;
	}
	while(j<=music_number)
	{
		display_string(bufmusic[j],fb_inf.w/8,(j+1)*fb_inf.h/10,fb_inf,0x0000ff00);
		j++;
		display_string("上一页",fb_inf.w/2,98*fb_inf.h/100,fb_inf, 0x00ff0000);
		display_string("下一页",3*fb_inf.w/4,98*fb_inf.h/100,fb_inf, 0x00ff0000);
		if(j == 10*i)
		{
			i++;
			pause();
		}
	}
	display_string("已到最后一曲",fb_inf.w,98*fb_inf.h/100,fb_inf, 0x00ff0000);
	return 0;
}
#endif
