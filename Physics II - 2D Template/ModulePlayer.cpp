#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	
	wintimer = -1;
	balltex = App->textures->Load("Assets/ball.png");

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
	
	

	//pos = b2Vec2(Abalas[i]->body->GetPosition());
	//App->renderer->DrawTexture(balltex, METERS_TO_PIXELS(App->physics->balls.at(0).x) - 5, METERS_TO_PIXELS(App->physics->balls.at(0).y) - 5);
	App->renderer->DrawTexture(balltex, App->physics->balls.at(0).x - 5, App->physics->balls.at(0).y - 5);

	if (App->physics->balls.at(0).shot == false) {
		App->physics->balls.at(0).x = App->physics->players.at(0).x+1;
		App->physics->balls.at(0).y = App->physics->players.at(0).y+1;
	}
	
	if (score == true)
	{
		wintimer = 800;
		App->physics->balls.at(0).x = 100;
		App->physics->balls.at(0).y = 100;
		
	}
	if (wintimer == 0) {
		win = true;
	}
	wintimer--;
	return UPDATE_CONTINUE;
}



