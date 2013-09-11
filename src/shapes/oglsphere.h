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
#ifndef OGLSPHERE_H
#define OGLSPHERE_H

// OpenGL
#ifdef Q_OS_MAC
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#elif defined(Q_OS_WIN)
#include <windows.h>
#include <GL/gl.h>
//#include <GL/glut.h>
#include <GL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class OglSphere{
public:
	OglSphere();
	OglSphere(double x, double y, double z, double r);

    void     setRadius( double r){r_=r;}
    double   getRadius() const {return r_;}

    void	 setColor(float r, float g, float b);
    void	 setColor(float r, float g, float b, float a);

    float    getR() const {return col_r_;}
    float    getG() const {return col_g_;}
    float    getB() const {return col_b_;}
    float    getA() const {return col_alp_;}

    double   getX() const {return ox_;}
    double   getY() const {return oy_;}
    double   getZ() const {return oz_;}

    void     moveTo(double x, double y, double z){ox_=x; oy_=y; oz_=z;}

    void     render(bool useCore);

    OglSphere&  operator=(const OglSphere&);

private:
    void     draw();
    void     drawCenter();

	void     glSphere(bool, GLfloat, GLfloat, GLfloat);
	void     glSphere(bool, GLfloat, GLfloat, GLfloat, GLfloat);
	void     glSphere(bool, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	void     glSphere(GLfloat, GLfloat, GLfloat, bool, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

	double	 r_;
	float    col_r_, col_g_, col_b_, col_alp_;
    double	 ox_, oy_, oz_;
};

#endif //OGLSPHERE_H

