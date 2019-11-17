#pragma once
#include "watermass.h"

using namespace std;

WaterMass::WaterMass(const Terrain& terrain,
	int x_start, int x_end, int z_start, int z_end, float offset) : 
	x_offset(x_start), z_offset(z_start){
	
	gen_water_from_terrain(terrain, x_start, x_end, z_start, z_end, offset);
	grid_size_x = x_end-x_start;
	grid_size_z = x_end-x_start;
	model = GenerateTerrain(this);
}

//generates the height_vetor from the terrain
void WaterMass::gen_water_from_terrain(const Terrain& terrain,
	int x_start, int x_end, int z_start, int z_end, float offset) {
	
	height_vector.resize(z_end-z_start, vector<GLfloat>(x_end - x_start));
	for (int z = 0; z < z_end-z_start; ++z)
		for (int x = 0; x < x_end - x_start; ++x) {
			height_vector[z][x] = terrain.height_vector[z + z_start][x + x_start] + offset;
		}
}