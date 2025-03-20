#ifndef __ITEM_H_
#define __ITEM_H_

//#include "gfc_input.h"

#include "font.h"
#include "entity.h"

typedef enum
{
	I_DEFAULT,
	I_DRINK,
	I_MEDICINE,
	I_MAX
}I_Type;

typedef struct Item_Data_S
{
	I_Type			item_type;

}Item_Data;

/**
* @brief  maka da item data but its 2D
* @return player data pointer or NULL on failure
*/
Item_Data* item_data_new();

/**
* @brief  maka da playa but its 2D
* @return player ent, or NULL on failure
*/
Entity* item_new();



void item_think(Entity* self);
void item_update(Entity* self);
void item_draw(Entity* self);
void item_free(void* data);

#endif