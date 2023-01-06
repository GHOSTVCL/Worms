#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"



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

	//Textures
	Background = App->textures->Load("Assets/Background.png");
	Ground = App->textures->Load("Assets/Ground.png");
	Water = App->textures->Load("Assets/Water.png");

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

	return UPDATE_CONTINUE;
}
