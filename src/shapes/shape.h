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
#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>

class Shape
{
public:
    virtual ~Shape() {}

    // position (by boundingRect)
    virtual void   move(const QRectF &boundRect) = 0;
    virtual QRectF rect() const = 0;

    // for 2d shapes (uses QPainter)
    virtual void draw(QPainter *painter) const { Q_UNUSED(painter) }
    // for 2d shapes (uses GL funcs)
    virtual void draw() const {}

    void    setInnerColor(const QColor&);
    QColor  getInnerColor() const;

    // this is main color
    void    setOuterColor(const QColor&);
    QColor  getOuterColor() const;

protected:
    Shape(const QPointF &pos);
    virtual void buildGeometry(){}

    // call update brush after
    // each color changes
    virtual void updateBrush() = 0;

    QColor  inColor_;
    QColor  outColor_;
    QBrush  brush_;
    QPointF pos_;

private:
    QColor randColor();
};

#endif // SHAPE_H
