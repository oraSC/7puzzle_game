#include <stdio.h>
#include "lib/lcd/LCD.h"
#include "lib/jpg/JPG.h"
#include <string.h>
#include <stdlib.h>

int main()
{
	
	

	//创建lcdinfo结构体
	pLcdInfo_t plcdinfo = malloc(sizeof(LcdInfo_t));

	lcd_create("/dev/fb0", plcdinfo);
	//decompress_jpg2bmp("pic.jpg", "1.bmp");

	//创建jpginfo结构体
	pJpgInfo_t pjpginfo = malloc(sizeof(JpgInfo_t));
	
	//jpg解码	
	decompress_jpg2buffer(pjpginfo, "pic.jpg");
	
	
		
	
	//完整lcd图片绘制
	//draw_pic(plcdinfo, 100, 100, pjpginfo);

	
	//创建子jpginfo结构体指针
	
	int COLS, ROWS;
	
	
	scanf("%d%d", &COLS, &ROWS);
	pJpgInfo_t pdiv_jpginfo[COLS * ROWS];
	

	div_jpg(pjpginfo, COLS, ROWS, pdiv_jpginfo);
	

	for(int rows = 0; rows < ROWS; rows++)
	{
		for(int cols = 0; cols  < COLS; cols++)
		{
		
			int num = COLS * rows + cols;
	
			draw_pic(plcdinfo, cols * pdiv_jpginfo[num]->width, rows * pdiv_jpginfo[num]->height, pdiv_jpginfo[num]);
			
		}
	}

	return 0;

}



