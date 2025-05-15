#include "simple_logger.h"
#include "body.h"


Body* body_setup(GFC_Shape shape)
{
	Body* self = malloc(sizeof(Body));
	if (!self) {
		slog("could not allocate body");
		return NULL;
	}

	self->velocity = gfc_vector2d(0, 0);
	self->acceleration = gfc_vector2d(0, 0)
		;
	//change this to account for other polygons later
	self->shape = shape;

	if (shape.type == ST_RECT) {
		self->position = gfc_vector2d(shape.s.r.x, shape.s.r.y);
	}
	else if (shape.type == ST_CIRCLE) {
		self->position = gfc_vector2d(shape.s.c.x - shape.s.c.r, shape.s.c.y - shape.s.c.r); //keep top left as position
	}

	self->dynamic = true;
	self->center_of_mass = gfc_vector2d(0,0); //will need to be changed to center by default, as offset from position
	self->rotation = 0; 
	self->layer = B_DEFAULT; 

	return self;
}

Body* rect_body_new(GFC_Vector2D position, GFC_Vector2D dimensions)
{
	GFC_Shape shape = { 0 };
	shape.s.r = gfc_rect(position.x, position.y, dimensions.x, dimensions.y);
	shape.type = ST_RECT;
	return body_setup(shape);
}

Uint8 detect_collision(Body* self, Body* other, GFC_Vector2D* poc, GFC_Vector2D* normal)
{
}

void resolve_collision(Body* self, Body* other)
{
}

void body_update(Body* self)
{
	gfc_vector2d_add(self->position, self->position, self->velocity);
	gfc_vector2d_add(self->velocity, self->velocity, self->acceleration);
}