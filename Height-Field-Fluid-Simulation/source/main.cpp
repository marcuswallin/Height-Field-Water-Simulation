
#include "MicroGlut.h"

#include "display/display.h"
#include <Windows.h>

using namespace std;

void timer(int i)
{   
	glutTimerFunc(time_diff, &timer, i);
	glutPostRedisplay();
}


int main(int argc, char *argv[])
{

	time_diff = 20;

	StartCounter();

	AllocConsole();
	freopen("CONOUT$", "a", stdout);

	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(1200, 900);
	glutCreateWindow ((char *)"Water Simulation");
#ifdef WIN32
	glewInit();
#endif
	glutDisplayFunc(display); 


	glutTimerFunc(time_diff, &timer, time_diff);
	init ();
	glutMainLoop();
	exit(0);
}
