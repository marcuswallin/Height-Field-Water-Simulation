#pragma once
//#include "MicroGlut.h"
#include "height_grid.h"

using namespace std;

HeightGrid::HeightGrid(int grid_x, int grid_z) : 
	grid_size_x{grid_x}, grid_size_z{grid_z}{}

vec4* HeightGrid::at(int x, int z) {
	return &height_array[x + z * grid_size_x];
}
