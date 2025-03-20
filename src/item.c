#include "simple_logger.h"


#include "item.h"


void item_think(Entity* self);
void item_update(Entity* self);
void item_free(void* data);



Item_Data* item_data_new() {
    Item_Data* data = malloc(sizeof(Item_Data));

    if (!data) {
        slog("failed to create item data");
        return NULL;
    }
    data->item_type = I_DRINK;  
    return data;
}

Entity* item_new()
{
    Entity* self;
    self = entity_new();

    if (!self) {
        slog("failed to create item entity");
        return NULL;
    }

    //data
    self->type = E_ITEM;
    self->sprite = gf2d_sprite_load_image("images/beverage.png");
    self->scale = gfc_vector2d(2, 2);
    self->collider = rect_collider_new(gfc_vector2d(0, 0), gfc_vector2d_multiply(gfc_vector2d(self->sprite->frame_w, self->sprite->frame_h), self->scale));
    self->collider->position = gfc_vector2d(1000, 400);
    self->collider->isTrigger = true;
    self->collider->layer = C_ITEM;
    self->collider->onTriggerEnter = item_triggered;
    self->rotation = 0;

    //self->sprite = gf2d_sprite_load_image("images/ghoul.png");
    self->color_shift = gfc_color(1, 1, 1, 1);
    //behavior
    //self->think = item_think;
    //self->update = item_update;
    self->free = item_free;
    //self->draw = item_draw;
    self->data = item_data_new();

    return self;
}

void item_think(Entity* self)
{
   // Player_Data* dat = (struct Player_Data*)(self->data);

}
void item_update(Entity* self)
{

}
void item_triggered( Collider* self, Collider* other)
{
    if (!self) return;
    if (!other) return;
    if (other->layer != C_PLAYER) return;

    Entity* self_ent = entity_get_by_collider(self);
    Entity* player_ent = entity_get_by_collider(other);

    Player_Data* pdat = (struct Player_Data*)(player_ent->data);
    Item_Data* idat = (struct Item_Data*)(self_ent->data);
    slog("item triggered");
    if (idat->item_type == I_DRINK) {
        pdat->sloshed = true;
        slog("tried to slosh player");
    }

    self_ent->alive = 0; //should free in future but I gotta figure out a queue or something
}
void item_draw(Entity* self) {

}


void item_free(void* data)
{
    if (!data) return;
    slog("freeing item data");
    free(data);
}