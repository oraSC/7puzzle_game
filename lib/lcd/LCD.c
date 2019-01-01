#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>




pLcdInfo_t lcd_create(const char *path, pLcdInfo_t plcdinfo)
{

	//打开设备文件
	plcdinfo->fd = open(path, O_RDWR);
	if(plcdinfo->fd < 0)
	{
		perror("fail to open dev");
		goto error;
	}
	//printf("%d\n", plcdinfo->fd);	
	//获取lcd信息
	
	struct fb_var_screeninfo info;
	struct fb_var_screeninfo *pinfo = &info;
		
	int ret = ioctl(plcdinfo->fd, FBIOGET_VSCREENINFO, pinfo);
	if(ret < 0)
	{
		perror("ioctl fail");
		goto error;
	}
	
	plcdinfo->width  = pinfo->xres;
	plcdinfo->height = pinfo->yres;
	plcdinfo->size   = plcdinfo->width * plcdinfo->height * pinfo->bits_per_pixel/8;
	printf("size:%d\n", plcdinfo->size);	
	//将lcd内核空间进行 mmap 映射
	plcdinfo->base = mmap(NULL, plcdinfo->size, PROT_READ | PROT_WRITE, MAP_SHARED, plcdinfo->fd, 0);
	if(plcdinfo->base == (void *)-1)
	{
		perror("fail to mmap ");
		goto error;
	}

	//返回结构体
	return plcdinfo;
error:	
	
	//lcd_destroy(plcdinfo);
	return 0;

}


bool draw_pic(pLcdInfo_t plcdinfo, int x, int y, pJpgInfo_t pjpginfo)
{
	unsigned int *base = plcdinfo->base + y * plcdinfo->width + x;
	
	//画图片
	
	int min_W = (plcdinfo->width  - x) < pjpginfo->width  ? plcdinfo->width -  x : pjpginfo->width;
	int min_H = (plcdinfo->height - y) < pjpginfo->height ? plcdinfo->height - y : pjpginfo->height;
	printf("minW:%d, minH:%d",min_W,min_H);
	for(int rows = 0; rows < min_H - 1; rows++)
	{
		
		for(int cols = 0; cols < min_W - 1; cols++)
		{
		
			memcpy(base + cols, pjpginfo->buff + rows * pjpginfo->rowsize  + cols * 3, 3 );
			//printf("%d\n",cols);	
		}
		//printf("base add%d\n", plcdinfo->width);	
		base += plcdinfo->width ;
	
	
	
	}







	
	
	
	
}



