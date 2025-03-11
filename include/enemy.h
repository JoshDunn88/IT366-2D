#ifndef __ENEMY_H_
#define __ENEMY_H_

#include "entity.h"

typedef struct Enemy_Data_S
{
	int			health;
}Enemy_Data;

/**
* @brief  maka da playa data but its 2D
* @return player data pointer or NULL on failure
*/
Enemy_Data* enemy_data_new();

/**
* @brief  maka da playa but its 2D
* @return player ent, or NULL on failure
*/
Entity* enemy_new();



void enemy_think(Entity* self);
void enemy_update(Entity* self);
void enemy_draw(Entity* self);
void enemy_free(void* data);

#endif