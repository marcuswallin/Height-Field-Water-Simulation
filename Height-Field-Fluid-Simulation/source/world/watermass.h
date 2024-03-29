#pragma once
#ifndef WATERMASS_H
#define WATERMASS_H

#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "height_grid.h"
#include "terrain.h"
#include <vector>
#include <algorithm>
#include "water_source.h"


class WaterMass : public Grid{
public:
	WaterMass() = default;
	WaterMass(Terrain& terrain,
		int x_start, int x_end, int z_start, int z_end, float offset, int resolution, int timestep_length);
	WaterMass(const WaterMass& w) = default;
	WaterMass & operator=(const WaterMass& w) = default;
	~WaterMass() = default;
	int x_offset;
	int z_offset;

	 
	void draw(const mat4& cam_mat, int time_diff, bool calc_water, bool show_grid, bool show_depth);
	void init_program(const mat4* proj_mat);
	void calculate_movements();
	void add_source(const vec3& pos, bool is_drain);
	void gen_water_from_terrain(Terrain&,
		int x_start, int x_end, int z_start, int z_end, float offset, int resolution);
	void draw_sources(const mat4& cam_mat);
	void show_parallax(bool);
private:

	//the texture that the height was read from
	GLuint water_height_tex;
	TextureData water_color_tex;

	//the shaders used by watermass
	GLuint program;
	GLuint	well_program;
	Model* well_model;
	int nr_wells = 0;


	WaterSource* sources;
	int source_index{ 0 };
	void set_source_height();
	
	//coeffiecients for calculation
	double friction_c = 0.1;
	double gravity = 9.82;
	float ground_e = 0.0001;
	float a_vel = 0.5;
	float beta = 2.0;
	float lambda_edge = 2 * 1.0 / resolution;
	float alpha_edge = 0.3;

	//in seconds
	GLfloat deltat;
	int resolution;

	//stores velocities in the x and z parts
	Grid velocities;

	float get_height_derivative(int x, int z);
	vec4* get_velocity(int x, int z, int x_offset, int z_offset);
	float get_hbar_x(int x, int z, int x_offset, int z_offset);
	float get_hbar_z(int x, int z, int x_offset, int z_offset);
	bool WaterMass::is_reflective(vec4* here, vec4* here_plus);
	void WaterMass::velocity_integration(void);
	float friction_force(float vel);
	float get_advection_h(int x, int z);
	void advect_velocities(void);

};

#endif