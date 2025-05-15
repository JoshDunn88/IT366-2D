#include "simple_logger.h"
#include "body.h"


Body* body_setup(GFC_Shape shape)
{
}

Body* rect_collider_new(GFC_Vector2D position, GFC_Vector2D dimensions)
{
}

void body_free(Body* self)
{
}

Uint8 detect_collision(Body* self, Body* other, GFC_Vector2D* poc, GFC_Vector2D* normal)
{
}

void resolve_collision(Body* self, Body* other)
{
}

void body_update(Body* self)
{
}