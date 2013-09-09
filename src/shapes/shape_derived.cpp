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

#include "shape_derived.h"

#include <cassert>

#include <QPen>
#include <QBrush>

// CIRCLE

Circle::Circle(int x, int y, int r, int n)
:Figure(x,y), r_(r), n_(n), ox_(x),oy_(y)
{
	assert(r>0);
}

Circle::Circle()
:Figure(0,0), r_(2), n_(0), ox_(0),oy_(0)
{
	
}

Circle& Circle::operator =(const Circle& r)
{
	ox_=r.getX();
	oy_=r.getY();
	n_=r.getNumber();
	r_=r.getRadius();
	col_=r.getColor();

	return (*this);
}

void Circle::draw(QPainter& hdc) const{

    QBrush brush(col_);
    QBrush oldBrush = hdc.brush();

    QPen pen(brush,1,Qt::SolidLine);
    QPen oldPen = hdc.pen();

    QString bfr = QString::number(n_);
	
    hdc.setBackgroundMode(Qt::TransparentMode);

    hdc.setPen(pen);
    hdc.setBrush(brush);
    hdc.drawEllipse(_x-r_, _y-r_, _x+r_, _y+r_);
    hdc.setBrush(oldBrush);

    hdc.setBackgroundMode(Qt::OpaqueMode);
    hdc.drawText(_x-4, _y-18, bfr);

    //brs=CreateSolidBrush(col_);
    hdc.setBrush(brush);
    hdc.setBackgroundMode(Qt::TransparentMode);
    hdc.drawEllipse(_x-2,_y-2,_x+2,_y+2);

    hdc.setBackgroundMode(Qt::OpaqueMode);
    hdc.setPen(oldPen);
    hdc.setBrush(oldBrush);
}


// BOX

Box::Box(int x, int y,int dx, int dy)
:Figure(x,y),dx_(dx),dy_(dy)
{}

void Box::draw(QPainter &hdc) const{
    QBrush brush(col_);
    QBrush oldBrush = hdc.brush();

    QPen pen(brush,1,Qt::SolidLine);
    QPen oldPen = hdc.pen();

    hdc.setPen(pen);
    hdc.setBrush(brush);
    hdc.setBackgroundMode(Qt::TransparentMode);

    hdc.drawRect(_x, _y, dx_, dy_);

    hdc.setBackgroundMode(Qt::OpaqueMode);
    hdc.setPen(oldPen);
    hdc.setBrush(oldBrush);
}
