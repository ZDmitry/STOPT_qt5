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
#ifndef SHAPE_DERIVED_H
#define SHAPE_DERIVED_H

#include "shape.h"

#pragma once

class Circle: public Figure{
public:
	Circle();
	Circle(int x, int y, int r,int n);

    void     setNumber(int n){n_=n;}
    int      getNumber() const {return n_;}

    void     setRadius(int r){r_=r;}
    int      getRadius() const {return r_;}

    int      getX() const {return ox_;}
    int      getY() const {return oy_;}

    virtual  void moveBy(int x, int y){_x=ox_=x;_y=oy_=y;}
    void     shiftBy(int x, int y){_x=ox_-x;_y=oy_-y;}

    virtual  void draw(QPainter &)const;

    Circle&  operator=(const Circle& r);

private:
	int r_;
	int n_;
	int ox_,oy_;
};

class Box: public Figure{
public:
	Box(int x, int y, int dx, int dy);

    void    resize(int dx, int dy){dx_=dx; dy_=dy;}
    int     length(){return dx_;}
    int     width(){return dy_;}

    virtual void moveBy(int x, int y){_x=10-x; _y=10-y;}

    virtual void draw(QPainter&)const;

private:
	int dx_,dy_;
};

#endif //SHAPE_DERIVED_H
