#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
//#include "gfc_input.h"
//#include "gf2d_sprite.h"

#include "player.h"
#include "enemy.h"
#include "font.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite* sprite;
    Entity* mouseEnt, *player, *enemy;
    
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
    camera_set_bounds(gfc_rect(0, 0, 2560, 1440));
    camera_set_size(gfc_vector2d(1200, 720));
    //camera_apply_bounds();
    camera_enable_binding(0);
    
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/wafflehouse.png");
    mouseEnt = entity_new();
    player = player_new();
    slog("player made");
    enemy = enemy_new();
    slog("enemy made");
    mouseEnt->sprite = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        //SDL_PumpEvents();   // update SDL's internal event structures
        //keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        gfc_input_update();
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        mouseEnt->position = gfc_vector2d(mx, my);
        mouseEnt->color_shift = mouseGFC_Color;
        mouseEnt->frame = mf;
        //slog("mouse position: %f, %f", mouseEnt->position.x, mouseEnt->position.y);
        //if (gfc_input_command_pressed("player_up")) { slog("goin up"); }
        entity_update_all();
        

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,camera_get_offset());
            
            //stuff
            entity_draw_all();

            //UI elements last
            font_draw_text("press ESCAPE to quit\n is fairly neat", FS_medium, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
            draw_hud(player);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame

        if (gfc_input_command_pressed("exit")) {done = 1;}
        
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    exit(0); //wasn't in here before
    return 0;
}
/*eol@eof*/
