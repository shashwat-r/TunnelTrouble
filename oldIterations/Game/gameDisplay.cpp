#include "tunnelInfo.h"

void octagonRoomPlacer(tunnelUnitInfo tui) {
	glPushMatrix();
	glTranslatef(tui.centerX, tui.centerY, tui.centerZ);
	float reqAngleX = atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength)) * 180/3.14;
	float reqAngleY = atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength)) * 180/3.14;
	glRotatef(reqAngleX, 0.0, 1.0, 0.0);
	glRotatef(reqAngleY, -1.0, 0.0, 0.0);
	tunnelUnit(tui.obstaclePos);
	glPopMatrix();
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

	sceneY = -tunnelCurve(globalZ + tunnelUnitsCount*zLength); 
	float nextsceneY = -tunnelCurve((globalZ - zLength) + tunnelUnitsCount*zLength);
	sceneDeltaY = (nextsceneY - sceneY)*division;

	sceneAngleX = -atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength)) * 180/3.14;
	float nextsceneAngleX = -atan(tunnelCurveDerivative((globalZ - zLength) + tunnelUnitsCount*zLength)) * 180/3.14;
	sceneDeltaAngleX = ((nextsceneAngleX) - (sceneAngleX))*division;

	sceneAngleY = -atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength)) * 180/3.14;
	float nextsceneAngleY = -atan(tunnelCurveDerivative((globalZ - zLength) + tunnelUnitsCount*zLength)) * 180/3.14;
	sceneDeltaAngleY = ((nextsceneAngleY) - (sceneAngleY))*division;
}

void lightMovementCalculations() {
	if(isPressed['f']) light0_Z -= 1.0;
	if(isPressed['b']) light0_Z += 1.0;
}

void showScore(string s) {
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -11.0);
	drawQuad(2, 4.5, 4.5, 3.5);
	glPopMatrix();
	glColor3f(1, 0, 0);
	printString("score: " + s, 2.1, 3.9, -11);
	glColor3f(1,1,1);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
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


void drawMenu() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);
	drawQuad(-1.0, 1.0, 1.0, 1.0/3);
	glColor3f(0.0, 1.0, 0.0);
	drawQuad(-1.0, 1.0/3, 1.0, -1.0/3);
	glColor3f(0.0, 0.0, 1.0);
	drawQuad(-1.0, -1.0/3, 1.0, -1.0);
	glColor3f(1, 1, 1);
	printString("easy", -0.2, 0.5, -0.1);
	printString("medium", -0.2, 0.0, -0.1);
	printString("hard", -0.2, -0.5, -0.1);
	glColor3f(0.1, 0.1, 0.1);
	glFlush();
}

void drawGame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	lightMovementCalculations();
	initLights();
	sceneMovementCalculations();
	glPushMatrix();
	glRotatef(sceneRotate, 0.0, 0.0, 1.0);
	glTranslatef(sceneX + sceneDeltaX, 0, 0);
	glTranslatef(0, sceneY + sceneDeltaY, 0);
	glRotatef(sceneAngleX + sceneDeltaAngleX, 0.0, 1.0, 0.0);
	glRotatef(sceneAngleY + sceneDeltaAngleY, -1.0, 0.0, 0.0);
	drawFullTunnel();
	glPopMatrix();
	showScore(to_string(-(int)globalZ - 50));
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.00, 0,00);
	// glutSolidCube(-1.0);
	glDisable(GL_COLOR_MATERIAL);
	glFlush();
}

void handleTimer(int)
{
	division += 0.1;
	for(int i = 0; i < dq.size(); i++) dq[i].centerZ += tunnelSpeed;

	if(dq[0].centerZ >= zLength) {
		if(detectCollision(dq[1].obstaclePos)) {
			system("killall paplay");
			// exit(0);
			// framerate = 0.0001;
    		// glutDisplayFunc(drawMenu);
		}
		division = 0.0;
		int lastIndex = dq.size() - 1;
		float zPos = dq[lastIndex].centerZ - zLength;
		obstacleRarity = levelToRarity(level);
		
		int oP = randObstaclePosition(obstacleRarity);
		tunnelUnitInfo tui(tunnelCurve(globalZ), tunnelCurve(globalZ), zPos, oP);
		dq.push_back(tui);
		dq.pop_front();
		globalZ -= zLength;
	}
	glutPostRedisplay();
	glutTimerFunc(1000.0/framerate, handleTimer, 0);
}


int main(int argc,char **argv) {
	glutBegin(argc, argv);
}