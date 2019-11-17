#pragma once
#ifndef HEIGHT_GRID_H
#define HEIGHT_GRID_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
//#include "world.h"
#include <vector>

//abstract class that is used to define terrain and watermass classes
class HeightGrid {
public:
	HeightGrid() = default;

	//rows are z, columns are x
	std::vector<std::vector<GLfloat>> height_vector;
	int grid_size_x;
	int grid_size_z;



private:

};

#endif