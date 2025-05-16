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
	self->acceleration = gfc_vector2d(0, 0);

	self->ang_velocity = 0;
	//change this to account for other polygons later
	self->shape = shape;

	if (shape.type == ST_RECT) {
		self->position = gfc_vector2d(shape.s.r.x, shape.s.r.y);
	}
	else if (shape.type == ST_CIRCLE) {
		self->position = gfc_vector2d(shape.s.c.x - shape.s.c.r, shape.s.c.y - shape.s.c.r); //keep top left as position
	}

	//initialize active polygon data to simplify future calculations
	if (shape.type == ST_RECT)
	{
		self->rot_points = gfc_allocate_array(sizeof(GFC_Vector2D), 4);
		self->rot_edges = gfc_allocate_array(sizeof(GFC_Edge2D), 4);
		self->rot_normals = gfc_allocate_array(sizeof(GFC_Vector2D), 4);

		//set rot values
		body_rot_update(self);
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
	Uint8 collided_axis = true;

	if (!self) {slog("no self body"); return false;}
	if (!other) {slog("no other body"); return false;}	
	if (!self->rot_points) { slog("no self points"); return false;}
	if (!self->rot_edges) { slog("no self edges"); return false; }
	if (!self->rot_normals) { slog("no self normals"); return false; }
	if (!other->rot_points) { slog("no other points"); return false; }
	if (!other->rot_edges) { slog("no other edges"); return false; }
	if (!other->rot_normals) { slog("no other normals"); return false; }

	//for now only rect to rect
	if (self->shape.type == ST_RECT && other->shape.type == ST_RECT)
	{
		//now project onto each normal
		int i;
		GFC_Vector2D axis = { 0 };
		//get normal count, this will eventually be derived from point count which will be part of polygon data
		int normal_count = 4;
		for (i = 0; i < normal_count; i++)
		{
			
			axis = self->rot_normals[i];
			//slog("checking axis: %f, %f", axis.x, axis.y);
			//slog("checking normal: %f, %f", self->rot_normals[i].x, self->rot_normals[i].y);
			collided_axis = check_axis(self, other, axis);

			if (!collided_axis)
			{
				return false;
			}

			//you need to do all 
			axis = other->rot_normals[i];
			//slog("checking other axis: %f, %f", axis.x, axis.y);
			collided_axis = check_axis(self, other, axis);
			if (!collided_axis)
			{
				return false;
			}
		}
		
	}
	//collision is true
	//do some other stuff for poc and normal
	// 
	//cleanup lists if made any

	return true;
}

Uint8 check_axis(Body* self, Body* other, GFC_Vector2D axis)
{
	float projected_s_p1 = gfc_vector2d_dot_product(self->rot_points[0], axis);
	float projected_s_p2 = gfc_vector2d_dot_product(self->rot_points[1], axis);
	float projected_s_p3 = gfc_vector2d_dot_product(self->rot_points[2], axis);
	float projected_s_p4 = gfc_vector2d_dot_product(self->rot_points[3], axis);

	float projected_o_p1 = gfc_vector2d_dot_product(other->rot_points[0], axis);
	float projected_o_p2 = gfc_vector2d_dot_product(other->rot_points[1], axis);
	float projected_o_p3 = gfc_vector2d_dot_product(other->rot_points[2], axis);
	float projected_o_p4 = gfc_vector2d_dot_product(other->rot_points[3], axis);

	//slog("projected s points: %f, %f, %f, %f", projected_s_p1, projected_s_p2, projected_s_p3, projected_s_p4);
	//slog("projected o points: %f, %f, %f, %f", projected_o_p1, projected_o_p2, projected_o_p3, projected_o_p4);

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

	//slog("min s point: %f, max s point %f", s_min, s_max);
	//slog("min o point: %f, max o point %f", o_min, o_max);

	if (s_min > o_max || s_max < o_min) //edge case for line fully inside another?
	{
		return false;
	}
	//slog("colliding axis");
	return true;
}

void resolve_collision(Body* self, Body* other)
{
}

void body_update(Body* self)
{
	if (!self) { slog("no self body"); return; }

	gfc_vector2d_add(self->position, self->position, self->velocity);
	gfc_vector2d_add(self->velocity, self->velocity, self->acceleration);

	self->rotation = self->rotation + self->ang_velocity;
	//todo angular acceleration
	
	//update rot values
	body_rot_update(self);
	
}

void body_rot_update(Body* self)
{
	if (!self) { slog("no self body"); return; }
	if (!self->rot_points) { slog("no self points"); return; }
	if (!self->rot_edges) { slog("no self edges"); return; }
	if (!self->rot_normals) { slog("no self normals"); return; }

	if (self->shape.type == ST_RECT)
	{
		self->rot_points[0] = self->position;
		gfc_vector2d_add(self->rot_points[1], self->rot_points[0], gfc_vector2d(self->shape.s.r.w, 0));
		gfc_vector2d_add(self->rot_points[2], self->rot_points[0], gfc_vector2d(self->shape.s.r.w, self->shape.s.r.h));
		gfc_vector2d_add(self->rot_points[3], self->rot_points[0], gfc_vector2d(0, self->shape.s.r.h));

		self->rot_points[1] = gfc_vector2d_rotate_around_center(self->rot_points[1], self->rotation, self->rot_points[0]);
		self->rot_points[2] = gfc_vector2d_rotate_around_center(self->rot_points[2], self->rotation, self->rot_points[0]);
		self->rot_points[3] = gfc_vector2d_rotate_around_center(self->rot_points[3], self->rotation, self->rot_points[0]);

		self->rot_edges[0].x1 = self->rot_points[0].x; self->rot_edges[0].y1 = self->rot_points[0].y;
		self->rot_edges[0].x2 = self->rot_points[1].x; self->rot_edges[0].y2 = self->rot_points[1].y;
		self->rot_edges[1].x1 = self->rot_points[1].x; self->rot_edges[1].y1 = self->rot_points[1].y;
		self->rot_edges[1].x2 = self->rot_points[2].x; self->rot_edges[1].y2 = self->rot_points[2].y;
		self->rot_edges[2].x1 = self->rot_points[2].x; self->rot_edges[2].y2 = self->rot_points[2].y;
		self->rot_edges[2].x2 = self->rot_points[3].x; self->rot_edges[2].y1 = self->rot_points[3].y;
		self->rot_edges[3].x1 = self->rot_points[3].x; self->rot_edges[3].y1 = self->rot_points[3].y;
		self->rot_edges[3].x2 = self->rot_points[0].x; self->rot_edges[3].y2 = self->rot_points[0].y;

		//slog("for edge: %f, %f to %f, %f", self->rot_edges[0].x1, self->rot_edges[0].y1, self->rot_edges[0].x2, self->rot_edges[0].y2);
		self->rot_normals[0] = get_edge_normal(self->rot_edges[0]);
		//slog("assigning normal: %f, %f", get_edge_normal(self->rot_edges[0]).x, get_edge_normal(self->rot_edges[0]).y);
		self->rot_normals[1] = get_edge_normal(self->rot_edges[1]);
		self->rot_normals[2] = get_edge_normal(self->rot_edges[2]);
		self->rot_normals[3] = get_edge_normal(self->rot_edges[3]);
	}

}

GFC_Vector2D get_edge_normal(GFC_Edge2D edge)
{
	GFC_Vector2D normal = { 0 };
	float dx = edge.x2 - edge.x1;
	float dy = edge.y2 - edge.y1;
	normal.x = -dy;
	normal.y = dx;
	//slog ("calculated normal as: %f, %f", normal.x, normal.y);
	return normal;
}
void body_free(Body* self)
{
	if (!self)
	{
		slog("no body to free data for");
		return;
	}
	if (self->rot_points) free(self->rot_points);
	if (self->rot_edges) free(self->rot_edges);
	if (self->rot_normals) free(self->rot_normals);
	self->rot_points = NULL;
	self->rot_edges = NULL;
	self->rot_normals = NULL;
}