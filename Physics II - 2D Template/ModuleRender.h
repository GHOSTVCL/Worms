#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL_Rect.h"
#include "SDL2_ttf-2.0.18/include/SDL_ttf.h"

#define MAX_ACTIVE_TEXT 150

// Properties >> Linker >> Input >> Additional dependencies >> add "SDL2_ttf.lib"

#include <iostream>
using namespace std;

class ModuleRender : public Module
{
public:
	ModuleRender(Application* app, bool start_enabled = true);
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true);
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true);
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true);
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;

	// Render text
	bool TextDraw(const char* words, int x, int y, int red, int green, int blue, int alpha, int size);

	// ttf cleanup
	void ttfQuit();

public:
	SDL_Renderer* renderer;
	SDL_Rect camera;

	// Rendering text
	SDL_Surface* ttf_surface = nullptr;
	SDL_Texture* ttf_texture = nullptr;
	TTF_Font* ttf_font = nullptr;
};