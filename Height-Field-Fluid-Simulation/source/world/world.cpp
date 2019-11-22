#pragma once

#include "world.h"

using namespace std;

World::World(char* terrain_tex_name, 
	int x_start, int x_end, int z_start, int z_end, float offset) :
	terrain(terrain_tex_name), water(terrain, x_start, x_end, z_start, z_end, offset) {

	world_scale = 1;
}



