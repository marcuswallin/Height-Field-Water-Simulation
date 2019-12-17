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
#include "watermass.h"

class World {
public:
	/*
	creates terrain_texture and terrain_model
	initializes grid_size based on terrain_texture
	*/
	World(char*, 
		int x_start, int x_end, int z_start, int z_end, float, int water_resolution, int timestep_length);
	World() = default;
	World(const World &) = default;
	World& operator=(const World&) = default;

	Terrain terrain;
	WaterMass water;

private:

};


#endif