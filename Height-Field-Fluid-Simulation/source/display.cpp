
#pragma once
#include "MicroGlut.h"

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "cameracontrol.c"
#include <iostream>
#include <string>
#include "display.h"
#include "world_generator.h"


using namespace std;

mat4 worldToViewMatrix, modelToWorldMatrix, projectionMatrix;
Model* m, * skybox_model,* ground_model;
GLuint ground_program, sky_program;
TextureData ground_color_tex, skybox_tex;

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

	vec3 start_pos = SetVector(100, 10, 100);
	initControls(start_pos, 0, M_PI / 2);

	// Load and compile shader
	ground_program = loadShaders("source/shaders/terrain.vert", "source/shaders/terrain.frag");
	sky_program = loadShaders("source/shaders/skybox.vert", "source/shaders/skybox.frag");

	// Upload geometry to the GPU:
	m = LoadModelPlus((char*)"objects/teapot.obj");
	skybox_model = LoadModelPlus((char*)"objects/skybox.obj");
	
	world = World("textures/fft-terrain.tga");

	//send matrices
	glUseProgram(ground_program);
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0);
	worldToViewMatrix = cameraPlacement();
	modelToWorldMatrix = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(ground_program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(ground_program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	
	glUseProgram(sky_program);
	glUniformMatrix4fv(glGetUniformLocation(sky_program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	
	//textures
	glActiveTexture(GL_TEXTURE0);
	LoadTGATexture( "textures/grass.tga", &ground_color_tex);
	glActiveTexture(GL_TEXTURE1);
	LoadTGATexture("textures/SkyBox512.tga", &skybox_tex);

	
	printError("init finished");
}




void display(void)
{
	mat4 model_to_view, cam_matrix, model_world;
	
	// start
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printError("pre display");

	//set up matrices
	cam_matrix = cameraPlacement();
	model_world = IdentityMatrix(); //change this
	model_to_view = cam_matrix * model_world;


	glUseProgram(sky_program);
	draw_sky_box(&model_to_view);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(ground_program);
	glUniformMatrix4fv(glGetUniformLocation(ground_program, "camMatrix"), 1, GL_TRUE, cam_matrix.m);
	glUniformMatrix4fv(glGetUniformLocation(ground_program, "mdlMatrix"), 1, GL_TRUE, model_to_view.m);

	//draw models
	glUseProgram(ground_program);
	glUniform1i(glGetUniformLocation(ground_program, "tex"), 0);
	//DrawModel(m, ground_program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(world.terrain_model, ground_program, "inPosition", "inNormal", "inTexCoord");
	//DrawModel(skybox_model, ground_program, "inPosition", "inNormal", "inTexCoord");
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



//GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 100.0;
//glUniform1f(glGetUniformLocation(program, "t"), t);