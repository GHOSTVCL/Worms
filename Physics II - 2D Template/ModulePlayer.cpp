#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{


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

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->physics->player.fx = 1.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		App->physics->player.fx = 0.0f;
	}
	
	return UPDATE_CONTINUE;
}



