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

#include "analitycs3d.h"

using namespace std;
using namespace STOPT;

/// Radiofield3D  CTOS
/////////////////////////////////////////

Radiofield3D::Radiofield3D()
    :Radiofield(0, 0, 0, 0), lenght_(0)
{
    points_.clear();
}

Radiofield3D::Radiofield3D(int width, int height)
    :Radiofield(width, height, 0, 0), lenght_(0)
{
    points_.clear();
}

Radiofield3D::Radiofield3D(int width, int height, int lenght)
    :Radiofield(width, height, 0, 0), lenght_(lenght)
{
    points_.clear();
}

Radiofield3D::Radiofield3D(int width, int height, int lenght, int radius)
    :Radiofield(width, height, radius, 0), lenght_(lenght)
{
    points_.clear();
    Radiofield3D::buildPoints(height, width, lenght, radius);
}

Radiofield3D::Radiofield3D(int width, int height, int lenght, int radius, int cost)
    :Radiofield(width, height, radius, cost), lenght_(lenght)
{
    points_.clear();
    Radiofield3D::buildPoints(height, width, lenght, radius);
}

///////////////////////////////////////////////////////////////

Radiofield3D::~Radiofield3D()
{
    if (!points_.empty()) points_.clear();
}


Radiofield3D& Radiofield3D::operator=(const Radiofield3D& r)
{
    am_ = r.getAlgoMode();
    vm_ = r.getVideoMode();

    row_    = r.getRows();
    column_ = r.getColumns();
    line_   = r.getLines();
    count_  = r.getCount();

    height_ = r.getHeight();
    width_  = r.getWidth();
    lenght_ = r.getLenght();

    radius_ = r.getRadius();
    cost_   = r.getCost();

    return (*this);
}

// get points packed into 3d vector
vector< vector< vector< POINT3D > > >  Radiofield3D::getPointsPacked()
{
    vector< vector< vector< POINT3D > > > packedData;
    packedData.reserve(getCount());

    vector<POINT3D>::const_iterator ii = points_.begin();
    for(int i=0; i<row_; i++) {
       vector< vector< POINT3D > > packedRow;
       packedRow.reserve(column_);

       for(int j=0; j<column_; j++) {
           vector<POINT3D> packedColumn;
           packedColumn.reserve(line_);

           for(int k=0; k<line_; k++, ii++) {
                packedColumn.push_back((*ii));
           }
           packedRow.push_back(packedColumn);
       }
       packedData.push_back(packedRow);
    }

    return packedData;
}


// calculate block count by given lenght and radius
int Radiofield3D::blockCount(int lenght, int radius)
{
	int res=0;
	double a,n,p;
	p=n=0;

    a=(2.0*radius)/sqrt(3.0);
    n=lenght/a;
	p=n-((int)n);
	
	if (p>0.07) res=(int)n + 1;
	else res=(int)n;
	
	return res;
}

// find optimal radius/cost variant among set
int  Radiofield3D::getOptimalVariant(const std::vector<Radiofield3D>* var)
{
    int optvar;   // optimal variant index
    int optcost;  // optimal cost
    int optvol;   // optimal volume

    int curcost;  // current cost
    int curvol;   // current volume

    curcost = var->front().getTotalCost();
    curvol = var->front().effectiveVolume();

    optcost = curcost;
    optvol = curvol;

    optvar = 0;

    int i=optvar;
    for (vector<Radiofield3D>::const_iterator it = (var->begin()+1); it != var->end(); ++it, ++i)
	{
        curcost = (*it).getTotalCost();
        curvol  = (*it).effectiveVolume();
		
		if (curcost<optcost)
		{
			optcost=curcost;
			optvar=i;
		}
		if (curcost==optcost)
		{
            if(curvol<optvol)
			{
                optvol=curvol;
				optvar=i;
			}
		}
	}

	return optvar;
}

// build points using dimensions and radius
void Radiofield3D::buildPoints(int height, int width, int lenght, int radius)
{
    row_ = blockCount(height, radius);
    column_ = blockCount(width,  radius);
    line_ = blockCount(lenght, radius);
    count_ = row_*column_*line_;

    float ds=0.f;

    if ( am_ == A_ALT ) ds=0.2;

    double a=lenght/(double)line_;
    double b=height/(double)row_;
    double c=width/(double)column_;
    //double rr=((sqrtf((a*a)+(b*b)+(c*c)))/2.0f)/sqrt(3.0f);
    float ry=a/2;//((sqrtf((a*a)+(b*b)))/2);//sqrt(2.0);
    float rx=b/2;//((sqrtf((a*a)+(c*c)))/2);//sqrt(2.0);
    float rz=c/2;//((sqrtf((c*c)+(b*b)))/2);//sqrt(2.0);

    // WARN: too big chunck size allocation!!! neet to optimize !!!
    //points_=(POINT3D*)VirtualAlloc(NULL,sizeof(POINT3D)*nn, MEM_RESERVE | MEM_COMMIT,PAGE_READWRITE);
    if (!points_.empty()) points_.clear();

    vector<POINT3D>::iterator ii = points_.begin();
    for(int i=0; i<row_; i++)
        for(int j=0; j<line_; j++)
            for(int k=0; k<column_; k++, ii++)
        {
            //ii=k*nw*nl+i*nl+j
            (*ii).x=a*j+rx+ds;
            (*ii).y=(-1)*(b*i+ry+ds);
            (*ii).z=c*k+rz+ds;
        }
}

// is volume covered by points?
bool Radiofield3D::isFilled()
{
	bool ok=0;

    vector<POINT3D>::const_iterator ii = points_.begin();
    for (int i=0; i<lenght_; i++)
	{
        for (int j=0; j<width_; j++)
		{
            for (int k=0; k<height_; k++)
			{
                POINT3D pp = {i, k, j}; //orig: i,j,w
                ok=pointCoverage( pp, (*ii), radius_);
				if (ok) break;
			}
			if(!ok) break;
		}
		if(!ok) break;
	}

	return ok;
}

// is point of volume covered by point (hotspot)
bool Radiofield3D::pointCoverage(STOPT::POINT3D p, STOPT::POINT3D s, int r)
{
    STOPT::POINT pp = { p.x, p.z };
    STOPT::POINT ps = { s.x, s.z };

    if( Radiofield::pointCoverage(pp, ps, r) ) {
       pp = { p.x, p.y };
       ps = { s.x, s.y };

       if ( Radiofield::pointCoverage(pp, ps, r) ) {
           pp = { p.z, p.y };
           ps = { s.z, s.y };

           if ( Radiofield::pointCoverage(pp, ps, r) )
                return true;
       }
    }

    return false;
}

// calculate volume filled by points
double Radiofield3D::filledVolume() const
{
	double s=0;
	float k=1.f;

	//if (GetAlgoMode()==A_ALT) k=1.02;
	
    for (int i=0; i<count_; i++)
	{
        s+=(3.0f/4.0f)*M_PI*powf(radius_*k,3.0f);
	}

	return s;
}

// calculate effective volume
double Radiofield3D::effectiveVolume() const
{
	double res=0;
    int bs=lenght_*height_*width_;

	float k=1.f;

	//if (GetAlgoMode()==A_ALT) k=1.02;

    double os=((3.0f/4.0f)*M_PI*powf(radius_*k,3))*count_;
	
	res=os/(double)bs;
    return res;
}
