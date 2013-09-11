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

#include "shape.h"

void Shape::setInnerColor(const QColor& col) {
    inColor_=col;
}

QColor Shape::getInnerColor() const {
    return inColor_;
}

void Shape::setOuterColor(const QColor& col) {
    outColor_=col;
}

QColor Shape::getOuterColor() const {
    return outColor_;
}

Shape::Shape(const QPointF &pos)
    : pos_(pos)
{
    inColor_ = randColor();
    outColor_ = randColor();

    //updateBrush();
}

QColor Shape::randColor()
{
    int red = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int green = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int blue = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int alpha = int(91 + 100.0*qrand()/(RAND_MAX+1.0));

    return QColor(red, green, blue, alpha);
}
