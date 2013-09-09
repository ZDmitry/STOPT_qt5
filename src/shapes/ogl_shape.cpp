/*
    This file is part of STOPT project.

    Copyright 2012-2013       by Dmitry Zagnoyko <hiroshidi@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

#include "ogl_shape.h"

#include <cassert>

//OpenGL Headers Files
//#include <gl\glut.h>


OglSphere::OglSphere(double x, double y, double z, double r)
{
	ox_=x; oy_=y; oz_=z;
	assert(r>0);
	r_=r;
}

OglSphere::OglSphere()
{
	ox_=oy_=oz_=0;
	r_=1;
}

OglSphere& OglSphere::operator=(const OglSphere& r)
{
	ox_=r.getX();
	oy_=r.getY();
	r_=r.getRadius();
	col_r_=r.getR();
	col_g_=r.getG();
	col_b_=r.getB();
	col_alp_=r.getA();

	return (*this);
}

void OglSphere::setColor(float r, float g, float b)
{
	setColor(r,g,b,1.0f);
}

void OglSphere::setColor(float r, float g, float b, float a)
{
	col_r_=r;
	col_g_=g;
	col_b_=b;
	col_alp_=a;
}

void  OglSphere::render(bool useCore)
{
    if (useCore) drawCenter();
    draw();
}

void OglSphere::drawCenter()
{
	glPushMatrix();
		glSphere(ox_, oy_, oz_, 0, r_/20, 0.9,0.9,0.9,1.f);
	glPopMatrix();
}

void OglSphere::draw()
{
	glPushMatrix();
		glSphere(ox_, oy_, oz_, 0, r_*1.16f, col_r_, col_g_, col_b_, col_alp_);
	glPopMatrix();
}

//SPHERE
void OglSphere::glSphere(bool bWired, GLfloat r, GLfloat g, GLfloat b)
{
	glSphere(bWired, r, g, b, 1.0f);
}

void OglSphere::glSphere(bool bWired, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glSphere(bWired, 1.0f, r, g, b, a);
}

void OglSphere::glSphere(bool bWired, GLfloat s, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glSphere(0.0f, 0.0f, 0.0f, bWired, s, r, g, b, a);
}

void OglSphere::glSphere(GLfloat x, GLfloat y, GLfloat z, bool bWired, GLfloat s, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    GLUquadric* params = gluNewQuadric();
    gluQuadricTexture(params,GL_TRUE);

	glPushMatrix();
		//glScalef(s,s,s);
		glTranslatef(x,y,z);
		
		glColor4f(r,g,b,a);

		//glMaterialfv(GL_FRONT,GL_AMBIENT,make_vect(0.2473f, 0.1995f, 0.0745f, 1.0f));
		//glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,make_vect(r,g,b,1.f));
		//glMaterialfv(GL_FRONT,GL_SPECULAR,make_vect(0.6283f, 0.5558f, 0.3661f, 1.0f));
		//glMaterialfv(GL_FRONT,GL_EMISSION,make_vect(0.0f, 0.0f, 0.0f, 0.0f));
		//glMaterialf(GL_FRONT,GL_SHININESS,3.0f);

		if (bWired)
            //glutWireSphere(s,60,60);
            gluQuadricDrawStyle(params,GLU_LINE);
		else
            //glutSolidSphere(s,60,60);
            gluQuadricDrawStyle(params,GLU_FILL);

        //args: [1]GLUquadric*, [2]GLdouble, [3]GLint, [4]GLint
        gluSphere(params,s,60,60);
	glPopMatrix();

    gluDeleteQuadric(params);
}
