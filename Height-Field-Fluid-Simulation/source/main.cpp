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
	initControls(1, 1);
	// Upload geometry to the GPU:
	m = LoadModelPlus((char *)"objects/billboard.obj");
	//should use LoadDataToModel
	// End of upload of geometry
	
	projectionMatrix = frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 30.0);
	worldToViewMatrix = lookAt(0, 1, 8, 0,0,0, 0,1,0);
	modelToWorldMatrix = IdentityMatrix();
	
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

}

//GLfloat a, b = 0.0;

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 rot, trans, total, cam_matrix;

	GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 100.0;
	cam_matrix = cameraPlacement();

	trans = T(0, -1, 0); // Move teapot to center it
	total = modelToWorldMatrix;// *cam_matrix;//trans * Rx(-M_PI/2); // trans centers teapot, Rx rotates the teapot to a comfortable default
	
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glUniform1f(glGetUniformLocation(program, "t"), t);

	DrawModel(m, program, (char *)"inPosition", NULL, (char *)"inTexCoord");
	
	glutSwapBuffers();
}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ((char *)"Ingemar's psychedelic teapot 2 c++");
#ifdef WIN32
	glewInit();
#endif
	glutDisplayFunc(display); 
//	glutMouseFunc(mouseUpDown);
	//	glutMotionFunc(mouseDragged);
	glutPassiveMotionFunc(mouse);
	glutRepeatingTimer(20);
	init ();
	glutMainLoop();
	exit(0);
}
