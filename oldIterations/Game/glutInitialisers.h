#include "globalsHeadersHandlers.h"
void initPerspectiveAndCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, windowWidth/windowHeight, 0.01, 1000);
    gluLookAt(eyePos[0], eyePos[1], eyePos[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}

void initWindow() {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(windowX, windowY);
    glutCreateWindow("Tunnel Trouble");
    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1,0.1,0.1,0.0);
}

void initGameAndControls() {
    system("paplay music.wav &");
	initPerspectiveAndCamera();
    for(int i = 0; i < tunnelUnitsCount; i++) {
        obstacleRarity = levelToRarity(level);
        int oP = randObstaclePosition(obstacleRarity);
        tunnelUnitInfo tui(tunnelCurve(globalZ), tunnelCurve(globalZ), globalZ, oP);
        dq.push_back(tui);
        globalZ -= zLength;
    }
	glutDisplayFunc(drawGame);
    glutTimerFunc(initialDelay, handleTimer, 0);
    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeyrelease);
}

void initBasicCallbacks() {
    glutDisplayFunc(drawMenu);
    glutReshapeFunc(handleResize);
    glutMouseFunc(handleMouse);
}

void glutBegin(int argc, char **argv) {
    glutInit(&argc,argv);
    initWindow();
    initBasicCallbacks();
    // obstacleTexture = LoadBMP("wall.bmp");
    glutMainLoop();
}
