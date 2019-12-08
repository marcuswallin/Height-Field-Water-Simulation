#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "height_grid.h"
#include "LoadTGA.h"
#include "world_generator.h"


class Terrain : public HeightGrid {
public:
	Terrain() = default;
	
	//initiate terrain from a texture file
	Terrain(char* ground_gen_file);
	void draw(const mat4& cam_mat, const mat4& mtv_mat);
	void init_program(const mat4& proj_mat, const mat4& wtv_mat);
	GLuint program;
	
private:
	TextureData terrain_texture;
	TextureData big_texture;
	
	//generate a 2D-array from the terrain_texture
	void tex_to_vector();

};

#endif