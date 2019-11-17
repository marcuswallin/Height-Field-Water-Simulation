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
	Terrain(char* ground_gen_file);
	//Model* model;
private:
	TextureData terrain_texture;
	void tex_to_vector();

};

#endif