#include <iostream>
#include <Gl/glut.h>
#include <half_edge_structure.h>
#define PI 3.1415926536
#include <GLFW/glfw3.h>
using namespace std;
double xrot = 0.0;
double yrot = 0.0;
double xrotr = 0.0;
double yrotr = 0.0;
bool displaymode = false;
int xp, yp;
bool bdown = false;
Solid* solid1;
Solid* solid2;

void  init1()
{
	//欧拉操作，三棱锥
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	double points[][3] = { { -5.0, 0.0, 0.0 }, { -2.0, 0.0, 0.0 }, { -2.0, 3.0, 0.0 }, { -5.0, 3.0, 0.0 },
	{ -5.0, 3.0, 3.0 }, { -2.0, 0.0, 3.0 }, { 3.0, 3.0, 3.0 }, { 0.0, 3.0, 3.0 },
	{ 3.0, 2.0, 3.0 }, { 2.0, 3.0, 3.0 } };
	solid1 = mvsf(1, 0, points[0]);
	mev(solid1, 1, 0, 1, points[1]);
	mev(solid1, 1, 1, 2, points[2]);
	mef(solid1, 1, 0, 2, 2);
	mev(solid1, 1, 0, 3, points[4]);
	mef(solid1, 1, 1, 3, 3);
	mef(solid1, 1, 2, 3, 4);
	Print(solid1, "BBB-rep.brp");
}
void  init2()
{
	//带有两个孔的扫成
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	double points[][3] = { { 0.0, 0.0, 0.0 }, { 5.0, 0.0, 0.0 }, { 5.0, 3.0, 0.0 }, { 0.0, 3.0, 0.0 },
	{ 1.0, 1.0, 0.0 }, { 2.0, 1.0, 0.0 }, { 2.0, 2.0, 0.0 }, { 1.0, 2.0, 0.0 },
	{ 3.0, 1.0, 0.0 }, { 4.0, 1.0, 0.0 }, { 4.0, 2.0, 0.0 }, { 3.0, 2.0, 0.0 },
	{ 3.0, 2.0, 3.0 }, { 2.0, 3.0, 3.0 } };
	solid2 = mvsf(1, 0, points[0]);
	mev(solid2, 1, 0, 1, points[1]);
	mev(solid2, 1, 1, 2, points[2]);
	mev(solid2, 1, 2, 3, points[3]);
	mef(solid2, 1, 0, 3, 2);

	mev(solid2, 1, 0, 4, points[4]);
	mev(solid2, 1, 4, 5, points[5]);
	mev(solid2, 1, 5, 6, points[6]);
	mev(solid2, 1, 6, 7, points[7]);
	mef(solid2, 1, 4, 7, 3);
	kemr(solid2, 1, 0, 4);

	mev(solid2, 1, 0, 8, points[8]);
	mev(solid2, 1, 8, 9, points[9]);
	mev(solid2, 1, 9, 10, points[10]);
	mev(solid2, 1, 10, 11, points[11]);
	mef(solid2, 1, 8, 11, 4);
	kemr(solid2, 1, 0, 8);
	kfmrh(solid2, 1, 3);
	kfmrh(solid2, 1, 4);
	sweep(solid2, 1, 0, 0, 2);
	Print(solid2, "BBB-rep.brp");
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -13.0);
	glRotatef(xrot + xrotr, 1.0, 0.0, 0.0);
	glRotatef(yrot + yrotr, 0.0, 1.0, 0.0);
	wirepaint(solid1);
	wirepaint(solid2);
	glTranslatef(3.0, 0, 0);
	glutSwapBuffers();
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)w / (double)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			bdown = true;
			xp = x;
			yp = y;
		}
		else
		{
			bdown = false;
			xrot += xrotr;
			yrot += yrotr;
		}
	}
}
void move(int x, int y)
{
	int deltx, delty;
	if (bdown)
	{
		deltx = x - xp;
		delty = y - yp;
		yrotr = ((double)deltx / 10.0 * 180.0 / PI * 0.04);
		xrotr = ((double)delty / 10.0 * 180.0 / PI * 0.04);
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowPosition(400, 250);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("Euler");
	init1();
	init2();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(move);
	glutMainLoop();

	return 0;
}
