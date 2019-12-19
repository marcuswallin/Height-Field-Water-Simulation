#pragma once
//#include "MicroGlut.h"
#include "height_grid.h"

using namespace std;

Grid::Grid(int grid_x, int grid_z, bool initiate_array) :
	grid_size_x{grid_x}, grid_size_z{grid_z}{

	if (initiate_array){
		grid_array = new vec4[grid_size_x * grid_size_z];
	}
}



Grid::Grid(const Grid& h) : 
	grid_size_x { h.grid_size_x }, grid_size_z{ h.grid_size_z }{
	grid_array = new vec4[grid_size_x * grid_size_z];
	for (int i = 0; i < grid_size_x * grid_size_z; ++i) {
		grid_array[i]=h.grid_array[i];
	}
}

//returns 0 if outside grid, should be handled elsewhere
vec4* Grid::at(int x, int z) {
	if (x < 0 || x >= grid_size_x || z < 0 || z >= grid_size_z)
		return new vec4{ 0,0,0,0 };
	return &grid_array[x + z * grid_size_x];
}


//uses bilinear interpolation
float Grid::interpolate_height(int x, int z, float off_x, float off_z) {
	
	if (x + 1 >= grid_size_x || z + 1 >= grid_size_z) {
		throw invalid_argument("Trying to interpolate at edge.");
	}

	float h = at(x, z)->x;
	float h_x = at(x+1, z)->x;
	float h_z = at(x, z + 1)->x;
	float h_xz = at(x + 1, z + 1)->x;

	//interpolation in x direction:
	float x1 = (1 - off_x) * h + off_x * h_x;
	float x2 = (1 - off_x) * h_z + off_x * h_xz;

	float height = (1 - off_z) * x1 + off_z * x2;

	return height;

}
