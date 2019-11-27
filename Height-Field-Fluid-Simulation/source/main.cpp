// Ingemar's psychedelic teapot 2+
// Rewritten for modern OpenGL
// Teapot model generated with the OpenGL Utilities for Geometry Generation
// (also written by Ingemar) based on the original teapot data.

// Here with comfortable mouse dragging interface!

// C++ version - and this time it is done right! (The previous version got problems interfacing to OpenGL and I made a kludgey hack around it.)
// The only C++ specific functionality I have used is operator overloadnig, but I hope that proves the C++ friendliness.


#include "MicroGlut.h"
// uses framework Cocoa
//#include "GL_utilities.h"

#include "display/display.h"
#include <Windows.h>


using namespace std;


void timer(int i)
{
	glutTimerFunc(1000, &timer, i);
	glutPostRedisplay();
}


int main(int argc, char *argv[])
{
	time_diff = 20;
	//AllocConsole();
	//freopen("CONOUT$", "a", stdout);
	
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(1200, 900);
	glutCreateWindow ((char *)"Water Simulation");
#ifdef WIN32
	glewInit();
#endif
	glutDisplayFunc(display); 

	glutTimerFunc(1000, &timer, 0);
	//glutRepeatingTimer(20);
	init ();
	glutMainLoop();
	exit(0);
}
