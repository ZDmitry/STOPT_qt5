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
#ifndef ANALITYCS2D_H
#define ANALITYCS2D_H

//#include <windows.h> 

#define _USE_MATH_DEFINES
#include <math.h>

#include "analitycs.h"

class Radiofield2D : public Radiofield {
public:
    Radiofield2D();
    virtual ~Radiofield2D();
    Radiofield2D(int width, int height);
    Radiofield2D(int width, int height, int radius);
    Radiofield2D(int width, int height, int radius, int cost);

    virtual void generatePoints();

    // ptr to vector of ptr - less memory consumption
    std::vector< std::vector< STOPT::POINT >* >* getPointsPacked();
    std::vector<STOPT::POINT> getPoints();

    STOPT::POINT* getRawPoints() { return points_; }

    void   pushData(STOPT::POINT p, int radius, int cost);

    virtual int  getTotalCost() const;
    virtual bool isFilled();

    static  int  getOptimalVariant(const std::vector<Radiofield2D* >* var);

    double filledArea() const;
    double effectiveArea() const;

    Radiofield2D&  operator=(const Radiofield2D& r);

private:
    virtual int blockCount(int lenght, int radius);

    void buildPoints(int width, int height, int radius);
    void buildPoints(int width, int height, int radius, int sh);

    // manual filling flag
    bool   dynGenerated_;

    // store in raw format points
    STOPT::POINT* points_;

    // store additional data for non-constant case
    STOPT::POINT* support_; // use cood: x - radius, y - cost

};

#endif //ANALITYCS2D_H
