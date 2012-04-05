#include "GLExFog.h"
#include "GLExTerrain.h"

float scaleValue = 0.15f;				// Scale Value For The Terrain (NEW)

GLExTerrain::GLExTerrain(int x,int y,const char *filename)
{
	LoadRawFile(filename, MAP_SIZE * MAP_SIZE, g_HeightMap);	// (NEW)

	CalcNormal();
	bRender = true;
	texRender = false;
	startx = x*MAP_SIZE;//; - x*STEP_SIZE;
	starty = y*MAP_SIZE;// - y*STEP_SIZE;
	ptexture = NULL;
	right =NULL;
	down = NULL;
}


GLExTerrain::~GLExTerrain(void)
{
	if(normal!=NULL)
	{
		for(std::vector<Point3D*>::iterator i = normal->begin();i != normal->begin();i++)
			delete *i;
		normal->clear();
		delete normal;
	}
	if(ptexture!=NULL)
		delete ptexture;
}

void GLExTerrain::DisplayTexture(bool b)
{
	this->texRender = b;
	if(b)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

QPixmap& GLExTerrain::LoadTexture(QString m)
{
	ptexture = new QPixmap(m);
	return *ptexture;
}

void GLExTerrain::SetTexture(GLuint tex)
{
	texture[0] = tex;
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, tex[0]->width(), tex[0]->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, tex[0]->bits());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//Linear Filering
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//Mipmapping
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	//While...it's old fasion mipmapping style, and decrease the performance clearly.
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture->width(), texture->height(), GL_RGB, GL_UNSIGNED_BYTE, texture->data());
	//New1
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	//New2
	//glGenerateMipmapEXT(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); /* or GL_REPLACE */
}

void GLExTerrain::LoadRawFile(const char* strName, int nSize, unsigned char *pHeightMap)
{
	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.
	pFile = fopen( strName, "rb" );

	// Check To See If We Found The File And Could Open It
	if ( pFile == NULL )	
	{
		// Display Error Message And Stop The Function
		//MessageBox(NULL, "Can't Find The Height Map!", "Error", MB_OK);
		return;
	}

	fread( pHeightMap, 1, nSize, pFile );

	// After We Read The Data, It's A Good Idea To Check If Everything Read Fine
	int result = ferror( pFile );

	// Check If We Received An Error
	if (result)
	{
	}

	// Close The File.
	fclose(pFile);
}

int GLExTerrain::Height(unsigned char *pHeightMap, int X, int Y)				// This Returns The Height From A Height Map Index
{
	int x = X % MAP_SIZE;								// Error Check Our x Value
	int y = Y % MAP_SIZE;								// Error Check Our y Value

	if(!pHeightMap) return 0;							// Make Sure Our Data Is Valid

	return pHeightMap[x + (y * MAP_SIZE)];				// Index Into Our Height Array And Return The Height
}

void GLExTerrain::SetVertexColor(unsigned char *pHeightMap, int x, int y)		// Sets The Color Value For A Particular Index, Depending On The Height Index
{
	if(!pHeightMap) return;								// Make Sure Our Height Data Is Valid

	float fColor = -0.15f + (Height(pHeightMap, x, y ) / 256.0f);

	// Assign This Blue Shade To The Current Vertex
	glColor3f(0, 0, 1.0/*fColor*/ );
}

void GLExTerrain::Render()
{
	//Position View Up Vector
	glPushMatrix();
	//glScalef(scaleValue, scaleValue * HEIGHT_RATIO, scaleValue);
	GLfloat ambient[4] = {1.0,1.0,1.0,1.0};
	GLfloat diffuse[4] = {1.0,1.0,1.0,1.0};
	GLfloat specular[4] = {1.0,1.0,1.0,1.0};
	GLfloat shiness = 30;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT,GL_SHININESS, &shiness);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glEnable(GL_COLOR_MATERIAL);

	int X = 0, Y = 0;									// Create Some Variables To Walk The Array With.
	
	int m = 0, n = 0;
	int step = MAP_SIZE/STEP_SIZE;
	int x, y, z;										// Create Some Variables For Readability

	if(!g_HeightMap) return;								// Make Sure Our Height Data Is Valid
	if(this->texRender)
		glBindTexture(GL_TEXTURE_2D, texture[0]);

	if(bRender)											// What We Want To Render
		glBegin( GL_QUADS );							// Render Polygons
	else 
		glBegin( GL_LINES );							// Render Lines Instead

	glEnable(GL_NORMALIZE);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	double texXBias = 0,texYBias = 0,texStep = 1.0 * STEP_SIZE / MAP_SIZE;

	for ( X = 0,m=0; X < (MAP_SIZE-STEP_SIZE); X += STEP_SIZE,++m )
		for ( Y = 0,n=0; Y < (MAP_SIZE-STEP_SIZE); Y += STEP_SIZE,++n )
		{
			// Get The (X, Y, Z) Value For The Bottom Left Vertex
			x = X;							
			y = Height(g_HeightMap, X, Y );	
			z = Y;							
			
			texXBias = 1.0* X / MAP_SIZE;
			texYBias = 1 - 1.0* Y / MAP_SIZE - texStep;

			Point3D *t = (*normal)[m*step+n];
			glNormal3f(t->v[0],t->v[1],t->v[2]);

			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias, texYBias + texStep);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);
			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered (Integer Points Are Faster)
			// Get The (X, Y, Z) Value For The Top Left Vertex
			x = X;										
			y = Height(g_HeightMap, X, Y + STEP_SIZE );  
			z = Y + STEP_SIZE;							
			
			t = (*normal)[(m+1)*step+n];
			glNormal3f(t->v[0],t->v[1],t->v[2]);

			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias, texYBias);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);
			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
			// Get The (X, Y, Z) Value For The Top Right Vertex
			x = X + STEP_SIZE; 
			y = Height(g_HeightMap, X + STEP_SIZE, Y + STEP_SIZE ); 
			z = Y + STEP_SIZE;
			
			t = (*normal)[(m+1)*step+n+1];
			glNormal3f(t->v[0],t->v[1],t->v[2]);

			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias + texStep, texYBias);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);

			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
			// Get The (X, Y, Z) Value For The Bottom Right Vertex
			x = X + STEP_SIZE; 
			y = Height(g_HeightMap, X + STEP_SIZE, Y ); 
			z = Y;

			t = (*normal)[(m)*step+n+1];
			glNormal3f(t->v[0],t->v[1],t->v[2]);

			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias + texStep, texYBias + texStep);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);
			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
		}	
	//gap right
	if(right != NULL)
	{
		for (int Yy = 0; Yy < (MAP_SIZE-STEP_SIZE); Yy += STEP_SIZE )
		{
			// Get The (X, Y, Z) Value For The Bottom Left Vertex
			x = X;							
			y = Height(g_HeightMap, X, Yy );	
			z = Yy;							
			
			texXBias = 1.0* X / MAP_SIZE;
			texYBias = 1 - 1.0* Yy / MAP_SIZE - texStep;

			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias, texYBias + texStep);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);
		
			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered (Integer Points Are Faster)
			// Get The (X, Y, Z) Value For The Top Left Vertex
			x = X;										
			y = Height(g_HeightMap, X, Yy + STEP_SIZE );  
			z = Yy + STEP_SIZE;							

			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias, texYBias);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);

			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
			// Get The (X, Y, Z) Value For The Top Right Vertex
			x = X + STEP_SIZE;
			y = Height(right->g_HeightMap, 0, Yy + STEP_SIZE ); 
			z = Yy + STEP_SIZE;
			
			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias + texStep, texYBias);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);

			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
			// Get The (X, Y, Z) Value For The Bottom Right Vertex
			x = X + STEP_SIZE; 
			y = Height(right->g_HeightMap, 0, Yy );
			z = Yy;
		
			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias + texStep, texYBias + texStep);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);
			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
		}
	}

	//gap down
	if(down != NULL)
	{
		for (int Xx = 0; Xx < (MAP_SIZE-STEP_SIZE); Xx += STEP_SIZE )
		{
			// Get The (X, Y, Z) Value For The Bottom Left Vertex
			x = Xx;							
			y = Height(g_HeightMap, Xx, Y );
			z = Y;							
			
			texXBias = 1.0* Xx / MAP_SIZE;
			texYBias = 1 - 1.0* Y / MAP_SIZE - texStep;

			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias, texYBias + texStep);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);
		
			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered (Integer Points Are Faster)
			// Get The (X, Y, Z) Value For The Top Left Vertex
			x = Xx;										
			y = Height(down->g_HeightMap, Xx, 0 ); 
			z = Y + STEP_SIZE;							

			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias, texYBias);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);

			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
			// Get The (X, Y, Z) Value For The Top Right Vertex
			x = Xx + STEP_SIZE;
			y = Height(down->g_HeightMap, Xx + STEP_SIZE, 0 ); 
			z = Y + STEP_SIZE;
			
			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias + texStep, texYBias);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);

			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
			// Get The (X, Y, Z) Value For The Bottom Right Vertex
			x = Xx + STEP_SIZE; 
			y = Height(g_HeightMap, Xx + STEP_SIZE, Y );
			z = Y;
		
			if(fogRender)
				glFogCoordf(100.0);

			if(texRender)
				glTexCoord2d(texXBias + texStep, texYBias + texStep);
			else
				// Set The Color Value Of The Current Vertex
				SetVertexColor(g_HeightMap, x, z);
			glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
		}
	}

	if( right != NULL && down != NULL && down->right != NULL )
	{
		x = X;							
		y = Height(g_HeightMap, X, Y );
		z = Y;							
			
		texXBias = 1.0* X / MAP_SIZE;
		texYBias = 1 - 1.0* Y / MAP_SIZE - texStep;

		if(fogRender)
			glFogCoordf(100.0);

		if(texRender)
			glTexCoord2d(texXBias, texYBias + texStep);
		else
			// Set The Color Value Of The Current Vertex
			SetVertexColor(g_HeightMap, x, z);
		
		glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered (Integer Points Are Faster)
		// Get The (X, Y, Z) Value For The Top Left Vertex
		x = X;										
		y = Height(down->g_HeightMap, X, 0 ); 
		z = Y + STEP_SIZE;							

		if(fogRender)
			glFogCoordf(100.0);

		if(texRender)
			glTexCoord2d(texXBias, texYBias);
		else
			// Set The Color Value Of The Current Vertex
			SetVertexColor(g_HeightMap, x, z);

		glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
		// Get The (X, Y, Z) Value For The Top Right Vertex
		x = X + STEP_SIZE;
		y = Height(down->right->g_HeightMap, 0, 0 ); 
		z = Y + STEP_SIZE;
			
		if(fogRender)
			glFogCoordf(100.0);

		if(texRender)
			glTexCoord2d(texXBias + texStep, texYBias);
		else
			// Set The Color Value Of The Current Vertex
			SetVertexColor(g_HeightMap, x, z);

		glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
		// Get The (X, Y, Z) Value For The Bottom Right Vertex
		x = X + STEP_SIZE; 
		y = Height(right->g_HeightMap, 0, Y );
		z = Y;
		
		if(fogRender)
			glFogCoordf(100.0);

		if(texRender)
			glTexCoord2d(texXBias + texStep, texYBias + texStep);
		else
			// Set The Color Value Of The Current Vertex
			SetVertexColor(g_HeightMap, x, z);
		glVertex3i(x + startx, y, z + starty);						// Send This Vertex To OpenGL To Be Rendered
	}
	glDisable(GL_NORMALIZE);
	glEnd();

	
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(0.0,1.0,0);
	for ( X = 0,m=0; X < (MAP_SIZE-STEP_SIZE); X += STEP_SIZE,++m )
		for ( Y = 0,n=0; Y < (MAP_SIZE-STEP_SIZE); Y += STEP_SIZE,++n )
		{
			// Get The (X, Y, Z) Value For The Bottom Left Vertex
			x = X;
			z = Y;
			y = Height(g_HeightMap, X, Y );
			Point3D *t = (*normal)[m*step+n];
			glVertex3f(x + startx, y, z + starty);
			glVertex3f(x + startx+10*t->v[0],y+10*t->v[1],z + starty+10*t->v[2]);
		}
	glEnd();
	glEnable(GL_LIGHTING);


	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glPopMatrix();
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);					// Reset The Color
}

void GLExTerrain::InitLookAt(float a,float b,float c,float d,float e,float f)
{
	centerx = d;
	centery = e;
	centerz = f;	
	eyex = a;
	eyey = b;
	eyez = c;
}

void GLExTerrain::SizeForward()
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

void GLExTerrain::SizeBackward()
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

void GLExTerrain::DisplayFog(bool b)
{
	fogRender = b;
}

void GLExTerrain::CalcNormal()
{
	int max = MAP_SIZE / STEP_SIZE;
	normal = new std::vector<Point3D*>();

	std::vector<Point3D*> *fnormal = new std::vector<Point3D*>();

	int X,Y;
	for ( X = 0; X < (MAP_SIZE-STEP_SIZE); X += STEP_SIZE)
		for ( Y = 0; Y < (MAP_SIZE-STEP_SIZE); Y += STEP_SIZE )
		{
			Point3D vx((GLfloat)X,(GLfloat)Height(g_HeightMap, X, Y ),(GLfloat)Y,0);
			Point3D vy((GLfloat)X,(GLfloat)Height(g_HeightMap, X, Y+ STEP_SIZE ),(GLfloat)Y+ STEP_SIZE,0);
			Point3D vz((GLfloat)X+ STEP_SIZE,(GLfloat)Height(g_HeightMap, X+ STEP_SIZE, Y+ STEP_SIZE ),(GLfloat)Y+ STEP_SIZE,0);
			// Get The (X, Y, Z) Value For The Bottom Left Vertex
			Point3D ea,eb;
			ea = vy - vx;
			eb = vz - vy;

			Point3D *vt = new Point3D(ea.CrossProduct(eb));	
			vt->Normalize();

			fnormal->push_back(vt);
		}
	int step = MAP_SIZE/STEP_SIZE;
	int m = 0,n = 0;
	for ( m = 0; m < step; ++m)
		for ( n = 0; n < step; ++n )
		{
			Point3D *a,*b,*c,*d;
			a = b = c = d = NULL;
			
			if( m > 0 && n > 0)
				a = (*fnormal)[(m-1)*(step-1) + n - 1];

			if( m > 0 && n < step - 1)
				b = (*fnormal)[ (m-1)*(step-1) + n];

			if( m < step - 1 && n < step - 1 )
				c = (*fnormal)[ m*(step-1) + n];
			
			if( m < step - 1 && n > 0 )
				d = (*fnormal)[ m*(step-1) + n - 1];

			Point3D *normTemp = new Point3D();
			//if(a!= NULL && b!= NULL && c!=NULL && d!=NULL)
			if(a!= NULL)
				*normTemp += *a;
			if(b!= NULL)
				*normTemp += *b;
			if(c!= NULL)
				*normTemp += *c;
			if(d!= NULL)
				*normTemp += *d;
			normTemp->Normalize();
			normal->push_back(normTemp);
		}
	if(fnormal!=NULL)
	{
		for(std::vector<Point3D*>::iterator i = fnormal->begin();i != fnormal->begin();i++)
			delete *i;
		fnormal->clear();
		delete fnormal;
	}
}

void GLExTerrain::SetRightTerrain(GLExTerrain *r)
{
	right = r;
}

void GLExTerrain::SetDownTerrain(GLExTerrain *d)
{	
	down = d;
}