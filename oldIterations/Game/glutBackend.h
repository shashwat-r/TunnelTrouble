#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

float   framerate = 120.0,
        eyePos[3] = {0.0, -0.8, 0.0},
        center[3] = {0.0, 0.0, -10.0},
        up[3] = {0.0, 1.0, 0.0},
        globalZ = 0,
        zLength = 1.0;

int     windowWidth = 600,
        windowHeight = 600,
        windowX = 0,
        windowY = 0,
        initialDelay = 0.0,
        showMenu = 1,
        level;

        int     tunnelSides = 12,
        tunnelUnitsCount = 50,
        obstacleRarity;

GLint   obstacleTexture;

float tunnelCurve(float);
int levelToRarity(int);
float randObstaclePosition(int);

map<char, bool> isPressed;

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

deque<tunnelUnitInfo> dq;
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
    glutPostRedisplay();
}

void handleKeyrelease(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = false;
    if(key == 27) exit(0);
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
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
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

void handleMouse(int button, int state, int x, int y) {
    if(showMenu == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        showMenu = 0;
        if(y < 200) level = 1;
        else if(y > 400) level = 3;
        else level = 2;
        initPerspectiveAndCamera();
        glutKeyboardFunc(handleKeypress);
        glutKeyboardUpFunc(handleKeyrelease);
        glutTimerFunc(initialDelay, handleTimer, 0);
        for(int i = 0; i < tunnelUnitsCount + 1; i++) {
            obstacleRarity = levelToRarity(level);
            int oP = randObstaclePosition(obstacleRarity);
            tunnelUnitInfo tui(tunnelCurve(globalZ), 0.0, globalZ, oP);
            dq.push_back(tui);
            globalZ -= zLength;
        }
    }
    glutPostRedisplay();
}

void initCallbacks() {
    glutReshapeFunc(handleResize);
    glutDisplayFunc(handleDraw);
    glutMouseFunc(handleMouse);
}

GLuint LoadBMP(const char *fileName)
{
    FILE *file;
    unsigned char header[54],*data;
    unsigned int dataPos,size,width, height;
    file = fopen(fileName, "rb");
    fread(header, 1, 54, file);
    dataPos = *(int*)&(header[0x0A]);
    size = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);
    if (size == 0) size = width * height * 3;
    if (dataPos == 0) dataPos = 54;
    data = new unsigned char[size];
    fread(data, 1, size, file);
    fclose(file);
    GLuint texture;
    glGenTextures(1, &texture); 
    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data); 
    return texture;
}

void glutBegin(int argc, char **argv) {
    glutInit(&argc,argv);
    initWindow();
    // initPerspectiveAndCamera();
    initCallbacks();
    
    obstacleTexture = LoadBMP("wall.bmp");
    glutMainLoop();
}

