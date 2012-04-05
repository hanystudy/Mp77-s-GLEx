#pragma once
#include "stdafx.h"

class GLExLight
{
public:
	GLExLight(void);
	~GLExLight(void);

	void InsertNewLight(GLfloat*,GLfloat*,GLfloat*,GLfloat*);

	void Render();

	void EnableLight();
	void DisableLight();
private:
	int lightNum;
	GLfloat *lightAmbient[8];
	GLfloat *lightDiffuse[8];
	GLfloat *lightSpecular[8];
	GLfloat *lightPosition[8];
};

