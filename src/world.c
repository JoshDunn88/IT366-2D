#include "simple_logger.h"


#include "world.h"

//change this to take position and dimensions later
Entity* world_new_static_object() {
    Entity* self;
    self = entity_new();

    if (!self) {
        slog("failed to create enemy entity");
        return NULL;
    }

    //data
    self->type = E_DEFAULT;

    self->rotation = 0;
    self->scale = gfc_vector2d(1, 1);
    self->sprite = gf2d_sprite_load_image("images/100x100_rect.png");

    //self->collider = rect_collider_new(gfc_vector2d(0, 0), gfc_vector2d(150, 50)); //very dumb make this a parameter for better cleanup
    //self->collider->position = gfc_vector2d(300, 550);
    //self->collider->isTrigger = false;
    //self->collider->layer = C_WORLD;

    self->body = rect_body_new(gfc_vector2d(0, 0), gfc_vector2d(100, 100));
    self->body->dynamic = false;
    self->body->layer = B_WORLD;

    self->color_shift = gfc_color(0, 0, 1, 1);
    //behavior
    //self->think = enemy_think;
    //self->update = enemy_update;
    //self->free = enemy_free;
    //self->draw = object_draw;
    //self->data = enemy_data_new();

    return self;
}

void object_draw(Entity* self) {

    GFC_Vector2D offset, position;
    position = gfc_vector2d(0, 0);
    offset = camera_get_offset();
    gfc_vector2d_add(position, self->position, offset);
    gf2d_draw_rect_filled(gfc_rect(position.x, position.y, self->body->shape.s.r.w, self->body->shape.s.r.h), self->color_shift);

}