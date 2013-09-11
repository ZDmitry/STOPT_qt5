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

#include "shape.h"

#include <qopengl.h>

class QVector3D;

class OGLSphere: public Shape {
public:
    OGLSphere(const QPointF   &position, qreal z, qreal radius);
    OGLSphere(const QVector3D &position, qreal radius);
    ~OGLSphere();

    // nothing to do
    void       move(const QRectF &boundRect) {}
    // correct move
    void       move(const QVector3D &newPos);

    QRectF     rect() const;
    qreal      zCoord() const;
    const QVector3D* position() { return pos3d_; }

    // nothing to do with simple paint
    void       draw(QPainter *painter) const {}
    // OpenGL instructions
    void       draw() const;

    // nothing to do
    void       updateBrush(){}

    void       setRadius(qreal r)  { radius_=r; }
    qreal      getRadius() const { return radius_; }

private:
    OGLSphere(const OGLSphere &) = delete;
    OGLSphere &operator=(const OGLSphere &) = delete;

    void       buildGeometry(int d, qreal s){}

    qreal      radius_;
    bool       wireframe_;
    // x, y, z
    QVector3D *pos3d_;
    GLfloat   *fColor_; //[4]

    static const int SPHERE_SLICE = 60;
    static const int SPHERE_STACK = 30;
};

#endif //OGLSPHERE_H

