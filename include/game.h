#ifndef __GAME_H_
#define __GAME_H_

//#include "gfc_input.h"
//#include "entity.h"

#include "player.h"
#include "enemy.h"
#include "font.h"
#include "world.h"
#include "item.h"

typedef enum
{
	G_MENU,
	G_SIM,
	G_MAIN,
	G_MAX
}G_Mode;

typedef struct Game_Manager_S
{
	Entity*			game_player;
	G_Mode			game_mode;
	Uint8			done;


}GameManager;

void game_init();
void menu_init();
void sim_init();
void main_init();

void game_update();
void menu_update();
void sim_update();
void main_update();


void game_draw();
void menu_draw();
void sim_draw();
void main_draw();

void switch_mode(G_Mode next_mode);

#endif