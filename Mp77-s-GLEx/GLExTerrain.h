#pragma once
/*
*	@author mp77
*	@time 2012/3/18
*	@description this is from nehe lesson 34 terrian render
*/
#include "GLExMath.h"
#include <QGLWidget>

#define	MAP_SIZE		1024	// Size Of Our .RAW Height Map (NEW)
#define	STEP_SIZE		16		// Width And Height Of Each Quad (NEW)
#define	HEIGHT_RATIO	1.5f	// Ratio That The Y Is Scaled According To The X And Z (NEW)

//extern BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];	// Holds The Height Map Data (NEW)

extern float scaleValue;				// Scale Value For The Terrain (NEW)

class GLExTerrain
{
public:
	GLExTerrain(int ,int,const char*);
	~GLExTerrain(void);
	int Height(unsigned char *pHeightMap, int X, int Y);
	void SetVertexColor(unsigned char *pHeightMap, int x, int y);
	void LoadRawFile(const char* strName, int nSize, unsigned char *pHeightMap);
	void Render();

	void InitLookAt(float,float,float,float,float,float);

	void SizeForward();
	void SizeBackward();

	void DisplayTexture(bool);
	void SetTexture(GLuint tex);

	void DisplayFog(bool);

	void CalcNormal();

	QPixmap& LoadTexture(QString m);

	void SetRightTerrain(GLExTerrain *);
	void SetDownTerrain(GLExTerrain *);
private:
	bool bRender;

	float centerx,centery,centerz;
	float eyex,eyey,eyez;

	bool texRender;

	GLuint	texture[1];

	QImage *tex[7];

	bool fogRender;

	std::vector<Point3D*> *normal;
	int startx,starty;

	QPixmap *ptexture;

	BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];	// Holds The Height Map Data (NEW)

	GLExTerrain *right;
	GLExTerrain *down;
};