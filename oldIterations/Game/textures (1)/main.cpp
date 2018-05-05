#include<stdlib.h>
#include<GL/glut.h>
#include<stdio.h>
#include<math.h>



void display();

GLint grass;

GLuint LoadBMP(const char *fileName)
{
	FILE *file;
	unsigned char header[54],*data;
	unsigned int dataPos,size,width, height;
	file = fopen(fileName, "rb");
	fread(header, 1, 54, file);
	dataPos	= *(int*)&(header[0x0A]);
	size = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (size == NULL)
		size = width * height * 3;
	if (dataPos == NULL)
		dataPos = 54;
	data = new unsigned char[size];
	fread(data, 1, size, file);
	fclose(file);
	GLuint texture;
	glGenTextures(1, &texture);	
	glBindTexture(GL_TEXTURE_2D, texture);	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data); 
	return texture;
}

void initSky()
{
	grass=LoadBMP("wall.bmp");
}

void draw_ground()
{		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,grass);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5, -5, -10);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5,5,-10);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5,5,-10);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5,-5,-10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void display(){
	glLoadIdentity();
	draw_ground();
		glutSwapBuffers();

}


void displayReshape(int width,int height)
{
	glViewport(0,0,width,height);						
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	gluPerspective(65,(GLfloat)width/(GLfloat)height,0.01f,1000.0f);
	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();			
}


int main(int argc, char** argv){
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
		glutInitWindowSize(800,600);
		glutCreateWindow("Amusement Park");
		initSky();
  		glutDisplayFunc(display);
	 	glutReshapeFunc(displayReshape);
	 												glutMainLoop();
		return 0;
}


