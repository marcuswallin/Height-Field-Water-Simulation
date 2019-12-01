#pragma once
#ifndef HEIGHT_GRID_H
#define HEIGHT_GRID_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include <vector>

//abstract class that is used to define terrain and watermass classes
class HeightGrid {
public:
	HeightGrid() = default;
	HeightGrid(int grid_x, int grid_z, bool intitiate_array);
	HeightGrid(const HeightGrid &h);
	~HeightGrid() = default;

	/*
	contains information about height of the grid;
	rows are z, columns are x
	possibly should be another struct in the future*/
	vec4* height_array;
	int grid_size_x;
	int grid_size_z;

	Model* model;

	//indicates the scale of the grid (if grid length is 0.2 meter, this value is 0.2)
	float scale = 1;

	//returns the element at pos x, z
	vec4* at(int x, int z);
	
	//interpolates the values at x+off_z, z+off_z 
	//x+off_x or z+off_z shoudl not be larger than grid_size, and off_x and off_z >= 0.
	float interpolate_height(int x, int z, float off_x, float off_z);
private:

};

#endif