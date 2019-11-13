
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

mat4 projectionMatrix;
mat4 worldToViewMatrix, modelToWorldMatrix;
Model* m, *ground_model;
GLuint program;
GLuint ground_color_tex;
vec3 start_pos = SetVector(100, 10, 100);
void init(void)
{
	mat4 mx;
	TextureData ground_tex;

	// GL inits
	glClearColor(1.0, 1.0, 1.0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);

	initControls(start_pos, 0, M_PI / 2);

	// Load and compile shader
	program = loadShaders("source/shaders/terrain.vert", "source/shaders/terrain.frag");
	glUseProgram(program);

	// Upload geometry to the GPU:
	m = LoadModelPlus((char*)"objects/teapot.obj");
	LoadTGATextureData("textures/fft-terrain.tga", &ground_tex);
	ground_model = GenerateTerrain(&ground_tex);


	//send matrices
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0);
	worldToViewMatrix = cameraPlacement();
	modelToWorldMatrix = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	
	//textures
	glUniform1i(glGetUniformLocation(program, "tex"), 0);
	LoadTGATextureSimple("textures/maskros512.tga", &ground_color_tex);
	//probably necessary when more textures
	//glBindTexture(GL_TEXTURE_2D, ground_color_tex);
	
	printError("init finished");
}




void display(void)
{
	mat4 total, cam_matrix, model_world;
	
	// start
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printError("pre display");
	glUseProgram(program);
	
	//send matrices
	cam_matrix = cameraPlacement();
	model_world = IdentityMatrix(); //change this
	total = cam_matrix * model_world;
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam_matrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	//draw models
	DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(ground_model, program, "inPosition", "inNormal", "inTexCoord");
	glutSwapBuffers();
}

//GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 100.0;
//glUniform1f(glGetUniformLocation(program, "t"), t);