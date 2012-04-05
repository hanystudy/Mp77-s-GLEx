#include "GLExLight.h"


GLExLight::GLExLight(void)
{
	lightNum = 0;
}


GLExLight::~GLExLight(void)
{
}

void GLExLight::InsertNewLight(GLfloat* a ,GLfloat* d,GLfloat* s,GLfloat *p)
{
	if(lightNum < 8)
	{
		lightAmbient[lightNum] = a;
		lightDiffuse[lightNum] = d;
		lightSpecular[lightNum] = s;
		lightPosition[lightNum] = p;		
		lightNum++;
	}
}

void GLExLight::EnableLight()
{
	glEnable(GL_LIGHTING);
}

void GLExLight::DisableLight()
{
	glDisable(GL_LIGHTING);
}

void GLExLight::Render()
{
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	for(int i= 0; i < lightNum; ++i)
	{
		glLightfv(GL_LIGHT0+i, GL_AMBIENT, lightAmbient[i]);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lightDiffuse[i]);
		glLightfv(GL_LIGHT0+i, GL_SPECULAR, lightSpecular[i]);
		glLightfv(GL_LIGHT0+i, GL_POSITION, lightPosition[i]);
		glEnable(GL_LIGHT0+i);	
	}
	glPopMatrix();
}