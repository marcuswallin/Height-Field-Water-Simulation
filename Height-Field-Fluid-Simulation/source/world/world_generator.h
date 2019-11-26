#pragma once
#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include <vector>
#include "terrain.h"


Model* GenerateTerrain(HeightGrid* height_grid, bool, int resolution);//TextureData* tex);
vec3 calculateNormal(GLfloat* vertexArray, int x, int z, const HeightGrid* height_grid);
vec3 getVertex(GLfloat* verticeArray, int x, int z, const HeightGrid* height_grid);
#endif