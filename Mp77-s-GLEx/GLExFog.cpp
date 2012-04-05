#include "GLExFog.h"

#include <stdlib.h>

GLuint	GLEx_fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLfloat	GLEx_fogColor[4] = {0.5f,0.5f,0.5f,1.0f};		// Fog Color

GLExFog::GLExFog(void)
{
	fogmode = 0;

	//char Extension_Name[] = "EXT_fog_coord";

	// Allocate Memory For Our Extension String
	//char* glextstring=(char *)malloc(strlen((char *)glGetString(GL_EXTENSIONS))+1);
	//strcpy (glextstring,(char *)glGetString(GL_EXTENSIONS));			// Grab The Extension List, Store In glextstring

	//free(glextstring);													// Free Allocated Memory

	// Setup And Enable glFogCoordEXT
}

GLExFog::~GLExFog(void)
{
}

void GLExFog::DisplayFog(bool g)
{
	if(g)
	{
		/*NEHE L16*/
		glFogi(GL_FOG_MODE, GLEx_fogMode[fogmode]);			// Fog Mode
		glFogfv(GL_FOG_COLOR, GLEx_fogColor);					// Set Fog Color
		glFogf(GL_FOG_DENSITY, 1.0f);						// How Dense Will The Fog Be
		glHint(GL_FOG_HINT, GL_NICEST);					// Fog Hint Value
		glFogf(GL_FOG_START, 50.0f);							// Fog Start Depth
		glFogf(GL_FOG_END, 500.0f);							// Fog End Depth
		glEnable(GL_FOG);									// Enables GL_FOG
		/*NEHE L41*/
		// Set Up Fog 
		/*glFogi(GL_FOG_MODE, GL_LINEAR);										// Fog Fade Is Linear
		glFogfv(GL_FOG_COLOR, GLEx_fogColor);									// Set The Fog Color
		glFogf(GL_FOG_START,  0.0f);										// Set The Fog Start
		glFogf(GL_FOG_END,    2000.0f);										// Set The Fog End
		glHint(GL_FOG_HINT, GL_DONT_CARE);										// Per-Pixel Fog Calculation
		glFogi(GL_FOG_COORDINATE_SOURCE, GL_FOG_COORDINATE);		// Set Fog Based On Vertice Coordinates
		*/
		// Set The Fog End
		//glFogi(GL_FOG_COORD_SRC,GL_FOG_COORD);
		//glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);		// Set Fog Based On Vertice Coordinates
	}
	else
	{
		glDisable(GL_FOG);									// Enables GL_FOG
	}
}

void GLExFog::SetFogFilter(int mode)
{
	fogmode = mode;
}