#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	bool score;
	bool direction;
	bool win;
	bool lose;
	int wintimer;
	int integrator;
	int losetimer;
	int movement;
	
	uint loseSFX;

	SDL_Texture* balltex = nullptr;
	SDL_Texture* wintext = nullptr;
	SDL_Texture* losetext = nullptr;
	SDL_Texture* playertex = nullptr;

	
	
};