#ifndef __WORLD_H__
#define __WORLD_H__

#include "entity.h"


Entity* world_new_static_object();

Entity* world_new_object_old();

Entity* world_new_dynamic_object();

void object_draw(Entity* self);



#endif