#include "glut.h"
#include "main.h"

void draw() {
	drawKinectData();
	glutSwapBuffers();
}

void execute() {
	glutMainLoop();
}

bool init(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("NAI 2.0 - Object Detection");
	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	return true;
}