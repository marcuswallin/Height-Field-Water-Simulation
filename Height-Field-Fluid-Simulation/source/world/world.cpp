
#pragma once
//#include "MicroGlut.h"

//#include "GL_utilities.h"
//#include "VectorUtils3.h"
//#include "loadobj.h"
//#include "LoadTGA.h"
#include "world.h"

using namespace std;

World::World(char* terrain_tex_name) : terrain(terrain_tex_name) {
	world_scale = 1;
}

