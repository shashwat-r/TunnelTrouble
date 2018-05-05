#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

float   framerate = 120.0,
        eyePos[3] = {0.0, -1.0, 0.0},
        center[3] = {0.0, 0.0, -10.0},
        up[3] = {0.0, 1.0, 0.0};

int     windowWidth = 600,
        windowHeight = 600,
        windowX = 0,
        windowY = 0,
        initialDelay = 0.0;

map<char, bool> isPressed;

void initWindow() {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(windowX, windowY);
    glutCreateWindow("Tunnel Trouble");
    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);
}

void initPerspectiveAndCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, windowWidth/windowHeight, 0.01, 1000);
    gluLookAt(eyePos[0], eyePos[1], eyePos[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}

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

void draw();
void handleDraw() {
    glClearColor(0.1,0.1,0.1,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw();
}

void redraw();
void handleTimer(int)
{
    redraw();
    glutPostRedisplay();
    glutTimerFunc(1000.0/framerate, handleTimer, 0);
}

void initCallbacks() {
	glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeyrelease);
    glutReshapeFunc(handleResize);
    glutDisplayFunc(handleDraw);
    glutTimerFunc(initialDelay, handleTimer, 0);
}

void glutBegin(int argc, char **argv) {
    glutInit(&argc,argv);
    initWindow();
    initPerspectiveAndCamera();
    initCallbacks();
    glutMainLoop();
}
