#pragma once
//#include "MicroGlut.h"
#include "height_grid.h"

using namespace std;

HeightGrid::HeightGrid(int grid_x, int grid_z, bool initiate_array) :
	grid_size_x{grid_x}, grid_size_z{grid_z}{

	if (initiate_array){
		height_array = new vec4[grid_size_x * grid_size_z];
	}
}

HeightGrid::HeightGrid(const HeightGrid& h) : 
	grid_size_x { h.grid_size_x }, grid_size_z{ h.grid_size_z }{
	height_array = new vec4[grid_size_x * grid_size_z];
	for (int i = 0; i < grid_size_x * grid_size_z; ++i) {
		height_array[i]=h.height_array[i];
	}
}

vec4* HeightGrid::at(int x, int z) {
	if (x < 0 || x >= grid_size_x || z < 0 || z >= grid_size_z)
		throw invalid_argument("Trying to get element outside heightgrid at: x="
			+ to_string(x) + " z=" + to_string(z));
	return &height_array[x + z * grid_size_x];
}
