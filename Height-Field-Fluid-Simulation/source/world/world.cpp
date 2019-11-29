#pragma once

#include "world.h"

using namespace std;

World::World(char* terrain_tex_name, 
	int x_start, int x_end, int z_start, int z_end, float offset, int water_resolution, int timestep_length) :
	terrain(terrain_tex_name), 
	water(terrain, x_start, x_end, z_start, z_end, offset, water_resolution, timestep_length) {

	//not used atm
	world_scale = 1;
}



