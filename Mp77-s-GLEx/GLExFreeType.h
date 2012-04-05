#pragma once

#include "freetype.h"		// Header for our little font library.

#include "GLExSystemInfo.h"

extern freetype::font_data our_font;

class GLExFreeType
{
public:
	GLExFreeType();
	~GLExFreeType(void);

	void PrintSystemInfo();

	void SetSystemInfo(GLExSystemInfo *);
	void SetWindowSize(GLfloat,GLfloat);
	void PrintSystemOption(bool tex);
private:
	GLExSystemInfo *systeminfo;

	char displaydevice[128];
	char displaydeviceid[128];
	GLfloat height,width;
};



