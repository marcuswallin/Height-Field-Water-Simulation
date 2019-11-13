#pragma once
#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

Model* GenerateTerrain(TextureData* tex);
vec3 calculateNormal(GLfloat* vertexArray, int x, int z,  TextureData* tex);
vec3 getVertex(GLfloat* verticeArray, int x, int z, TextureData* tex);
#endif