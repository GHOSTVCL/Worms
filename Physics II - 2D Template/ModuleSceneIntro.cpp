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
	bg_texture = App->textures->Load("Assets/fons.png");
	Background = App->textures->Load("Assets/fondo.png");
	Ground = App->textures->Load("Assets/terra.png");
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
	App->renderer->Blit(bg_texture, 0, 10, NULL);

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
	Water_ = { 0,0,525,100 };

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{
	App->renderer->Blit(bg_texture, 0, 10, NULL);

	App->renderer->TextDraw("holaaaa", 30, 400, 255, 0, 0, 255, 15);
	App->renderer->TextDraw("holaaaa", 500, 750, 255, 0, 0, 255, 15);
	App->renderer->TextDraw("holaaaa", 0, 0, 255, 0, 0, 255, 15);
	App->renderer->TextDraw("holaaaa", 499, 800, 255, 0, 0, 100, 30);
	App->renderer->TextDraw("HOLAAA", 30, 400, 255, 0, 0, 255, 15);
	App->renderer->TextDraw("HOLAAA", 500, 750, 255, 0, 0, 255, 15);
	App->renderer->TextDraw("HOLAAA", 0, 0, 255, 0, 0, 255, 15);
	App->renderer->TextDraw("HOLAAA", 499, 800, 255, 0, 0, 100, 30);
	return UPDATE_CONTINUE;
}