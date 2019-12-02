#pragma once

#include "cameracontrol.c"
#include <iostream>
#include "display.h"

using namespace std;
int time_diff;

Model* m, * skybox_model;
GLuint sky_program;
TextureData ground_color_tex, skybox_tex, water_color_tex, grid_tex;

bool calc_water = false;
bool print_time = false;
bool display_grid = false;

//World world{};
World world;

void init(void)
{
	mat4 mx;

	// GL inits
	glClearColor(1.0, 1.0, 1.0, 0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//vec3 start_pos = SetVector(120, 10,140);
	vec3 start_pos = SetVector(40, 20,180);


	int x_size = 60;
	int z_size = 60;
	int water_resolution = 2;

	initControls(start_pos, 0, M_PI / 2);
	
	sky_program = loadShaders("source/shaders/skybox.vert", "source/shaders/skybox.frag");

	// Upload geometry to the GPU:
	m = LoadModelPlus((char*)"objects/teapot.obj");
	skybox_model = LoadModelPlus((char*)"objects/skybox.obj");
	
	world = World("textures/fft-terrain.tga", 
		start_pos.x - x_size/2, start_pos.x + x_size/2, start_pos.z - 30 - z_size/2, 
		start_pos.z - 30 + z_size/2, 0.5 , water_resolution, time_diff);
	
	mat4 projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0);
	mat4 worldToViewMatrix = cameraPlacement();

	glUseProgram(sky_program);
	glUniformMatrix4fv(glGetUniformLocation(sky_program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	//textures-------------------------------------------------------
	glActiveTexture(GL_TEXTURE0);
	LoadTGATexture( "textures/grass.tga", &ground_color_tex);
	glActiveTexture(GL_TEXTURE1);
	LoadTGATexture("textures/SkyBox512.tga", &skybox_tex);
	glActiveTexture(GL_TEXTURE2);
	LoadTGATexture("textures/grid_dots_clipped.tga", &grid_tex);
	glActiveTexture(GL_TEXTURE3);
	LoadTGATexture("textures/water512.tga", &water_color_tex);


	world.terrain.init_program(projectionMatrix, worldToViewMatrix);
	world.water.init_program(&projectionMatrix);
	

	printError("init finished");
}


int old_t;

void display(void)
{
	mat4 model_to_view, cam_matrix, model_world;
	// start
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printError("pre display");
	keyboard_interaction();

	//set up matrices
	cam_matrix = cameraPlacement();
	model_world = IdentityMatrix(); //change this
	model_to_view = cam_matrix * model_world;

	//cout << get_view_pos().x << " "  << get_view_pos().y << " " << get_view_pos().z << endl;

	glUseProgram(sky_program);
	draw_sky_box(&model_to_view);
	glEnable(GL_DEPTH_TEST);

	world.terrain.draw(cam_matrix, model_to_view);
	int start_t = glutGet(GLUT_ELAPSED_TIME);

	// = start_t;
	world.water.draw(cam_matrix, time_diff, calc_water, display_grid);
	int end_t = glutGet(GLUT_ELAPSED_TIME);
	
	if (print_time) {
	//	old_t++;
		cout << "Update freq: " + to_string(start_t-old_t) << endl;
	}
		
	old_t = start_t;
	glutSwapBuffers();
}


void draw_sky_box(const mat4 * mtv_matrix)
{
	glUseProgram(sky_program);
	mat4 skymtv = *mtv_matrix;
	glDisable(GL_DEPTH_TEST);
	
	skymtv.m[3] = 0.0f;
	skymtv.m[7] = 0.0f;
	skymtv.m[11] = 0.0f;
	glUniformMatrix4fv(glGetUniformLocation(sky_program, "mtvMatrix"), 1, GL_TRUE,
		skymtv.m);
	glUniform1i(glGetUniformLocation(sky_program, "tex"), 1);
	DrawModel(skybox_model, sky_program, "inPosition", NULL, "inTexCoord");
}


bool key_is_down = false;

void keyboard_interaction() {
	if (glutKeyIsDown('p') && !key_is_down)
	{
		calc_water = !calc_water;
		key_is_down = true;
	}
	if (glutKeyIsDown('t') && !key_is_down)
	{
		print_time = !print_time;
		key_is_down = true;
	}
	if(glutKeyIsDown('g') && !key_is_down)
	{
		if (display_grid)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		display_grid = !display_grid;
		key_is_down = true;
	}
	if(!glutKeyIsDown('p') && !glutKeyIsDown('t') && !glutKeyIsDown('g') && key_is_down)
		key_is_down = false;
}




//GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 100.0;
//glUniform1f(glGetUniformLocation(program, "t"), t);