#pragma once

#include "stdafx.h"

extern GLuint	GLEx_fogMode[];	// Storage For Three Types Of Fog
extern GLfloat	GLEx_fogColor[];		// Fog Color


class GLExFog
{
public:
	GLExFog(void);
	~GLExFog(void);

	void DisplayFog(bool);
	void SetFogFilter(int);

private:
	int fogmode;
};

