#include <GL/glut.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string.h>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

#define mag 10

//Structure to represent a point
typedef struct
{
	double x;
	double y;
	double z;
}Point;

//Structure to represent a face
typedef struct
{
	Point a;
	Point b;
	Point c;
	int id;
	int colour;
}Face;

vector<Point> points;
vector<Face> faces;

float deltaAngle = 0.0f;
float deltaMove = 0;

// angle of rotation for the camera direction
float angle = 0.0f;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float ox=0.0f, oz=5.0f;
int xOrigin = -1;

void computePos(float deltaMove) {

	ox += deltaMove * lx * 0.1f;
	oz += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void renderScene(void)
{
	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);
	int i;
	vector<Face>::iterator it;

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK,GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Point p;
	glPushMatrix();
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	ox, 1.0f, oz,
				ox+lx, 1.0f,  oz+lz,
				0.0f, 1.0f,  0.0f);
	
	for (i=0;i<faces.size();i++)
	{
		switch(faces[i].colour)
		{
			case 1:
				glColor3f(1,0,0);
				break;
			case 2:	
				glColor3f(0,1,0);
				break;
			case 3:
				glColor3f(0,0,1);
				break;
			case 4:
				glColor3f(1,1,0);
				break;
			case 5:
				glColor3f(1,0.647,0);
				break;
			case 6:	
				glColor3f(0.647,0.1647,0.1647);
				break;
			case 7:
				glColor3f(0.5019,0.5019,0.5019);
				break;
			case 8:
				glColor3f(0.5019,0.5019,0);
				break;
			case 9:
				glColor3f(0.5568,0.2196,0.5568);
				break;
			case 10:	
				glColor3f(0.8039,0.7294,0.5882);
				break;
			case 11:
				glColor3f(0,0.9333,0.9333);
				break;
			case 12:
				glColor3f(0.5019,0,0);
				break;
		}
		
		glBegin(GL_TRIANGLES);
		p = faces[i].a;
		glVertex3f(p.x*mag, p.y*mag, p.z*mag);
		p = faces[i].b;
		glVertex3f(p.x*mag, p.y*mag, p.z*mag);
		p = faces[i].c;
		glVertex3f(p.x*mag, p.y*mag, p.z*mag);
		glEnd();
	}

	glPopMatrix();
    glutSwapBuffers();
	glFlush();
}

void processNormalKeys(unsigned char key, int xx, int yy)
{ 	
        if (key == 27)
              exit(0);
} 

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void pressKey(int key, int xx, int yy)
{
	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

void mouseMove(int x, int y) { 	

         // this will only be true when the left button is down
         if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

int main(int argc, char** argv)
{
	int i;
	ifstream infile("colour.obj");
	Point p;
	char** endptr;
	Face f;
	char comm[100];
	char co[100];
	string line; 

	//Reading the obj file
	while(getline( infile, line ))
	{
		istringstream iStrStream(line);
		iStrStream >> comm;
		if(comm[0] == 'v')
		{
			iStrStream >> p.x >> p.y >> p.z ;
			points.push_back(p);
		}
		else if(strcmp(comm,"usemtl") == 0)
		{
			iStrStream >> co;
			if(strcmp(co,"RED") == 0)
				f.colour = 1;
			if(strcmp(co,"GREEN") == 0)
				f.colour = 2;
			if(strcmp(co,"BLUE") == 0)
				f.colour = 3;
			if(strcmp(co,"YELLOW") == 0)
				f.colour = 4;
			if(strcmp(co,"ORANGE") == 0)
				f.colour = 5;
			if(strcmp(co,"BROWN") == 0)
				f.colour = 6;
			if(strcmp(co,"GRAY") == 0)
				f.colour = 7;
			if(strcmp(co,"OLIVE") == 0)
				f.colour = 8;
			if(strcmp(co,"BEET") == 0)
				f.colour = 9;
			if(strcmp(co,"WHEAT") == 0)
				f.colour = 10;
			if(strcmp(co,"CYAN") == 0)
				f.colour = 11;
			if(strcmp(co,"MAROON") == 0)
				f.colour = 12;
		}
		else if(comm[0] == 'f')
		{
			int a,b,c;
			iStrStream >> a >> b >> c;
			
			f.a = points[a-1];
			f.b = points[b-1];
			f.c = points[c-1];
			faces.push_back(f);
		}
	}

	glutInit(&argc, argv);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("GLUT");

	glutDisplayFunc(renderScene);	
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutMainLoop();

	return 0;
} 
