
#pragma once
#include "MicroGlut.h"

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "world.h"

using namespace std;

World::World(char* terrain_tex_name) {
	LoadTGATextureData(terrain_tex_name, &terrain_texture);
	terrain_model = GenerateTerrain(&terrain_texture);
	grid_size_x = terrain_texture.width;
	grid_size_z = terrain_texture.height;
	world_scale = 1;
}

