#include <stdio.h> 
#include <GL/glut.h>
GLuint LoadBMP(const char *fileName)
{
    FILE *file;
    unsigned char header[54],*data;
    unsigned int dataPos,size,width, height;
    file = fopen(fileName, "rb");
    fread(header, 1, 54, file);
    dataPos = *(int*)&(header[0x0A]);
    size = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);
    if (size == 0) size = width * height * 3;
    if (dataPos == 0) dataPos = 54;
    data = new unsigned char[size];
    fread(data, 1, size, file);
    fclose(file);
    GLuint texture;
    glGenTextures(1, &texture); 
    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
    return texture;
}
GLint texture;

void display(void) 
{ 
 glClear( GL_COLOR_BUFFER_BIT); 
 // glColor3f(0.0, 1.0, 0.0); 
 glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
 glBegin(GL_POLYGON); 
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, 0.0); 
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 0.5, 0.0); 
  glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.5, 0.0); 
  glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5, 0.0); 
 glEnd(); 
 glDisable(GL_TEXTURE_2D);

 glFlush(); 
}

int main(int argc, char **argv) 
{ 
 printf("hello world\n"); 
 glutInit(&argc, argv); 
 glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

 glutInitWindowPosition(100,100); 
 glutInitWindowSize(500,500); 
 glViewport(0, 0, 500, 500);
 glutCreateWindow ("square");

 glClearColor(0.0, 0.0, 0.0, 0.0);         // black background 
 glMatrixMode(GL_PROJECTION);              // setup viewing projection 
 glLoadIdentity();                           // start with identity matrix 

 glutDisplayFunc(display); 
 texture = LoadBMP("wall.bmp");
 glutMainLoop();

 return 0; 
}
