#pragma once
#ifndef WATERMASS_H
#define WATERMASS_H

#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "height_grid.h"
#include "terrain.h"
#include <vector>


class WaterMass : public HeightGrid{
public:
	WaterMass() = default;
	WaterMass(const Terrain& terrain,
		int x_start, int x_end, int z_start, int z_end, float offset);

	double x_offset;
	double z_offset;

private:
	void gen_water_from_terrain(const Terrain&,
		int x_start, int x_end, int z_start, int z_end, float offset);

};

#endif