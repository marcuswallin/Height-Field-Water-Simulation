
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

void init(void)
{
	mat4 mx;
	TextureData ground_tex;

	// GL inits
	glClearColor(1.0, 1.0, 1.0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);

	// Load and compile shader
	program = loadShaders("source/shaders/terrain.vert", "source/shaders/terrain.frag");
	glUseProgram(program);
	initControls();
	// Upload geometry to the GPU:
	m = LoadModelPlus((char*)"objects/teapot.obj");
	//should use LoadDataToModel
	// End of upload of geometry

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0);
	worldToViewMatrix = cameraPlacement();
	modelToWorldMatrix = IdentityMatrix();

	LoadTGATextureData("textures/fft-terrain.tga", &ground_tex);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);
	LoadTGATextureSimple("textures/maskros512.tga", &ground_color_tex);
	ground_model = GenerateTerrain(&ground_tex);
	printError("init terrain");
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

}





void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total, cam_matrix, model_world;

	printError("pre display");

	glUseProgram(program);
	
	cam_matrix = cameraPlacement();
	model_world = T(0, 0, -4.0);
	total = cam_matrix * model_world;
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam_matrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 100.0;
	glUniform1f(glGetUniformLocation(program, "t"), t);

	cout << to_string_vec3(get_view_pos())  << endl;
	glBindTexture(GL_TEXTURE_2D, ground_color_tex);
	DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(ground_model, program, "inPosition", "inNormal", "inTexCoord");
	glutSwapBuffers();
}
