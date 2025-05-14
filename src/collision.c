#include "simple_logger.h"
#include "gfc_matrix.h"
#include "collision.h"


Collider* collider_setup(GFC_Shape shape) {
	Collider* self = malloc(sizeof(Collider));
	if (!self) {
		slog("could not allocate collider");
		return NULL;
	}

	self->velocity = gfc_vector2d(0, 0);
	//change this to account for other prim types later
	self->shape = shape;

	if (shape.type == ST_RECT) {
		self->position = gfc_vector2d(shape.s.r.x, shape.s.r.y);
	}
	else if (shape.type == ST_CIRCLE) {
		self->position = gfc_vector2d(shape.s.c.x - shape.s.c.r, shape.s.c.y - shape.s.c.r); //keep top left as position
	}

	//self->offset = gfc_vector3d(0, 0, 0);
	//self->gravity = 0;
	//self->scale = gfc_vector3d(1, 1, 1);
	self->isTrigger = 0;
	self->triggerActive = 0;

	return self;
}

Collider* circle_collider_new(GFC_Vector2D position, float radius) {
	GFC_Shape shape = { 0 };
	shape.s.c = gfc_circle(position.x, position.y, radius);
	shape.type = ST_CIRCLE;
	return collider_setup(shape);
}

Collider* rect_collider_new(GFC_Vector2D position, GFC_Vector2D dimensions) {
	GFC_Shape shape = { 0 };
	shape.s.r = gfc_rect(position.x, position.y, dimensions.x, dimensions.y);
	shape.type = ST_RECT;
	return collider_setup(shape);
}

void collider_free(Collider* self) {
	//expand when adding sector list and other stuff
}

Uint8 check_collision(Collider* self, Collider* other) { 
	if (!self || !other) return 0;
	if (self == other) {
		//slog("no thats me");
		return 0;
	}//do not collide with self

	if (self->shape.type == ST_RECT) { // && Layer!= LAYER
		if (other->shape.type == ST_RECT) {
			return predictive_rect_overlap(self->shape.s.r, other->shape.s.r, self->position, other->position); //account for offset
		}
		if (other->shape.type == ST_CIRCLE)
			return 0; //not implemented
	}

	if (self->shape.type == ST_CIRCLE) { // && Layer!= LAYER
		if (other->shape.type == ST_RECT) {
			return 0; //not implemented
		}
		if (other->shape.type == ST_CIRCLE)
			return predictive_circle_overlap(self->shape.s.c, other->shape.s.c, self->position, other->position); //todo make predictive
	}
	return 0;
}
void do_collision(Collider* self, Collider* other) {
	if (!self || !other) return;

	if (self->shape.type == ST_CIRCLE && other->shape.type == ST_CIRCLE) {

		GFC_Vector2D distance;
		GFC_Vector2D scaledDistance;
		gfc_vector2d_sub(distance, self->position, other->position);
		gfc_vector2d_normalize(&distance);
		//edit scale for collision force/elasticity
		//gfc_vector3d_scale(scaledDistance, distance, 0.035);

		//use layers to determine if any body is fixed
		if (self->layer != C_WORLD)
			gfc_vector2d_add(self->position, self->position, self->shape.s.c.r + other->shape.s.c.r - distance);

		//only do self maybe?
		if (other->layer != C_WORLD)
			gfc_vector2d_add(other->position, other->position, self->shape.s.c.r + other->shape.s.c.r - distance);

		return;
	}

	//THIS IS BOX 
	//the concept of minimum overlap came from https://youtu.be/oOEnWQZIePs?si=ZMblmzP0ep0f-bJT
	if (self->shape.type == ST_RECT && other->shape.type == ST_RECT) {
		GFC_Vector2D boxDistance = { 0 };
		float xOverlap, yOverlap;
		Uint8 left, above;

		float slop = 0.001f;

		gfc_vector2d_sub(boxDistance, self->position, other->position);

		if (boxDistance.x < 0) {
			xOverlap = self->shape.s.r.w - SDL_fabsf(boxDistance.x);
			left = true;
		}
		else {
			xOverlap = other->shape.s.r.w - SDL_fabsf(boxDistance.x);
			left = false;
		}

		if (boxDistance.y < 0) {
			yOverlap = self->shape.s.r.h - SDL_fabsf(boxDistance.y);
			above = true;
		}
		else {
			yOverlap = other->shape.s.r.h - SDL_fabsf(boxDistance.y);
			above = false;
		}

		slog("overlaps: x %f, y %f", xOverlap, yOverlap);
		float xDistRel = xOverlap / (self->shape.s.r.w);
		float yDistRel = yOverlap / (self->shape.s.r.h);
		//slog("dists: x %f, y %f", xDist, yDist);
		float* max;
		max = &xDistRel;
		if (yDistRel > *max) max = &yDistRel;
		//slog("max %f", *max);

		if (max == &yDistRel) {
			//slog("do x");
			if (self->layer != C_WORLD) {

				if (left)
					self->position.x -= xOverlap + slop;
				else
					self->position.x += xOverlap + slop;
				self->velocity.x /= 2;
			}
			return;
		}
		else if (max == &xDistRel) {
			//slog("do y");
			if (self->layer != C_WORLD) {

				if (above)
					self->position.y -= yOverlap + slop;
				else
					self->position.y += yOverlap + slop;

				self->velocity.y /= 2;
			}
			return;
		}
	}
}

Uint8 predictive_circle_overlap(GFC_Circle a, GFC_Circle b, GFC_Vector2D a_d, GFC_Vector2D b_d)
{
	GFC_Vector2D v;
	gfc_vector2d_set(v, (a.x + a_d.x) - (b.x + b_d.x), (a.y + a_d.y) - (b.y + b_d.y));
	if (gfc_vector2d_magnitude_compare(v, a.r + b.r) <= 0)
	{
		return 1;
	}
	return 0;
}

Uint8 predictive_rect_overlap(GFC_Rect a, GFC_Rect b, GFC_Vector2D a_d, GFC_Vector2D b_d)
{
	if ((a.x + a_d.x > b.x + b_d.x + b.w) || (b.x + b_d.x > a.x + a_d.x + a.w) ||
		(a.y + a_d.y > b.y + b_d.y + b.h) || (b.y + b_d.y > a.y + a_d.y + a.h))
	{
		return 0;
	}
	return 1;
}

//collider will no longer have its own global position, just offset
void collider_update(Collider* self) {
	//update stuff
	gfc_vector2d_add(self->position, self->position, self->velocity);
}

void set_as_trigger(Collider* self, Uint8 toBeTrigger) {
	if (!self) return;
	self->isTrigger = toBeTrigger;
}