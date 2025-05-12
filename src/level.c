#include "simple_logger.h"

#include "level.h"


Level load_level_config_from_file(const char* filename) 
{
	
    SJson* config = sj_load(filename);
    SJson* current = NULL;
    SJson* enemylist, *objectlist, *listitem;
    Entity* enemy, * object;
    
    Level newlevel = { 0 };
    int i;

    //start level stuff like entity init etc? or separate function?



    if (!config) {
        slog("failed to load level from ");
        return newlevel;
    }
    current = sj_object_get_value(config, "world");
    if (current) {
        //meshfile = sj_object_get_value_as_string(current, "mesh");
       // environment = environment_new();
        //environment->position = gfc_vector3d(0, 0, 0);
        //environment->scale = gfc_vector3d(1, 1, 1);
        //environment->rotation = gfc_vector3d(0, 0, 0);
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
        if (newlevel.object_total == 0)
        {
            slog("no objects in this level");
        }
        else
        {
            for (i = 0; i < newlevel.object_total; i++)
            {
                listitem = sj_array_get_nth(objectlist, i);
                if (!listitem)continue;
                //enemy = enemy_new(); 
                // load enemy function instead of this?
                //sj_value_as_vector3d(sj_object_get_value(item, "spawn"), &predator->collider->position); 
                //collider_update(predator->collider); run bounds on collider?
                //slog("spawned new enemy at %f, %f, %f ", predator->position.x, predator->position.y, predator->position.z);
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
        if (newlevel.enemy_total == 0)
        {
            slog("no enemies in this level");
        }
        else
        {
            for (i = 0; i < newlevel.enemy_total; i++)
            {
                listitem = sj_array_get_nth(enemylist, i);
                if (!listitem)continue;
                //enemy = enemy_new(); 
                // load enemy function instead of this?
                //sj_value_as_vector3d(sj_object_get_value(item, "spawn"), &predator->collider->position); 
                //collider_update(predator->collider); run bounds on collider?
                //slog("spawned new enemy at %f, %f, %f ", predator->position.x, predator->position.y, predator->position.z);
            }
        }
    }
    else
        slog("failed to load enemy list");

    slog("finna free");
    sj_free(config);
    //sj_free(current); frees itself?
    //slog("finna free 2");
    //free(config); 
    slog("returning level");


	return newlevel;
}


void clear_level_data(Level currentlevel)
{
    if (currentlevel.level_background)
	    gf2d_sprite_delete(currentlevel.level_background);
}