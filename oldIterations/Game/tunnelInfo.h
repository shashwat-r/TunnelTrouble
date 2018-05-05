#include "glutInitialisers.h"
#include "lightingAndMaterials.h"

void singleSide(float xWidth) {
	float miniSquares = 1;
	for(float z = -zLength/2; z < zLength/2; z += zLength/miniSquares) {
		for(float x = -xWidth/2; x < xWidth/2; x += xWidth/miniSquares) {
			// glEnable(GL_TEXTURE_2D);
			// glBindTexture(GL_TEXTURE_2D, obstacleTexture);
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(x, 0, z);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(x + xWidth/miniSquares, 0, z);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(x + xWidth/miniSquares, 0, z + zLength/miniSquares);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(x, 0, z + zLength/miniSquares);
			glEnd();
			// glDisable(GL_TEXTURE_2D);
		}
	}
}

void singleObstacle(float xWidth, float yHeight, float zThickness) {
	glPushMatrix();
	glScalef(xWidth, yHeight, zThickness);
	glTranslatef(0.0, 0.5, 0.5);
	// glPushMatrix();
	// glTranslatef(0.0, 0.0, 0.6);
	// glEnable(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE_2D, obstacleTexture);
	// glBegin(GL_QUADS);
	// 	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, 0);
	// 	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5,0.5,0);
	// 	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5,0.5,0);
	// 	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5,-0.5,0);
	// glEnd();
	// glDisable(GL_TEXTURE_2D);
	// glPopMatrix();
	// glutSolidCube(+1.0);
	glBegin(GL_POLYGON);
		glVertex3f(-0.5, -0.5, 0);
		glVertex3f(0.0, 0.5,0);
		glVertex3f(0.5, -0.5,0);
	glEnd();
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