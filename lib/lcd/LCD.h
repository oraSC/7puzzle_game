#ifndef _LCD_H_
#define _LCD_H_
#include <stdbool.h>
#include "../jpg/JPG.h"

/*
@author:czw;


*/


typedef struct{
	int fd;		//lcd 文件描述符
	unsigned int *base;	//lcd mmap映射空间首地址
	int width;	//lcd 宽
	int height;	//lcd 高
	int size;	//lcd 大小

}LcdInfo_t, *pLcdInfo_t;


pLcdInfo_t 	lcd_create(const char *path, pLcdInfo_t plcdinfo);
bool     	lcd_destroy(pLcdInfo_t plcdinfo);
bool		draw_pic(pLcdInfo_t plcdinfo, int x, int y, pJpgInfo_t pjpginfo);


#endif
