#include "Application.h"
#include "ModuleWindow.h"
#include <sstream>

#define _CRT_SECURE_NO_WARNINGS

Application::Application()
{
	renderer = new ModuleRender(this);
	window = new ModuleWindow(this);
	textures = new ModuleTextures(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	player = new ModulePlayer(this);
	scene_intro = new ModuleSceneIntro(this);
	physics = new ModulePhysics(this);
	font = new ModuleFont(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(physics);
	AddModule(renderer);
	AddModule(textures);
	AddModule(input);
	AddModule(audio);
	AddModule(font);
	
	// Scenes
	AddModule(scene_intro);
	
	// Player
	AddModule(player);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if(item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}
	
	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PreUpdate();
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
  			ret = item->data->Update();
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PostUpdate();
		item = item->next;
	}



	//Timer FPS
	frameCount++;
	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();
	// Amount of ms took the last update
	dt = frameTime.ReadMSec();
	// Amount of frames during the last second
	lastSecFrameCount++;
	LOG("%f", lastSecFrameTime.ReadMSec());
	if (lastSecFrameTime.ReadMSec() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		// Average FPS for the whole game life
		averageFps = (averageFps + framesPerSecond) / 2;
	}
	
	// Shows the time measurements in the window title
	std::string auxFPS = std::to_string(averageFps);
	//static char title[256] = "Av.FPS: ";

	static char fps[8] = {'F', 'P', 'S', ':', ' '};
	for (int i = 5; i < 8; i++) {
		fps[i] = auxFPS[i - 5];
	}
	
	//_CRT_SECURE_NO_WARNINGS(fps, title);

	window->SetTitle(fps); //Print title

	float delay = float(maxFrameDuration) - dt;

	PerfTimer delayTimer = PerfTimer();
	delayTimer.Start();
	if (maxFrameDuration > 0 && delay > 0) {
		SDL_Delay(delay);
		LOG("We waited for %f milliseconds and the real delay is % f", delay, delayTimer.ReadMs());
		dt = maxFrameDuration;
	}
	else {
		//LOG("No wait");
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}