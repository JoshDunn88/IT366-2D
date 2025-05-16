#include "simple_logger.h"
#include "body.h"


typedef struct
{
	Body* bodyList;
	Uint32 bodyMax;
}BodyManager;

static BodyManager _body_manager = { 0 }; //initialize local global body manager

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
	if (!self) {
		slog("no self body");
		return false;
	}
	if (!other) {
		slog("no other body");
		return false;
	}
	
	//rect detection only right now

	//get world coord edges, should be optimized to just exist as part of body that gets updated every body_update
	GFC_Vector2D s_p1 = { 0 }, s_p2 = { 0 }, s_p3 = { 0 }, s_p4 = { 0 };
	s_p1 = self->position;
	gfc_vector2d_add(s_p2, s_p1, gfc_vector2d(self->shape.s.r.w, 0));
	gfc_vector2d_add(s_p3, s_p1, gfc_vector2d(self->shape.s.r.w, self->shape.s.r.h));
	gfc_vector2d_add(s_p4, s_p1, gfc_vector2d(0, self->shape.s.r.h));

	s_p2 = gfc_vector2d_rotate_around_center(s_p2, self->rotation, s_p1);
	s_p3 = gfc_vector2d_rotate_around_center(s_p3, self->rotation, s_p1);
	s_p4 = gfc_vector2d_rotate_around_center(s_p4, self->rotation, s_p1);
	//checking all normals for now, can be optimized to just 2 since there's dupes maybe
	GFC_Vector2D s_edge1 = { 0 }, s_edge2 = { 0 }, s_edge3 = { 0 }, s_edge4 = { 0 };
	gfc_vector2d_sub(s_edge1, s_p2, s_p1);
	gfc_vector2d_sub(s_edge2, s_p3, s_p2);
	gfc_vector2d_sub(s_edge3, s_p4, s_p3);
	gfc_vector2d_sub(s_edge4, s_p1, s_p4);

	GFC_Vector2D s_normal1 = gfc_vector2d_get_normal(s_edge1);
	GFC_Vector2D s_normal2 = gfc_vector2d_get_normal(s_edge2);
	GFC_Vector2D s_normal3 = gfc_vector2d_get_normal(s_edge3);
	GFC_Vector2D s_normal4 = gfc_vector2d_get_normal(s_edge4);

	//now for the other body
	GFC_Vector2D o_p1 = { 0 }, o_p2 = { 0 }, o_p3 = { 0 }, o_p4 = { 0 };
	o_p1 = other->position;
	gfc_vector2d_add(o_p2, o_p1, gfc_vector2d(other->shape.s.r.w, 0));
	gfc_vector2d_add(o_p3, o_p1, gfc_vector2d(other->shape.s.r.w, other->shape.s.r.h));
	gfc_vector2d_add(o_p4, o_p1, gfc_vector2d(0, other->shape.s.r.h));

	o_p2 = gfc_vector2d_rotate_around_center(o_p2, other->rotation, o_p1);
	o_p3 = gfc_vector2d_rotate_around_center(o_p3, other->rotation, o_p1);
	o_p4 = gfc_vector2d_rotate_around_center(o_p4, other->rotation, o_p1);
	//checking all normals for now, can be optimized to just 2 since there's dupes maybe
	GFC_Vector2D o_edge1 = { 0 }, o_edge2 = { 0 }, o_edge3 = { 0 }, o_edge4 = { 0 };
	gfc_vector2d_sub(o_edge1, o_p2, o_p1);
	gfc_vector2d_sub(o_edge2, o_p3, o_p2);
	gfc_vector2d_sub(o_edge3, o_p4, o_p3);
	gfc_vector2d_sub(o_edge4, o_p1, o_p4);

	GFC_Vector2D o_normal1 = gfc_vector2d_get_normal(o_edge1);
	GFC_Vector2D o_normal2 = gfc_vector2d_get_normal(o_edge2);
	GFC_Vector2D o_normal3 = gfc_vector2d_get_normal(o_edge3);
	GFC_Vector2D o_normal4 = gfc_vector2d_get_normal(o_edge4);

	//now project onto each normal
	//test with x axis first
	GFC_Vector2D x_axis = gfc_vector2d(1, 0);
	float projected_s_p1 = gfc_vector2d_dot_product(s_p1, x_axis);
	float projected_s_p2 = gfc_vector2d_dot_product(s_p2, x_axis);
	float projected_s_p3 = gfc_vector2d_dot_product(s_p3, x_axis);
	float projected_s_p4 = gfc_vector2d_dot_product(s_p4, x_axis);

	float projected_o_p1 = gfc_vector2d_dot_product(o_p1, x_axis);
	float projected_o_p2 = gfc_vector2d_dot_product(o_p2, x_axis);
	float projected_o_p3 = gfc_vector2d_dot_product(o_p3, x_axis);
	float projected_o_p4 = gfc_vector2d_dot_product(o_p4, x_axis);

	slog("projected s points: %f, %f, %f, %f", projected_s_p1, projected_s_p2, projected_s_p3, projected_s_p4);
	slog("projected o points: %f, %f, %f, %f", projected_o_p1, projected_o_p2, projected_o_p3, projected_o_p4);

	//get mins and maxes
	float s_min = projected_s_p1;
	s_min = min(s_min, projected_s_p2);
	s_min = min(s_min, projected_s_p3);
	s_min = min(s_min, projected_s_p4);

	float s_max = projected_s_p1;
	s_max = max(s_max, projected_s_p2);
	s_max = max(s_max, projected_s_p3);
	s_max = max(s_max, projected_s_p4);

	float o_min = projected_o_p1;
	o_min = min(o_min, projected_o_p2);
	o_min = min(o_min, projected_o_p3);
	o_min = min(o_min, projected_o_p4);

	float o_max = projected_o_p1;
	o_max = max(o_max, projected_o_p2);
	o_max = max(o_max, projected_o_p3);
	o_max = max(o_max, projected_o_p4);

	slog("min s point: %f, max s point %f", s_min, s_max);
	slog("min o point: %f, max o point %f", o_min, o_max);

	return false;
}

void resolve_collision(Body* self, Body* other)
{
}

void body_update(Body* self)
{
	gfc_vector2d_add(self->position, self->position, self->velocity);
	gfc_vector2d_add(self->velocity, self->velocity, self->acceleration);
}