//header files
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

//globals
float   framerate = 120.0,
		eyePos[3] = {0.0, -0.0, 0.0},
		center[3] = {0.0, 0.0, -10.0},
		up[3] = {0.0, 1.0, 0.0},
        zLength = 1.0,
        tunnelAngle = 0.0,
        tunnelSpeed = 0.1,
        piby180 = 3.14/180,
        sceneX,
        sceneY,
        division = 0.0,
        sceneDeltaX,
        sceneDeltaY,
        sceneAngleX,
        sceneAngleY,
        sceneDeltaAngleX,
        sceneDeltaAngleY,
        obstacleHeight = 0.5,
        light0_X = 0.0,
        light0_Y = 0.0,
        light0_Z = 0.0,
        obstacleThickness = 0.1,
        sceneRotate = 0.0,
		globalZ = 0;

int     windowWidth = 600,
		windowHeight = 600,
		windowX = 0,
		windowY = 0,
		initialDelay = 0.0,
		showMenu = 1,
		level,
		tunnelSides = 12,
		tunnelUnitsCount = 51,
		obstacleRarity;

//function declarations
void initPerspectiveAndCamera();
void handleTimer(int);
float randObstaclePosition(int);
float tunnelCurve(float);
void drawGame();
void drawMenu();
void initGameAndControls();

//data structures
struct tunnelUnitInfo {
    float   centerX,
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
map<char, bool> isPressed;
deque<tunnelUnitInfo> dq;

//useful functions
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

int levelToRarity(int level) {
    if(level == 1) return 6;
    if(level == 2) return 3;
    if(level == 3) return 2;
}

void drawQuad(float x0, float y0, float x2, float y2) {
	glBegin(GL_QUADS);
    glVertex3f(x0, y0, 0.0);
    glVertex3f(x2, y0, 0.0);
    glVertex3f(x2, y2, 0.0);
    glVertex3f(x0, y2, 0.0);
    glEnd();
}

void printString(string str, float x, float y, float z) {
	glRasterPos3f(x, y, z);
	for (int i = 0; i < str.size(); i ++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}

void resetAll() {
    system("killall paplay");
    system("paplay music.wav &");
    sceneRotate = 0.0;
    globalZ = 0;
    dq.clear();
    for(int i = 0; i < tunnelUnitsCount; i++) {
        obstacleRarity = levelToRarity(level);
        int oP = randObstaclePosition(obstacleRarity);
        tunnelUnitInfo tui(tunnelCurve(globalZ), 0.0, globalZ, oP);
        dq.push_back(tui);
        globalZ -= zLength;
    }
}

//handlers
void handleKeypress(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = true;
    if(key == 27) {
        system("killall paplay");
        exit(0);
    }
    if(key == 'r') resetAll();
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

void handleMouse(int button, int state, int x, int y) {
    if(showMenu == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        showMenu = 0;
        level = (y/(windowHeight/3)) + 1;
        initGameAndControls();
    }
    glutPostRedisplay();
}
