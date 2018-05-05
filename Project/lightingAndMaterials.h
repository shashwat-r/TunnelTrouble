#include "globalsHeadersHandlers.h"
void initLights() {
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    float intensityInput = (float)globalZ/10;
    GLfloat intensity = (GLfloat)0.9*(sin(intensityInput))*(sin(intensityInput))+0.1;
    if (level<3) intensity = 1;
    //global ambient light
    GLfloat lmodel_ambient[] = { 0.5f*intensity, 0.5f*intensity, 0.5f*intensity, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    //individual lights
    glEnable(GL_LIGHT0);
    GLfloat light_ambient[] = { 1.0f*intensity, 1.0f*intensity, 1.0f*intensity, 1.0f };
    GLfloat light_diffuse[] = { 1.0f*intensity, 1.0f*intensity, 1.0f*intensity, 1.0f };
    GLfloat light_specular[] = { 1.0f*intensity, 1.0f*intensity, 1.0f*intensity, 1.0f };
    GLfloat light_position[] = { light0_X, light0_Y, light0_Z, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0);

    glEnable(GL_LIGHT1);
    GLfloat light_position2[] = { light0_X, light0_Y, light0_Z - 10*zLength, 1.0 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1.0);
}

void initMaterialProperties() {
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glShadeModel(GL_SMOOTH);
}

void initObstacleProperties() {
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
}