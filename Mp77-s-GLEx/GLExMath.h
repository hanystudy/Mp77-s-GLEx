#pragma once
#include "stdafx.h"

class Point3D
{
public:
	Point3D(){
		v[0]=v[1]=v[2]=0;
		v[3]=1;
	};
	Point3D(GLfloat a,GLfloat b,GLfloat c,GLfloat d){
		v[0]=a;
		v[1]=b;
		v[2]=c;
		v[3]=d;
	};
	Point3D operator +(Point3D &b)
	{
		return Point3D(v[0]+b.v[0],v[1]+b.v[1],v[2]+b.v[2],1); 
	};
	Point3D operator -(Point3D &b)
	{
		return Point3D(v[0]-b.v[0],v[1]-b.v[1],v[2]-b.v[2],1); 
	};
	Point3D operator +=(Point3D &b)
	{
		v[0]+=b.v[0];
		v[1]+=b.v[1];
		v[2]+=b.v[2];
		return Point3D(v[0],v[1],v[2],1); 
	};
	Point3D operator -=(Point3D &b)
	{
		v[0]-=b.v[0];
		v[1]-=b.v[1];
		v[2]-=b.v[2];
		return Point3D(v[0],v[1],v[2],1); 
	};
	GLfloat DotProduct(Point3D &b)
	{
		return v[0]*b.v[0]+v[1]*b.v[1]+v[2]*b.v[2]; 
	};
	Point3D CrossProduct(Point3D &b)
	{
		return Point3D( v[1]*b.v[2]-v[2]*b.v[1], -v[0]*b.v[2]+v[2]*b.v[0], v[0]*b.v[1]-v[1]*b.v[0],1); 
	};
	void Normalize()
	{
		GLfloat ssum = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
		if(ssum > 0)
		{
			v[0] /= ssum;
			v[1] /= ssum;
			v[2] /= ssum;
		}
	};
	GLfloat v[4];
};

class GLExMath
{
public:
	GLExMath(void);
	~GLExMath(void);
};

