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
	WaterMass(Terrain& terrain,
		int x_start, int x_end, int z_start, int z_end, float offset);
	WaterMass(const WaterMass& w); 

	int x_offset;
	int z_offset;
	double friction_c = 0.001;
	double gravity = 9.82;
	 

	void init_water_tex();
	void calculate_movements(int time_diff);
	GLuint program;

private:
	void gen_water_from_terrain( Terrain&,
		int x_start, int x_end, int z_start, int z_end, float offset);

	GLuint water_height_tex;

	float ground_e = 0.0001;
	float a_vel = 0.5;

	//stores velocities in the x and z parts
	HeightGrid velocities;

	float get_height_derivative(int x, int z);
	vec4* get_velocity(int x, int z, int x_offset, int z_offset);
	float get_hbar_x(int x, int z, int x_offset, int z_offset);
	float get_hbar_z(int x, int z, int x_offset, int z_offset);
	bool WaterMass::is_reflective_x(int x, int z);
	bool WaterMass::is_reflective_z(int x, int z);
	void WaterMass::velocity_integration(void);

};

#endif