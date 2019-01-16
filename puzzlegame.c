#include "puzzlegame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define POS_X 200
#define POS_Y 40
#define POS_TIME_X 680
#define POS_TIME_Y 100





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
	//释放结构体指向的buff和结构体本身
	free(pjpginfo->buff);
	free(pjpginfo);
	
	
	
	
	
	
	
	return true;
	

}


/*
backlog:堆内存未释放
*/
Option_t opt(pLcdInfo_t plcdinfo)
{

	pJpgInfo_t pbg_jpginfo = (JpgInfo_t *)malloc(sizeof(JpgInfo_t));
	//pBtnInfo_t pbtninfo = (BtnInfo_t *)malloc(sizeof(BtnInfo_t));
	if(pbg_jpginfo == NULL)
	{
		perror("fail to malloc");
		return ;
		
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
	Option_t option;
	while(1)
	{
		if(ts_point.update == true)
		{
			option.game = find_which_btn_click(head, ts_point.X, ts_point.Y);
			ts_point.update = false;	
			
			if(option.game)
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
	while(1)
	{
		if(ts_point.update == true)
		{
			option.difficulty = find_which_btn_click(head, ts_point.X, ts_point.Y);
			ts_point.update = false;	
			
			if(option.difficulty)
			{
				break;
				//printf("%d\n", difficulty);
			}
			
		}

	
	
	
	}
	
	//释放内存
	destroy_btn_sqlist(&head);
	free(pbg_jpginfo->buff);
	free(pbg_jpginfo);
	
	return option;
}





bool enter_game(pLcdInfo_t plcdinfo, Option_t option)
{
	//读取背景、倒计时数字、场景和相关按键图片
	pJpgInfo_t pbg_jpginfo = (JpgInfo_t *)malloc(sizeof(JpgInfo_t));
	pJpgInfo_t pgame_jpginfo = (JpgInfo_t *)malloc(sizeof(JpgInfo_t));
	JpgInfo_t  num_jpginfo[9];

	if(pgame_jpginfo == NULL || pbg_jpginfo == NULL)
	{
		perror("fail to malloc for pgame_jpginfo or pbg_jpginfo");
		return -1;
	}

	for(int i = 0; i < 10; i++)
	{
		unsigned char num_path[20] = {0};
		sprintf(num_path, "./pic/game/num%d.jpg", i);
		decompress_jpg2buffer(&num_jpginfo[i], num_path);
	}



	//printf("%s\n", path);
	//背景
	decompress_jpg2buffer(pbg_jpginfo, "./pic/game/bg.jpg");
	draw_pic(plcdinfo, 0, 0, pbg_jpginfo);	


	//场景
	unsigned char path[20] = {0};
	sprintf(path, "./pic/game/%d.jpg", option.game);
	decompress_jpg2buffer(pgame_jpginfo, path);
	
	int COLS = 2*option.difficulty + 2;
	int ROWS = 2*option.difficulty + 2;
	

	/*
	bug:当COLS=3,ROWS=2出现图片错误，可能时分割错误可能是绘制错误
	*/
	//COLS = 3;
	//ROWS = 3;

	//展示5秒钟
	draw_pic(plcdinfo, POS_X, POS_Y, pgame_jpginfo);
	for(int i = 0; i < 5; i++)
	{
		draw_pic(plcdinfo, POS_TIME_X+40, POS_TIME_Y, &num_jpginfo[5-i]);
		sleep(1);
	
	}
	draw_pic(plcdinfo, POS_TIME_X+40, POS_TIME_Y, &num_jpginfo[0]);



	

	//拆分图片
	pJpgInfo_t pdiv_jpginfo[COLS * ROWS];
	div_jpg(pgame_jpginfo, COLS, ROWS, pdiv_jpginfo);
	
	//乱序排列图片
	pJpgInfo_t rand_pjpginfo[COLS * ROWS];
	//利用字符串记录当前排序
	unsigned char rank[COLS * ROWS + 1];
	unsigned char finish_rank[COLS * ROWS + 1];
	memset(rank, 0, COLS * ROWS + 1);
	memset(finish_rank, 0, COLS * ROWS + 1);
	int len = COLS * ROWS;
	//协助数组
	int help_arr[COLS * ROWS];
	for(int i = 0; i < COLS * ROWS; i++)
	{
		help_arr[i] = i + 1;
		//printf("%d\t", help_arr[i]);
	}
	srandom(3);
	printf("\n");
	for(int i = 0; i < COLS * ROWS; i++)
	{
		int index = random()%len;
		//printf("%d\t", index);
		rank[i] = help_arr[index];
		finish_rank[i] = i+1;
		rand_pjpginfo[i] = pdiv_jpginfo[help_arr[index] - 1];
		printf("%d\t", rank[i]);
		help_arr[index] = help_arr[len - 1];
		len--;
		
	}
	printf("\n");
	//创建链表
	pBtn_SqList_t head = create_btn_sqlist();
		
	/*
	backlog:通过创建pbtninfo记录btn位置
	*/
	pBtnInfo_t pbtninfo[ROWS * COLS];

	//开始拼图
	for(int rows = 0; rows < ROWS; rows++)
	{
		for(int cols = 0; cols  < COLS; cols++)
		{
		
			int num = COLS * rows + cols;
	
			pbtninfo[num] = draw_btn(plcdinfo, cols * pdiv_jpginfo[num]->width + POS_X, rows * pdiv_jpginfo[num]->height + POS_Y, rand_pjpginfo[num]);
			
			//将新按键加入到链表中
			pBtn_SqList_t node = pbtninfo[num];
			AddFromTail_btn_sqlist(head, node);

			//printf("num=%d\n", num);	
		}
	}

	int pic1;
	int pic2;
	//int div_width  = rand_pjpginfo[0].width;
	//int div_height = rand_pjpginfo[0].height;
	long buff_size  = rand_pjpginfo[0]->rowsize * rand_pjpginfo[0]->height;
	Rect_t tips_rect={	rand_pjpginfo[0]->width,
				rand_pjpginfo[0]->height,
				0x00FF00,
				5
			};
	pRect_t ptips_rect = &tips_rect;
	//
	printf("开始:");
	for(int i = 0; i < COLS * ROWS; i++)
	{
		printf("%d\t", rank[i]);
		
	}
	printf("\n");
	
	printf("开始:");
	for(int i = 0; i < COLS * ROWS; i++)
	{
		printf("%d\t", finish_rank[i]);
		
	}
	printf("\n");
	while(1)
	{
		//选取两张图片互换
		if(ts_point.update)
		{
			pic1 = find_which_btn_click(head, ts_point.X, ts_point.Y);
			ts_point.update = false;
			printf("pic1:%d\n", pic1);
			//选中图片加框
			draw_rect(plcdinfo, pbtninfo[pic1-1]->X, pbtninfo[pic1-1]->Y, ptips_rect);

			if(pic1 == 0)
			{
				continue;
			}
			while(1)
			{
				if(ts_point.update)
				{
					pic2 = find_which_btn_click(head, ts_point.X, ts_point.Y);
					ts_point.update = false;
					printf("pic2:%d\n", pic2);
					draw_rect(plcdinfo, pbtninfo[pic2-1]->X, pbtninfo[pic2-1]->Y, ptips_rect);
					if(pic2 ==0)
					{
						continue;
					}
					
					//仅互换buff,链表位置不改变
					unsigned char *buff = NULL;
					//memcpy(buff_temp, rand_pjpginfo[pic1].buff,buff_size);
					//memcpy(rand_pjpginfo[pic1].buff, rand_pjpginfo[pic2], buff_size)
					buff 			 = rand_pjpginfo[pic1-1]->buff;
					rand_pjpginfo[pic1-1]->buff = rand_pjpginfo[pic2-1]->buff;
					rand_pjpginfo[pic2-1]->buff = buff;
					//更新rank
					if(pic1 != pic2)
					{
						rank[pic1-1] = rank[pic1-1] + rank[pic2-1];
						rank[pic2-1] = rank[pic1-1] - rank[pic2-1];
						rank[pic1-1] = rank[pic1-1] - rank[pic2-1];
					}
					printf("调整:");
					for(int i = 0; i < COLS * ROWS; i++)
					{
						printf("%d\t", rank[i]);
				
					}
					printf("\n");
					printf("目标:");
					for(int i = 0; i < COLS * ROWS; i++)
					{
						printf("%d\t", rank[i]);
				
					}
					printf("\n");

					//更新
					draw_pic(plcdinfo, pbtninfo[pic1-1]->X, pbtninfo[pic1-1]->Y, rand_pjpginfo[pic1-1]);
					draw_pic(plcdinfo, pbtninfo[pic2-1]->X, pbtninfo[pic2-1]->Y, rand_pjpginfo[pic2-1]);
					break;
				}
			}
		
		printf("比较结果:%d\n", strcmp(rank, finish_rank));
		}
		if(!strcmp(rank, finish_rank))
		{
			break;
		}


	}
	//printf("succeed\n");
	
	//释放内存
	free(pbg_jpginfo->buff);
	free(pbg_jpginfo);
	free(pgame_jpginfo->buff);
	free(pgame_jpginfo);

	
	return true;	
	




}

void congratulations(pLcdInfo_t plcdinfo)
{

	sleep(1);

	pJpgInfo_t pjpginfo = (JpgInfo_t *)malloc(sizeof(JpgInfo_t));
	decompress_jpg2buffer(pjpginfo, "./pic/game/congratulations.jpg");

	

	draw_pic(plcdinfo, 0, 0, pjpginfo);

	sleep(3);
	//释放内存
	free(pjpginfo->buff);
	free(pjpginfo);


}







