// #include "glutBackend.h"
#include "tunnel.h"

void octagonRoomPlacer(tunnelUnitInfo tui) {
    glPushMatrix();
    glTranslatef(tui.centerX, 0.0, tui.centerZ);
    float xTranslated = -tunnelCurve(globalZ + tunnelUnitsCount*zLength); 
    float reqAngle = atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength)) * 180/3.14;
    glRotatef(reqAngle, 0.0, 1.0, 0.0);
    tunnelUnit(tui.obstaclePos);
    glPopMatrix();
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

bool detectCollision(int oP) {
    if (oP == -1) return false;
    float angle = (360/tunnelSides) * (float)oP;
    float error = 3.0;
    float halfRange = 360/(2*tunnelSides) - error;
    float startAngle = angle - halfRange, endAngle = angle + halfRange;
    adjustAngle(startAngle); adjustAngle(endAngle);
    // cout<<startAngle<<" "<<sceneRotate<<" "<<endAngle<<" "<<oP<<endl;
    return checkCircularlyIncreasing(startAngle, sceneRotate, endAngle);
}

void drawFullTunnel() {
    for(int i = 0; i < dq.size(); i++) octagonRoomPlacer(dq[i]);
}

void sceneMovementCalculations() {
    if(isPressed['a']) sceneRotate += 1.0; adjustAngle(sceneRotate);
    if(isPressed['d']) sceneRotate -= 1.0; adjustAngle(sceneRotate);

    sceneX = -tunnelCurve(globalZ + tunnelUnitsCount*zLength); 
    float nextSceneX = -tunnelCurve((globalZ - zLength) + tunnelUnitsCount*zLength);
    sceneDeltaX = (nextSceneX - sceneX)*division;

    sceneAngle = -atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength)) * 180/3.14;
    float nextsceneAngle = -atan(tunnelCurveDerivative((globalZ - zLength) + tunnelUnitsCount*zLength)) * 180/3.14;
    sceneDeltaAngle = ((nextsceneAngle) - (sceneAngle))*division;
}

void lightMovementCalculations() {
    if(isPressed['f']) light0_Z -= 1.0;
    if(isPressed['b']) light0_Z += 1.0;
}

void showScore(string s) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_POLYGON);
    glVertex3f(2, 3.5, -11);
    glVertex3f(2, 4.5, -11);
    glVertex3f(4.5, 4.5, -11);
    glVertex3f(4.5, 3.5, -11);
    glEnd();
    glColor3f(1,0,0);
    glRasterPos3f(2.1, 3.9, -11);
    string score = "score: ";
    for (int i = 0; i < score.size(); i ++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[i]);
    }
    for (int i = 0; i < s.size(); i ++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
    glColor3f(1,1,1);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
void draw() {
    if(showMenu) {
        glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
        glColor3f(1, 1, 1);
        string easy = "easy", medium = "medium", hard = "hard";
        glRasterPos3f(-0.2, 0.5, 0);
        for (int i = 0; i < easy.size(); i ++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, easy[i]);
        }
        glRasterPos3f(-0.2, 0.0, 0);
        for (int i = 0; i < medium.size(); i ++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, medium[i]);
        }
        glRasterPos3f(-0.2, -0.5, 0);
        for (int i = 0; i < hard.size(); i ++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, hard[i]);
        }
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3f(-1.0, 1.0/3, 0.0);
        glVertex3f(-1.0, 1.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(1.0, 1.0/3, 0.0);
        glEnd();
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3f(-1.0, -1.0/3, 0.0);
        glVertex3f(-1.0, 1.0/3, 0.0);
        glVertex3f(1.0, 1.0/3, 0.0);
        glVertex3f(1.0, -1.0/3, 0.0);
        glEnd();
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_QUADS);
        glVertex3f(-1.0, -1.0, 0.0);
        glVertex3f(-1.0, -1.0/3, 0.0);
        glVertex3f(1.0, -1.0/3, 0.0);
        glVertex3f(1.0, -1.0, 0.0);
        glEnd();
        glColor3f(0.1, 0.1, 0.1);
        glFlush();
        return;
    }
    lightMovementCalculations();
    initLights();
    sceneMovementCalculations();
    glPushMatrix();
    glRotatef(sceneRotate, 0.0, 0.0, 1.0);
    glTranslatef(sceneX + sceneDeltaX, 0, 0);
    glRotatef(sceneAngle + sceneDeltaAngle, 0.0, 1.0, 0.0);
    drawFullTunnel();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(light0_X, light0_Y, light0_Z);
    // glutSolidCube(-1.0);
    glPopMatrix();
    showScore(to_string(-(int)globalZ - 50));
    glFlush();
}
int levelToRarity(int level) {
    if(level == 1) return 6;
    if(level == 2) return 3;
    if(level == 3) return 2;
}

void redraw()
{
    cout<<obstacleRarity<<endl;
    division += 0.1;
    for(int i = 0; i < dq.size(); i++) dq[i].centerZ += tunnelSpeed;

    if(dq[0].centerZ >= zLength) {
        if(detectCollision(dq[1].obstaclePos)) ;//exit(0);
        division = 0.0;
        int lastIndex = dq.size() - 1;
        float zPos = dq[lastIndex].centerZ - zLength;
        obstacleRarity = levelToRarity(level);
        
        int oP = randObstaclePosition(obstacleRarity);
        tunnelUnitInfo tui(tunnelCurve(globalZ), 0.0, zPos, oP);
        dq.push_back(tui);
        dq.pop_front();
        globalZ -= zLength;
    }
}


int main(int argc,char **argv)
{
    glutBegin(argc, argv);
}
