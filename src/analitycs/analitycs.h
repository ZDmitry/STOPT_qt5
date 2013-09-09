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
#ifndef ANALITYCS_H
#define ANALITYCS_H

//#include <windows.h> 
#include "support/stoptdef.h"

#define _USE_MATH_DEFINES
#include <math.h>

// STL
#include <vector>

#pragma once


class Radiofield {
public:
    Radiofield(int width, int height, int radius, int cost);

    virtual int  getCount() const { return row_*column_; }
    virtual int  getTotalCost() const { return cost_*count_; }
    virtual bool isFilled() = 0;

    void   setVideoMode(STOPT::VMODE v) { vm_ = v; }
    STOPT::VMODE getVideoMode() const { return vm_; }

    void   setAlgoMode(STOPT::AMODE a) { am_ = a; }
    STOPT::AMODE getAlgoMode() const { return am_; }

    void   setRadius(int r) { radius_ = r; }
    int    getRadius() const { return radius_; }

    void   setCost(int cost) { cost_ = cost; }
    int    getCost() const { return cost_; }

    int    getRows() const { return row_; }
    int    getColumns() const { return column_; }

    void   setHeight(int h) { height_ = h; }
    int    getHeight() const { return height_; }

    void   setWidth(int w) { width_ = w; }
    int    getWidth() const { return width_; }

    double area() { return height_*width_; }

protected:
    virtual int  blockCount(int lenght, int radius) = 0;
    bool pointCoverage(STOPT::POINT p, STOPT::POINT c, int r);

    STOPT::AMODE am_;
    STOPT::VMODE vm_;

    int row_;
    int column_;
    int count_;

    int height_;
    int width_;

    int radius_;
    int cost_;
};

#endif //ANALITYCS_H
