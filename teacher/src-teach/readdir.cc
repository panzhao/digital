#include<dirent.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include "common.h"

int read_photo(void)
{
	struct dirent *photo;
	int i = 0;
    rd_dir r_dir;
	DIR *dir;

	dir = opendir("../photo");
	if(dir == NULL)
	{
		perror("open dir");
		exit(-1);
	}
	while((photo = readdir(dir))!= NULL)
	{
		if((photo->d_name[0])== '.')
			continue;
		//printf("%s\n",ptr);
		strcpy(r_dir.name[i],photo->d_name);
		i++;
	}
	
	return 0;
}
