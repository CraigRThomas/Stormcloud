#pragma comment(lib,"inc/DevIL.lib")
#pragma comment(lib,"inc/ILUT.lib")
#pragma comment(lib,"inc/fmodex_vc.lib")

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
Shader* vertexShdr, *pixelShdr;
Object obj;

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
	pixelShdr->send("shininess",1500.f); //1500 seems to be high enough to prevent any shininess
	ShaderMgr.update();
	InputMgr.update();

	redraw();
}

void redraw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Quaternion q = rotationQuat(yRotation,Vector3f(0,1,0)) * rotationQuat(xRotation,Vector3f(1,0,0));
	glTranslatef(0,0,-20);
	q.applyRotation();

	obj.draw();

	glutSwapBuffers();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Stormcloud");
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
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float amb[] = {0.3,0.3,0.3};
	float dif[] = {1.0,1.0,1.0};
	glLightfv(GL_LIGHT0,GL_AMBIENT,dif);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,dif);
	glLightfv(GL_LIGHT0,GL_SPECULAR,dif);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	ilInit();
	ilutInit();

	InputMgr;
	ShaderMgr;
	obj.loadFromFile("models/manta.mesh");

	vertexShdr = ShaderMgr.createShader(ShaderType::VERTEX,"shaders/normalVS.glsl");
	pixelShdr = ShaderMgr.createShader(ShaderType::FRAGMENT,"shaders/normalPS.glsl");
	ShaderMgr.push(vertexShdr);
	ShaderMgr.push(pixelShdr);
	ShaderMgr.checkInvalid();

	QueryPerformanceCounter(&tick1); //record the first tick just before we start
	
	glutWarpPointer(400,300);
	glutMainLoop();
}

void windowResizeHandler(int w, int h){
	int vw = 0, vh = 0, vx = 0, vy = 0;
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	if(w*3<h*4){
		vw = w;
		vh = w*(3.f/4.f);
		vy = h/2.f-vh/2;
		glViewport(0, vy, w, vh);
	} else {
		vw = h*(4.f/3.f);
		vh = h;
		vx = w/2.f-vw/2.f;
		glViewport(vx, 0, vw, h);
	}

	glScissor(vx,vy,vw,vh);

	gluPerspective(60, 4.f/3.f, 1.0, 1000.0);
	
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