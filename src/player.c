#include "simple_logger.h"


#include "player.h"


void player_think(Entity* self);
void player_update(Entity* self);
void player_free(void* data);

float move_speed = 0;

Player_Data* player_data_new() {
    Player_Data* data = malloc(sizeof(Player_Data));

    if (!data) {
        slog("failed to create player data");
        return NULL;
    }
    data->health = 100;
    data->max_speed = 5;
    move_speed = 5; //idk why man ill better this later nvm I know why its for resets lol
    data->sloshed = false;
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
    self->color_shift = gfc_color(1, 1, 1, 1);
    //behavior
    self->think = player_think;
    self->update = player_update;
    self->free = player_free;
    self->draw = player_draw;
    self->data = player_data_new();

    return self;
}

void player_think(Entity* self)
{
    Player_Data* dat = (struct Player_Data*)(self->data);

    if (dat->sloshed) {
        move_speed = gfc_random() * dat->max_speed;
        slog("am sloshed");
    }
    else {
        move_speed = dat->max_speed;
    }
}
void player_update(Entity* self)
{

    if (!self) return;
    if (!self->collider) return;
    if (!self->data) return;
    //Player_Data* dat = (struct Player_Data*)(self->data);
    //float move_speed = dat->max_speed;
    //slog("left state %i", gfc_input_command_get_state("player_left"));
    //slog("right state %i", gfc_input_command_get_state("player_right"));
    //slog("down state %i", gfc_input_command_get_state("player_down"));
   // slog("up state %i", gfc_input_command_get_state("player_up"));
    if (gfc_input_command_down("player_left"))
    {
        //slog("player pressed left");
        self->flip.x = 1;
        self->collider->velocity.x = -move_speed;
    }
    if (gfc_input_command_released("player_left"))
    {
        //slog("player released left");
        self->collider->velocity.x = 0;
    }

    if (gfc_input_command_down("player_right"))
    {
        self->flip.x = 0;
        self->collider->velocity.x = move_speed;
    }
    if (gfc_input_command_released("player_right"))
    {
        self->collider->velocity.x = 0;
    }

    if (gfc_input_command_down("player_up"))
    {
        self->collider->velocity.y = -move_speed;
    }
    if (gfc_input_command_released("player_up"))
    {
        self->collider->velocity.y = 0;
    }

    if (gfc_input_command_down("player_down"))
    {
        self->collider->velocity.y = move_speed;
    }
    if (gfc_input_command_released("player_down"))
    {
        self->collider->velocity.y = 0;
    }
    if (gfc_input_command_pressed("jump"))
    {
        //slog("player velocity: %f, %f", self->velocity.x, self->velocity.y);
        slog("player position: %f, %f", self->position.x, self->position.y);
        slog("camera position: %f, %f", camera_get_position().x, camera_get_position().y);
    }

    //slog("player velocity: %f, %f", self->velocity.x, self->velocity.y);
    //slog("player position: %f, %f", self->position.x, self->position.y);

    check_world_bounds(self->collider); //todo add rect collision kinda done

    camera_center_on(self->position);
    return;
}

void player_draw(Entity* self) {

    GFC_Vector2D offset, position;
    position = gfc_vector2d(0, 0);
    offset = camera_get_offset();
    gfc_vector2d_add(position, self->position, offset);

    gf2d_sprite_draw(
        self->sprite,
        position,
        &self->scale,
        &self->center,
        &self->rotation,
        &self->flip,
        &self->color_shift,
        self->frame
    );
}

void draw_hud(Entity* self) {
    if (!self) return;
    char healthVal[10], speed[20];
    Player_Data* dat;
    dat = (struct Player_Data*)(self->data);

    snprintf(healthVal, sizeof(healthVal), "%i", dat->health);
    snprintf(speed, sizeof(speed), "%f", gfc_vector2d_magnitude(self->collider->velocity));

    //hud
    //slog("health %i, %i", dat->health, *healthVal);
    //slog("food %i, %i", dat->prey_eaten, *foodVal);
    //slog("speed %f, %f", gfc_vector3d_magnitude(self->collider->velocity), *speed);
    font_draw_text(healthVal, FS_large, GFC_COLOR_DARKGREEN, gfc_vector2d(400, 10));
    font_draw_text(speed, FS_large, GFC_COLOR_BLUE, gfc_vector2d(900, 10));
}
void player_free(void* data)
{
    if (!data) return;
    slog("freeing player data");
    free(data);
}

void check_world_bounds(Collider* self) {
    int left_bound = 0;
    int right_bound = 1200;
    int top_bound = 0;
    int bottom_bound = 720;
    if (!self) return;
    if (self->position.x + self->shape.s.r.w > right_bound) self->position.x = right_bound - self->shape.s.r.w - 1;
    if (self->position.x < left_bound) self->position.x = left_bound + 1;
    if (self->position.y + self->shape.s.r.h > bottom_bound) self->position.y = bottom_bound - self->shape.s.r.h - 1;
    if (self->position.y < top_bound) self->position.y = top_bound + 1;
}