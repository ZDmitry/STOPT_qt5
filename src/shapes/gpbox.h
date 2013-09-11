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
#ifndef GPBOX_H
#define GPBOX_H

#include "shape.h"

class GPBox: public Shape {
public:
    GPBox(const QPointF &position, qreal width, qreal height);

    void   move(const QRectF &boundRect);
    QRectF rect() const;

    void   draw(QPainter *painter) const;
    void   draw() const {;}

    // GPBox uses only innerColor
    void   updateBrush();

    void   setWidth(qreal w) { box_.setWidth(w); }
    int    getWidth() const { return box_.width(); }

    void   setHeight(qreal h) { box_.setHeight(h); }
    int    getHeight() const { return box_.height(); }

private:
    GPBox(const GPBox &) = delete;
    GPBox &operator=(const GPBox &) = delete;

    void buildGeometry(int d, qreal s);

    QRectF box_;
};

#endif //GPBOX_H
