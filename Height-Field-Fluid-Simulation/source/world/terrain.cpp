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


void Terrain::tex_to_vector() {
	height_vector.resize(grid_size_z, vector<GLfloat>(grid_size_x));
	
	for (int z = 0; z < grid_size_z; z++)
		for (int x = 0; x < grid_size_x; x++)
		{
			height_vector[z][x] = terrain_texture.imageData[(x + z * terrain_texture.width) * (terrain_texture.bpp / 8)];
			/*vertexArray[(x + z * tex->width) * 3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width) * 3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / 20.0;
			vertexArray[(x + z * tex->width) * 3 + 2] = z / 1.0;


			texCoordArray[(x + z * tex->width) * 2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width) * 2 + 1] = z; // (float)z / height_grid->grid_size_z;*/
		}
}