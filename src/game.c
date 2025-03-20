#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
//#include "gfc_input.h"
//#include "gf2d_sprite.h"

#include "player.h"
#include "enemy.h"
#include "font.h"
#include "world.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite* sprite, * mousesprite;
    Entity *player, *enemy, *floor, *wall;
    
    int mx,my;
    float mf = 0;
    GFC_Color mouseGFC_Color = gfc_color8(255,100,255,200);
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);

    gfc_input_init("config/input.cfg");

    //my inits
    entity_system_init(1024);
    font_init();

    //todo make world object and do cam shit in there
    camera_set_bounds(gfc_rect(0, 0, 5000, 2000));
    camera_set_size(gfc_vector2d(1200, 720));
    //camera_apply_bounds();
    camera_enable_binding(0);
    
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/wafflehouse.png");
    player = player_new();
    floor = world_new_obstacle();
    //wall = world_new_obstacle();
    //wall->collider = rect_collider_new(gfc_vector2d(0, 0), gfc_vector2d(50, 200)); //not how this shit works man 
    //wall->collider->position = gfc_vector2d(300,200);
    slog("player made");
    //enemy = enemy_new();
    slog("enemy made");
    mousesprite = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        gfc_input_update();
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        //slog("mouse position: %f, %f", mouseEnt->position.x, mouseEnt->position.y);
        //if (gfc_input_command_pressed("player_up")) { slog("goin up"); }
        entity_update_all();
        

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite, gfc_vector2d(0, 0));
            
            //stuff
            entity_draw_all();

            //UI elements last
            font_draw_text("press ESCAPE to quit\n is fairly neat", FS_medium, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
            draw_hud(player);

            gf2d_sprite_draw(
                mousesprite,
                gfc_vector2d(mx, my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame

        if (gfc_input_command_pressed("exit")) {done = 1;}
        
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    exit(0); //wasn't in here before
    return 0;
}
/*eol@eof*/
