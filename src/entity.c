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

	gf2d_sprite_draw(
		self->sprite,
		self->position,
		&self->scale,
		&self->center,
		&self->rotation,
		&self->flip,
		&self->color_shift,
		self->frame
	);
	//draw bounding box

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