#pragma once

#include "world.h"

using namespace std;

World::World(char* terrain_tex_name) : 
	terrain(terrain_tex_name), water(terrain_tex_name, 0, 20, 0, 20, 10) {

	world_scale = 1;
}


/*
vector<vector<GLfloat>>& World::create_water_from_terrain(
	int x_start, int x_end, int z_start, int z_end, float offset) {

}*/



