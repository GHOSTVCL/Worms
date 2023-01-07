#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleFont.h"
#include "ModulePlayer.h"
#include <iostream>
using namespace std;
#include <sstream>



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	scoreFont = App->font->Load("Assets/rtype_font3.png", lookupTable, 2);

	//Textures
	Background = App->textures->Load("Assets/fons.png");
	Ground = App->textures->Load("Assets/terra.png");
	Water = App->textures->Load("Assets/Water.png");
	Basket = App->textures->Load("Assets/ring.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	Background_ = { 0,0,1024,758 };
	App->renderer->Blit(Background, 0, 0, &Background_);
	Ground_ = { 0,0,500,160 };
	App->renderer->Blit(Ground, 0, 598, &Ground_);
	Ground_2 = { 0,0,400,60 };
	App->renderer->Blit(Ground, 0, 358, &Ground_2);
	Platform = { 0,0,300,40 };
	App->renderer->Blit(Ground, 660, 538, &Platform);
	Water_ = { 0,0,525,100 };
	App->renderer->Blit(Water, 500, 658, &Water_);
	Basket_ = { 0,0,75,98 };
	App->renderer->Blit(Basket, 300, 260, &Basket_);

	App->font->BlitText(58, 248, scoreFont, scoreText);

	string puntuacion = std::to_string(Score);
	const char* scorechar = puntuacion.c_str();

	/*string vidas = std::to_string(lifes);
	const char* lifeschar = vidas.c_str();*/

	if (App->player->movement == 1) {
		App->font->BlitText(10, 15, scoreFont, "m o v e m e n t   t y p e ;   v e l o c i t y");

	}
	if (App->player->movement == 2) {
		App->font->BlitText(10, 15, scoreFont, "m o v e m e n t   t y p e ;   p o s i t i o n");

	}
	if (App->player->movement == 3) {
		App->font->BlitText(10, 15, scoreFont, "m o v e m e n t   t y p e ;   f o r c e");

	}
	//App->font->BlitText(620, 480, scoreFont, lifeschar);
	//App->font->BlitText(620,15, scoreFont, scorechar);
	//App->font->BlitText(620,15, scoreFont, scorechar);

	return UPDATE_CONTINUE;
}
