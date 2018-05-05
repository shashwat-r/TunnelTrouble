#include "shapeSpecifications.h"

void octagonRoomPlacer(tunnelUnitInfo tui) {
    glPushMatrix();
    glTranslatef(tui.centerX, 0.0, tui.centerZ);
    float xTranslated = -tunnelCurve(globalZ + tunnelUnitsCount*zLength); 
    float reqAngle = atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength)) * 180/3.14;
    glRotatef(reqAngle, 0.0, 1.0, 0.0);
    tunnelUnit(tui.obstaclePos);
    glPopMatrix();
}

bool detectCollision(int oP) {
    if (oP == -1) return false;
    float angle = (360/tunnelSides) * (float)oP;
    float halfRange = 360/(2*tunnelSides);
    float startAngle = angle - halfRange, endAngle = angle + halfRange;
    cout<<startAngle<<" "<<sceneRotate<<" "<<endAngle<<" "<<oP<<endl;
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

void draw() {
    initDraw();
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
    glFlush();
}

int main(int argc,char **argv)
{
    for(int i = 0; i < tunnelUnitsCount + 1; i++) {
        int oP = randObstaclePosition(obstacleRarity);
        tunnelUnitInfo tui(tunnelCurve(globalZ), 0.0, globalZ, oP);
        dq.push_back(tui);
        globalZ -= zLength;
    }
    glutEverything(argc, argv);
}
