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

typedef struct {
	int game;
	int difficulty;

}Option_t;


//界面开启
bool 		welcome(pLcdInfo_t plcdinfo);
Option_t 	opt(pLcdInfo_t plcdinfo);
bool 		enter_game(pLcdInfo_t plcdinfo, Option_t option);
void		congratulations(pLcdInfo_t plcdinfo);




#endif


