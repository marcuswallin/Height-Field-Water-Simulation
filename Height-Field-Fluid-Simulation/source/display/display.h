#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include "MicroGlut.h"

#include "VectorUtils3.h"
#include "loadobj.h"
#include "../world/world.h"
//#include "cameracontrol.c"


void init(void);
void display(void);
void draw_sky_box(const mat4* mtv_matrix);
void keyboard_interaction();

void add_well();
extern int time_diff;


#endif