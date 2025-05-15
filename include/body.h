#ifndef __BODY_H__
#define __BODY_H__

#include "gfc_types.h"
//#include "gfc_vector.h"
//#include "entity.h"
#include "gfc_shape.h"

typedef enum
{
	B_DEFAULT,
	B_PLAYER,
	B_ENEMY,
	B_ITEM,
	B_WORLD,
	B_MAX
}B_Layer;

//do I need collision struct? yes for register_collision from ents who need it for taking damage and shit
typedef struct Collision_S
{
	GFC_Vector2D	poc; //point of contact, idk if world or relative yet
	GFC_Vector2D	normal; //point of contact, idk if world or relative yet

	float			force; //magnitude of the force of the collision

	Uint8			registered; //flag for if registered by ent or not

}Collision;

typedef struct Body_S
{
	//data
	GFC_Shape		shape; //shape
	GFC_Vector2D	center_of_mass; //center of mass (offset from origin) maybe just make this a function?
	//add storing bounds scale for easier access
	Collision		last_collision; //info about most recent collision, set registered flag when ent uses it 

	GFC_Vector2D	position; //top left position,
	float			rotation; //rotation in radians
	float			mass;

	GFC_Vector2D	velocity; //how much to move per update
	GFC_Vector2D	acceleration; //how much to increment velocity per update

	B_Layer			layer; //collision layer
	Uint8			dynamic; //if body is dynamic or static


}Body;

//TODO: make body system like entity management, can't really do this until I have actual level editing really,
// then you could just draw colliders over the background image maybe, or something else idk


Body* body_setup(GFC_Shape shape);

/*
 * @brief create new collider for circles
 * @param origin of shape
 * @param radius of circle
 * @return the created body
*/
Body* circle_body_new(GFC_Vector2D position, float radius);


/*
 * @brief create new body for rects
 * @param origin of shape
 * @param dimensions
 * @return the created body
*/
Body* rect_body_new(GFC_Vector2D position, GFC_Vector2D dimensions);

/*
 * @brief free collider data
 * @param self this collider
*/
void body_free(Body* self); //not needed yet, no pointers


/*
 * @brief check collisions for this object and all others nearby
 * @param self this collider
*/
//void check_collisions(Collider* self); gonna do this in entity instead, might be needed here for static world bodies, or do thatin world?

/*
 * @brief test if this body colliding with other
 * @param this collider
 * @param other collider
 * @param poc pointer to fill
 * @param normal idk if this is needed yet
 * @return 1 if yes, 0 if no
*/
Uint8 detect_collision(Body* self, Body* other, GFC_Vector2D *poc, GFC_Vector2D* normal);

/*
 * @brief do collision effect for this and other
 * @param self this collider
 * @param other other collider
*/
void resolve_collision(Body* self, Body* other);

/*
 * @brief apply movement to collider
 * @param self this collider
 * @param other other collider
*/
void body_update(Body* self);




//TODO Raycast
//TODO sphere and rect collision





#endif