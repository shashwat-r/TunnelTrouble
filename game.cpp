#include<iostream>
#include<math.h>
#include<GL/glut.h>
#include<vector>
#include<deque>
#include<map>
using namespace std;

float globalvar = 0.3, globaldiff = 0.1; // for debugging purposes only
float viewDist = 10.0, xl = 0.0, yl = 0.0, zl = 10.0, initAngle = 0.0;
std::deque<float> dq;

void timer(int)
{
    /* update animation */
    for(int i = 0; i < dq.size(); i++) {
        dq[i] += 0.1;
    }
    // for(int i = 0; i < dq.size(); i++) {
    //     cout<<dq[i]<<" ";
    // }
    // cout<<endl;
    while(dq[0] >= 25.0) {
        dq.push_back(dq[dq.size() - 1] - 1 - 1);
        dq.pop_front();
    }
    glutPostRedisplay();
    glutTimerFunc(1000.0/60.0, timer, 0);
}
map<char, bool> isPressed;
void handleKeypress(unsigned char key, int x, int y) {    //The current mouse coordinates
    switch (key) {
        case 27: //Escape key
            exit(0);
        break;
        case 's':
            if(!isPressed['s']) timer(0);
            isPressed['s'] = 1;
        break;
        case 'a':
            isPressed['a'] = 1;
        break;
        case 'd':
            isPressed['d'] = 1;
        break;
        case 'm':
            globalvar += globaldiff;
        break;
        case 'n':
            globalvar -= globaldiff;
        break;
    }
    glutPostRedisplay();
}
void handleKeyrelease(unsigned char key, int x, int y) {    //The current mouse coordinates
    switch (key) {
        case 'a':
            isPressed['a'] = 0;
        break;
        case 'd':
            isPressed['d'] = 0;
        break;
    }
    glutPostRedisplay();
}
void wall(float xWidth, float yThickness, float zLength) {
    glPushMatrix();
    glTranslatef(0, yThickness/2, 0);
    glScalef(xWidth, yThickness, zLength);
    glutSolidCube(1.0);
    glPopMatrix();
}

void octagonRoom(float zPos, float initAngle) {
    int sides = 16;
    float angle = 360.0/(float)sides;
    float length = 1.0;
    float thickness = 0.05;
    float width = 0.5;
    float radius = (width/2)/tan((angle/2)*(3.14159/180));
    float obstacleHeight = globalvar;
    vector<bool> obstacleExists(sides, false);
    obstacleExists[0] = obstacleExists[4] = 1;
    for(int i = 0; i < sides; i++) {
        if (!obstacleExists[i]) {
            glPushMatrix();
                glRotatef(-angle*i + initAngle, 0.0, 0.0, 1.0);
                glTranslatef(0.0, radius, zPos);
                float colorinput = (i%2+1)*0.2;
                glColor3f(colorinput, colorinput, colorinput);
                wall(width, thickness, length);
            glPopMatrix();
        }
    }
    for(int i = 0; i < sides; i++) {
        if (obstacleExists[i]) {
            glPushMatrix();
                glRotatef(-angle*i + initAngle, 0.0, 0.0, 1.0);
                glTranslatef(0.0, radius - obstacleHeight, zPos);
                float colorinput = (i%2+1)*0.2;
                glColor3f(1.0, 0.0, 0.0);
                wall(width, thickness + obstacleHeight, length);
            glPopMatrix();
        }
    }
}

void draw() {
    if(isPressed['a']) initAngle += 1.0;
    if(isPressed['d']) initAngle -= 1.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.333, 0.01, 100);
    gluLookAt(0.0, -1.0, 20.0,       0.0, 0.0, 0.0,     0.0, 1.0, 0.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for(int i = 0; i < dq.size(); i++) {
        octagonRoom(dq[i], initAngle);
    }

    glFlush();
}

int main(int argc,char **argv)
{
    for(int i = 0; i < 10; i++) {
        dq.push_back((float)(-2.0*i));
    }
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Tunnel Trouble");
    glutDisplayFunc(draw);
    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeyrelease);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1,0.1,0.1,0.0);
    glViewport(0,0,640,480);
    glutMainLoop();
}
