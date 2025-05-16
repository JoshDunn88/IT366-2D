#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
//#include "gfc_input.h"
//#include "gf2d_sprite.h"




#include "game.h"

static GameManager _game_manager = { 0 }; //initialize local game manager

void game_init() 
{
    _game_manager.done = false;
    //_game_manager.level_data.level_background = NULL; //this may break

    /*program initializtion*/
    init_logger("gf2d.log", 0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0, 0, 0, 255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);

    gfc_input_init("config/input.cfg");

    //my inits
    entity_system_init(1024);
    font_init();

    
}

void menu_init() {
    slog("initializing menu");
    _game_manager.game_mode = G_MENU;
    _game_manager.menu_background = gf2d_sprite_load_image("images/backgrounds/menu_v1.png");
    SDL_ShowCursor(SDL_ENABLE);
}

void main_init() {
    slog("initializing main mode");
    _game_manager.game_mode = G_MAIN;
    Sprite* mousesprite;
    Entity* player;

    //moved entity init to game_init because used in sim mode too


    //todo make world object and do cam shit in there
    camera_set_bounds(gfc_rect(0, 0, 5000, 2000));
    camera_set_size(gfc_vector2d(1200, 720));
    //camera_apply_bounds();
    camera_enable_binding(0);

    /*demo setup*/
    player = player_new();
    //floor = world_new_obstacle();
    //bev = item_new();
    
   
    //wall = world_new_obstacle();
    //wall->collider = rect_collider_new(gfc_vector2d(0, 0), gfc_vector2d(50, 200)); //not how this shit works man 
    //wall->collider->position = gfc_vector2d(300,200);
    
    //enemy = enemy_new();
    

    //game manager setup
    _game_manager.game_player = player;
    _game_manager.level_data = load_level_config_from_file("config/level1.cfg");

    SDL_ShowCursor(SDL_DISABLE);
    slog("main game initialized");
}

void sim_init() 
{
    slog("initializing sim mode");
    _game_manager.game_mode = G_SIM;

    //reset camera in case switched from main game
    
    //todo make world object and do cam shit in there
    camera_set_bounds(gfc_rect(0, 0, 5000, 2000));
    camera_set_size(gfc_vector2d(1200, 720));
    //camera_apply_bounds();
    camera_enable_binding(0);
    camera_center_on(gfc_vector2d(600, 360));

    _game_manager.level_data = load_level_config_from_file("config/simulation.cfg");
    SDL_ShowCursor(SDL_ENABLE);
}

void game_update() 
{
    if (gfc_input_command_released("exit"))
    {
        if (_game_manager.game_mode == G_MENU) 
        {
            slog("quitting app");
            _game_manager.done = true;
        }
        else 
        {
            slog("quitting to menu");
            switch_mode(G_MENU);
            //do closing for ents and shit
        }
    }
    //if (gfc_input_command_released("three"))
    //{
        //debugging ent system shenanigans
    //    entity_check_all();
   // }
}
void menu_update() 
{
    if (gfc_input_command_released("one"))
    {
        slog("entering main game");
        switch_mode(G_MAIN);
    }
    else if (gfc_input_command_released("two"))
    {
        slog("entering sim mode");
        switch_mode(G_SIM);
    }
}

void main_update()
{
    entity_think_all();
    entity_update_all();
}

void sim_update() 
{
    //do sim stuff here

    //sim inputs
    if (gfc_input_command_released("one"))
    {
        int x, y;
        GFC_Vector2D spawnpos = { 0 };
        SDL_GetMouseState(&x, &y);
        spawnpos.x = (float) x;
        spawnpos.y = (float) y;
        slog("spawning rect body at %f, %f", spawnpos.x, spawnpos.y);
        Entity* newObj = world_new_static_object();
        newObj->body->position = spawnpos; 
        newObj->body->rotation = 0.25 * GFC_PI;
        body_rot_update(newObj->body); //update arrays here for testing
        //detect_collision(newObj->body, newObj->body, NULL, NULL);
    }

  
    entity_think_all();
    entity_update_all();
    //static bodies update maybe separate? world manager? space?
    //slog("we simmin fr fr");
    return;
}

void menu_draw() 
{
    gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        gf2d_sprite_draw_image(_game_manager.menu_background, gfc_vector2d(0, 0)); //backgrounds drawn first
        font_draw_text("press ESCAPE to quit", FS_medium, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
        font_draw_text("Bar Wars", FS_large, GFC_COLOR_YELLOW, gfc_vector2d(400, 300));
    gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
}
void main_draw()
{
    gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
        gf2d_sprite_draw_image(_game_manager.level_data.level_background, gfc_vector2d(0, 0));

        //stuff
        entity_draw_all();

        //UI elements last
        font_draw_text("press ESCAPE to quit\n is fairly neat", FS_medium, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
        draw_hud(_game_manager.game_player);
     gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
}

void sim_draw()
{
    int i = 0;
    float pos = 0;
    float dis = 100;
    gf2d_graphics_clear_screen();// clears drawing buffers
        gf2d_sprite_draw_image(_game_manager.level_data.level_background, gfc_vector2d(0, 0));
        
        //draw grid
        while (pos <= 1200)
        {
            pos = dis * i;
            gf2d_draw_line(gfc_vector2d(pos, 0), gfc_vector2d(pos, 720), GFC_COLOR_LIGHTRED);
            i++;
        }
        i = 0;
        pos = 0;
        while (pos <= 720)
        {
            pos = dis * i;
            gf2d_draw_line(gfc_vector2d(0, pos), gfc_vector2d(1200, pos), GFC_COLOR_LIGHTRED);
            i++;
        }

        //draw bodies and ents
        entity_draw_all();
        //draw UI
        font_draw_text("you are in sim mode", FS_medium, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
    gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
    return;
}

void change_level(const char* filename)
{
    clear_level_data(_game_manager.level_data);
    //save player data here, or don't if we just keep player existing
    entity_clear_all(_game_manager.game_player); //clear all except player
    _game_manager.level_data = load_level_config_from_file(filename);
    _game_manager.game_player->collider->position = _game_manager.level_data.player_spawn;
    slog("successfully changed level");
}

void switch_mode(G_Mode next_mode) 
{
    switch (next_mode)
    {
    case(G_MENU):
        slog("doing menu switch");
        entity_clear_all(NULL); //instead of close, keep ent system open while in menu
        //need to do bodies to if that becomes it's own system
        //if player exists (in main mode) then save player temp data to cfg? or only do this for level switch prolly
        slog("about to menu init");
        menu_init();
        break;
    case(G_MAIN):
        main_init();
        break;
    case(G_SIM):
        sim_init();
        break;
    default:
        slog("no valid game mode to switch to");
        //probably just exit if somehow no game mode
    }
}

int main(int argc, char* argv[])
{
    /*inits*/
    game_init();
    menu_init(); 

    while (!_game_manager.done)
    {
        //moved input reading back because it pained kehoe
        gfc_input_update();
        game_update();

        switch (_game_manager.game_mode) 
        {
            case(G_MENU):
                menu_update();
                menu_draw();
                break;
            case(G_MAIN):
                main_update();
                main_draw();
                break;
            case(G_SIM):
                sim_update();
                sim_draw();
                break;
            default:
                slog("somehow you have no game mode, goodbye");
                return 0;
        }
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    exit(0); //maybe not needed?
    return 0;
}

//mouse stuff for sim later
// //init 
//  Sprite* mousesprite;
   

    //revisit for sim mouse controls
    //int mx, my;
    //float mf = 0;
    //GFC_Color mouseGFC_Color = gfc_color8(255, 100, 255, 200);
// //mousesprite = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);
//update
//SDL_GetMouseState(&mx, &my);
        //mf += 0.1;
        //if (mf >= 16.0)mf = 0;
        //slog("mouse position: %f, %f", mx, my);

//draw
// gf2d_sprite_draw(
        //    mousesprite,
         //   gfc_vector2d(mx, my),
         //   NULL,
         //   NULL,
        //    NULL,
        //NULL,
         //   &mouseGFC_Color,
          //  (int)mf);

/*eol@eof*/