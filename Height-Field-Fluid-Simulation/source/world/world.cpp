#pragma once

#include "world.h"

using namespace std;

World::World(char* terrain_tex_name) : 
	terrain(terrain_tex_name), water(terrain_tex_name, 60, 80, 60, 80, 10) {

	world_scale = 1;
}



