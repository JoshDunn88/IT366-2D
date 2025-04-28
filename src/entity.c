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
		//skip same layer collisions
		//if (self->layer == _entity_manager.entityList[i].collider->layer) return;
		//int it;
		//int iterations = 40;
		//for (it = 1; it <= iterations; it++) {
			//gfc_vector2d_scale(self_sub_velocity, self->velocity, (it / iterations));
			//gfc_vector2d_scale(other_sub_velocity, _entity_manager.entityList[i].collider->velocity, (it / iterations));
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

Entity* entity_get_by_collider(Collider* self) {
	if (!self) return NULL;
	int i;
	for (i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (_entity_manager.entityList[i].collider == self) return &_entity_manager.entityList[i];

	}
	return NULL;
}

void entity_clear_all(Entity* ignore)
{

	int i;
	for (i = 0; i < _entity_manager.entityMax; i++) {
		if (ignore && &_entity_manager.entityList[i] == ignore) continue;
		_entity_manager.entityList[i]._inuse = false;
		//this was wrong?
		//do specific free
		entity_free(&_entity_manager.entityList[i]);

	}
}

void entity_think_all()
{
	int i;
	for (int i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (!_entity_manager.entityList[i].alive) continue;
		entity_think(&_entity_manager.entityList[i]);
	}
}


void entity_update_all()
{
	int i;
	for (int i = 0; i < _entity_manager.entityMax; i++) {
		if (!_entity_manager.entityList[i]._inuse) continue;
		if (!_entity_manager.entityList[i].alive) continue;
		entity_update(&_entity_manager.entityList[i]);
	}
}

void entity_draw_all()
{
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
		_entity_manager.entityList[i].collider = NULL;
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
	if (self->sprite) {
		gf2d_sprite_free(self->sprite);
		slog("sprite freed");
	}

	if (self->collider) {
		free(self->collider);
		slog("collider freed");
	}
	//free anything special that may have been allocated FOR this
	if (self->free) {
		self->free(self->data);
		slog("data freed");
	}
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