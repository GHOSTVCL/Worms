#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include <cmath>

// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	ScoreFX = App->audio->LoadFx("Assets/Audios/Score.wav");

	// Create ground
	ground = Ground();
	ground.x = 0.0f; // [m]
	ground.y = 0.0f; // [m]
	ground.w = 25.0f; // [m]
	ground.h = 8.0f; // [m]

	ground_2 = Ground();
	ground_2.x = 0.0f; // [m]
	ground_2.y = 17.0f; // [m]
	ground_2.w = 20.0f; // [m]
	ground_2.h = 3.0f; // [m]

	platform = Ground();
	platform.x = 33.0f; // [m]
	platform.y = 9.0f; // [m]
	platform.w = 15.0f; // [m]
	platform.h = 2.0f; // [m]

	basket = Ground();
	basket.x = 16.0f; // [m]
	basket.y = 20.7f; // [m]
	basket.w = 2.0f; // [m]
	basket.h = 3.5f; // [m]



	// Create Water
	water = Water();
	water.x = ground.x + ground.w; // Start where ground ends [m]
	water.y = 0.0f; // [m]
	water.w = 35.0f; // [m]
	water.h = 5.0f; // [m]
	water.density = 50.0f; // [kg/m^3]
	water.vx = 0.0f; // [m/s]
	water.vy = 0.0f; // [m/s]

	// Create atmosphere
	atmosphere = Atmosphere();
	atmosphere.windx = 0.1f; // [m/s]
	atmosphere.windy = 0.1f; // [m/s]
	atmosphere.density = 1.0f; // [kg/m^3]

	// Create a ball
	PhysBall ball = PhysBall();

	// Set static properties of the ball
	ball.mass = 10.0f; // [kg]
	ball.surface = 1.0f; // [m^2]
	ball.radius = 0.5f; // [m]
	ball.cd = 0.4f; // [-]
	ball.cl = 1.2f; // [-]
	ball.b = 10.0f; // [...]
	ball.coef_friction = 0.9f; // [-]
	ball.coef_restitution = 0.8f; // [-]

	// Set initial position and velocity of the ball
	ball.x = 5.0f;
	ball.y = (ground.y + ground.h) + 2.0f;
	ball.vx = 0.f;
	ball.vy = 0.f;
	ball.shot = false;
	ball.fvecx = 0;
	ball.fvecy = 0;
	ball.onair = false;
	// Add ball to the collection
	balls.emplace_back(ball);

	
	player.mass = 5.0f; // [kg]
	player.surface = 1.0f; // [m^2]
	player.radius = 1.5f; // [m]
	player.cd = 0.1f; // [-]
	player.cl = 0.1f; // [-]
	player.b = 0.1f; // [...]
	player.coef_friction = 0.9f; // [-]
	player.coef_restitution = 0.5f; // [-]
	player.fx = 1.0f;

	// Set initial position and velocity of the ball
	player.x = 2.0f;
	player.y = (ground.y + ground.h) + 0.8f;
	player.vx = 0.1f;
	player.vy = 0.0f;

	players.emplace_back(player);

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	// Process all balls in the scenario
	
	for (auto& player : players)
	{
		// Skip ball if physics not enabled
		if (!player.physics_enabled)
		{
			continue;
		}

		// Step #0: Clear old values
		// ----------------------------------------------------------------------------------------

		// Reset total acceleration and total accumulated force of the ball
		player.fx = player.fy = 0.0f;
		player.ax = player.ay = 0.0f;

		// Step #1: Compute forces
		// ----------------------------------------------------------------------------------------

		// Gravity force
		float fgx = player.mass * 0.0f;
		float fgy = player.mass * -10.0f; // Let's assume gravity is constant and downwards
		player.fx += fgx; player.fy += fgy; // Add this force to ball's total force

		// Aerodynamic Drag force (only when not in water)
		if (!is_colliding_with_water(player, water))
		{
			float fdx = 0.0f; float fdy = 0.0f;
			compute_aerodynamic_drag(fdx, fdy, player, atmosphere);
			player.fx += fdx; player.fy += fdy; // Add this force to ball's total force
		}

		// Hydrodynamic forces (only when in water)
		if (is_colliding_with_water(player, water))
		{
			// Hydrodynamic Drag force
			float fhdx = 0.0f; float fhdy = 0.0f;
			compute_hydrodynamic_drag(fhdx, fhdy, player, water);
			player.fx += fhdx; player.fy += fhdy; // Add this force to ball's total force

			// Hydrodynamic Buoyancy force
			float fhbx = 0.0f; float fhby = 0.0f;
			compute_hydrodynamic_buoyancy(fhbx, fhby, player, water);
			player.fx += fhbx; player.fy += fhby; // Add this force to ball's total force
		}
		//May the Force Movement
		if (App->player->movement == 3) {
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				player.fx = 50;
			}
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				player.fx = -50;

			}

		}
		// Other forces
		// ...

		// Step #2: 2nd Newton's Law
		// ----------------------------------------------------------------------------------------
		
		//Velocity movement
		if (App->player->movement == 1) {
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				player.vx = 5.0f;
			}
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				player.vx = -5.0f;

			}
			
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
		{
			player.vx = 0.0f;
		}
		// SUM_Forces = mass * accel --> accel = SUM_Forces / mass
		player.ax = player.fx / player.mass;
		player.ay = player.fy / player.mass;

		// Step #3: Integrate --> from accel to new velocity & new position
		// ----------------------------------------------------------------------------------------

		// We will use the 2nd order "Velocity Verlet" method for integration.
		integrator_velocity_verlet(player, dt);

		// Step #4: solve collisions
		// ----------------------------------------------------------------------------------------

		// Solve collision between ball and ground
		if (is_colliding_with_ground(player, ground))
		{
			if (player.y > ground.y + ground.h) {
				player.y = ground.y + ground.h + player.radius;
				player.vy = -player.vy;
				player.vy *= player.coef_restitution;

			}
			if (player.y < ground.y) {

				player.y = ground.y - player.radius;
				player.vy = -player.vy;
				player.vy *= player.coef_restitution;

			}
			if (player.x < ground.x)
			{
				player.x = ground.x - player.radius;
				player.vx = -player.vx + 3;
				player.vx *= player.coef_friction;

			}
			if ((player.x + 0.5 - player.radius)  >  (ground.x + ground.w) && player.y < ground.y + ground.h)
			{
				player.x = ground.x + ground.w + player.radius;
				player.vx = -player.vx + 3;
				player.vx *= player.coef_friction;

			}

			// FUYM non-elasticity
		}
		if (is_colliding_with_ground(player, ground_2))
		{
			if (player.y > ground_2.y + ground_2.h) {
				player.y = ground_2.y + ground_2.h + player.radius;
				player.vy = -player.vy;
				player.vy *= player.coef_restitution;

			}
			if (player.y < ground_2.y) {

				player.y = ground_2.y - player.radius;
				player.vy = -player.vy;
				player.vy *= player.coef_restitution;

			}
			if (player.x < ground_2.x)
			{
				player.x = ground_2.x - player.radius;
				player.vx = -player.vx + 3;
				player.vx *= player.coef_friction;

			}
			if ((player.x + 0.5 - player.radius) > (ground_2.x + ground_2.w) && player.y < ground_2.y + ground_2.h)
			{
				player.x = ground_2.x + ground_2.w + player.radius;
				player.vx = -player.vx + 3;
				player.vx *= player.coef_friction;

			}
		}
		if (is_colliding_with_ground(player, platform))
		{
			// TP ball to ground surface
			if (player.y > platform.y + platform.h) {
				player.y = platform.y + platform.h + player.radius;
				player.vy = -player.vy;
				player.vy *= player.coef_restitution;

			}
			if (player.y < platform.y) {

				player.y = platform.y - player.radius;
				player.vy = -player.vy;
				player.vy *= player.coef_restitution;

			}
			if (player.x - player.radius < platform.x && player.y < platform.y + platform.h)
			{
				player.x = platform.x - player.radius;
				player.vx = -player.vx + 3;
				player.vx *= player.coef_friction;

			}
			if ((player.x + 0.5 - player.radius) > (platform.x + platform.w) && player.y < platform.y + platform.h)
			{
				player.x = platform.x + platform.w + player.radius;
				player.vx = -player.vx + 3;
				player.vx *= player.coef_friction;

			}
		}
	}
	for (auto& ball : balls)
	{
		// Skip ball if physics not enabled

		if (!ball.physics_enabled)
		{
			continue;
		}

		// Step #0: Clear old values
		// ----------------------------------------------------------------------------------------

		// Reset total acceleration and total accumulated force of the ball
		ball.fx = ball.fy = 0.0f;
		ball.ax = ball.ay = 0.0f;

		// Step #1: Compute forces
		// ----------------------------------------------------------------------------------------

		// Gravity force
		float fgx = ball.mass * 0.0f;
		float fgy = ball.mass * -10.0f; // Let's assume gravity is constant and downwards
		ball.fx += fgx; ball.fy += fgy; // Add this force to ball's total force

		// Aerodynamic Drag force (only when not in water)
		if (!is_colliding_with_water(ball, water))
		{
			float fdx = 0.0f; float fdy = 0.0f;
			compute_aerodynamic_drag(fdx, fdy, ball, atmosphere);
			ball.fx += fdx; ball.fy += fdy; // Add this force to ball's total force
		}

		// Hydrodynamic forces (only when in water)
		if (is_colliding_with_water(ball, water))
		{
			// Hydrodynamic Drag force
			float fhdx = 0.0f; float fhdy = 0.0f;
			compute_hydrodynamic_drag(fhdx, fhdy, ball, water);
			ball.fx += fhdx; ball.fy += fhdy; // Add this force to ball's total force

			// Hydrodynamic Buoyancy force
			float fhbx = 0.0f; float fhby = 0.0f;
			compute_hydrodynamic_buoyancy(fhbx, fhby, ball, water);
			ball.fx += fhbx; ball.fy += fhby; // Add this force to ball's total force
		}

		// Other forces
		// ...


		// Step #2: 2nd Newton's Law
		// ----------------------------------------------------------------------------------------
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_IDLE && ball.canshoot == true) {

			ball.vx = ball.fvecx / 7;
			ball.vy = -ball.fvecy / 7;
			ball.shot = true;
			ball.canshoot = false;

		}
		// SUM_Forces = mass * accel --> accel = SUM_Forces / mass
		ball.ax = ball.fx / ball.mass;
		ball.ay = ball.fy / ball.mass;

		// Step #3: Integrate --> from accel to new velocity & new position
		// ----------------------------------------------------------------------------------------

		// We will use the 2nd order "Velocity Verlet" method for integration.
		integrator_velocity_verlet(ball, dt);

		// Step #4: solve collisions
		// ----------------------------------------------------------------------------------------

		// Solve collision between ball and ground
		if (is_colliding_with_ground(ball, ground))
		{
			if (ball.y > ground.y + ground.h) {
				ball.y = ground.y + ground.h + ball.radius;
				ball.vy = -ball.vy;
				ball.vy *= ball.coef_restitution;

			}
			if (ball.y < ground.y) {

				ball.y = ground.y - ball.radius;
				ball.vy = -ball.vy;
				ball.vy *= ball.coef_restitution;

			}
			if (ball.x < ground.x)
			{
				ball.x = ground.x - ball.radius;
				ball.vx = -ball.vx + 3;
				ball.vx *= ball.coef_friction;

			}
			if ((ball.x + 0.5 - ball.radius) > (ground.x + ground.w) && ball.y < ground.y + ground.h)
			{
				ball.x = ground.x + ground.w + ball.radius;
				ball.vx = -ball.vx + 3;
				ball.vx *= ball.coef_friction;

			}

			// FUYM non-elasticity
		}
		if (is_colliding_with_ground(ball, ground_2))
		{
			if (ball.y > ground_2.y + ground_2.h) {
				ball.y = ground_2.y + ground_2.h + ball.radius;
				ball.vy = -ball.vy;
				ball.vy *= ball.coef_restitution;

			}
			if (ball.y < ground_2.y) {

				ball.y = ground_2.y - ball.radius;
				ball.vy = -ball.vy;
				ball.vy *= ball.coef_restitution;

			}
			if (ball.x < ground_2.x)
			{
				ball.x = ground_2.x - ball.radius;
				ball.vx = -ball.vx + 3;
				ball.vx *= ball.coef_friction;

			}
			if ((ball.x + 0.5 - ball.radius) > (ground_2.x + ground_2.w) && ball.y < ground_2.y + ground_2.h)
			{
				ball.x = ground_2.x + ground_2.w + ball.radius;
				ball.vx = -ball.vx + 3;
				ball.vx *= ball.coef_friction;

			}
		}
		if (is_colliding_with_ground(ball, platform))
		{
			// TP ball to ground surface
			if (ball.y > platform.y + platform.h) {
				ball.y = platform.y + platform.h + ball.radius;
				ball.vy = -ball.vy;
				ball.vy *= ball.coef_restitution;

			}
			if (ball.y < platform.y) {

				ball.y = platform.y - ball.radius;
				ball.vy = -ball.vy;
				ball.vy *= ball.coef_restitution;

			}
			if (ball.x - ball.radius < platform.x && ball.y < platform.y + platform.h)
			{
				ball.x = platform.x - ball.radius;
				ball.vx = -ball.vx + 3;
				ball.vx *= ball.coef_friction;

			}
			if ((ball.x + 0.5 - ball.radius) > (platform.x + platform.w) && ball.y < platform.y + platform.h)
			{
				ball.x = platform.x + platform.w + ball.radius;
				ball.vx = -ball.vx + 3;
				ball.vx *= player.coef_friction;

			}
		}
		if (is_colliding_with_ground(ball, basket))
		{
			App->player->score = true;
			App->audio->PlayFx(ScoreFX);
		}
	}
	// Continue game
	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate()
{
	// Colors
	int color_r, color_g, color_b, alpha_a;

	// Draw Ground
	if (debug == true) {
		color_r = 0; color_g = 255; color_b = 0, alpha_a = 255;
		App->renderer->DrawQuad(ground.pixels(), color_r, color_g, color_b, alpha_a, false);

		// Draw Ground 2
		color_r = 0; color_g = 255; color_b = 0, alpha_a = 255;
		App->renderer->DrawQuad(ground_2.pixels(), color_r, color_g, color_b, alpha_a, false);

		// Draw Platform
		color_r = 0; color_g = 255; color_b = 0, alpha_a = 255;
		App->renderer->DrawQuad(platform.pixels(), color_r, color_g, color_b, alpha_a, false);

		// Draw Basket
		color_r = 255; color_g = 0; color_b = 0, alpha_a = 255;
		App->renderer->DrawQuad(basket.pixels(), color_r, color_g, color_b, alpha_a, false);

		// Draw water
		color_r = 0; color_g = 0; color_b = 255, alpha_a = 255;
		App->renderer->DrawQuad(water.pixels(), color_r, color_g, color_b, alpha_a, false);
	}

	// Draw all balls in the scenario
	for (auto& ball : balls)
	{
		// Convert from physical magnitudes to geometrical pixels
		int pos_x = METERS_TO_PIXELS(ball.x);
		int pos_y = SCREEN_HEIGHT - METERS_TO_PIXELS(ball.y);
		int size_r = METERS_TO_PIXELS(ball.radius);

		// Select color
		
		color_r = 255; color_g = 0; color_b = 0;
		

		// Draw ball
		if (debug == true) {
			App->renderer->DrawCircle(pos_x, pos_y, size_r, color_r, color_g, color_b);
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && ball.shot == false) {
			int mou_x = App->input->GetMouseX();
			int mou_y = App->input->GetMouseY();
			App->renderer->DrawLine(mou_x, mou_y, METERS_TO_PIXELS(ball.x), -METERS_TO_PIXELS(ball.y)+754, 255, 255, 0);
			ball.fvecx = METERS_TO_PIXELS(ball.x) - mou_x;
			ball.fvecy = -METERS_TO_PIXELS(ball.y) + 754 - mou_y;
			ball.canshoot = true;
		}
	}
	for (auto& player : players)
	{
		// Convert from physical magnitudes to geometrical pixels
		int pos_x = METERS_TO_PIXELS(player.x);
		int pos_y = SCREEN_HEIGHT - METERS_TO_PIXELS(player.y);
		int size_r = METERS_TO_PIXELS(player.radius);
		

		// Select color
	

		color_r = 255; color_g = 0; color_b = 0;
		

		// Draw player
		if (debug == true) {
			App->renderer->DrawCircle(pos_x, pos_y, size_r, color_r, color_g, color_b);
		}
	}
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	return true;
}

// Compute modulus of a vector
float modulus(float vx, float vy)
{
	return std::sqrt(vx * vx + vy * vy);
}

// Compute Aerodynamic Drag force
void compute_aerodynamic_drag(float &fx, float& fy, const PhysBall &ball, const Atmosphere &atmosphere)
{
	float rel_vel[2] = { ball.vx - atmosphere.windx, ball.vy - atmosphere.windy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = 0.5f * atmosphere.density * speed * speed * ball.surface * ball.cd; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Drag force
void compute_hydrodynamic_drag(float& fx, float& fy, const PhysBall& ball, const Water& water)
{
	float rel_vel[2] = { ball.vx - water.vx, ball.vy - water.vy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = ball.b * speed; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Buoyancy force
void compute_hydrodynamic_buoyancy(float& fx, float& fy, const PhysBall& ball, const Water& water)
{
	// Compute submerged area (assume ball is a rectangle, for simplicity)
	float water_top_level = water.y + water.h; // Water top level y
	float h = 2.0f * ball.radius; // Ball "hitbox" height
	float surf = h * (water_top_level - ball.y); // Submerged surface
	if ((ball.y + ball.radius) < water_top_level) surf = h * h; // If ball completely submerged, use just all ball area
	surf *= 0.4; // FUYM to adjust values (should compute the area of circle segment correctly instead; I'm too lazy for that)

	// Compute Buoyancy force
	double fbuoyancy_modulus = water.density * 10.0 * surf; // Buoyancy force (modulus)
	fx = 0.0; // Buoyancy is parallel to pressure gradient
	fy = fbuoyancy_modulus; // Buoyancy is parallel to pressure gradient
}

// Integration scheme: Velocity Verlet
void integrator_velocity_verlet(PhysBall& ball, float dt)
{
	ball.x += ball.vx * dt + 0.5f * ball.ax * dt * dt;
	ball.y += ball.vy * dt + 0.5f * ball.ay * dt * dt;
	ball.vx += ball.ax * dt;
	ball.vy += ball.ay * dt;
}

// Detect collision with ground
bool is_colliding_with_ground(const PhysBall& ball, const Ground& ground)
{
	float rect_x = (ground.x + ground.w / 2.0f); // Center of rectangle
	float rect_y = (ground.y + ground.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.x, ball.y, ball.radius, rect_x, rect_y, ground.w, ground.h);
}

// Detect collision with water
bool is_colliding_with_water(const PhysBall& ball, const Water& water)
{
	float rect_x = (water.x + water.w / 2.0f); // Center of rectangle
	float rect_y = (water.y + water.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.x, ball.y, ball.radius, rect_x, rect_y, water.w, water.h);
}

// Detect collision between circle and rectange
bool check_collision_circle_rectangle(float cx, float cy, float cr, float rx, float ry, float rw, float rh)
{
	// Algorithm taken from https://stackoverflow.com/a/402010

	// Distance from center of circle to center of rectangle
	float dist_x = std::abs(cx - rx);
	float dist_y = std::abs(cy - ry);

	// If circle is further than half-rectangle, not intersecting
	if (dist_x > (rw / 2.0f + cr)) { return false; }
	if (dist_y > (rh / 2.0f + cr)) { return false; }

	// If circle is closer than half-rectangle, is intersecting
	if (dist_x <= (rw / 2.0f)) { return true; }
	if (dist_y <= (rh / 2.0f)) { return true; }

	// If all of above fails, check corners
	float a = dist_x - rw / 2.0f;
	float b = dist_y - rh / 2.0f;
	float cornerDistance_sq = a * a + b * b;
	return (cornerDistance_sq <= (cr * cr));
}

// Convert from meters to pixels (for SDL drawing)
SDL_Rect Ground::pixels()
{
	SDL_Rect pos_px{};
	pos_px.x = METERS_TO_PIXELS(x);
	pos_px.y = SCREEN_HEIGHT - METERS_TO_PIXELS(y);
	pos_px.w = METERS_TO_PIXELS(w);
	pos_px.h = METERS_TO_PIXELS(-h); // Can I do this? LOL
	return pos_px;
}

