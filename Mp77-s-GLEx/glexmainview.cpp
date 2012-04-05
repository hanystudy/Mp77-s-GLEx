#include "glexmainview.h"

#include <QWheelEvent>

#include "GLExArcBall.h"

const float PI2 = 2.0*3.1415926535f;								// PI Squared

Matrix4fT   Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,				// NEW: Final Transform
                             0.0f,  1.0f,  0.0f,  0.0f,
                             0.0f,  0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  0.0f,  1.0f };

Matrix3fT   LastRot     = {  1.0f,  0.0f,  0.0f,					// NEW: Last Rotation
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

Matrix3fT   ThisRot     = {  1.0f,  0.0f,  0.0f,					// NEW: This Rotation
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

ArcBallT    ArcBall(120.0f, 120.0f);				                // NEW: ArcBall Instance
Point2fT    MousePt;												// NEW: Current Mouse Point
bool        isClicked  = false;										// NEW: Clicking The Mouse?
bool        isRClicked = false;										// NEW: Clicking The Right Mouse Button?
bool        isDragging = false;					                    // NEW: Dragging The Mouse?

GLExMainView::GLExMainView(QWidget *parent)
	: QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
{
	terrainList = new std::vector<GLExTerrain*>();
	/*
	GLExTerrain* terrain = new GLExTerrain(0,0,"..\\Data\\gcanyon_height_bmp_01.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);

	terrain = new GLExTerrain(1,0,"..\\Data\\gcanyon_height_bmp_02.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[0]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(2,0,"..\\Data\\gcanyon_height_bmp_03.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[1]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(3,0,"..\\Data\\gcanyon_height_bmp_04.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[2]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(0,1,"..\\Data\\gcanyon_height_bmp_05.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[0]->SetDownTerrain(terrain);

	terrain = new GLExTerrain(1,1,"..\\Data\\gcanyon_height_bmp_06.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[1]->SetDownTerrain(terrain);
	(*terrainList)[4]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(2,1,"..\\Data\\gcanyon_height_bmp_07.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[2]->SetDownTerrain(terrain);
	(*terrainList)[5]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(3,1,"..\\Data\\gcanyon_height_bmp_08.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	
	(*terrainList)[3]->SetDownTerrain(terrain);
	(*terrainList)[6]->SetRightTerrain(terrain);
	*/
	GLExTerrain* terrain = new GLExTerrain(0,0,"..\\Data\\newcontinent_01.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);

	terrain = new GLExTerrain(1,0,"..\\Data\\newcontinent_02.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[0]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(2,0,"..\\Data\\newcontinent_03.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[1]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(3,0,"..\\Data\\newcontinent_04.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[2]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(0,1,"..\\Data\\newcontinent_05.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[0]->SetDownTerrain(terrain);

	terrain = new GLExTerrain(1,1,"..\\Data\\newcontinent_06.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[1]->SetDownTerrain(terrain);
	(*terrainList)[4]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(2,1,"..\\Data\\newcontinent_07.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	(*terrainList)[2]->SetDownTerrain(terrain);
	(*terrainList)[5]->SetRightTerrain(terrain);

	terrain = new GLExTerrain(3,1,"..\\Data\\newcontinent_08.raw");
	terrain->DisplayFog(rfog);
	terrainList->push_back(terrain);
	
	(*terrainList)[3]->SetDownTerrain(terrain);
	(*terrainList)[6]->SetRightTerrain(terrain);
	//terrain->InitLookAt(212, 60, 194,  186, 55, 171);
	//terrain->InitLookAt(212, 60, 194,  0, 0, 0);
	eyex = 500;
	eyey = 100;
	eyez = 500;
	centerx = 480;
	centery = 50;
	centerz = 480;

	isRBClicked = false;

	setFocusPolicy(Qt::FocusPolicy::ClickFocus);
	systeminfo = new GLExSystemInfo();

	fogmode = 2;
	fog = new GLExFog();
	fog->SetFogFilter(fogmode);
	rfog = false;
	velocity = 3;
	tex = true;

	light = new GLExLight();
	light->InsertNewLight(TerrainAmbient,TerrainDiffuse,TerrainSpecular,TerrainPosition);
	rlight = true;
}

GLExMainView::~GLExMainView()
{

}

void GLExMainView::initializeGL()
{	
	glFogCoordf = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordf");

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	for(std::vector<GLExTerrain*>::iterator it = terrainList->begin();it != terrainList->end(); ++it)
	{
		GLuint tex = bindTexture((*it)->LoadTexture("..\\Data\\gcanyon_color_4k2k_0"+QString::number(it-terrainList->begin()+1)+".jpg"), GL_TEXTURE_2D);
		(*it)->SetTexture(tex);
		(*it)->DisplayTexture(this->tex);
	}

	freetype = new GLExFreeType();
	freetype->SetSystemInfo(systeminfo);
	
	fog->DisplayFog(rfog);

	if(rlight)
		light->EnableLight();
}

void GLExMainView::paintGL()
{
	systeminfo->Update();

	/*
	glClearColor(0.0,0,0.0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);
	*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The Matrix

	light->Render();

	gluLookAt( eyex, eyey, eyez,  centerx, centery, centerz,0,1,0 );	// This Determines Where The Camera's Position And View Is

	glPushMatrix();	
    glMultMatrixf(Transform.M);										// NEW: Apply Dynamic Transform
	for(std::vector<GLExTerrain*>::iterator it = terrainList->begin();it != terrainList->end(); ++it)
	{
		(*it)->Render();
	}
	glPopMatrix();

	glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(0.0f,0.0f,-6.0f);
	glMultMatrixf(Transform.M);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(1000.0,0,0);
	glVertex3f(0,0,0);
	glVertex3f(0,1000.0,0);
	glVertex3f(0,0,0);
	glVertex3f(0,0,1000.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(0.0f,0.0f,-6.0f);
	glMultMatrixf(Transform.M);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3f(TerrainPosition[0],TerrainPosition[1],TerrainPosition[2]);
	glVertex3f(1000.0,0,0);
	glVertex3f(TerrainPosition[0],TerrainPosition[1],TerrainPosition[2]);
	glVertex3f(0,1000.0,0);
	glVertex3f(TerrainPosition[0],TerrainPosition[1],TerrainPosition[2]);
	glVertex3f(0,0,1000.0);
	glEnd();
	glPopMatrix();
	
	systeminfo->Update();
	freetype->PrintSystemInfo();
	freetype->PrintSystemOption(tex);
	
}

void GLExMainView::resizeGL(int width, int height)
{
	freetype->SetWindowSize(width,height);
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window.  Farthest Distance Changed To 500.0f (NEW)
	gluPerspective(45.0f, 1.0*width/height, 0.1f, 50000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	this->width = width;
	this->height = height;
	glViewport(0,0, width, height);

	ArcBall.setBounds((GLfloat)width, (GLfloat)height);
	updateGL();
}

void GLExMainView::mousePressEvent(QMouseEvent *event)
{
	int x = event->x(),y= event->y();
	if (event->button() == Qt::LeftButton )
	{
		MousePt.s.X = x;
		MousePt.s.Y = y;
		isClicked = true;
		LastRot = ThisRot;										// Set Last Static Rotation To Last Dynamic One
		ArcBall.click(&MousePt);								// Update Start Vector And Prepare For Dragging
	}
	if(event->button() == Qt::RightButton )
	{
		isRBClicked = true;
		biasx = x;
		biasy = y;
	}
	updateGL();
}

void GLExMainView::mouseReleaseEvent(QMouseEvent *event)
{
	int x = event->x(),y= event->y();
	if (event->button() == Qt::LeftButton )
	{
		MousePt.s.X = x;
		MousePt.s.Y = y;
		isClicked = false;
	}
	if(event->button() == Qt::RightButton )
	{
		isRBClicked = false;
		biasx = x;
		biasy = y;
	}
	updateGL();
}


void GLExMainView::mouseMoveEvent(QMouseEvent *event)
{
	int x = event->x(),y= event->y();
	MousePt.s.X = x;
	MousePt.s.Y = y;
	if (isClicked)												// Still Clicked, So Still Dragging
    {
        Quat4fT ThisQuat;
        ArcBall.drag(&MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
        Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT
        Matrix3fMulMatrix3f(&ThisRot, &LastRot);				// Accumulate Last Rotation Into This One
        Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);	// Set Our Final Transform's Rotation From This One
    }
	if(isRBClicked)
	{
		GLfloat bx,by;
		GLfloat tempx,tempy,tempz;
		bx = x - biasx;
		by = y - biasy;
		GLfloat rmat[16] = {0};

		glPushMatrix();
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-centerx,-centery,-centerz);
		if(fabs(bx)>fabs(by))
		{	
			bx = 360.0 * bx / this->width;
			by = 360.0 * by / this->height;
			glRotatef(-bx,0,1,0);
		}
		else
		{
			GLfloat deltax,deltay,deltaz,newx,newy,newz,length;
			deltax = centerx - eyex;
			deltay = centery - eyey;
			deltaz = centerz - eyez;
			//length = sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
			tempx = deltaz;
			tempy = 0;
			tempz = -deltax;
			length = sqrt(tempx*tempx+tempy*tempy+tempz*tempz);
			newx = tempx / length;
			newy = tempy / length;
			newz = tempz / length;

			bx = 360.0 * bx / this->width;
			by = 360.0 * by / this->height;
			
			glRotatef(by,newx,newy,newz);
		}
		glTranslatef(centerx,centery,centerz);

		eyex = eyex - centerx;
		eyey = eyey - centery;
		eyez = eyez - centerz;
		glGetFloatv(GL_MODELVIEW_MATRIX,rmat);
		tempx = rmat[0]*eyex + rmat[4]*eyey + rmat[8]*eyez;
		tempy = rmat[1]*eyex + rmat[5]*eyey + rmat[9]*eyez;
		tempz = rmat[2]*eyex + rmat[6]*eyey + rmat[10]*eyez;

		Point3D ta(tempx,tempy,tempz,1.0),tb(0,1,0,1);
		ta.Normalize();
		GLfloat result = ta.DotProduct(tb);
		if(result < 0.95 && result > -0.95)
		{
			eyex = centerx + tempx;
			eyey = centery + tempy;
			eyez = centerz + tempz;
		}
		else
		{
			eyex = eyex + centerx;
			eyey = eyey + centery;
			eyez = eyez + centerz;
		}
		glPopMatrix();
	}
	biasx = x;
	biasy = y;
	updateGL();
}

void GLExMainView::wheelEvent(QWheelEvent *event)
{
	if( event->delta() > 0 )
		SizeForward();
	else if( event->delta() < 0 )
		SizeBackward();
	updateGL();
}

void GLExMainView::SizeForward()
{
	float deltax,deltay,deltaz;
	float length;
	deltax = centerx - eyex;
	deltay = centery - eyey;
	deltaz = centerz - eyez;
	length = sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
	if(length > 1)
	{
		eyex += deltax / length;
		eyey += deltay / length; 
		eyez += deltaz / length; 
	}
}

void GLExMainView::SizeBackward()
{
	float deltax,deltay,deltaz;
	float length;
	deltax = centerx - eyex;
	deltay = centery - eyey;
	deltaz = centerz - eyez;
	length = sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
	if(length < 1000)
	{
		eyex -= deltax / length;
		eyey -= deltay / length; 
		eyez -= deltaz / length; 
	}
}

void GLExMainView::keyPressEvent( QKeyEvent * event )
{
	float deltax,deltay,deltaz,newx,newy,newz;
	float tempx,tempy,tempz;
	float length;
	
	if(event->text() == "W" || event->text() == "w" )
	{
		deltax = centerx - eyex;
		deltay = centery - eyey;
		deltaz = centerz - eyez;
		length = sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
		if(length > 1)
		{
			newx = deltax / length*velocity;
			newy = deltay / length*velocity;
			newz = deltaz / length*velocity;
			eyex += newx;
			eyey += newy; 
			eyez += newz; 
			centerx += newx;
			centery += newy;
			centerz += newz;
		}
	}
	else if(event->text() == "A" || event->text() == "a" )
	{
		deltax = centerx - eyex;
		deltay = centery - eyey;
		deltaz = centerz - eyez;
		//length = sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
		tempx = deltaz;
		tempy = 0; 
		tempz = -deltax;
		length = sqrt(tempx*tempx+tempy*tempy+tempz*tempz);
		newx = tempx / length*velocity/2;
		newy = tempy / length*velocity/2;
		newz = tempz / length*velocity/2;
		eyex += newx;
		eyey += newy; 
		eyez += newz; 
		centerx += newx;
		centery += newy;
		centerz += newz;
	}
	else if(event->text() == "S" || event->text() == "s" )
	{
		deltax = centerx - eyex;
		deltay = centery - eyey;
		deltaz = centerz - eyez;
		length = sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
		if(length > 1)
		{
			newx = deltax / length*velocity;
			newy = deltay / length*velocity;
			newz = deltaz / length*velocity;
			eyex -= newx;
			eyey -= newy; 
			eyez -= newz; 
			centerx -= newx;
			centery -= newy;
			centerz -= newz;
		}
	}
	else if(event->text() == "D" || event->text() == "d" )
	{
		deltax = centerx - eyex;
		deltay = centery - eyey;
		deltaz = centerz - eyez;
		//length = sqrt(deltax*deltax+deltay*deltay+deltaz*deltaz);
		tempx =  - deltaz;
		tempy = 0; 
		tempz = deltax;
		length = sqrt(tempx*tempx+tempy*tempy+tempz*tempz);
		newx = tempx / length*velocity/2;
		newy = tempy / length*velocity/2;
		newz = tempz / length*velocity/2;
		eyex += newx;
		eyey += newy; 
		eyez += newz; 
		centerx += newx;
		centery += newy;
		centerz += newz;
	}
	else if(event->text() == "T" || event->text() == "t" )
	{
		tex = !tex;
		for(std::vector<GLExTerrain*>::iterator it = terrainList->begin();it != terrainList->end(); ++it)
		{
			(*it)->DisplayTexture(tex);
		}
	}
	else if(event->text() == "F" || event->text() == "f" )
	{
		rfog = !rfog;
		fog->DisplayFog(rfog);
	}
	updateGL();
}