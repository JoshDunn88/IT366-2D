#include "simple_logger.h"

#include "enemy.h"


void enemy_think(Entity* self);
void enemy_update(Entity* self);
void enemy_free(void* data);


Enemy_Data* enemy_data_new() {
    Enemy_Data* data = malloc(sizeof(Enemy_Data));

    if (!data) {
        slog("failed to create enemy data");
        return NULL;
    }
    data->health = 100;
    return data;
}

Entity* enemy_new()
{
    Entity* self;
    self = entity_new();

    if (!self) {
        slog("failed to create enemy entity");
        return NULL;
    }

    //data
    self->type = E_ENEMY;
    self->position = gfc_vector2d(200, 500);
    self->rotation = 0;
    self->scale = gfc_vector2d(5, 5);
    self->sprite = gf2d_sprite_load_image("images/ghoul.png");
    self->color_shift = gfc_color(1, 0, 0, 1);
    //behavior
    self->think = enemy_think;
    self->update = enemy_update;
    //self->free = enemy_free;
    self->draw = NULL;
    //self->data = enemy_data_new();

    return self;
}

void enemy_think(Entity* self)
{

}
void enemy_update(Entity* self)
{
    if (!self) return;

    Entity* player = get_player();

    if (player->position.x > self->position.x)
        self->position.x += 1;
    if (player->position.x < self->position.x)
        self->position.x += -1;
    if (player->position.y > self->position.y)
        self->position.y += 1;
    if (player->position.y < self->position.y)
        self->position.y += -1;
    if (player->position.y == self->position.y && player->position.x == self->position.x) {
        self->alive = false;
        slog("killed enemy with ghastly power");
    }
        

    //slog("player velocity: %f, %f", self->velocity.x, self->velocity.y);
    //slog("player position: %f, %f", self->position.x, self->position.y);
    return;
}

void enemy_draw(Entity* self) {
    //nothin yet
}

void enemy_free(void* data)
{
    if (!data) return;
    slog("freeing enemy data");
    free(data);
}