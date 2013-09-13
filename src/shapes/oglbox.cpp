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

const qreal OGLBox::SCALE     = 1.0;

static inline void qSetColor(float colorVec[], QColor c)
{
    colorVec[0] = c.redF();
    colorVec[1] = c.greenF();
    colorVec[2] = c.blueF();
    colorVec[3] = c.alphaF();
}

OGLBox::OGLBox(const QPointF   &position, qreal z, qreal width, qreal height, qreal lenght)
    : Shape( QPointF(position.x(), position.y()) ),
      geom_( new GLGeometry())
{
    pos3d_ = new QVector3D(pos_.x(),pos_.y(), z);
    dim3d_ = new QVector3D(width, height, lenght);
    wireframe_ = false;

    outColor_.setNamedColor( QColor(Qt::darkGray).name() );
    updateBrush();

    buildGeometry();
}

OGLBox::OGLBox(const QVector3D &position, qreal width, qreal height, qreal lenght)
    : Shape( QPointF(position.x(), position.y()) ),
      geom_( new GLGeometry() )
{
    pos3d_ = new QVector3D(position.x(), position.y(), position.z());
    dim3d_ = new QVector3D(width, height, lenght);
    wireframe_ = false;

    outColor_.setNamedColor( QColor(Qt::darkGray).name() );
    updateBrush();

    buildGeometry();
}

OGLBox::OGLBox(const QVector3D &position, const QVector3D &dimensions)
    : Shape( QPointF(position.x(), position.y()) ),
      geom_( new GLGeometry() )
{
    pos3d_ = new QVector3D(position.x(),position.y(), position.z());
    dim3d_ = new QVector3D(dimensions.x(), dimensions.y(), dimensions.z());
    wireframe_ = false;

    outColor_.setNamedColor( QColor(Qt::darkGray).name() );
    updateBrush();

    buildGeometry();
}

void  OGLBox::buildGeometry()
{
   qreal width  = dim3d_->x() * SCALE; // w  0.113137
   qreal height = dim3d_->y() * SCALE; // h  0.311126
   qreal depth  = dim3d_->z() * SCALE; // d  0.11

   // geom, w, h, d
   GLCube cube(geom_, height, width, depth);

//   QVector3D rot(0.0, 1.0, 0.0);
//   cube.rotate(25.0, rot);
//   rot.setX(1.0);
//   rot.setY(0.0);
//   cube.rotate(20.0, rot);

   parts_ << cube.parts;

   geom_->finalize();
}

OGLBox::~OGLBox()
{
    delete pos3d_;
    delete dim3d_;
    delete geom_;

    qDeleteAll(parts_);
}

void OGLBox::updateBrush()
{
    for (int i = 0; i < parts_.count(); ++i)
        qSetColor(parts_[i]->faceColor, outColor_);
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
    geom_->loadArrays();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    for (int i = 0; i < parts_.count(); ++i)
        parts_[i]->draw();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

}
