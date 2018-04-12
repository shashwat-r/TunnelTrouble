#include "tunnelInfo.h"

// the tunnel was initially an octagon, so the name persisted.
// this function places a unit of tunnel in the scene.
void octagonRoomPlacer(tunnelUnitInfo tui) {
	glPushMatrix();
	glTranslatef(tui.centerX, tui.centerY, tui.centerZ);
	float reqAngleX = atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength, sin)) * 180/3.14;
	float reqAngleY = atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength, cos)) * 180/3.14;
	glRotatef(reqAngleX, 0.0, 1.0, 0.0);
	glRotatef(reqAngleY, -1.0, 0.0, 0.0);
	tunnelUnit(tui.obstaclePos);
	glPopMatrix();
}

// draws full tunnel
void drawFullTunnel() {
	for(int i = 0; i < dq.size(); i++) octagonRoomPlacer(dq[i]);
}

// precalculations for the scene
void sceneMovementCalculations() {
	if(isPressed['a']) sceneRotate += 1.0; adjustAngle(sceneRotate);
	if(isPressed['d']) sceneRotate -= 1.0; adjustAngle(sceneRotate);
	sceneX = -tunnelCurve(globalZ + tunnelUnitsCount*zLength, sin); 
	float nextSceneX = -tunnelCurve((globalZ - zLength) + tunnelUnitsCount*zLength, sin);
	sceneDeltaX = (nextSceneX - sceneX)*division;

	sceneY = -tunnelCurve(globalZ + tunnelUnitsCount*zLength, cos); 
	float nextsceneY = -tunnelCurve((globalZ - zLength) + tunnelUnitsCount*zLength, cos);
	sceneDeltaY = (nextsceneY - sceneY)*division;

	sceneAngleX = -atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength, sin)) * 180/3.14;
	float nextsceneAngleX = -atan(tunnelCurveDerivative((globalZ - zLength) + tunnelUnitsCount*zLength, sin)) * 180/3.14;
	sceneDeltaAngleX = ((nextsceneAngleX) - (sceneAngleX))*division;

	sceneAngleY = -atan(tunnelCurveDerivative(globalZ + tunnelUnitsCount*zLength, cos)) * 180/3.14;
	float nextsceneAngleY = -atan(tunnelCurveDerivative((globalZ - zLength) + tunnelUnitsCount*zLength, cos)) * 180/3.14;
	sceneDeltaAngleY = ((nextsceneAngleY) - (sceneAngleY))*division;
}

// precalculations for moving lights
void lightMovementCalculations() {
	if(isPressed['w']) light0_Z -= 1.0;
	if(isPressed['s']) light0_Z += 1.0;
}

// function to display score
void showScore(string s) {
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(0.0f, 4.25, -10.0);
	// drawQuad(2, 4.5, 4.5, 3.5);
	drawQuad(-0.8,-0.3, 0.7,-0.9);
	glColor3f(1, 0, 0);
	// printString("score: " + s, 2.1, 3.9, -10);
	printString(s, -0.17-0.05*s.size(), -0.7, 0);
	glPopMatrix();
	glColor3f(1,1,1);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

// detects collision
bool detectCollision(int oP) {
	// return false;
	if (oP == -1) return false;
	float angle = (360/tunnelSides) * (float)oP;
	float error = 1.0;
	float halfRange = 360/(2*tunnelSides) - error;
	float startAngle = angle - halfRange, endAngle = angle + halfRange;
	adjustAngle(startAngle); adjustAngle(endAngle);
	// cout<<startAngle<<" "<<sceneRotate<<" "<<endAngle<<" "<<oP<<endl;
	return checkCircularlyIncreasing(startAngle, sceneRotate, endAngle);
}

// functions to draw menu
void drawMenu() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	makeMenu();
	// makeGameOver();
	glFlush();
}

// function to draw score screen
void drawGameOver() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	// makeMenu();
	makeGameOver();
	glFlush();
}

// draws main game
void drawGame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	lightMovementCalculations(); initLights();
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
	glFlush();
}

// generates and acts upon timer event, which is triggered in accordance to framerate
void handleTimer(int)
{
	if(showMenu) return;
	division += 0.1;
	for(int i = 0; i < dq.size(); i++) dq[i].centerZ += tunnelSpeed;

	if(dq[0].centerZ >= zLength) {
		if(detectCollision(dq[1].obstaclePos)) {
    		glutDisplayFunc(drawGameOver);
			resetAll();
			//exit(0);
		}
		division = 0.0;
		int lastIndex = dq.size() - 1;
		float zPos = dq[lastIndex].centerZ - zLength;
		obstacleRarity = levelToRarity(level);
		
		int oP = randObstaclePosition(obstacleRarity);
		tunnelUnitInfo tui(tunnelCurve(globalZ, sin), tunnelCurve(globalZ, cos), zPos, oP);
		dq.push_back(tui);
		dq.pop_front();
		globalZ -= zLength;
	}
	glutPostRedisplay();
	glutTimerFunc(1000.0/framerate, handleTimer, 0);
}

// The master function that begins the game
int main(int argc,char **argv) {
	glutBegin(argc, argv);
}