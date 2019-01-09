#ifndef _PUZZLEGAME_H_
#define _PUZZLEGAME_H_


#include "./lib/jpg/JPG.h"
#include "./lib/lcd/LCD.h"
#include <stdbool.h>

typedef struct {
	int X;
	int Y;
	int lastX;
	int lastY;
	bool update;
}Point_t;



//界面开启
bool 	welcome(pLcdInfo_t plcdinfo);
int 	pic_opt(pLcdInfo_t plcdinfo);
int 	grade_opt();
bool 	enter_game(pLcdInfo_t plcdinfo, pJpgInfo_t pjpginfo, int grade);





#endif


