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
    self->sprite = gf2d_sprite_load_image("images/ghoul.png");
    self->scale = gfc_vector2d(5, 5);
    self->collider = rect_collider_new(gfc_vector2d(0, 0), gfc_vector2d_multiply(gfc_vector2d(self->sprite->frame_w, self->sprite->frame_h), self->scale));
    self->collider->position = gfc_vector2d(500, 300);
    self->collider->isTrigger = false;
    self->collider->layer = C_PLAYER;
    self->rotation = 0;
    
    //self->sprite = gf2d_sprite_load_image("images/ghoul.png");
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
        self->collider->position.x += -5;
    }
    else if (gfc_input_command_released("player_left"))
    {
        //slog("player released left");
        //self->collider->position.x += 0;
    }

    if (gfc_input_command_down("player_right"))
    {
        self->flip.x =0;
        self->collider->position.x += 5;
    }
    else if (gfc_input_command_released("player_right"))
    {
        //self->collider->position.x += 0;
    }

    if (gfc_input_command_down("player_up"))
    {
        self->collider->position.y += -5;
    }
    else if (gfc_input_command_released("player_up"))
    {
        //self->collider->position.y;
    }

    if (gfc_input_command_down("player_down"))
    {
        self->collider->position.y += 5;
    }
    else if (gfc_input_command_released("player_down"))
    {
        //self->collider->position.y;
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
