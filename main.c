#include <stdio.h>

#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "lib/lcd/LCD.h"
#include "lib/jpg/JPG.h"
#include <string.h>
#include <stdlib.h>
#include "puzzlegame.h"

Point_t ts_point;


void *monitor_ts(void *argv);

int main()
{
	
	

	//创建lcdinfo结构体
	pLcdInfo_t plcdinfo = (LcdInfo_t *)malloc(sizeof(LcdInfo_t));

	lcd_create("/dev/fb0", plcdinfo);
	//decompress_jpg2bmp("pic.jpg", "1.bmp");

	printf("main fd:%d,main base address:%p\n", plcdinfo->fd, plcdinfo->base);
	//创建触摸屏监控子线程
	pthread_t ts_pthid;
	
	int ret = pthread_create(&ts_pthid, NULL, monitor_ts, NULL);
	if(ret < 0)
	{
		perror("fail to create monitor_ts pthread");
		return -1;
	}
	
	while(1)
	{
		//进入游戏
		while(!welcome(plcdinfo));
		//printf("welcome\n");	

		//场景选择
		Option_t option = opt(plcdinfo);
		//printf("-0---%d----\n", option.difficulty);	
		
		//开始拼图
		if(enter_game(plcdinfo, option))
		{
			//成功
			succeed_or_die(plcdinfo, true);
		
		}
		else
		{
			succeed_or_die(plcdinfo, false);
		
		}
	}		

	/*
	bug: 如何解决关闭线程中的打开文件
	*/
	//close(fd_ts);
	
	
	return 0;

}

void *monitor_ts(void *argv)
{
	
	struct input_event tsinfo;
	//打开触摸屏设备文件
	int fd_ts = open("/dev/input/event0", O_RDWR);
	if(fd_ts < 0)
	{
		perror("fail to open event0");
		return (void *)-1;
	}



	while(1)
	{
		int ret = read(fd_ts, &tsinfo, sizeof(tsinfo));
		if(ret < 0)
		{
			perror("fail to read tsinfo");
			continue;
		}

		//读取X
		if(tsinfo.type == EV_ABS && tsinfo.code == ABS_X)
		{
			printf("x = %d\t", tsinfo.value);
			ts_point.X = tsinfo.value;
		}
		
		ret = read(fd_ts, &tsinfo, sizeof(tsinfo));
		if(ret < 0)
		{
			perror("fail to read tsinfo");
			continue;
		}
		//读取Y
		if(tsinfo.type == EV_ABS && tsinfo.code == ABS_Y)
		{
			printf("y = %d\n", tsinfo.value);
			ts_point.Y = tsinfo.value;
		}
		//防抖处理
		if(ts_point.X != ts_point.lastX && ts_point.Y != ts_point.lastY)
		{
			ts_point.lastX = ts_point.X;
			ts_point.lastY = ts_point.Y;
			ts_point.update = true;			
		
		}
	
	}
	






}






