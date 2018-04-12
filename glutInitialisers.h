#include "lightingAndMaterials.h"

// initialize Perspective parameters and position of viewer
void initPerspectiveAndCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, windowWidth/windowHeight, 0.01, 1000);
    gluLookAt(eyePos[0], eyePos[1], eyePos[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}

// generate Window
void initWindow() {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(windowX, windowY);
    glutCreateWindow("Tunnel Trouble");
    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.1,0.1,0.1,0.0);
}

// begin game and set all keyboard controls
void initGameAndControls() {
    glutDisplayFunc(drawGame);
    system("killall paplay");
    system("amixer -D pulse sset Master 100%");
    system("paplay music.wav &");
    initDeque();
    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeyrelease);
    glutSpecialFunc(handleSpecialKeypress);
    glutSpecialUpFunc(handleSpecialKeyrelease);
    glutTimerFunc(initialDelay, handleTimer, 0);
    initPerspectiveAndCamera();
}

// initializing Resize Menu and Mouse functions
void initBasicCallbacks() {
    glutReshapeFunc(handleResize);
    glutDisplayFunc(drawMenu);
    glutMouseFunc(handleMouse);
}

// loads Textures
void loadTextures() {
    tunnelTexture[0] = LoadBMP("textures/zigzag.bmp");
    tunnelTexture[1] = LoadBMP("textures/white.bmp");
    tunnelTexture[2] = LoadBMP("textures/binary.bmp");
    obstacleTexture = LoadBMP("textures/obstacle.bmp");
    menuTexture = LoadBMP("textures/menu.bmp");
    gameOverTexture = LoadBMP("textures/game_over.bmp");
}

// The Master Function
void glutBegin(int argc, char **argv) {
    glutInit(&argc,argv);
    initWindow();
    initBasicCallbacks();
    loadTextures();
    glutMainLoop();
}
