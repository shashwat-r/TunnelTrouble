#include "shapes.h"

void octagonRoomPlacer(float zPos, float xPos) {
    glPushMatrix();
    glTranslatef(xPos, 0.0, zPos);
    // glRotatef(tunnelDerivativeCurve(zPos + 50*zLength), 0.0, 1.0, 0.0);
    room();
    glPopMatrix();
}

void draw() {
    glColor3f(0.0, 1.0, 0.0);
    wall(0.1);
    wall(100, 1);
    glTranslatef(-tunnelCurve(globalZ + 50*zLength), 1, 0);
    // glRotatef(-tunnelDerivativeCurve(globalZ + 50*zLength), 0.0, 1.0, 0.0);
    for(int i = 0; i < dq.size(); i++) {
        octagonRoomPlacer(dq[i].first, dq[i].second);
    }
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(+tunnelCurve(globalZ + 50*zLength), 1, 0);
    glutSolidCube(1.0);
    glPopMatrix();
}

int main(int argc,char **argv)
{
    int octagonCount = 50;
    for(int i = 0; i < octagonCount; i++) {
        dq.push_back( {globalZ, tunnelCurve(globalZ)} );
        globalZ -= zLength;
    }
    glutSabKuch(argc, argv);
}
