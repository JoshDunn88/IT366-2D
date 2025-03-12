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
    
    self->rotation = 0;
    self->scale = gfc_vector2d(5, 5);
    self->sprite = gf2d_sprite_load_image("images/ghoul.png");
    self->collider = rect_collider_new(gfc_vector2d(0, 0), gfc_vector2d_multiply(gfc_vector2d(self->sprite->frame_w, self->sprite->frame_h), self->scale));
    self->collider->position = gfc_vector2d(200, 100);
    self->collider->isTrigger = false;
    self->collider->layer = C_ENEMY;
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

    if (player->collider->position.x > self->collider->position.x)
        self->collider->position.x += 1;
    if (player->collider->position.x < self->collider->position.x)
        self->collider->position.x += -1;
    if (player->collider->position.y > self->collider->position.y)
        self->collider->position.y += 1;
    if (player->collider->position.y < self->collider->position.y)
        self->collider->position.y += -1;
    if (player->collider->position.y == self->collider->position.y && player->collider->position.x == self->collider->position.x) {
        self->alive = false;
        slog("killed enemy");
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