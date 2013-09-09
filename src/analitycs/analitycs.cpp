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

#include "analitycs.h"

using namespace std;
using namespace STOPT;

Radiofield::Radiofield(int width, int height, int radius, int cost)
:width_(width), height_(height), radius_(radius), cost_(cost)
{
    am_ = STOPT::A_STD;
    vm_ = STOPT::V_2D;

    count_ = row_ = column_ = 0;
}

/////////////////////////////////////////

// is point of region covered by point (hotspot)
bool Radiofield::pointCoverage(POINT p, POINT c, int r)
{
    bool ok;
    double a,rx,ry;
    a=0;
    ok=1;

    for (int i=0; i<360; i++)
    {
        a=(M_PI/180)*i;
        rx=(c.x-10)+r*sin(a);
        ry=(c.y-10)-r*cos(a);


        if (i<91)
        {
            if (p.x>rx && p.y<ry)
            {
                ok=0;
                break;
            }
        }
        else if (i<181)
        {
            if (p.x>rx && p.y>ry)
            {
                ok=0;
                break;
            }
        }
        else if (i<271)
        {
            if (p.x<rx && p.y>ry)
            {
                ok=0;
                break;
            }
        }
        else
        {
            if (p.x<rx && p.y<ry)
            {
                ok=0;
                break;
            }
        }
    }

    return ok;
}
