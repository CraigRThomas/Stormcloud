#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <gl/glext.h>
#include <il/il.h>
#include <il/ilut.h>

#include "inputMgr.h"
#include "shaderMgr.h"
#include "object.h"
#include "quaternion.h"

LARGE_INTEGER ticksPerSecond, tick1, tick2;
float lastStepTime, stepTime, accumulator, t, dt, ts = 0.f;
const float deltaTime = 10.0f, timeScale = 1.0;
bool isPaused = false;
float yRotation = 0;
float xRotation = 0;
Vector3f translation(0,0,-20);
Object obj;
Matrix projectionMat(4,4);

void mouseWrap(int b, int s, int x, int y);
void mouseMoveWrap(int x, int y);
void keydownWrap(int k, int x, int y);
void keyupWrap(int k, int x, int y);
void keydownWrap(unsigned char k, int x, int y);
void keyupWrap(unsigned char k, int x, int y);
void windowResizeHandler(int w, int h);
void redraw();
void display();

void glutStep(){
	ts = timeScale;								// Are we paused?
	dt = deltaTime*ts;							// Scale time by timescale, allows for slow motion/sped up/paused.
	QueryPerformanceFrequency(&ticksPerSecond); //save tick rate (timer resolution)
	QueryPerformanceCounter(&tick2);			//save second tick

	stepTime = (tick2.QuadPart - tick1.QuadPart) * 1000.0 / ticksPerSecond.QuadPart; //calc how long the last step took

	QueryPerformanceCounter(&tick1);			//save first tick

	accumulator += stepTime*ts;
	
	if (InputMgr.keys['a']){
		yRotation-=0.2;
	}
	if (InputMgr.keys['d']){
		yRotation+=0.2;
	}
	if (InputMgr.keys['w']){
		xRotation-=0.2;
	} 
	if (InputMgr.keys['s']){
		xRotation+=0.2;
	}

	if (InputMgr.keys['q']){
		exit(0);
	}
	while (accumulator > dt){ //Runge-Kutta Integration
		t += dt;
		accumulator -= dt;
		obj.update(dt);
	}
	ShaderMgr.update();
	InputMgr.update();

	redraw();
}

void redraw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Quaternion q = rotationQuat(yRotation,Vector3f(0,1,0)) * rotationQuat(xRotation,Vector3f(1,0,0));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0,0,-15);
	//std::cout<<yRotation<<" "<<xRotation<<std::endl;
	q.applyRotation();

	Matrix t(4,4);
	/*float data[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,data);
	t.populate(data);
	t = projectionMat * t;*/
	t.homogeneous3D(xRotation,yRotation,0,translation);
	t = projectionMat ;//* t;

	obj.draw(t);

	glutSwapBuffers();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lavaflow");
	glClearColor(0,0,0,1.0f);

	glutDisplayFunc(glutStep);
	glutIdleFunc(glutStep);
	glutReshapeFunc(windowResizeHandler);

	glutKeyboardFunc(keydownWrap);
	glutKeyboardUpFunc(keyupWrap);
	glutSpecialFunc(keydownWrap);
	glutSpecialUpFunc(keyupWrap);
	glutMouseFunc(mouseWrap);
	glutPassiveMotionFunc(mouseMoveWrap);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	InputMgr;
	ShaderMgr;
	obj.loadFromFile("models/manta.mesh");

	QueryPerformanceCounter(&tick1); //record the first tick just before we start
	
	glutWarpPointer(400,300);
	glutMainLoop();
}

void windowResizeHandler(int w, int h){

	float aspect = 4.f/3.f;
	float zFar = 1000.f;
	float zNear = 1.f;
	float fovy = 60.f;

	float top = tan(fovy / 360.f * 3.1415) * zNear; 
	float bottom = -top;
	float right = top * aspect;
	float left = -right;

	projectionMat.populate(0,0,zNear/(right));
	projectionMat.populate(0,1,0);
	projectionMat.populate(0,2,0);
	projectionMat.populate(0,3,0);

	projectionMat.populate(1,0,0);
	projectionMat.populate(1,1,zNear/(top));
	projectionMat.populate(1,2,0);
	projectionMat.populate(1,3,0);

	projectionMat.populate(2,0,0);
	projectionMat.populate(2,1,0);
	projectionMat.populate(2,2,-(zFar+zNear)/(zFar-zNear)); 
	projectionMat.populate(2,3,-(2*zFar*zNear)/(zFar-zNear));

	projectionMat.populate(3,0,0);
	projectionMat.populate(3,1,0);
	projectionMat.populate(3,2,-1);
	projectionMat.populate(3,3,0);
	
	glutPostRedisplay();
	glutStep();
}

void mouseWrap(int b, int s, int x, int y){
	InputMgr.mousebutton(b,s,x,y);
}

void mouseMoveWrap(int x, int y){
	InputMgr.mouseMove(x,y);
}

void keydownWrap(int k, int x, int y){
	InputMgr.speckeydown(k,x,y);
}

void keyupWrap(int k, int x, int y){
	InputMgr.speckeyup(k,x,y);
}

void keydownWrap(unsigned char k, int x, int y){
	InputMgr.keydown(k,x,y);
}

void keyupWrap(unsigned char k, int x, int y){
	InputMgr.keyup(k,x,y);
}