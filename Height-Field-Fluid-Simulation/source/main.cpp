// Ingemar's psychedelic teapot 2+
// Rewritten for modern OpenGL
// Teapot model generated with the OpenGL Utilities for Geometry Generation
// (also written by Ingemar) based on the original teapot data.

// Here with comfortable mouse dragging interface!

// C++ version - and this time it is done right! (The previous version got problems interfacing to OpenGL and I made a kludgey hack around it.)
// The only C++ specific functionality I have used is operator overloadnig, but I hope that proves the C++ friendliness.

// Compiles and runs under Linux Mint with no warnings using this command-line.
// gcc psychteapot2.cpp ../common/*.c ../common/Linux/*.c -lGL -o psychteapot2 -I../common -I../common/Linux -DGL_GLEXT_PROTOTYPES  -lXt -lX11 -lm

#include "MicroGlut.h"
// uses framework Cocoa
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "cameracontrol.c"
#include <string>
#include <Windows.h>
#include <iostream>

using namespace std;

mat4 projectionMatrix;
mat4 worldToViewMatrix, modelToWorldMatrix;

Model *m;
// Reference to shader program
GLuint program;

void init(void)
{
	mat4 mx;
	
	// GL inits
	glClearColor(1.0,1.0,1.0,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);

	// Load and compile shader
	program = loadShaders("source/shaders/psych.vert", "source/shaders/psych.frag");
	glUseProgram(program);
	initControls();
	// Upload geometry to the GPU:
	m = LoadModelPlus((char *)"objects/billboard.obj");
	//should use LoadDataToModel
	// End of upload of geometry
	
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0);
	worldToViewMatrix = cameraPlacement();//lookAt(0, 1, 8, 0,0,0, 0,1,0);
	modelToWorldMatrix = IdentityMatrix();
	
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

}

//GLfloat a, b = 0.0;

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total, cam_matrix, model_world;
	//

	printError("pre display");

	glUseProgram(program);
	GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 100.0;
	cam_matrix = cameraPlacement();
	//model_world = T(0,0,-4.0);
	model_world = IdentityMatrix();
	total = cam_matrix * model_world;
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam_matrix.m);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glUniform1f(glGetUniformLocation(program, "t"), t);

	vec3 test_vec = total * SetVector(1, 0, 0);
	
	cout << to_string(get_view_pos()) + " total: " + to_string(test_vec)<< endl;

	DrawModel(m, program, (char *)"inPosition", NULL, (char *)"inTexCoord");
	
	glutSwapBuffers();
}


void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}


int main(int argc, char *argv[])
{

	AllocConsole();
	freopen("CONOUT$", "a", stdout);
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(900, 900);
	glutCreateWindow ((char *)"Ingemar's psychedelic teapot 2 c++");
#ifdef WIN32
	glewInit();
#endif
	glutDisplayFunc(display); 
//	glutMouseFunc(mouseUpDown);
	//	glutMotionFunc(mouseDragged);
	//
	//glutPassiveMotionFunc(mouse);
	glutTimerFunc(20, &timer, 0);
	init ();
	glutMainLoop();
	exit(0);
}
