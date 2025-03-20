#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "gfc_input.h"

#include "font.h"
#include "entity.h"


typedef struct Player_Data_S
{
	int			health;
	float		max_speed;
	Uint8		sloshed;
}Player_Data;

/**
* @brief  maka da playa data but its 2D
* @return player data pointer or NULL on failure
*/
Player_Data* player_data_new();

/**
* @brief  maka da playa but its 2D
* @return player ent, or NULL on failure
*/
Entity* player_new();



void player_think(Entity* self);
void player_update(Entity* self);
void player_draw(Entity* self);
void player_free(void* data);

void draw_hud(Entity* self);
void check_world_bounds(Collider* self);

#endif