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

#include "analitycs2d.h"

using namespace std;
using namespace STOPT;

/// Radiofield2D  CTOS
/////////////////////////////////////////

Radiofield2D::Radiofield2D()
    :Radiofield(0, 0, 0, 0), dynGenerated_(false)
{
    points_  = nullptr;
    support_ = nullptr;
}

Radiofield2D::Radiofield2D(int width, int height)
    :Radiofield(width, height, 0, 0), dynGenerated_(false)
{
    points_  = nullptr;
    support_ = nullptr;
}

Radiofield2D::Radiofield2D(int width, int height, int radius)
    :Radiofield(width, height, radius, 0), dynGenerated_(false)
{
    points_  = nullptr;
    support_ = nullptr;

    buildPoints(width, height, radius);
}

Radiofield2D::Radiofield2D(int width, int height, int radius, int cost)
    :Radiofield(width, height, radius, cost), dynGenerated_(false)
{
    points_  = nullptr;
    support_ = nullptr;

    Radiofield2D::buildPoints(width, height, radius);
}

/////////////////////////////////////////

Radiofield2D::~Radiofield2D()
{
    if (points_  != nullptr) delete[] points_;
    if (support_ != nullptr) delete[] support_;
}


Radiofield2D& Radiofield2D::operator=(const Radiofield2D& r)
{
    am_ = r.getAlgoMode();
    vm_ = r.getVideoMode();

    row_    = r.getRows();
    column_ = r.getColumns();
    count_  = r.getCount();

    height_ = r.getHeight();
    width_  = r.getWidth();

    radius_ = r.getRadius();
    cost_   = r.getCost();

    return (*this);
}

// get points packed into 2d vector
std::vector< std::vector< POINT > > Radiofield2D::getPointsPacked()
{
    vector< vector< POINT > > packedData;
    packedData.reserve(getCount());

    for(int i=0; i<row_; i++) {
        vector<POINT> packedRow;
        packedRow.reserve(column_);

        for(int j=0; j<column_; j++) {
            packedRow.push_back(points_[i*column_+j]);
        }
        packedData.push_back(packedRow);
    }

    return packedData;
}

// wrap raw points into vector
vector<POINT> Radiofield2D::getPoints()
{
    vector<POINT> packedData;

    for (int i=0; i<count_; i++) {
        packedData.push_back(points_[i]);
    }

    return packedData;
}

// add points (manual)
void Radiofield2D::pushData(POINT p, int radius, int cost)
{
    dynGenerated_ = true;

    if (points_  != nullptr) {
        POINT* st = new POINT[ (count_+1) ];

        copy(points_, points_ + count_, st);
        st[ count_ ].x = radius;
        st[ count_ ].y = cost;

        delete[] points_;
        support_ = st;
    }
    else {
        points_ = new POINT[1];
        points_[0].x = radius;
        points_[0].y = cost;

        // if points array exist at this point of time
        // then something is wrong. release memory
        if (points_  != nullptr) delete[] points_;
    }

    if (points_  != nullptr) {
        POINT* pt = new POINT[ (count_+1) ];

        copy(points_, points_ + count_, pt);
        pt[count_] = p;

        delete[] points_;
        points_ = pt;
    }
    else {
        POINT* points_ = new POINT[1];
        points_[0] = p;
        count_=1;
    }

}

int Radiofield2D::getTotalCost() const
{
    int cost = 0;

    if (dynGenerated_ && support_!= nullptr)
    {
        for (int i=0; i < getCount(); i++)
            cost += support_[i].y;
    }
    else
        cost = Radiofield::getTotalCost();

    return cost;
}

// calculate block count by given lenght and radius
int Radiofield2D::blockCount(int lenght, int radius)
{
    int res=0;
    double a,n,p;
    p=n=0;

    a=(2.0*radius)/sqrt(2.0);
    n=lenght/a;
    p=n-((int)n);

    if (p>0.07) res=(int)n + 1;
    else res=(int)n;

    return res;
}

// find optimal radius/cost variant among set
int Radiofield2D::getOptimalVariant(const std::vector<Radiofield2D>* var)
{
    int optvar;   // optimal variant index
    int optcost;  // optimal cost
    int optsq;    // optimal square

    int curcost;  // current cost
    int cursq;    // current square

    curcost = var->front().getTotalCost();
    cursq = var->front().effectiveArea();

    optcost = curcost;
    optsq = cursq;

    optvar = 0;

    int i=optvar;
    for (vector<Radiofield2D>::const_iterator it = (var->begin()+1); it != var->end(); ++it, ++i)
    {
        curcost = (*it).getTotalCost();
        cursq = (*it).effectiveArea();

        if (curcost<optcost)
        {
            optcost=curcost;
            optvar=i;
        }
        if (curcost==optcost)
        {
            if(cursq<optsq)
            {
                optsq=cursq;
                optvar=i;
            }
        }
    }

    return optvar;
}

// build points using dimensions and radius
void Radiofield2D::buildPoints(int height, int width, int radius)
{
    return Radiofield2D::buildPoints(height, width, radius, 0);
}

// build points using dimensions and radius + some shift
void Radiofield2D::buildPoints(int height, int width, int radius, int sh)
{
    dynGenerated_ = false;

    row_ = blockCount(height, radius);
    column_ = blockCount(width,  radius);
    count_ = row_*column_;

    int ds=0;

    if ( am_ == A_ALT ) ds=2;

    if (points_ != nullptr) delete[] points_;

    points_ = new POINT[ count_ ];

    float a = width/(float)column_;
    float b = height/(float)row_;
    float rx = a/2;
    float ry = b/2;

    if ( vm_ == V_2D )
    {
        float rr=((sqrtf((a*a)+(b*b)))/2)/sqrt(2.0);
        rx=ry=rr;
    }

    for(int i=0; i<row_; i++)
        for(int j=0; j<column_; j++)
        {
            points_[i*column_+j].x = a*j+rx+sh+ds;
            points_[i*column_+j].y = b*i+ry+sh+ds;
        }
}

// is region covered by points?
bool Radiofield2D::isFilled()
{
    bool ok=0;

    for (int i=0; i<width_; i++)
    {
        for (int j=0; j<height_; j++)
        {
            for (int k=0; k < getCount(); k++)
            {
                int radius = radius_;
                if ( dynGenerated_ && support_!= nullptr ) radius = support_[k].x;

                POINT pp = { i, j };
                POINT ps = { points_[k].x, points_[k].y };
                ok = pointCoverage( pp, ps, radius);
                if (ok) break;
            }
            if(!ok) break;
        }
        if(!ok) break;
    }

    return ok;
}

// calculate area filled by points
double Radiofield2D::filledArea() const
{
    double s=0;
    float k=1.f;

    //if ( am_ == A_ALT ) k=1.02f;

    for (int i=0; i < getCount(); i++)
    {
        s += M_PI * powf(support_[i].x*k,2.0);
    }

    return s;
}

// calculate effective area
double Radiofield2D::effectiveArea() const
{
    double res,os;
    int bs=height_*width_;
    float k=1.f;

    res=os=0;

    //if ( am_ == A_ALT ) k=1.02f;

    if (dynGenerated_ && support_!= nullptr)
        os=filledArea();
    else
        os=(M_PI*powf(radius_*k,2))*getCount();

    res=os/(double)bs;
    return res;
}
