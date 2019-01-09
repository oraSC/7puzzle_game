#include "puzzlegame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define POS_X 200
#define POS_Y 40





extern Point_t ts_point;


bool welcome(pLcdInfo_t plcdinfo)
{


	//创建jpginfo结构体
	pJpgInfo_t pjpginfo = (JpgInfo_t *)malloc(sizeof(JpgInfo_t));
	if(pjpginfo == NULL)
	{
		perror("fail to malloc");
		return false;
		
	}
	//jpg解码	
	decompress_jpg2buffer(pjpginfo, "./pic/interface.jpg");
	
	
	//开始界面显示
	draw_pic(plcdinfo, 0, 0, pjpginfo);
	
	//等待用户开始游戏
	while(1)
	{
		if(ts_point.X >= 326 && ts_point.X <= 326+152 && ts_point.Y >= 358 && ts_point.Y <= 358+63)
		{
			break;
		}
	
	}
	
	free(pjpginfo);
	return true;


}

int pic_opt(pLcdInfo_t plcdinfo)
{

	pJpgInfo_t pbg_jpginfo = (JpgInfo_t *)malloc(sizeof(JpgInfo_t));
	//pBtnInfo_t pbtninfo = (BtnInfo_t *)malloc(sizeof(BtnInfo_t));
	if(pbg_jpginfo == NULL)
	{
		perror("fail to malloc");
		return false;
		
	}


	decompress_jpg2buffer(pbg_jpginfo, "./pic/opt/bg.jpg");

	draw_pic(plcdinfo, 0, 0, pbg_jpginfo);
	
	//创建链表
	pBtn_SqList_t head = create_btn_sqlist();
	

	/*
	待完善
	*/
	
	//设置闯关模式
	int all_level = 10;
	int level = 7;
	
	pJpgInfo_t popt_jpginfo = (JpgInfo_t *)malloc(sizeof(JpgInfo_t));
	
	//依次读取 opt 图片
	char optName[20] = {0};
	int i = 0;
	for(i = 1; i <= level; i++)
	{
		
		memset(optName, 0, 20);
		sprintf(optName, "./pic/opt/%d.jpg",i);
		printf("%s\n", optName);

		decompress_jpg2buffer(popt_jpginfo, optName);

		if(i > 5)
		{
			pBtn_SqList_t node = draw_btn(plcdinfo, 150*(i - 5 -1) + 50, 300, popt_jpginfo);
			AddFromTail_btn_sqlist(head, node);
		}
		else 
		{
			pBtn_SqList_t node = draw_btn(plcdinfo, 150*(i - 1) + 50, 100, popt_jpginfo);
			AddFromTail_btn_sqlist(head, node);
		}	
		//释放 opt_jpginfo 指向的 buff 内存
		free(popt_jpginfo->buff);
	}
	




	//读取锁图标
	decompress_jpg2buffer(popt_jpginfo, "./pic/opt/lock.jpg");
	for(i; i <= all_level; i++)
	{
		
		if(i > 5)
		{
			draw_pic(plcdinfo, 150*(i - 5 -1) + 50, 300, popt_jpginfo);
		}
		else 
		{
			draw_pic(plcdinfo, 150*(i - 1) + 50, 100, popt_jpginfo);
		
		}
		
	
	}
	//释放 opt_jpginfo 指向的 buff 内存
	free(popt_jpginfo->buff);

	//释放 opt_jpginfo 内存
	free(popt_jpginfo);

	//用户选择场景，记录用户选择的场景
	int game = 0;
	while(1)
	{
		if(ts_point.update == true)
		{
			game = find_which_btn_click(head, ts_point.X, ts_point.Y);
			ts_point.update = false;	
			
			if(game)
			{
				break;	
			}
			
		}
	
	
	}

	//清空链表
	clear_btn_sqlist(&head);

	//难度选择i
	char dif_pic_path1[15]  = "./pic/opt/";
	char dif_pic_path2[][15] ={"dif_bg.jpg","4X4.jpg","6X6.jpg","8X8.jpg"};
	

	//结构体数组？？？？？
	
	JpgInfo_t dif_jpginfo[4];
	pJpgInfo_t pdif_jpginfo = &dif_jpginfo[0];
	//读取图片信息
	for(int i = 0; i < 4; i++)
	{
		char path[30] = {0};

		strcat(path, dif_pic_path1);
		strcat(path, dif_pic_path2[i]);
		decompress_jpg2buffer(&dif_jpginfo[i], path);
	}
	
	//bg
	draw_pic(plcdinfo, 200, 120, &dif_jpginfo[0]);
	//cancel
	pBtn_SqList_t node;
	//node = draw_btn(plcdinfo, 300, 140, &dif_jpginfo[1]);
	//AddFromTail_btn_sqlist(head, node);
	//4X4
	node = draw_btn(plcdinfo, 200+128, 120+50, &dif_jpginfo[1]);
	AddFromTail_btn_sqlist(head, node);
	//6X6
	node = draw_btn(plcdinfo, 200+128, 120+50+60, &dif_jpginfo[2]);
	AddFromTail_btn_sqlist(head, node);
	//8X8
	node = draw_btn(plcdinfo, 200+128, 120+50+60+60, &dif_jpginfo[3]);
	AddFromTail_btn_sqlist(head, node);
	//ergodic_btn_sqlist(head);	
	
	//用户选择难度，记录用户选择的难度
	int difficulty = 0;
	while(1)
	{
		if(ts_point.update == true)
		{
			game = find_which_btn_click(head, ts_point.X, ts_point.Y);
			ts_point.update = false;	
			
			if(game)
			{
				//break;
				printf("%d\n", game);
			}
			
		}

	
	
	
	}
	
	destroy_btn_sqlist(&head);
	free(pbg_jpginfo);


}





bool enter_game(pLcdInfo_t plcdinfo, pJpgInfo_t pjpginfo, int grade)
{

	int COLS = grade;
	int ROWS = grade;


	pJpgInfo_t pdiv_jpginfo[COLS * ROWS];
	

	div_jpg(pjpginfo, COLS, ROWS, pdiv_jpginfo);
	
	
	//乱序排列图片
	pJpgInfo_t rand_pjpginfo[COLS * ROWS];
	//利用字符串记录当前排序
	unsigned char rank[COLS * ROWS + 1];
	
	int len = COLS * ROWS;
	//协助数组
	int help_arr[COLS * ROWS];
	for(int i = 0; i < COLS * ROWS; i++)
	{
		help_arr[i] = i + 1;
		printf("%d\t", help_arr[i]);
	}
	srandom(3);
	printf("\n");
	for(int i = 0; i < COLS * ROWS; i++)
	{
		int index = random()%len;
		printf("%d\t", index);
		rank[i] = help_arr[index];

		rand_pjpginfo[i] = pdiv_jpginfo[help_arr[index] - 1];
		printf("%d\n", rank[i]);
		help_arr[index] = help_arr[len - 1];
		len--;
		
	}
	

	//开始拼图
	for(int rows = 0; rows < ROWS; rows++)
	{
		for(int cols = 0; cols  < COLS; cols++)
		{
		
			int num = COLS * rows + cols;
	
			draw_pic(plcdinfo, cols * pdiv_jpginfo[num]->width + POS_X, rows * pdiv_jpginfo[num]->height + POS_Y, rand_pjpginfo[num]);
			printf("num=%d\n", num);	
		}
	}
	

}









