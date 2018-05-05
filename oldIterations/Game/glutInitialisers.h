#include "headersGlobalsHandlers.h"

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
}

void initCallbacks() {
	glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeyrelease);
    glutReshapeFunc(handleResize);
    glutDisplayFunc(draw);
    glutTimerFunc(initialDelay, timerFunc, 0);
}

void initDraw() {
	glClearColor(0.1,0.1,0.1,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

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

void glutEverything(int argc, char **argv) {
    glutInit(&argc,argv);
    initWindow();
    initCallbacks();
    initPerspectiveAndCamera();
    glutMainLoop();
}
