#include<iostream>
#include<math.h>
#include<GL/glut.h>
#include<vector>
#include<deque>
#include<map>
using namespace std;

float globalvar = 0.3, globaldiff = 0.1; // for debugging purposes only
float lx = 0.0, ly = 0.0, lz = 15, zLength = 2, tunnelAngle = 0.0;
std::deque<float> dq;
map<char, bool> isPressed;

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
        dq.push_back(dq[dq.size() - 1] - zLength - 1);
        dq.pop_front();
    }
    glutPostRedisplay();
    glutTimerFunc(1000.0/60.0, timer, 0);
}
void handleKeypress(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = true;
    if (key == 27) exit(0);
    if (key == 'p') if(!isPressed['p']) {timer(0); isPressed['p'] = true;}
    glutPostRedisplay();
}

void handleKeyrelease(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = false;
    glutPostRedisplay();
}

void wall(float xWidth) {
    glPushMatrix();
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
    glPopMatrix();
}

void octagonRoom(float zPos, float initAngle) {
    float sides = 16, thickness = 0.05, width = 0.5;
    float angle = 360.0/sides, radius = (width/2)/tan((angle/2)*(3.14159/180));    
    for(int i = 0; i < sides; i++) {
        glPushMatrix();
            glRotatef(-angle*i + initAngle, 0.0, 0.0, 1.0);
            glTranslatef(0.0, radius, zPos);
            float colorinput = 0.8; glColor3f(colorinput, colorinput, colorinput);
            wall(width);
        glPopMatrix();
    }
}
void draw() {
    int speed = 5;
    if(isPressed['a']) lx -= 0.01 * speed;
    if(isPressed['d']) lx += 0.01 * speed;
    if(isPressed['w']) ly += 0.01 * speed;
    if(isPressed['s']) ly -= 0.01 * speed;
    if(isPressed['b']) lz += 0.1 * speed;
    if(isPressed['f']) lz -= 0.1 * speed;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.333, 0.01, 1000);
    gluLookAt(0.0, 0.0, 0.0,       0.0, 0.0, -10.0,     0.0, 1.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPos0[] = {lx, ly, lz, 1.0f};
    // glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);

    // GLfloat lightColor1[] = {0.2f, 0.2f, 0.2f, 1.0f};
    // GLfloat lightPos1[] = {1, 1, 1, 0.0f};
    // // glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor0);
    // // glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    // // glLightfv(GL_LIGHT1, GL_AMBIENT, lightColor1);
    // glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    // // glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1.0);

    GLfloat mat_color[] = {0.0, 1.0, 0.0, 1.0};
    // GLfloat shine_color[] = {10.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_color);
    // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_color);
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color);
    // glMaterialfv(GL_FRONT, GL_SHININESS, shine_color);
    for(int i = 0; i < dq.size(); i++) {
        octagonRoom(dq[i], tunnelAngle);
    }
    glPushMatrix();
    glTranslatef(lx, ly, lz);
    glutSolidCube(-0.1);
    glPopMatrix();

    glFlush();
}

int main(int argc,char **argv)
{
    int octagonCount = 50;
    for(int i = 0; i < octagonCount; i++) {
        dq.push_back((float)(-(zLength + 1)*i));
    }
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Tunnel Trouble");
    glutDisplayFunc(draw);
    glutTimerFunc(25, timer, 0);
    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeyrelease);

    // glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // glEnable(GL_LIGHT1);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1,0.1,0.1,0.0);
    glViewport(0,0,640,480);
    glutMainLoop();
}
