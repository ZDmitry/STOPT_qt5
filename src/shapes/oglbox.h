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
#ifndef OGLBOX_H
#define OGLBOX_H

#include "shape.h"

#include <qopengl.h>

class QVector3D;
class GLRenderSupport;
struct GLGeometry;

class OGLBox: public Shape {
public:
    OGLBox(const QPointF   &position, qreal z, qreal width, qreal height, qreal lenght);
    OGLBox(const QVector3D &position, qreal width, qreal height, qreal lenght);
    OGLBox(const QVector3D &position, const QVector3D &dimensions);
    ~OGLBox();

    // nothing to do
    void       move(const QRectF &boundRect) {}
    // correct move
    void       move(const QVector3D &newPos);

    QRectF     rect() const;
    qreal      zCoord() const;
    const QVector3D* position() {   return pos3d_; }
    const QVector3D* dimensions() { return dim3d_; }
    void       setDimensions(const QVector3D &newDim);

    // OpenGL instructions
    void       draw() const;

    qreal      getWidth() const;
    void       setWidth(qreal w);

    qreal      getHeight() const;
    void       setHeight(qreal h);

    qreal      getLenght() const;
    void       setLenght(qreal l);

private:
    OGLBox(const OGLBox &) = delete;
    OGLBox &operator=(const OGLBox &) = delete;

    // OGLBox uses only outerColor
    void       updateBrush();

    void       buildGeometry();

    bool       wireframe_;
    // coord      x, y, z
    QVector3D  *pos3d_;
    // dimensions x, y, z
    QVector3D  *dim3d_;

    QList<GLRenderSupport *>  parts_;
    GLGeometry               *geom_;

    static const qreal SCALE;
};

#endif //OGLBOX_H
