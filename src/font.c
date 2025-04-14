#include "simple_logger.h"

#include "font.h"

typedef struct FontCache_S {
	char* text;
	FontStyle style;
	SDL_Texture* texture;
	GFC_Vector2D size;
	GFC_Color color;
	Uint32 timestamp;
}FontCache;

typedef struct FontManager_S{
	GFC_List* fonts;
	GFC_List* recents;
}FontManager;

static FontManager font_manager = { 0 };
void font_cache_free(FontCache* cache);


void font_close()
{
	//cleanup first
	TTF_Font* font;
	FontCache* cache;
	int i, c;
	c = gfc_list_get_count(font_manager.fonts);
	for (i = 0; i < c; i++) {
		font = gfc_list_get_nth(font_manager.fonts, i);
		if (!font) continue;
		TTF_CloseFont(font);
	}
	gfc_list_delete(font_manager.fonts);

	c = gfc_list_get_count(font_manager.recents);
	for (i = 0; i < c; i++) {
		cache = gfc_list_get_nth(font_manager.recents, i);
		if (!cache) continue;
		font_cache_free(cache);
	}
	gfc_list_delete(font_manager.recents);

	memset(&font_manager, 0, sizeof(FontManager));
	TTF_Quit();
}
void font_init() 
{
	TTF_Font* font;
	int i;
	if (TTF_Init() != 0) {
		slog("failed to initialize TTF system");
		return;
	}

	font_manager.fonts = gfc_list_new();
	font_manager.recents = gfc_list_new();
	//error slog
	for (i = 0; i < FS_MAX; i++) {
		font = TTF_OpenFont("fonts/Marlboro.ttf", 10 + i * 20);
		if (!font) {
			slog("failed to open font fonts/Marlboro.ttf");
			continue;
		}
		gfc_list_append(font_manager.fonts, font);
	}
	atexit(font_close);
}
void font_cache_free(FontCache* cache) {
	if (!cache) {
		slog("no cache");
		return;
	}

	if (cache->text) free(cache->text);
	if (cache->texture) SDL_DestroyTexture(cache->texture);
	free(cache);
}
void font_cleanup() {
	FontCache* cache;
	Uint32 now;
	now = SDL_GetTicks();

	int i, c;
	c = gfc_list_get_count(font_manager.recents);
	for (i = c - 1; i > -1; i--) { //baclwards because something with the lists deletion
		cache = gfc_list_get_nth(font_manager.recents, i);
		if (!cache) continue;
		if (cache->timestamp + 2000 < now) {

			font_cache_free(cache);
			gfc_list_delete_nth(font_manager.recents, i);

		}
	}
}
void font_add_recent(const char* text, FontStyle style, GFC_Color color, GFC_Vector2D size, SDL_Texture* texture) {
	FontCache* cache;
	size_t length;

	if (!text) {
		slog("no text bruh");
		return NULL;
	}
	if (!texture) {
		slog("no texture bruh");
		return NULL;
	}
	cache = gfc_allocate_array(sizeof(FontCache), 1);
	if (!cache) {
		slog("no cache bruh");
		return NULL;
	}

	cache->texture = texture;
	gfc_vector2d_copy(cache->size, size);
	cache->style = style;
	//cache->color = color;
	gfc_color_copy(cache->color, color);

	length = strlen(text) + 1;
	cache->text = gfc_allocate_array(sizeof(char), length);
	strncpy(cache->text, text, length);
	cache->timestamp = SDL_GetTicks();
	gfc_list_append(font_manager.recents, cache);
}

FontCache* font_get_recent(const char* text, FontStyle style, GFC_Color color) {
	FontCache* cache;
	if (!text) {
		return NULL;
	}

	int i, c;
	c = gfc_list_get_count(font_manager.recents);
	for (i = 0; i < c; i++) {
		cache = gfc_list_get_nth(font_manager.recents, i);
		if (!cache) continue;
		if (gfc_strlcmp(text, cache->text) != 0) continue;
		if (cache->style != style) continue;
		//should compare size here too no?
		if (!gfc_color_cmp(color, cache->color)) continue;
		
		return cache;
	}

	return NULL;
}
void font_draw_text(const char* text, FontStyle style, GFC_Color color, GFC_Vector2D position)
{
	TTF_Font* font;
	FontCache* cache;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;

	cache = font_get_recent(text, style, color);
	if (cache) {
		rect.x = position.x;
		rect.y = position.y;
		rect.w = cache->size.x;
		rect.h = cache->size.y;

		SDL_RenderCopy(gf2d_graphics_get_renderer(), cache->texture, NULL, &rect);

		cache->timestamp = SDL_GetTicks();
		//slog("draw text from cache");
		return;
	}

	//slog("draw text fresh");

	font = gfc_list_get_nth(font_manager.fonts, style);
	if (!font) {
		slog("failed to render text %s, missing font stlye %i", text, style);
		return;
	}

	surface = TTF_RenderUTF8_Blended_Wrapped(font, text, gfc_color_to_sdl(color), 0);
	if (!surface) {
		slog("failed to render font to surface");
		return;
	}
	surface = gf2d_graphics_screen_convert(&surface);
	if (!surface) {
		slog("failed to convert surface");
		return;
	}

	texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);
	if (!texture) {
		SDL_FreeSurface(surface);
		slog("failed to create texture from surface");
		return;
	}

	rect.x = position.x;
	rect.y = position.y;
	rect.w = surface->w;
	rect.h = surface->h;

	SDL_RenderCopy(gf2d_graphics_get_renderer(), texture, NULL, &rect);
	SDL_FreeSurface(surface);
	font_add_recent(text, style, color, gfc_vector2d(rect.w, rect.h), texture);
	
//	SDL_DestroyTexture(texture);
}