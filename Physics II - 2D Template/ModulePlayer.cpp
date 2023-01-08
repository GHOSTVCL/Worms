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
	integrator = 1;
	wintimer = -1;
	losetimer = -1;
	balltex = App->textures->Load("Assets/ball2.png"); //Ball2 es la textura de la bola 5 veces más pequeña (Mismo radio que el PhysBall)
	wintext = App->textures->Load("Assets/wintexture.png");
	losetext = App->textures->Load("Assets/losetexture.png");
	playertex = App->textures->Load("Assets/player2.png");
	loseSFX = App->audio->LoadFx("Assets/Audios/Lose.wav");
	

	direction = true;
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
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		direction = true; //Dreta
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		direction = false; //Esquerra
	}
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

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		integrator = 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		integrator = 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		integrator = 3;
	}

	if (App->physics->balls.at(0).shot == false) {
		if (direction == true) {
			App->physics->balls.at(0).x = App->physics->players.at(0).x + 1.5;
			App->physics->balls.at(0).y = App->physics->players.at(0).y + 3;
		}
		if (direction == false) {
			App->physics->balls.at(0).x = App->physics->players.at(0).x - 1.5;
			App->physics->balls.at(0).y = App->physics->players.at(0).y + 3;
		}
	}
	
	if (score == true)
	{
		
		wintimer = 450;
		App->physics->balls.at(0).x = 100;
		App->physics->balls.at(0).y = 100;
		score = false;
		
	}
	if (App->physics->balls.at(0).shot == true && losetimer < -1 && win == false) {
		losetimer = 900;
	}
	if (losetimer == 0) {
		lose = true;
		App->audio->PlayFx(loseSFX);
	}
	//lose image
	if (lose == true) {
		App->renderer->DrawTexture(losetext, 0, 0);
	}
	if (wintimer == 0) {
		win = true;
	}
	//win image
	if (win == true) {
		App->renderer->DrawTexture(wintext, 0, 0);

	}

	if (direction == true && win == false && lose == false) {
		App->renderer->DrawTexture(playertex, (App->physics->players.at(0).x - 1.1) * 20, -(App->physics->players.at(0).y - 35.3) * 20); //Draw player

	}
	if (direction == false && win == false && lose == false) {
		App->renderer->DrawTexture(playertex, (App->physics->players.at(0).x - 1.1) * 20, -(App->physics->players.at(0).y - 35.3) * 20); //Draw player
	}
	if (win == false && lose == false) {
		App->renderer->DrawTexture(balltex, (App->physics->balls.at(0).x - 0.5) * 20, -(App->physics->balls.at(0).y - 37.5) * 20); //Draw bola
	}

	wintimer--;
	losetimer--;
	return UPDATE_CONTINUE;
}



