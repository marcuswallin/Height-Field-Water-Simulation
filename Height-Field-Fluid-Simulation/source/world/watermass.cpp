#pragma once
#include "watermass.h"
#include <iostream>
using namespace std;

//velocities initiated as staggered grid
WaterMass::WaterMass(Terrain& terrain,
	int x_start, int x_end, int z_start, int z_end, float offset) : 
	HeightGrid{ x_end - x_start,  z_end - z_start, false }, x_offset(x_start), z_offset(z_start),
	velocities{ x_end - x_start + 1,  (z_end - z_start)*2 + 1, true}{

	gen_water_from_terrain(terrain, x_start, x_end, z_start, z_end, offset);
	model = GenerateTerrain(this, true);
}

WaterMass::WaterMass(const WaterMass& w) : HeightGrid(w) {};

//generates the height_vetor from the terrain
void WaterMass::gen_water_from_terrain(Terrain& terrain,
	int x_start, int x_end, int z_start, int z_end, float offset) {
	
	//height_vector.resize(z_end-z_start, vector<GLfloat>(x_end - x_start));
	height_array = new vec4[(z_end - z_start) * (x_end - x_start)];
	for (int z = 0; z < z_end-z_start; ++z)
		for (int x = 0; x < x_end - x_start; ++x) {
			//stores water depth in x
			at(x, z)->x = offset;	
			//stores ground height in y
			at(x, z)->y = terrain.at(x + x_start, z + z_start)->x;
		}
}


void WaterMass::init_water_tex() {

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

}

//timestep in milliseconds
void WaterMass::calculate_movements(int timestep_length) {
	HeightGrid height_copy { *this };
	//float time = (float)glutGet(GLUT_ELAPSED_TIME) / 100.0;
	for (int z = 0; z < grid_size_z; ++z) {
		for (int x = 0; x < grid_size_x; x++)
		{
			float dhdt = get_height_derivative(x, z);
			height_copy.at(x, z)->x += dhdt*timestep_length/1000;
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
	
	float dhdt = -(
		(hbarx_plus * get_velocity(x,z,1,0)->x - 
		hbarx_minus * get_velocity(x, z, -1, 0)->x) + 
		(hbarz_plus * get_velocity(x, z, 0, 1)->z -
		hbarz_minus * get_velocity(x, z, 0, -1)->z));
	return dhdt;
}


//test eq.5 
float WaterMass::get_hbar_x(int x, int z, int x_offset, int z_offset)
{   
	
	float hbar = at(x, z)->x;
	if (get_velocity(x, z, x_offset, z_offset)->x <= 0 &&
		x+x_offset < grid_size_x && x+x_offset >=0) {
		hbar = at(x + x_offset, z)->x;
	}
	
	return hbar;
}

float WaterMass::get_hbar_z(int x, int z, int x_offset, int z_offset)
{
	float hbar = at(x, z)->x;
	if (get_velocity(x, z, x_offset, z_offset)->z <= 0 && 
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

			float deltat = (20.0 / 1000.0);
			if (x >= grid_size_x-1) {
				v_xplus->x = 0;
			}
			else {
				vec4* h_xplus = at(x + 1, z);
				v_xplus->x += -(1-friction_c) * (gravity / scale) * deltat *
					((h_xplus->x + h_xplus->y) - (h->x + h->y));
				*v_xplus *= (1 - friction_c);
			}

			
			if (z >= grid_size_z - 1) {
				v_zplus->z = 0;
			}
			else {
				vec4* h_zplus = at(x, z + 1);
				v_zplus->z += -(gravity / scale) * deltat *
					((h_zplus->x + h_zplus->y) - (h->x + h->y));
				*v_zplus *= (1 - friction_c);
			}
		}
	}
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
