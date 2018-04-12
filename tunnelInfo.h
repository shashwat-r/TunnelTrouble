#include "glutInitialisers.h"

// Designing a Single Side
void singleSide(float xWidth) {
	float miniSquares = 1;
	for(float z = -zLength/2; z < zLength/2; z += zLength/miniSquares) {
		for(float x = -xWidth/2; x < xWidth/2; x += xWidth/miniSquares) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tunnelTexture[level-1]);
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(x, 0, z);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(x + xWidth/miniSquares, 0, z);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(x + xWidth/miniSquares, 0, z + zLength/miniSquares);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(x, 0, z + zLength/miniSquares);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
}

// Designing a Single Obstacle
void singleObstacle(float xWidth, float yHeight, float zThickness) {
	glPushMatrix();
	glScalef(xWidth, yHeight, zThickness);
	glTranslatef(0.0, 0.5, 0.5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, obstacleTexture);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, 0);
		glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0, 0.5,0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Designing single Tunnel Ring
void tunnelUnit(int oP) {
	float   wallWidth = 0.5,
			wallAngle = 360.0/tunnelSides,
			wallDistFromCenter = (wallWidth/2)/tan((wallAngle/2)*(3.14159/180));

	for(int i = 0; i < tunnelSides; i++) {
		glPushMatrix();
		glRotatef(-wallAngle*i, 0.0, 0.0, 1.0);
		glTranslatef(0.0, -wallDistFromCenter, 0);
		if( i == oP ) {
			initObstacleProperties();
			singleObstacle(wallWidth, obstacleHeight, obstacleThickness);
		}
		initMaterialProperties();
		singleSide(wallWidth);
		glPopMatrix();
	}
}

// Function to calculate tunnel position
float tunnelCurve(float zPos, float (*curveFunc)(float)) {
	float stretch = 1000;
	float radToDeg = zPos/piby180;
	float amplitude = 1;
	return amplitude * curveFunc( radToDeg / stretch );
}

// Function to calculate derivative of tunnel curve
float tunnelCurveDerivative(float zPos, float (*curveFunc)(float)) {
	// float stretch = 1000;
	// float radToDeg = zPos/piby180;
	// float amplitude = 2;
	// return amplitude * cos( radToDeg / stretch) * (1 / piby180) * (1 / stretch); 
	return (tunnelCurve(zPos + 0.01, curveFunc) - tunnelCurve(zPos, curveFunc)) / 0.01;
}

// function to place obstacles
float randObstaclePosition(int rarity) {
	int oP = rand()%(rarity*tunnelSides);
	if(oP < 0 || oP >= tunnelSides) oP = -1;
	return oP;
}

// function to initialize starting deque()
void initDeque() {
	for(int i = 0; i < tunnelUnitsCount; i++) {
        obstacleRarity = levelToRarity(level);
        int oP = randObstaclePosition(obstacleRarity);
        if(i <= 10) oP = -1;
        tunnelUnitInfo tui(tunnelCurve(globalZ, sin), tunnelCurve(globalZ, cos), globalZ, oP);
        dq.push_back(tui);
        globalZ -= zLength;
    }
}