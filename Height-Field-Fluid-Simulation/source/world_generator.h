#pragma once
#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

Model* GenerateTerrain(TextureData* tex);

#endif