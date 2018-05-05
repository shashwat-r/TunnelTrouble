#include "shapes.h"

int octagonCount = 51;
void octagonRoomPlacer(float zPos, float xPos, float yPos) {
    glPushMatrix();
    glTranslatef(0.0, 0, zPos);
    glTranslatef(0.0, yPos, 0);
    glTranslatef(xPos, 0.0, 0);
    float xTranslated = -tunnelCurveX(globalZ + octagonCount*zLength); 
    float prevXTranslated = -tunnelCurveX((globalZ + zLength) + octagonCount*zLength);
    float yTranslated = -tunnelCurveY(globalZ + octagonCount*zLength); 
    float prevYTranslated = -tunnelCurveY((globalZ + zLength) + octagonCount*zLength);
    float reqXAngle = atan((xTranslated - prevXTranslated) / zLength) * 180/3.14;
    float reqYAngle = atan((yTranslated - prevYTranslated) / zLength) * 180/3.14;
    glRotatef(reqXAngle, 0.0, 1.0, 0.0);
    glRotatef(-reqYAngle, 1.0, 0.0, 0.0);
    room();
    glPopMatrix();
}

void draw() {
    glColor3f(0.0, 1.0, 0.0);
    //stud camera movements
    float xTranslated = -tunnelCurveX(globalZ + octagonCount*zLength); 
    float prevXTranslated = -tunnelCurveX((globalZ + zLength) + octagonCount*zLength);
    float nextXTranslated = -tunnelCurveX((globalZ - zLength) + octagonCount*zLength);

    float yTranslated = -tunnelCurveY(globalZ + octagonCount*zLength); 
    float prevYTranslated = -tunnelCurveY((globalZ + zLength) + octagonCount*zLength);
    float nextYTranslated = -tunnelCurveY((globalZ - zLength) + octagonCount*zLength);

    float reqXAngle = atan((xTranslated - prevXTranslated) / zLength) * 180/3.14;
    float nextReqXAngle = atan((nextXTranslated - xTranslated) / zLength) * 180/3.14;

    float reqYAngle = atan((yTranslated - prevYTranslated) / zLength) * 180/3.14;
    float nextReqYAngle = atan((nextYTranslated - yTranslated) / zLength) * 180/3.14;

    float deltaX = (nextXTranslated - xTranslated)*division;
    float deltaY = (nextYTranslated - yTranslated)*division;
    float xDeltaTheta = ((nextReqXAngle) - (reqXAngle))*division;
    float yDeltaTheta = ((nextReqYAngle) - (reqYAngle))*division;

    glTranslatef(0, deltaY, 0);
    glTranslatef(0.0, yTranslated, 0);
    glTranslatef(deltaX, 0, 0);
    glTranslatef(xTranslated, 0.0, 0);
    glRotatef(yDeltaTheta, 1.0, 0.0, 0.0);
    glRotatef(reqYAngle, 1.0, 0.0, 0.0);
    glRotatef(-xDeltaTheta, 0.0, 1.0, 0.0);
    glRotatef(-reqXAngle, 0.0, 1.0, 0.0);

    //studness finish
    for(int i = 0; i < dq.size(); i++) {
        octagonRoomPlacer(dq[i].first, dq[i].second.first, dq[i].second.second);
    }
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    // glTranslatef(+tunnelCurveX(globalZ + octagonCount*zLength), 0, 0);
    glTranslatef(0, +tunnelCurveY(globalZ + octagonCount*zLength), 0);
    // glutSolidCube(1.0);
    glPopMatrix();
}

int main(int argc,char **argv)
{
    for(int i = 0; i < octagonCount; i++) {
        dq.push_back( {globalZ, {tunnelCurveX(globalZ), tunnelCurveY(globalZ)}} );
        globalZ -= zLength;
    }
    glutSabKuch(argc, argv);
}
