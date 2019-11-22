#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include "MicroGlut.h"



//#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "../world/world.h"


void init(void);
void display(void);
void draw_sky_box(const mat4* mtv_matrix);

void init_water_tex(void);

#endif