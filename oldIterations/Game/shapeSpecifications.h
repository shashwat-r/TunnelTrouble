#include "glutInitialisers.h"

void singleSide(float xWidth) {
	float miniSquares = 5;
	for(float z = -zLength/2; z < zLength/2; z += zLength/miniSquares) {
		for(float x = -xWidth/2; x < xWidth/2; x += xWidth/miniSquares) {
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glVertex3f(x, 0, z);
				glVertex3f(x + xWidth/miniSquares, 0, z);
				glVertex3f(x + xWidth/miniSquares, 0, z + zLength/miniSquares);
				glVertex3f(x, 0, z + zLength/miniSquares);
			glEnd();
		}
	}
}

void singleObstacle(float xWidth, float yHeight, float zThickness) {
	glPushMatrix();
	// glColor3f(1.0, 1.0, 0.0);
	glScalef(xWidth, yHeight, zThickness);
	glTranslatef(0.0, +0.5, 0.0);
	glutSolidCube(+1.0);
	// glColor3f(1.0, 1.0, 1.0);
	// glutWireCube(1.0);
	glPopMatrix();
}

void tunnelUnit(int oP) {
	float   wallWidth = 0.5,
			wallAngle = 360.0/tunnelSides,
			wallDistFromCenter = (wallWidth/2)/tan((wallAngle/2)*(3.14159/180));

	for(int i = 0; i < tunnelSides; i++) {
		glPushMatrix();
		glRotatef(-wallAngle*i, 0.0, 0.0, 1.0);
		glTranslatef(0.0, -wallDistFromCenter, 0);
		// glColor3f(0.5*(i%2), 0.5*(i%2), 0.5*(i%2));
		if( i == oP ) {
			initObstacleProperties();
			singleObstacle(wallWidth, obstacleHeight, obstacleThickness);
		}
		initMaterialProperties();
		singleSide(wallWidth);
		glPopMatrix();
	}
}

float tunnelCurve(float zPos) {
	float stretch = 1000;
	float radToDeg = zPos/piby180;
	float amplitude = 2;
	return amplitude * sin( radToDeg / stretch );
}

float tunnelCurveDerivative(float zPos) {
	float stretch = 1000;
	float radToDeg = zPos/piby180;
	float amplitude = 2;
	return amplitude * cos( radToDeg / stretch) * (1 / piby180) * (1 / stretch); 
}

float randObstaclePosition(int rarity) {
	int oP = rand()%(rarity*tunnelSides);
	if(oP < 0 || oP >= tunnelSides) oP = -1;
	return oP;
}

void timerFunc(int)
{
	division += 0.1;
	for(int i = 0; i < dq.size(); i++) dq[i].centerZ += tunnelSpeed;

	if(dq[0].centerZ >= zLength) {
		if(detectCollision(dq[2].obstaclePos)) ;//exit(0);
		division = 0.0;
		int lastIndex = dq.size() - 1;
		float zPos = dq[lastIndex].centerZ - zLength;
		int oP = randObstaclePosition(obstacleRarity);
		tunnelUnitInfo tui(tunnelCurve(globalZ), 0.0, zPos, oP);
		dq.push_back(tui);
		dq.pop_front();
		globalZ -= zLength;
	}

	glutPostRedisplay();
	glutTimerFunc(1000.0/framerate, timerFunc, 0);
}