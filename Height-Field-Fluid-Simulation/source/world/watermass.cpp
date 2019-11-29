#pragma once
#include "watermass.h"
#include <iostream>

using namespace std;

//velocities initiated as staggered grid
WaterMass::WaterMass(Terrain& terrain,
	int x_start, int x_end, int z_start, int z_end, float offset, int resolution_,  int timestep_length) :
	HeightGrid{ (x_end - x_start)*resolution_,  (z_end - z_start)*resolution_, true  },
	x_offset(x_start), z_offset(z_start), resolution{resolution_}, deltat{ (float)timestep_length / 1000},
	velocities{ (x_end - x_start)*resolution_ + 1,  (z_end - z_start) * 2 * resolution_ + 1, true }{

	gen_water_from_terrain(terrain, x_start, x_end, z_start, z_end, offset, resolution);
	model = GenerateTerrain(this, true, resolution);
}

void WaterMass::draw(const mat4& cam_mat, int time_diff, bool calc_water){
	glUseProgram(program);
	mat4 model_world = T(x_offset, 0, z_offset); //change this
	//model_to_view = cam_matrix * model_world;
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam_mat.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, model_world.m);

	if (calc_water) calculate_movements();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, grid_size_x, grid_size_z, 0,
		GL_RGBA, GL_FLOAT, &height_array[0].x);
	glUniform1i(glGetUniformLocation(program, "waterHeight"), 15);
	//glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(program, "tex"), 2);

	DrawModel(model, program, "inPosition", NULL, "inTexCoord");

}

void WaterMass::init_program(const mat4* proj_mat) {
	program = loadShadersG("source/shaders/water.vert", 
		"source/shaders/water.frag", "source/shaders/water.geom");
	glUseProgram(program);
	//init texture data--------------------------------------------------
	glActiveTexture(GL_TEXTURE15);
	glGenTextures(1, &water_height_tex);
	glBindTexture(GL_TEXTURE_2D, water_height_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//change this part later
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, grid_size_x, grid_size_z, 0,
		GL_RGBA, GL_FLOAT, &height_array[0].x);//&smoke_array[0].pos.x);
	//water_program should be coitained by watermass
	glUniform1i(glGetUniformLocation(program, "waterHeight"), 15);
	glUniform1i(glGetUniformLocation(program, "resolution"), resolution);
	glUniform1i(glGetUniformLocation(program, "grid_x"), grid_size_x);
	glUniform1i(glGetUniformLocation(program, "grid_z"), grid_size_z);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, proj_mat->m);

	glUniform1i(glGetUniformLocation(program, "tex"), 2);

}


//generates the height_vetor from the terrain
//resolution is the number of nodes per ground grid
void WaterMass::gen_water_from_terrain(Terrain& terrain,
	int x_start, int x_end, int z_start, int z_end, float offset, int resolution) {

	for (int z = 0; z < grid_size_z; ++z)
		for (int x = 0; x < grid_size_x; ++x) {
			
			vec4* t_height = terrain.at(x / resolution + x_start, z / resolution + z_start);

			//stores water depth in x
			at(x, z)->x = offset;
			//stores ground height in y
			//interpolation
			float off_x = 0; 
			if(x% resolution != 0)
				off_x = (float)(x % resolution) / resolution;
			
			float off_z = 0;
			if (z % resolution != 0)
				off_z = (float)(z % resolution) / resolution;

			float height = terrain.interpolate_height(x / resolution + x_start, z / resolution + z_start, 
				off_x, off_z);
			at(x, z)->y = height; //t_height->x;
	
		}
}



//timestep in milliseconds
void WaterMass::calculate_movements() {
	HeightGrid height_copy{ *this };
	//float time = (float)glutGet(GLUT_ELAPSED_TIME) / 100.0;
	for (int z = 0; z < grid_size_z; ++z) {
		for (int x = 0; x < grid_size_x; x++)
		{

			vec4* h = height_copy.at(x, z);

			float dhdt = get_height_derivative(x, z);
			h->x += dhdt * deltat;
			if (h->x < 0){
				h->x = 0;
			}
			/*if (x != 0 && x != grid_size_x - 1 && z != 0 && z != grid_size_z - 1) {
				vec4* h_xp = height_copy.at(x + 1, z);
				vec4* h_xm = height_copy.at(x - 1, z);
				vec4* h_zp = height_copy.at(x, z + 1);
				vec4* h_zm = height_copy.at(x, z - 1);

				//this should be separate function
				if ((h->x + h->y) - (h_xm->x + h_xm->y) > lambda_edge &&
					(h->x + h->y) > (h_xp->x + h_xp->y))
					h->x += alpha_edge * (max((double)0, 0.5 * (h->x + h_xp->x) - h->x));

				if ((h->x + h->y) - (h_xp->x + h_xp->y) > lambda_edge &&
					(h->x + h->y) > (h_xm->x + h_xm->y))
					h->x += alpha_edge * (max((double)0, 0.5 * (h->x + h_xm->x) - h->x));

				if ((h->x + h->y) - (h_zm->x + h_zm->y) > lambda_edge &&
					(h->x + h->y) > (h_zp->x + h_zp->y))
					h->x += alpha_edge * (max((double)0, 0.5 * (h->x + h_zp->x) - h->x));

				if ((h->x + h->y) - (h_zp->x + h_zp->y) > lambda_edge &&
					(h->x + h->y) > (h_zm->x + h_zm->y))
					h->x += alpha_edge * (max((double)0, 0.5 * (h->x + h_zm->x) - h->x));
			}*/
		}
	}
	height_array = height_copy.height_array;
	velocity_integration();
}

float WaterMass::get_height_derivative(int x, int z) {
	float hbarx_plus, hbarx_minus, hbarz_plus, hbarz_minus;
		
	//test eq.5 
	hbarx_plus = get_hbar_x(x, z, 1, 0);
	hbarx_minus = get_hbar_x(x, z, -1, 0);
	hbarz_plus = get_hbar_z(x, z, 0, 1);
	hbarz_minus = get_hbar_z(x, z, 0, -1);
	
	float h_avg_max = beta * 1 / (resolution * gravity * deltat);

	float h_adj = max((float)0,
		(hbarx_plus + hbarx_minus + hbarz_plus + hbarz_minus) / 4 - h_avg_max);

	//does low difference to height increase
	hbarx_plus -= h_adj;
	hbarx_minus -= h_adj;
	hbarz_plus -= h_adj;
	hbarz_minus -= h_adj;

	float dhdt = -(
		(hbarx_plus * get_velocity(x, z, 1, 0)->x - 
		hbarx_minus * get_velocity(x, z, -1, 0)->x) + 
		(hbarz_plus * get_velocity(x, z, 0, 1)->z -
		hbarz_minus * get_velocity(x, z, 0, -1)->z)) * resolution;
	
	float friction_f = friction_force(dhdt);


	return dhdt - friction_f * deltat;
}

float WaterMass::friction_force(float vel) {
	
	int mult = 1;
	if (vel < 0)
		mult = -1;

	return  mult*friction_c * vel * vel / 2;
}

//test eq.5 
float WaterMass::get_hbar_x(int x, int z, int x_offset, int z_offset)
{   
	
	float hbar = at(x, z)->x;
	if (x_offset*get_velocity(x, z, x_offset, z_offset)->x <= 0 &&
		x+x_offset < grid_size_x && x+x_offset >=0) {
		hbar = at(x + x_offset, z)->x;
	}
	
	return hbar;
}

float WaterMass::get_hbar_z(int x, int z, int x_offset, int z_offset)
{
	float hbar = at(x, z)->x;
	if (z_offset*get_velocity(x, z, x_offset, z_offset)->z <= 0 && 
		z + z_offset < grid_size_z && z + z_offset >= 0) {
		hbar = at(x, z+z_offset)->x;
	}

	return hbar;
}

//IS PROPERLY TESTED

//returns a velocity v_(x+offset/2),(z+offset/2)
//offsets are either -1, 0 +1
// one offset must be 0, the other -1 or 1, does not check for this
vec4* WaterMass::get_velocity(int x, int z, int x_offset, int z_offset)
{
	if (x_offset < -1 || x_offset > 1 || z_offset < -1 || z_offset > 1) {
		throw invalid_argument("x_offset or z_offset not within {-1, 1}");
	}

	int tmp_x = x_offset;
	if (x_offset == -1) {
		tmp_x = 0;
	}
	int x_vel = x + tmp_x;

	int z_vel;
	
	if (z_offset == -1) {
		z_offset = 0;
	}
	z_vel = (z + z_offset) * 2;
	//we know to look in odd z numbers
	if (x_offset != 0)
		z_vel += 1;

	return velocities.at(x_vel, z_vel);
}


void WaterMass::velocity_integration(void) {
	for (int z = 0; z < grid_size_z; ++z) {
		for (int x = 0; x < grid_size_x; x++)
		{
			vec4 * v_xplus = get_velocity(x, z, 1, 0);
			vec4* v_zplus = get_velocity(x, z, 0, 1);
			vec4* h = at(x, z);

			//reflective
			if (x+1 >= grid_size_x || is_reflective_x(x,z)) {
				v_xplus->x = 0;
			}
			else {
				vec4* h_xplus = at(x + 1, z);
				v_xplus->x += -(gravity * resolution ) * deltat *
					((h_xplus->x + h_xplus->y) - (h->x + h->y));
				if (v_xplus->x > a_vel * 1 / deltat) {
					v_xplus->x = a_vel * 1 / deltat;
				}
				v_xplus->x -= friction_force(v_xplus->x)*deltat;
			}

			//reflective
			if (z + 1 >= grid_size_z || is_reflective_z(x, z)) {
				v_zplus->z = 0;
			}
			else {
				vec4* h_zplus = at(x, z + 1);
				v_zplus->z += -(gravity * resolution) * deltat *
					((h_zplus->x + h_zplus->y) - (h->x + h->y));
				if (v_zplus->z > a_vel * 1 / deltat) {
					v_zplus->z = a_vel * 1 / deltat;
				}
				v_zplus->z -= friction_force(v_zplus->z) * deltat;
			}
		}
	}
}


//checks if water is so close to the ground that it should be dry 
//should have input either x_offset = 1 or z_offset = 1
bool WaterMass::is_reflective_x(int x, int z) {

	vec4* h = at(x, z);
	vec4* h_plus = at(x+1, z);
	bool first_cond = h->x <= ground_e && h->y > (h_plus->x + h_plus->y);
	bool second_cond = h_plus->x <= ground_e && h_plus->y > (h->x + h->y);
	return first_cond || second_cond;

}


//same as above but for z
bool WaterMass::is_reflective_z(int x, int z) {

	vec4* h = at(x, z);
	vec4* h_plus = at(x, z+1);
	bool first_cond = h->x <= ground_e && h->y > (h_plus->x + h_plus->y);
	bool second_cond = h_plus->x <= ground_e && h_plus->y > (h->x + h->y);
	return first_cond || second_cond;

}


/*get_velocity(0, 0, 0, -1);
get_velocity(0, 0, 0, 1);
get_velocity(1, 0, 1, 0);
get_velocity(0, 0, 1, 0);
get_velocity(1, 0, -1, 0);
get_velocity(1, 1, -1, 0);
get_velocity(1, 2, 1, 0);
get_velocity(1, 2, 0, 1);
get_velocity(1, 2, 0, -1);
get_velocity(2, 1, -1, 0);
get_velocity(2, 1, 1, 0);
*/
