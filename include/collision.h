#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "gfc_types.h"
//#include "gfc_vector.h"
//#include "entity.h"
#include "gfc_shape.h"

typedef enum
{
	C_DEFAULT,
	C_PLAYER,
	C_ENEMY,
	C_WORLD
}C_Layer;

typedef struct Collider_S
{
	//data
	GFC_Shape		shape; //shape
	GFC_Vector2D	velocity; //how much to move per update
	GFC_Vector2D	position; //center position 

	//GFC_Vector3D	scale; // hmmm just do this with the shape itself
	//GFC_Vector3D	offset; //same as above prolly
	//float			gravity; should be world var?

	C_Layer			layer;
	Uint8			isTrigger;	//collide or trigger activation
	Uint8			triggerActive;	//is something in trigger

	//behavior
	void (*onTriggerEnter) (struct Entity* self, struct Entity* other); //called when another collider enters the trigger
	void (*onTriggerExit) (struct Collider_S* self, struct Collider_S* other); //called when another collider exits the trigger probably don't need this, might for hiding bushes though
	void (*whileTrigger) (struct Collider_S* self, struct Collider_S* other); //called while in trigger after entry before exit

}Collider;

Collider* collider_setup(GFC_Shape shape);

/*
 * @brief create new collider for spheres and planes
 * @param type type of primitive to set
 * @param position origin of primitive
 * @param parameter radius or distance
 * @return self this collider
*/
Collider* circle_collider_new(GFC_Vector2D position, float radius);


/*
 * @brief create new collider for boxes
 * @param type type of primitive to set
 * @param position origin of primitive
 * @param position2 2nd position or dimensions
 * @return self this collider
*/
Collider* rect_collider_new(GFC_Vector2D position, GFC_Vector2D dimensions);

/*
 * @brief free collider
 * @param self this collider
*/
void collider_free(Collider* self);

/*
 * @brief make collider trigger or not
 * @param self this collider
 * @param toBeTrigger trigger if 1 non-trigger if 0
*/
void set_as_trigger(Collider* self, Uint8 toBeTrigger);

/*
 * @brief check collisions for this object and all others nearby
 * @param self this collider
*/
//void check_collisions(Collider* self); gonna do this in entity instead

/*
 * @brief test if this object colliding with other
 * @param self this collider
 * @param other other collider
 * @return 1 if yes, 0 if no
*/
Uint8 check_collision(Collider* self, Collider* other);

/*
 * @brief do collision effect for this and other
 * @param self this collider
 * @param other other collider
*/
void do_collision(Collider* self, Collider* other);

/*
 * @brief apply movement to collider
 * @param self this collider
 * @param other other collider
*/
void collider_update(Collider* self);

/*
 * @brief check rect overlap with offset parameters
 * @param a rect 1
 * @param b rect 2
 * @param a_delta rect 1 delta
 * @param b_delta rect 2 delta
*/
Uint8 predictive_rect_overlap(GFC_Rect a, GFC_Rect b, GFC_Vector2D a_delta, GFC_Vector2D b_delta);

/*
 * @brief check circle overlap with offset parameters
 * @param a circle 1
 * @param b circle 2
 * @param a_delta circle 1 delta
 * @param b_delta circle 2 delta
*/
Uint8 predictive_circle_overlap(GFC_Circle a, GFC_Circle b, GFC_Vector2D a_delta, GFC_Vector2D b_delta);


//TODO Raycast
//TODO sphere and box collision





#endif