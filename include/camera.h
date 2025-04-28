#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_vector.h"
#include "gfc_shape.h"

void camera_set_bounds(GFC_Rect bounds);


void camera_enable_binding(Uint8 toBind);


void camera_set_size(GFC_Vector2D bounds);
/*
 * @brief get cam pos in world space
 * @return pos
*/
GFC_Vector2D camera_get_position();

/*
 * @brief get cam offset from world space
 * @return offset
*/
GFC_Vector2D camera_get_offset();

/*
 * @brief keep camera in bounds
*/
void camera_apply_bounds();

void camera_set_position(GFC_Vector2D position);

void camera_center_on(GFC_Vector2D target);

#endif
