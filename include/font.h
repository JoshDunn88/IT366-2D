#ifndef __FONT_H__
#define __FONT_H__

#include <SDL_ttf.h>

#include "gfc_list.h"
#include "gfc_text.h"
#include "gfc_color.h"

#include "gf2d_graphics.h"

//#include <SDL.h>

typedef enum {
	FS_small,
	FS_medium,
	FS_large,
	FS_huge,
	FS_enormous,
	FS_astronomical,
	FS_MAX
}FontStyle;


/*
 * @brief initializes font management system and queues cleanup on exit
*/
void font_init();

/*
 * @brief render text to screen
 * @param text to render
 * @param font style
 * @param color
 * @param where to draw
*/
void font_draw_text(const char* text, FontStyle style, GFC_Color color, GFC_Vector2D position);

/*
 * @brief periodically call to cleanup any internal cache
*/
void font_cleanup();

#endif