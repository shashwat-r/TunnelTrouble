//header files
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

//globals
float   framerate,
        eyePos[3] = {0.0, -0.8, 0.0},
        center[3] = {0.0, 0.0, -10.0},
        up[3] = {0.0, 1.0, 0.0},
        tunnelSpeed = 0.1,
        piby180 = 3.14/180,
        sceneX,
        sceneY,
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
        division = 0.0,
        sceneRotate = 0.0,
        tunnelAngle = 0.0,
        globalZ = 0,
        zLength = 1.0;
        

int     windowWidth = 600,
        windowHeight = 600,
        windowX = 0,
        windowY = 0,
        initialDelay = 0.0,
        showMenu = 1,
        showGameOver = 0,
        level,
        tunnelSides = 12,
        tunnelUnitsCount = 51,
        obstacleRarity;

GLint   tunnelTexture[3],
        obstacleTexture,
        gameOverTexture,
        menuTexture;


//function declarations
void initPerspectiveAndCamera();
void handleTimer(int);
float randObstaclePosition(int);
float tunnelCurve(float, float (*)(float));
void drawGame();
void drawMenu();
void initGameAndControls();
void initDeque();
void showScore(string);

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
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
    delete [] data;
    return texture;
}

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

void handleNothing(int a, int b, int c) {}
void handleNothing(unsigned char a, int b, int c) {}
void handleNothing(int) {}

void drawQuad(float x0, float y0, float x2, float y2) {
	glBegin(GL_QUADS);
    glVertex3f(x0, y0, 0.0);
    glVertex3f(x2, y0, 0.0);
    glVertex3f(x2, y2, 0.0);
    glVertex3f(x0, y2, 0.0);
    glEnd();
}

void drawMenuQuad(float x0, float y0, float x2, float y2, int levelNo) {
    float xratio = (1.0/3)*(float)windowWidth/(float)windowHeight;
    float levelUp = 1.0f - (float)(levelNo-1)/3;
    float levelDown = 1.0f - (float)(levelNo)/3;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, menuTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.5f-xratio/2, levelUp); glVertex3f(x0, y0, 0.0);
        glTexCoord2f(0.5f+xratio/2, levelUp); glVertex3f(x2, y0, 0.0);
        glTexCoord2f(0.5f+xratio/2, levelDown); glVertex3f(x2, y2, 0.0);
        glTexCoord2f(0.5f-xratio/2, levelDown); glVertex3f(x0, y2, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawGameOverQuad(float x0, float y0, float x2, float y2) {
    float xratio = (0.6)*(float)windowWidth/(float)windowHeight;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gameOverTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.5f-xratio/2, 1.0f); glVertex3f(x0, y0, 0.0);
        glTexCoord2f(0.5f+xratio/2, 1.0f); glVertex3f(x2, y0, 0.0);
        glTexCoord2f(0.5f+xratio/2, 0.0f); glVertex3f(x2, y2, 0.0);
        glTexCoord2f(0.5f-xratio/2, 0.0f); glVertex3f(x0, y2, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void printString(string str, float x, float y, float z) {
    glRasterPos3f(x, y, z);
    for (int i = 0; i < str.size(); i ++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
}

void makeMenu() {
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    drawMenuQuad(-1.0, 1.0, 1.0, 1.0/3, 1);
    drawMenuQuad(-1.0, 1.0/3, 1.0, -1.0/3, 2);
    drawMenuQuad(-1.0, -1.0/3, 1.0, -1.0, 3);
    glColor3f(0.1, 0.1, 0.1);
    glEnable(GL_LIGHTING);
}

void makeGameOver() {
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    drawGameOverQuad(-1.0, 1.0, 1.0, -1.0);
    printString("SCORE: " + to_string(-(int)globalZ - 50), -0.2, -0.2, -1.0);
    glColor3f(0.1, 0.1, 0.1);
    glEnable(GL_LIGHTING);
}

void resetAll() {
    division = 0.0;
    sceneRotate = 0.0;
    tunnelAngle = 0.0;
    zLength = 1.0;
    dq.clear();
    isPressed.clear();
    showGameOver = 1;
    glutKeyboardFunc(handleNothing);
    glutKeyboardUpFunc(handleNothing);
    glutSpecialFunc(handleNothing);
    glutSpecialUpFunc(handleNothing);
    system("amixer -D pulse sset Master 70%");
}

//handlers
void handleKeypress(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = true;
    if(key == 27) exit(0);
    glutPostRedisplay();
}

void handleKeyrelease(unsigned char key, int x, int y) {
    if (key >= 'a' && key <= 'z') isPressed[key] = false;
    glutPostRedisplay();
}

void handleSpecialKeypress(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) isPressed['a'] = true;
    if (key == GLUT_KEY_RIGHT) isPressed['d'] = true;
    if (key == GLUT_KEY_UP) isPressed['w'] = true;
    if (key == GLUT_KEY_DOWN) isPressed['s'] = true;
    glutPostRedisplay();
}

void handleSpecialKeyrelease(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) isPressed['a'] = false;
    if (key == GLUT_KEY_RIGHT) isPressed['d'] = false;
    if (key == GLUT_KEY_UP) isPressed['w'] = false;
    if (key == GLUT_KEY_DOWN) isPressed['s'] = false;
    glutPostRedisplay();
}

void handleResize(int newWidth, int newHeight) {
    windowWidth = newWidth; windowHeight = newHeight;
    glViewport(0, 0, newWidth, newHeight);
    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective 
    glLoadIdentity(); //Reset the camera
    gluPerspective(45.0, (float)newWidth/(float)newHeight, 0.01, 1000);
    gluLookAt(eyePos[0], eyePos[1], eyePos[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    // cout << newWidth << " " << newHeight << endl;
}

void handleMouse(int button, int state, int x, int y) {
    if(showMenu == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        showMenu = 0;
        globalZ = 0;
        level = (y/(windowHeight/3)) + 1;
        if(level == 1) framerate = 60;
        else framerate = 120;
        handleResize(windowWidth, windowHeight);
        initGameAndControls();
    }
    if(showGameOver == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        showGameOver = 0;
        showMenu = 1;
        // system("killall paplay");
        handleResize(windowWidth, windowHeight);
        glutDisplayFunc(drawMenu);
    }

    glutPostRedisplay();
}