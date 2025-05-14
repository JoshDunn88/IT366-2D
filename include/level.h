#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gf2d_sprite.h"
//#include "entity.h"
#include "world.h"
#include "enemy.h"
#include "item.h"

typedef struct Level_S
{
	Sprite*			level_background;
	//char*			level_filename;
	GFC_Vector2D	player_spawn;
	//int				object_total;
	//int				enemy_total;

}Level;


Level load_level_config_from_file(const char* filename);

void clear_level_data(Level currentlevel);
#endif