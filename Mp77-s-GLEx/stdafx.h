#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/glext.h>

#include <vector>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib, "psapi.lib")


#define GL_FOG_COORDINATE_SOURCE_EXT			0x8450					// Value Taken From GLEXT.H
#define GL_FOG_COORDINATE_EXT					0x8451					// Value Taken From GLEXT.H

typedef void (APIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);		// Declare Function Prototype

extern GLfloat TerrainAmbient[];
extern GLfloat TerrainDiffuse[];
extern GLfloat TerrainPosition[];
extern GLfloat TerrainSpecular[];