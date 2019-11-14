#pragma once
#ifndef WATERMASS_H
#define WATERMASS_H

#include "MicroGlut.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "loadobj.h"
//change name to terrain.h
#include "world_generator.h"

class World {
public:
	/*
	creates terrain_texture and terrain_model
	initializes grid_size based on terrain_texture
	*/
	World() = default;
	World(char*);

	Model* terrain_model;
private:
	//mprobably not needed
	TextureData terrain_texture;
	int grid_size_x;
	int grid_size_z;
	float world_scale;
};


#endif