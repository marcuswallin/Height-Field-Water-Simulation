#pragma once
//#include "MicroGlut.h"
#include "terrain.h"

using namespace std;

Terrain::Terrain(char * ground_gen_file) {
	LoadTGATextureData(ground_gen_file, &terrain_texture);
	grid_size_x = terrain_texture.width;
	grid_size_z = terrain_texture.height;
	tex_to_vector();
	model = GenerateTerrain(this);
}

//generate a 2D-vector from the terrain_texture
void Terrain::tex_to_vector() {

	height_array = new vec4[grid_size_x * grid_size_z];
	for (int z = 0; z < grid_size_z; z++)
		for (int x = 0; x < grid_size_x; x++){
			at(x,z)->x = terrain_texture.imageData[(x + z * terrain_texture.width) * (terrain_texture.bpp / 8)];
		}
}