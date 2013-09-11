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

#include "oglbox.h"

#include <QGLWidget>
#include <QVector3D>

//extra gl functions
#include <GL/glu.h>
#include "support/glrendersupport.h"

static inline void qSetColor(float colorVec[], QColor c)
{
    colorVec[0] = c.redF();
    colorVec[1] = c.greenF();
    colorVec[2] = c.blueF();
    colorVec[3] = c.alphaF();
}

OGLBox::OGLBox(const QPointF   &position, qreal z, qreal width, qreal height, qreal lenght)
    : Shape(position)
{
    pos3d_ = new QVector3D(pos_.x(),pos_.y(), z);
    dim3d_ = new QVector3D(width, height, lenght);
    wireframe_ = false;

    fColor_ = new GLfloat[4];
    qSetColor(fColor_, QColor(Qt::darkGray));

    // alpha got from innerColor
    fColor_[4] = inColor_.alphaF();
}

OGLBox::OGLBox(const QVector3D &position, qreal width, qreal height, qreal lenght)
    : Shape( QPointF(position.x(), position.y()) )
{
    pos3d_ = new QVector3D(position.x(), position.y(), position.z());
    dim3d_ = new QVector3D(width, height, lenght);
    wireframe_ = false;

    fColor_ = new GLfloat[4];
    qSetColor(fColor_, QColor(Qt::darkGray));

    // alpha got from innerColor
    fColor_[4] = inColor_.alphaF();
}

OGLBox::~OGLBox()
{
    delete pos3d_;
    delete dim3d_;
    delete[] fColor_;
}

void OGLBox::move(const QVector3D &newPos)
{
    pos3d_->setX(newPos.x());
    pos3d_->setY(newPos.y());
    pos3d_->setZ(newPos.z());
}

QRectF OGLBox::rect() const
{
    return QRectF(pos_.x(), pos_.y(),
                  dim3d_->x(), dim3d_->y());
}

qreal OGLBox::zCoord() const
{
    return pos3d_->z();
}

qreal OGLBox::getWidth() const
{
    return dim3d_->x();
}

qreal OGLBox::getHeight() const
{
    return dim3d_->y();
}

qreal OGLBox::getLenght() const
{
    return dim3d_->z();
}

void OGLBox::setDimensions(const QVector3D &newDim)
{
    dim3d_->setX(newDim.x());
    dim3d_->setY(newDim.y());
    dim3d_->setZ(newDim.z());
}

void OGLBox::draw() const
{
    GLUquadric* params = gluNewQuadric();
    gluQuadricTexture(params,GL_TRUE);

    glPushMatrix();
        //glScalef(s,s,s);
        //glTranslatef(x,y,z);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fColor_);

        //glMaterialfv(GL_FRONT,GL_AMBIENT,make_vect(0.2473f, 0.1995f, 0.0745f, 1.0f));
        //glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,make_vect(r,g,b,1.f));
        //glMaterialfv(GL_FRONT,GL_SPECULAR,make_vect(0.6283f, 0.5558f, 0.3661f, 1.0f));
        //glMaterialfv(GL_FRONT,GL_EMISSION,make_vect(0.0f, 0.0f, 0.0f, 0.0f));
        //glMaterialf(GL_FRONT,GL_SHININESS,3.0f);

        if (wireframe_)
            //glutWireSphere(s,60,60);
            gluQuadricDrawStyle(params,GLU_LINE);
        else
            //glutSolidSphere(s,60,60);
            gluQuadricDrawStyle(params,GLU_FILL);

        //args: [1]GLUquadric*, [2]GLdouble, [3]GLint, [4]GLint
        //gluSphere(params, radius_*1.16f, SPHERE_SLICE, SPHERE_STACK);

    glPopMatrix();

    gluDeleteQuadric(params);

}
