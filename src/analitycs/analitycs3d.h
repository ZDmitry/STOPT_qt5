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
#ifndef ANALITYCS3D_H
#define ANALITYCS3D_H

//#include <windows.h> 

#define _USE_MATH_DEFINES
#include <math.h>

#include "analitycs.h"


class Radiofield3D : public Radiofield {
public:
    Radiofield3D();
    virtual ~Radiofield3D();
    Radiofield3D(int width, int height);
    Radiofield3D(int width, int height, int lenght);
    Radiofield3D(int width, int height, int lenght, int radius);
    Radiofield3D(int width, int height, int lenght, int radius, int cost);

    virtual void generatePoints();

    // ptr to vector of ptr - less memory consumption
    const std::vector< std::vector< std::vector< STOPT::POINT3D >* >* >* getPoints(){ return points_; }

    static  int  getOptimalVariant(const std::vector<Radiofield3D *> *var);

    virtual int  getCount() const { return row_*column_*line_; }
    virtual bool isFilled();

    void   setLenght(int w) { lenght_ = w; }
    int    getLenght() const { return lenght_; }

    int    getLines() const { return line_; }

    double filledVolume() const;
    double effectiveVolume() const;

    Radiofield3D&  operator=(const Radiofield3D& r);

private:
    virtual int  blockCount(int lenght, int radius);
    bool pointCoverage(STOPT::POINT3D p, STOPT::POINT3D s, int r);

    void buildPoints(int height, int width, int lenght, int radius);

    void freeMem();

    int line_;

    int lenght_;

    // store in vector format points
    // see notes inside implimentation
    // of func - buildPoints(..)
    // PACKGAGE FORMAT:
    // vector <   z    <   y    <   x    > > >
    // vector < line   < row    < column > > >
    // vector < lenght < height < width  > > >
    std::vector< std::vector< std::vector< STOPT::POINT3D >* >* >* points_;
};

#endif //ANALITYCS3D_H
