#include <SDL.h>
#include "simple_logger.h"

#include "gfc_shape.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "camera.h"
#include "level.h"
#include "path.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    Uint32 press;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    
    Level *level;
    Vector2D cam;
    List *path = NULL;
    SDL_Point search;
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    level_system_init();
    
    camera_set_size(vector2d(1200,720));
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    
    srand(SDL_GetTicks());  //for completelye random each time
    level = level_generate(100,50);
    level_generate_tile_layer(level);
    if (level->tileLayer)
    {
        camera_set_bounds(gfc_rect(0,0,level->tileLayer->frame_w,level->tileLayer->frame_h));
    }
    //test path finding
    if (level)
    {
        path = path_find(level->tileMap,
                         level->bounds.w,
                         level->bounds.h,
                         vector2d(level->start.x,level->start.y),
                         vector2d(level->end.x,level->end.y));
    }
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        press = SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        if (press)
        {
            search = level_get_tile_from_mouse_position(level);
            path = path_find(level->tileMap,
                            level->bounds.w,
                            level->bounds.h,
                            vector2d(level->start.x,level->start.y),
                            vector2d(search.x,search.y));
        }
        
        cam = camera_get_position();
        if ((mx <= 10)||(keys[SDL_SCANCODE_A]))
        {
            cam.x -= 5;
        }
        if ((mx >= 1190)||(keys[SDL_SCANCODE_D]))
        {
            cam.x += 5;
        }
        if ((my <= 10)||(keys[SDL_SCANCODE_W]))
        {
            cam.y -= 5;
        }
        if ((my >= 710)||(keys[SDL_SCANCODE_S]))
        {
            cam.y += 5;
        }
        camera_set_postition(cam);
        camera_apply_bounds();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            level_draw(level, camera_get_offset());
            if (path)path_draw(path,camera_get_offset());
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    level_free(level);//cleanup what you create
    path_free(path);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
