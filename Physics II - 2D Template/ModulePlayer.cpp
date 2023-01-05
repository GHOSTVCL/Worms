#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include <windows.h>
#include <iostream>
ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	movement = 1;
	wintimer = -1;
	balltex = App->textures->Load("Assets/ball.png");
	wintext = App->textures->Load("Assets/winscreen.png");

	score = false;
	win = false;
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		if (movement == 1) {
			movement = 3;
		}
		else{
			movement--;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		if (movement == 3) {
			movement = 1;
		}
		else {
			movement++;
		}
	}
	std::cout << movement;
	//Position Movement
	if (movement == 2) {
		
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			App->physics->players.at(0).x = App->physics->players.at(0).x + 0.1;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			App->physics->players.at(0).x = App->physics->players.at(0).x - 0.1;

		}
		
	}
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		App->physics->debug = !App->physics->debug;
	}

	App->renderer->DrawTexture(balltex, (App->physics->balls.at(0).x - 5) * 20, -(App->physics->balls.at(0).y - 35) * 20);

	if (App->physics->balls.at(0).shot == false) {
		App->physics->balls.at(0).x = App->physics->players.at(0).x+1;
		App->physics->balls.at(0).y = App->physics->players.at(0).y+1;
	}
	
	if (score == true)
	{
		
		wintimer = 500;
		App->physics->balls.at(0).x = 100;
		App->physics->balls.at(0).y = 100;
		score = false;
		
	}
	if (wintimer == 0) {
		win = true;
	}
	if (win == true) {
		App->renderer->DrawTexture(wintext, 0,0);

	}
	wintimer--;
	return UPDATE_CONTINUE;
}



