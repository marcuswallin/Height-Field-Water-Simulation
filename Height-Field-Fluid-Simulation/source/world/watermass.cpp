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

void WaterMass::init_water_tex() {

	//init texture data--------------------------------------------------
	glActiveTexture(GL_TEXTURE15);
	glGenTextures(1, &water_height_tex);
	glBindTexture(GL_TEXTURE_2D, water_height_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//change this part later
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, grid_size_x, grid_size_z, 0,
		GL_RED, GL_FLOAT, &height_vector[0][0]);//&smoke_array[0].pos.x);
	//water_program should be coitained by watermass
	glUniform1i(glGetUniformLocation(program, "waterHeight"), 15);

}