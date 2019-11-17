#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "MicroGlut.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "loadobj.h"
//change name to terrain.h
#include "world_generator.h"
#include "terrain.h"

class World {
public:
	/*
	creates terrain_texture and terrain_model
	initializes grid_size based on terrain_texture
	*/
	World(char*);
	World() = default;

	Terrain terrain;
	//Model* terrain_model;
private:
	//mprobably not needed
	//TextureData terrain_texture;
	//int grid_size_x;
	//int grid_size_z;
	float world_scale;
};


#endif