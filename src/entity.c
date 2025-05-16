#include "simple_logger.h"
#include "entity.h"



typedef struct
{
	Entity* entityList;
	Uint32 entityMax;
}EntityManager;

static EntityManager _entity_manager = { 0 }; //initialize local global entity manager

void entity_system_close(void) {

	entity_clear_all(NULL);
	if (_entity_manager.entityList) free(_entity_manager.entityList);
	memset(&_entity_manager, 0, sizeof(EntityManager));
}

void entity_system_init(Uint32 maxEnts)
{
	if (!maxEnts)
	{
		slog("cannot allocate 0 entities");
	}
	if (_entity_manager.entityList) {
		slog("entity manager already exists");
		return;
	}
	_entity_manager.entityList = gfc_allocate_array(sizeof(Entity), maxEnts);
	if (!_entity_manager.entityList) {
		slog("failed to allocate %i entities for the entity manager", maxEnts);
	}
	_entity_manager.entityMax = maxEnts;
	atexit(entity_system_close);
}

void check_collisions(Collider* self) {
	if (!self) return;
	int i;
	//GFC_Vector2D self_sub_velocity;
	//GFC_Vector2D other_sub_velocity;
	Uint8 collided = 0;
	for (i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (!_entity_manager.entityList[i].alive) continue;
		if (!_entity_manager.entityList[i].collider) continue;
		if (_entity_manager.entityList[i].collider == self) continue; //dont do self
		
		collided = check_collision(self, _entity_manager.entityList[i].collider); //self_sub_velocity, other_sub_velocity
		if (!collided) continue;
		//else slog("collided at substep %i", it);
		//break;
	//}


		if (collided && !self->isTrigger) { //change this to be trigger function of enemy ghost
			//die
			//if (self->layer == C_ENEMY && _entity_manager.entityList[i].collider->layer == C_PLAYER) {
				//_entity_manager.entityList[i].alive = 0;
			//}
			do_collision(self, _entity_manager.entityList[i].collider); //do you need to do this in reverse also??
			continue;
		}
		//redo this horrendous shit later
		if (!self->isTrigger) continue;

		if (collided) {
			if (!self->triggerActive) {
				slog("about to trigger");
				self->onTriggerEnter(self, _entity_manager.entityList[i].collider);
				self->triggerActive = 1;
				continue;
			}
			self->whileTrigger(self, _entity_manager.entityList[i].collider);
		}
		else {
			//slog("no more trigger");
			if (self->triggerActive) {
				self->onTriggerExit(self, _entity_manager.entityList[i].collider);
				self->triggerActive = 0;
				continue;
			}
		}
	}
}

void detect_collisions(Body* self) {
	if (!self) return;
	int i;
	Uint8 collided = 0;
	Uint8 collided_once = 0;
	for (i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (!_entity_manager.entityList[i].alive) continue;
		if (!_entity_manager.entityList[i].body) continue;
		if (_entity_manager.entityList[i].body == self) continue; //skip self

		collided = detect_collision(self, _entity_manager.entityList[i].body, NULL, NULL); //self_sub_velocity, other_sub_velocity
		if (!collided) {
			continue;
		}
		else {
			collided_once = true;
		}
		//do stuff?
		
	}
	if (collided_once) 
	{
		entity_get_by_body(self)->color_shift = gfc_color8(0, 255, 0, 255);

	}
	else
	{
		entity_get_by_body(self)->color_shift = gfc_color8(255, 255, 255, 255);
	}

}

Entity* entity_get_by_collider(Collider* self) {
	if (!self) return NULL;
	int i;
	for (i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (_entity_manager.entityList[i].collider == self) return &_entity_manager.entityList[i];

	}
	return NULL;
}

Entity* entity_get_by_body(Body* self) {
	if (!self) return NULL;
	int i;
	for (i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (_entity_manager.entityList[i].body == self) return &_entity_manager.entityList[i];

	}
	return NULL;
}

void entity_clear_all(Entity* ignore)
{
	if (!_entity_manager.entityList)
	{
		slog("no entity list to clear");
		return;
	}

	int i;
	for (i = 0; i < _entity_manager.entityMax; i++) {
		if (ignore && &_entity_manager.entityList[i] == ignore) continue;
		_entity_manager.entityList[i]._inuse = false;

		slog("about to free an entity");
		entity_free(&_entity_manager.entityList[i]);

	}
}

void entity_think_all()
{
	if (!_entity_manager.entityList)
	{
		slog("no entity list to think");
		return;
	}

	int i;
	for (int i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (!_entity_manager.entityList[i].alive) continue;
		entity_think(&_entity_manager.entityList[i]);
	}
}


void entity_update_all()
{
	if (!_entity_manager.entityList)
	{
		slog("no entity list to update");
		return;
	}

	int i;
	for (int i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (!_entity_manager.entityList[i].alive) continue;
		entity_update(&_entity_manager.entityList[i]);
	}
}

void entity_check_all()
{
	if (!_entity_manager.entityList)
	{
		slog("no entity list to check");
		return;
	}

	int i;
	for (int i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		slog("ent in slot %i", i);
	}
}

void entity_draw_all()
{
	if (!_entity_manager.entityList)
	{
		slog("no entity list to draw");
		return;
	}

	int i;
	for (i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (!_entity_manager.entityList[i].alive) continue;
		//do draw
		entity_draw(&_entity_manager.entityList[i]);
	}
	//draw entity ui like bounds or health bar after the sprite draw
	for (i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (!_entity_manager.entityList[i].alive) continue;
		//do draw
		entity_draw_ui(&_entity_manager.entityList[i]);
	}



}

Entity* entity_new()
{
	int i;
	for (i = 0; i < _entity_manager.entityMax; i++)
	{
		if (_entity_manager.entityList[i]._inuse) continue;//skip ones in use
		memset(&_entity_manager.entityList[i], 0, sizeof(Entity));
		_entity_manager.entityList[i]._inuse = 1;
		_entity_manager.entityList[i].scale = gfc_vector2d(1, 1);
		_entity_manager.entityList[i].center = gfc_vector2d(0, 0);
		_entity_manager.entityList[i].color_shift = gfc_color(1, 1, 1, 1);
		_entity_manager.entityList[i].collider = NULL;
		_entity_manager.entityList[i].body = NULL;
		_entity_manager.entityList[i].alive = 1;
		_entity_manager.entityList[i].frame = 0;
		_entity_manager.entityList[i].type = E_DEFAULT;
		slog("new entity success");
		return &_entity_manager.entityList[i];
	}
	slog("no more open entity slots");
	return NULL; //no more entity slots

}

void entity_free(Entity* self)
{
	if (!self) return;
	self->_inuse = false; //double check
	if (self->sprite) {
		gf2d_sprite_free(self->sprite);
		self->sprite = NULL; //forgot to do this before and safe to say it caused some problems rip
		slog("sprite freed");
	}
	if (self->collider) {
		free(self->collider);
		self->collider = NULL;
		slog("collider freed");
	}
	if (self->body) {
		body_free(self->body); //free body allocated data first
		free(self->body);
		self->body = NULL;
		slog("collider freed");
	}
	//free anything special that may have been allocated FOR this
	if (self->free) {
		self->free(self->data);
		self->data = NULL;
	}
	self->sprite = NULL;
	self->update = NULL;
	self->think = NULL;
	self->data = NULL;
	self->draw = NULL;
	self->free = NULL;
	slog("freed dat ent");
}

void entity_think(Entity* self)
{
	if (!self) return;
	//basic think stuff here before self think
	//methink
	if (self->think) self->think(self);
}

void entity_update(Entity* self)
{
	if (!self) return;
	//basic update stuff here before self update
	// physics etc.
	//gfc_vector2d_add(self->position, self->position, self->velocity);
	if (self->collider) {
		check_collisions(self->collider);
		collider_update(self->collider);
		//set entity pos to collider pos
		gfc_vector2d_scale(self->position, self->collider->position, 1);
	}
	else if (self->body){ ///todo fix dis
		//check_collisions(self->collider);
		//slog("about to update body");
		detect_collisions(self->body);
		body_update(self->body);
		//set entity pos to collider pos
		gfc_vector2d_scale(self->position, self->body->position, 1);
		self->rotation = self->body->rotation;
	}
	//meupdate
	if (self->update) self->update(self);
}

void entity_draw(Entity* self)
{
	if (!self) return;
	if (self->draw) {
		self->draw(self);
		return;
	}

	GFC_Vector2D offset, position;
	position = gfc_vector2d(0, 0);
	offset = camera_get_offset();
	gfc_vector2d_add(position, self->position, offset);

	float rot = self->rotation * 180 / GFC_PI;
	gf2d_sprite_draw(
		self->sprite,
		position,
		&self->scale,
		&self->center,
		&rot,
		&self->flip,
		&self->color_shift,
		self->frame
	);
	//draw bounding box

}

void entity_draw_ui(Entity* self) {
	if (!self) return;

	GFC_Vector2D offset, position;
	offset = camera_get_offset();
	gfc_vector2d_add(position, self->position, offset);

	if (self->collider) {
		if (self->collider->layer == C_WORLD)
			gf2d_draw_shape(self->collider->shape, gfc_color8(0, 255, 0, 200), position);
		else
			gf2d_draw_shape(self->collider->shape, gfc_color8(0, 255, 0, 200), position);
	}

	if (self->body) {
		GFC_Color line_color = gfc_color8(100, 255, 0, 250);
		//this sucks, should really just make my own polygon struct later
		if (self->body->shape.type == ST_RECT) 
		{
			GFC_Vector2D p1 = {0}, p2 = { 0 }, p3 = { 0 }, p4 = { 0 };
			p1 = position;
			gfc_vector2d_add(p2, position, gfc_vector2d(self->body->shape.s.r.w, 0));
			gfc_vector2d_add(p3, position, gfc_vector2d(self->body->shape.s.r.w, self->body->shape.s.r.h));
			gfc_vector2d_add(p4, position, gfc_vector2d(0, self->body->shape.s.r.h));

			p2 = gfc_vector2d_rotate_around_center(p2, self->rotation, p1);
			p3 = gfc_vector2d_rotate_around_center(p3, self->rotation, p1);
			p4 = gfc_vector2d_rotate_around_center(p4, self->rotation, p1);
			//slog("p1 at %f, %f", p1.x, p1.y);
			gf2d_draw_line(p1,p2, line_color);
			gf2d_draw_line(p2,p3, line_color);
			gf2d_draw_line(p3,p4, line_color);
			gf2d_draw_line(p1,p4, line_color);
		}
			//gf2d_draw_shape(self->body->shape, gfc_color8(0, 255, 0, 200), position);
	}
	//might have to check for circle type because of shape offset
}


Entity* get_player()
{
	int i;
	for (int i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (_entity_manager.entityList[i].type == E_PLAYER)
			return &_entity_manager.entityList[i];
	}
	return NULL;
}