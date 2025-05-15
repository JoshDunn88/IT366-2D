#ifndef __BODY_H__
#define __BODY_H__

#include "gfc_types.h"
//#include "gfc_vector.h"
//#include "entity.h"
#include "gfc_shape.h"

typedef enum
{
	C_DEFAULT,
	C_PLAYER,
	C_ENEMY,
	C_ITEM,
	C_WORLD,
	C_MAX
}C_Layer;

//do I need collision struct?

typedef struct Body_S
{
	//data
	GFC_Shape		shape; //shape
	GFC_Vector2D	center_of_mass; //center of mass (offset from origin) maybe just make this a function?

	GFC_Vector2D	position; //top left position, or center if circle 
	float			rotation; //top left position, or center if circle 

	GFC_Vector2D	velocity; //how much to move per update
	GFC_Vector2D	acceleration; //how much to increment velocity per update

	C_Layer			layer; //collision layer
	Uint8			dynamic; //if body is dynamic or static


}Body;

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
Body* rect_collider_new(GFC_Vector2D position, GFC_Vector2D dimensions);

/*
 * @brief free collider
 * @param self this collider
*/
void body_free(Body* self);


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