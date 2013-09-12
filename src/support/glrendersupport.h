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

#ifndef GLRENDERSUPPORT_H
#define GLRENDERSUPPORT_H

#include <GL/gl.h>
#include <QVector>
#include <QVector3D>

class QMatrix4x4;

struct GLGeometry
{
    QVector<GLushort> faces;
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    void appendSmooth(const QVector3D &a, const QVector3D &n, int from);
    void appendFaceted(const QVector3D &a, const QVector3D &n);
    void finalize();
    void loadArrays() const;
};

class GLRenderSupport
{
public:
    enum Smoothing { Faceted, Smooth };

    GLRenderSupport(GLGeometry *);
    ~GLRenderSupport();

    void setSmoothing(Smoothing s) { sm = s; }
    void translate(const QVector3D &t);
    void rotate(qreal deg, QVector3D axis);

    void draw() const;

    void addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n);
    void addQuad(const QVector3D &a, const QVector3D &b,  const QVector3D &c, const QVector3D &d);

    GLushort start;
    GLushort count;
    GLushort initv;

    GLfloat    *faceColor; //[4];
    QMatrix4x4 *mat;
    Smoothing   sm;
    GLGeometry *geom;
};

class GLPart
{
public:
    void translate(const QVector3D &t)
    {
        for (int i = 0; i < parts.count(); ++i)
            parts[i]->translate(t);
    }

    void rotate(qreal deg, QVector3D axis)
    {
        for (int i = 0; i < parts.count(); ++i)
            parts[i]->rotate(deg, axis);
    }

    // No GLPart destructor - the parts are fetched out of this member
    // variable, and destroyed by the new owner
    QList<GLRenderSupport *> parts;
};

class GLCube : public GLPart
{
public:
    GLCube(GLGeometry *g, qreal width, qreal height, qreal depth);
};

#endif // GLRENDERSUPPORT_H
