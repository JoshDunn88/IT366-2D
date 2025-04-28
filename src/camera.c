#include "simple_logger.h"

#include "camera.h"

typedef struct Camera_S {
	GFC_Vector2D position; //position in world space of camera
	GFC_Vector2D size; //width and height of screen space
	GFC_Rect	 bounds; //bounds to keep camera in
	Uint8		bindCamera; //keep cam in bounds if true
}Camera;




static Camera camera = { 0 };

void camera_set_bounds(GFC_Rect bounds) {
	gfc_rect_copy(camera.bounds, bounds);
}

void camera_set_size(GFC_Vector2D size) {
	gfc_vector2d_copy(camera.size, size);
}

void camera_enable_binding(Uint8 toBind) {
	camera.bindCamera = toBind;
}

GFC_Vector2D camera_get_position() {
	return camera.position;
}

GFC_Vector2D camera_get_offset() {
	return gfc_vector2d(-camera.position.x, -camera.position.y);
}

void camera_apply_bounds() {
	if (camera.position.x + camera.size.x > camera.bounds.x + camera.bounds.w)
		camera.position.x = camera.bounds.x - camera.size.x;
	if (camera.position.x < camera.bounds.x)
		camera.position.x = camera.bounds.x;

	if (camera.position.y + camera.size.y > camera.bounds.y + camera.bounds.h)
		camera.position.y = camera.bounds.y - camera.size.y;
	if (camera.position.y < camera.bounds.y)
		camera.position.y = camera.bounds.y;
}
void camera_set_position(GFC_Vector2D position) {
	camera.position.x = position.x;
	camera.position.y = position.y;

	if (camera.bindCamera) {
		camera_apply_bounds();
	}
}

void camera_center_on(GFC_Vector2D target) {
	GFC_Vector2D position;
	//slog("target x: %f, target y: %f", target.x, target.y);
	//slog("size x: %f, size y: %f", camera.size.x, camera.size.y);
	position.x = target.x - (camera.size.x * 0.5f);
	position.y = target.y - (camera.size.y * 0.5f);
	//slog("position x: %f, position y: %f", position.x, position.y);
	camera_set_position(position);
	if (camera.bindCamera) camera_apply_bounds();
}