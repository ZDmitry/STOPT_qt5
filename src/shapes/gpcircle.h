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
#ifndef GPCIRCLE_H
#define GPCIRCLE_H

#include "shape.h"

class GPCircle: public Shape {
public:
    GPCircle(const QPointF &position, qreal radius, int num);

    void   move(const QRectF &boundRect);
    QRectF rect() const;

    void   draw(QPainter *painter) const;
    void   draw() const {;}

    void   updateBrush();

    void   setNumber(int n){ number_=n; }
    int    getNumber() const { return number_; }

    void   setRadius(int r){ radius_=r; }
    int    getRadius() const { return radius_; }

private:
    GPCircle(const GPCircle &) = delete;
    GPCircle &operator=(const GPCircle &) = delete;

    void buildGeometry(int d, qreal s);

    qreal   radius_;
    int     number_;
};

#endif //GPCIRCLE_H
