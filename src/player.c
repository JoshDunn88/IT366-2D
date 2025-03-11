#include "simple_logger.h"

#include "player.h"


void player_think(Entity* self);
void player_update(Entity* self);
void player_free(void* data);


Player_Data* player_data_new() {
    Player_Data* data = malloc(sizeof(Player_Data));

    if (!data) {
        slog("failed to create player data");
        return NULL;
    }
    data->health = 100;
    return data;
}

Entity* player_new()
{
    Entity* self;
    self = entity_new();

    if (!self) {
        slog("failed to create player entity");
        return NULL;
    }
 
    //data
    self->type = E_PLAYER;
    self->position = gfc_vector2d(500, 300);
    self->rotation = 0;
    self->scale = gfc_vector2d(5, 5);
    self->sprite = gf2d_sprite_load_image("images/ghoul.png");
    self->color_shift = gfc_color(1,1,1,1);
    //behavior
    self->think = player_think;
    self->update = player_update;
    self->free = player_free;
    self->draw = NULL;
    self->data = player_data_new();

    return self;
}

void player_think(Entity* self)
{
    
}
void player_update(Entity* self)
{
    
    if (!self) return;

    if (gfc_input_command_down("player_left"))
    {
        //slog("player moved left");
        self->flip.x = 1;
        self->position.x += -5;
    }
    else if (gfc_input_command_released("player_left"))
    {
        //slog("player released left");
        self->position.x += 0;
    }

    if (gfc_input_command_down("player_right"))
    {
        self->flip.x =0;
        self->position.x += 5;
    }
    else if (gfc_input_command_released("player_right"))
    {
        self->position.x += 0;
    }

    if (gfc_input_command_down("player_up"))
    {
        self->position.y += -5;
    }
    else if (gfc_input_command_released("player_up"))
    {
        self->position.y += 0;
    }

    if (gfc_input_command_down("player_down"))
    {
        self->position.y += 5;
    }
    else if (gfc_input_command_released("player_down"))
    {
        self->position.y += 0;
    }

    //slog("player velocity: %f, %f", self->velocity.x, self->velocity.y);
    //slog("player position: %f, %f", self->position.x, self->position.y);
    return;
}

void player_draw(Entity* self) {
    //nothin yet
}

void player_free(void* data)
{
    if (!data) return;
    slog("freeing player data");
    free(data);
}
