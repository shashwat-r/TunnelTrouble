//headers
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

//variables
float 	globalZ = 0,
		division = 0.0,
		framerate = 120.0,
		zLength = 1.0,
		tunnelAngle = 0.0,
		tunnelSpeed = 0.1,
		piby180 = 3.14/180,
		eyePos[3] = {0.0, -1.0, 0.0},
		center[3] = {0.0, 0.0, -10.0},
		up[3] = {0.0, 1.0, 0.0},
		sceneRotate = 0.0,
		sceneX,
		sceneDeltaX,
		sceneAngle,
		sceneDeltaAngle,
		obstacleHeight = 0.5,
		light0_X = 0.0,
		light0_Y = 0.0,
		light0_Z = 0.0,
		obstacleThickness = 0.1;

int 	windowWidth = 600,
		windowHeight = 600,
		windowX = 0,
		windowY = 0,
		initialDelay = 0.0,
		tunnelSides = 18,
		tunnelUnitsCount = 50,
		obstacleRarity = 2;

//data structures
struct tunnelUnitInfo {
	float 	centerX,
			centerY,
			centerZ;
	int obstaclePos;
	tunnelUnitInfo(float cX, float cY, float cZ, int oP) {
		centerX = cX;
		centerY = cY;
		centerZ = cZ;
		obstaclePos = oP;
	}
};
std::deque<tunnelUnitInfo> dq;
map<char, bool> isPressed;

//function declarations
void draw();
void timerFunc(int);
bool detectCollision(int);

//input handlers
void handleKeypress(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = true;
    if (key == 27) exit(0);
    glutPostRedisplay();
}

void handleKeyrelease(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = false;
    glutPostRedisplay();
}

void handleResize(int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective	
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0, (float)newWidth/(float)newHeight, 0.01, 1000);
    gluLookAt(eyePos[0], eyePos[1], eyePos[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}

void adjustAngle(float &angle) {
	if(angle < 0) angle += 360.0;
	if(angle >= 360) angle -= 360.0;
}

bool checkLinearlyIncreasing(float a, float b, float c) {
	return (a <= b && b <= c);
}

bool checkCircularlyIncreasing(float a, float b, float c) {
	return (checkLinearlyIncreasing(a, b, c) || (checkLinearlyIncreasing(0, b, c) && a > c) || (checkLinearlyIncreasing(a, b, 359) && c < a)); 
}