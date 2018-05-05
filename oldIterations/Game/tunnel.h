#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

float 	globalZ = 0,
		division = 0.0,
		zLength = 1.0,
		tunnelAngle = 0.0,
		tunnelSpeed = 0.1,
		piby180 = 3.14/180,
		sceneRotate = 0.0,
		sceneX,
		sceneDeltaX,
		sceneAngle,
		sceneDeltaAngle,
		obstacleHeight = 0.5,
		light0_X = 0.0,
		light0_Y = 0.0,
		light0_Z = 0.0,
		obstacleThickness = 0.1;

int		tunnelSides = 18,
		tunnelUnitsCount = 50,
		obstacleRarity = 2;

//data structures
struct tunnelUnitInfo {
	float 	centerX,
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
std::deque<tunnelUnitInfo> dq;

void initLights() {
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    //global ambient light
    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glEnable(GL_LIGHT0);
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { light0_X, light0_Y, light0_Z, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0);
}

void initMaterialProperties() {
    GLfloat mat_ambient[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
}

void initObstacleProperties() {
    GLfloat mat_ambient[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
}

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