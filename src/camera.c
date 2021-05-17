#include "simple_logger.h"

#include "gfc_input.h"
#include "gf2d_mouse.h"
#include "camera.h"

typedef struct
{
    Rect view;
    Rect bounds;
}Camera;
static Camera _camera = {0};

void camera_update_by_keys()
{
    if (gfc_input_command_down("cameradown"))
    {
        camera_move(vector2d(0,10));
    }
    if (gfc_input_command_down("cameraup"))
    {
        camera_move(vector2d(0,-10));
    }
    if (gfc_input_command_down("cameraleft"))
    {
        camera_move(vector2d(-10,0));
    }
    if (gfc_input_command_down("cameraright"))
    {
        camera_move(vector2d(10,0));
    }
}

void camera_set_dimensions(Sint32 x,Sint32 y,Uint32 w,Uint32 h)
{
    gf2d_rect_set(_camera.view,x,y,w,h);
}

Rect camera_get_bounds()
{
    return _camera.bounds;
}

Rect camera_get_dimensions()
{
    return _camera.view;
}

Vector2D camera_get_mouse_position()
{
    Vector2D mouse;
    mouse = gf2d_mouse_get_position();
    return vector2d(mouse.x + _camera.view.x,mouse.y + _camera.view.y);    
}

Vector2D camera_position_to_screen(Vector2D position)
{
    return vector2d(position.x - _camera.view.x,position.y - _camera.view.y);    
}

Vector2D camera_get_position()
{
    return vector2d(_camera.view.x,_camera.view.y);
}

Vector2D camera_get_offset()
{
    return vector2d(-_camera.view.x,-_camera.view.y);
}

void camera_set_bounds(Sint32 x,Sint32 y,Uint32 w,Uint32 h)
{
    gf2d_rect_set(_camera.bounds,x,y,w,h);
}

void camera_bind()
{
    if (_camera.view.x < _camera.bounds.x)_camera.view.x = _camera.bounds.x;
    if (_camera.view.x + _camera.view.w > _camera.bounds.x + _camera.bounds.w)_camera.view.x = _camera.bounds.x + _camera.bounds.w - _camera.view.w;
    if (_camera.view.y < _camera.bounds.y)_camera.view.y = _camera.bounds.y;
    if (_camera.view.y + _camera.view.h > _camera.bounds.y + _camera.bounds.h)_camera.view.y = _camera.bounds.y + _camera.bounds.h - _camera.view.h;
}

void camera_move(Vector2D v)
{
    vector2d_add(_camera.view,v,_camera.view);
}

void camera_set_position(Vector2D position)
{
    vector2d_copy(_camera.view,position);
}

void camera_set_position_absolute(Vector2D position)
{
    vector2d_copy(_camera.view,position);
}

/*eol@eof*/
