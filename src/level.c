#include "simple_logger.h"

#include "level.h"


Level load_level_config_from_file(const char* filename) 
{
	
    SJson* config = sj_load(filename);
    SJson* current = NULL;
    SJson* itemlist, *enemylist, *objectlist, *listnth;
    Entity* enemy, *object, *item;
    
    Level newlevel = { 0 };
    int i, total;

    //start level stuff like entity init etc? or separate function?



    if (!config) {
        slog("failed to load level from ");
        return newlevel;
    }
    current = sj_object_get_value(config, "world");
    if (current) {
        //meshfile = sj_object_get_value_as_string(current, "mesh");
        //environment->scale = gfc_vector3d(1, 1, 1);
        //environment->rotation = gfc_vector3d(0, 0, 0);
        sj_object_get_value_as_float(current, "playerspawnx", &newlevel.player_spawn.x);
        sj_object_get_value_as_float(current, "playerspawny", &newlevel.player_spawn.y);

        newlevel.level_background = gf2d_sprite_load_image(sj_object_get_value_as_string(current, "background"));
        //sj_value_as_vector2d(sj_object_get_value(current, "player_spawn"), &newlevel.player_spawn); need to make own vector function?
    }
    else
        slog("did not find world");

    current = sj_object_get_value(config, "objects");
    if (current)
    {
        slog("about to get count");
        objectlist = sj_object_get_value(current, "objectlist");
        total = sj_array_count(objectlist);
        if (total == 0)
        {
            slog("no objects in this level");
        }
        else
        {
            for (i = 0; i < total; i++)
            {
                listnth = sj_array_get_nth(objectlist, i);
                if (!listnth)continue;
                object = world_new_static_object();
                sj_object_get_value_as_float(listnth, "spawnx", &object->body->position.x);
                sj_object_get_value_as_float(listnth, "spawny", &object->body->position.y);
                //object->scale = gfc_vector2d(1, 1);
                //load object type function instead of all this?
                
                slog("spawned new object at %f, %f ", object->body->position.x, object->body->position.y);
            }
        }
    }
    else
        slog("failed to load object list");

    current = sj_object_get_value(config, "enemies");
    if (current)
    {
        slog("about to get count");
        enemylist = sj_object_get_value(current, "enemylist");
        total = sj_array_count(enemylist);
        if (total == 0)
        {
            slog("no enemies in this level");
        }
        else
        {
            for (i = 0; i < total; i++)
            {
                listnth = sj_array_get_nth(enemylist, i);
                if (!listnth)continue;
                enemy = enemy_new();
                sj_object_get_value_as_float(listnth, "spawnx", &enemy->collider->position.x); //remember to change to body
                sj_object_get_value_as_float(listnth, "spawny", &enemy->collider->position.y);
                enemy->scale = gfc_vector2d(1, 1);
                //load enemy type function instead of all this?
                
                slog("spawned new enemy at %f, %f ", enemy->collider->position.x, enemy->collider->position.y);
            }
        }
    }
    else
        slog("failed to load enemy list");

    current = sj_object_get_value(config, "items");
    if (current)
    {
        slog("about to get count");
        itemlist = sj_object_get_value(current, "itemlist");
        total = sj_array_count(itemlist);
        if (total == 0)
        {
            slog("no items in this level");
        }
        else
        {
            for (i = 0; i < total; i++)
            {
                listnth = sj_array_get_nth(itemlist, i);
                if (!listnth)continue;
                item = item_new();
                sj_object_get_value_as_float(listnth, "spawnx", &item->collider->position.x);
                sj_object_get_value_as_float(listnth, "spawny", &item->collider->position.y);
                //item->scale = gfc_vector2d(1, 1);
                //load enemy type function instead of all this?
                // 
                slog("spawned new item at %f, %f ", item->collider->position.x, item->collider->position.y);
            }
        }
    }
    else
        slog("failed to load item list");

    slog("finna free level config");
    sj_free(config);
    //sj_free(current); frees itself? 

    slog("returning level");
	return newlevel;
}


void clear_level_data(Level currentlevel)
{
    if (currentlevel.level_background)
	    gf2d_sprite_delete(currentlevel.level_background);
    //entity_clear_all(NULL); need to feed 
}