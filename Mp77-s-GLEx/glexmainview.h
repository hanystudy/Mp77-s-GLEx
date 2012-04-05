#ifndef GLEXMAINVIEW_H
#define GLEXMAINVIEW_H

#include "stdafx.h"

#include <QGLWidget>
#include "GLExTerrain.h"
#include "GLExFreeType.h"
#include "GLExSystemInfo.h"
#include "GLExFog.h"
#include "GLExLight.h"

class GLExMainView : public QGLWidget
{
	Q_OBJECT

public:
	GLExMainView(QWidget *parent);
	~GLExMainView();

private:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

	GLfloat width,height;

	std::vector<GLExTerrain*> *terrainList;
	GLExFreeType *freetype;
	GLExSystemInfo *systeminfo;

	float centerx,centery,centerz;
	float eyex,eyey,eyez;


	void SizeForward();
	void SizeBackward();

	bool isRBClicked;
	GLfloat biasx;
	GLfloat biasy;

	float velocity;

	bool tex;

	GLExFog *fog;
	int fogmode;
	bool rfog;

	GLExLight *light;
	bool rlight;
protected:
	
	virtual void keyPressEvent( QKeyEvent * event );

private slots:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

};

#endif // GLEXMAINVIEW_H
