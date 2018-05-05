#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

float globalZ = 0;

float eye[3] = {0.0, 0.0, 0.0}, center[3] = {0.0, 0.0, -0.01}, up[3] = {0.0, 1.0, 0.0};

void draw();
float lx = 0.0, ly = 0.0, lz = 15, zLength = 1, tunnelAngle = 0.0, piby180 = 3.14/180;
std::deque<pair<float, float> > dq;
map<char, bool> isPressed;

void handleKeypress(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = true;
    if (key == 27) exit(0);
    glutPostRedisplay();
}

void handleKeyrelease(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = false;
    glutPostRedisplay();
}

void wall(float xWidth, float zLength = 100) {
    float miniSquares = 5;
    for(float z = -zLength/2; z < zLength/2; z += zLength/miniSquares) {
        for(float x = -xWidth/2; x < xWidth/2; x += xWidth/miniSquares) {
            glBegin(GL_QUADS);
                glNormal3f(0.0, -1.0, 0.0);
                glVertex3f(x, 0, z);
                glVertex3f(x + xWidth/miniSquares, 0, z);
                glVertex3f(x + xWidth/miniSquares, 0, z + zLength/miniSquares);
                glVertex3f(x, 0, z + zLength/miniSquares);
            glEnd();
        }
    }
}

void room() {
    float sides = 16, thickness = 0.05, width = 0.5;
    float angle = 360.0/sides, radius = (width/2)/tan((angle/2)*(3.14159/180));    
    for(int i = 0; i < sides; i++) {
        glPushMatrix();
        glRotatef(-angle*i, 0.0, 0.0, 1.0);
        glTranslatef(0.0, radius, 0);
        glColor3f(0.5*(i%2), 0.5*(i%2), 0.5*(i%2));
        wall(width, zLength);
        glPopMatrix();
    }
}

float tunnelCurve(float zPos) {
    return 2*sin(zPos/(piby180 * 1000));
    // return zPos/20;
}

float tunnelDerivativeCurve(float zPos) {
    return atan(5*cos(zPos/(piby180 * 1000))/(piby180 * 1000))/piby180;
    // return zPos/20;
}

void timer(int)
{
    for(int i = 0; i < dq.size(); i++) {
        dq[i].first += 0.1;
    }
    if(dq[0].first >= 0.0) {
        float zPos = dq[dq.size() - 1].first - zLength;
        dq.push_back( {zPos, tunnelCurve(globalZ)} );
        dq.pop_front();
        globalZ -= zLength;
    }
    glutPostRedisplay();
    glutTimerFunc(1000.0/120.0, timer, 0);
}

void initEnv() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.333, 0.01, 1000);
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw();
    glFlush();
}

void glutSabKuch(int argc, char **argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Tunnel Trouble");
    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeyrelease);
    glutDisplayFunc(initEnv);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1,0.1,0.1,0.0);
    glViewport(0,0,640,480);
    glutTimerFunc(25, timer, 0);
    glutMainLoop();
}