#include "shapes.h"

int octagonCount = 70;
void octagonRoomPlacer(float zPos, float xPos) {
    glPushMatrix();
    glTranslatef(xPos, 0.0, zPos);
    float xTranslated = -tunnelCurve(globalZ + octagonCount*zLength); 
    float prevXTranslated = -tunnelCurve((globalZ + zLength) + octagonCount*zLength);
    float reqAngle = atan((xTranslated - prevXTranslated) / zLength) * 180/3.14;
    glRotatef(reqAngle, 0.0, 1.0, 0.0);
    room();
    glPopMatrix();
}

void draw() {
    glColor3f(0.0, 1.0, 0.0);
    // wall(0.1);
    // wall(100, 1);
    //stud camera movements
    float xTranslated = -tunnelCurve(globalZ + octagonCount*zLength); 
    float prevXTranslated = -tunnelCurve((globalZ + zLength) + octagonCount*zLength);
    float nextXTranslated = -tunnelCurve((globalZ - zLength) + octagonCount*zLength);

    float reqAngle = atan((xTranslated - prevXTranslated) / zLength) * 180/3.14;
    float nextReqAngle = atan((nextXTranslated - xTranslated) / zLength) * 180/3.14;

    float deltaX = (nextXTranslated - xTranslated)*division;
    float deltaTheta = ((nextReqAngle) - (reqAngle))*division;

    // glTranslatef(deltaX, 0, 0);
    // glTranslatef(xTranslated, 0, 0);
    // glRotatef(-deltaTheta, 0.0, 1.0, 0.0);
    // glRotatef(-reqAngle, 0.0, 1.0, 0.0);


    //studness finish
    for(int i = 0; i < dq.size(); i++) {
        octagonRoomPlacer(dq[i].first, dq[i].second);
    }
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    // glTranslatef(+tunnelCurve(globalZ + octagonCount*zLength), 1, 0);
    // glutSolidCube(1.0);
    glPopMatrix();
}

int main(int argc,char **argv)
{
    for(int i = 0; i < octagonCount; i++) {
        dq.push_back( {globalZ, tunnelCurve(globalZ)} );
        globalZ -= zLength;
    }
    glutSabKuch(argc, argv);
}
