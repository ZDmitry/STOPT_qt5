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

// STOPT
#include "iodata.h"

// STD
#include <stdlib.h> 
#include <iostream>


//void IOSystem::saveToEmfFile(OPENFILENAME &of, bool useBorder, const Figure* border, const vector<Figure>* pCircle)
//{
//	//of.lpstrFile = szFileName;
//	//of.Flags = OFN_OVERWRITEPROMPT;
	
//	if (GetSaveFileName(&of))
//	{
//		HDC hdc=CreateEnhMetaFile(NULL,of.lpstrFile,NULL,NULL);

//		if (useBorder) //bPt.x!=0)
//            border->draw(hdc);
//		if (pCircle != NULL)
//            for_each(pCircle->begin(),pCircle->end(), [hdc](Figure f){ f.draw(hdc); });

//		HENHMETAFILE hTemp=CloseEnhMetaFile(hdc);
//        DeleteEnhMetaFile(hTemp);
//	}
//}

IOSystem::IOSystem()
{
    //def. name:  points.txt
    std::string file = "points.txt";

    openFile(file);
}

IOSystem::IOSystem(const std::string &fname)
{
    openFile(fname);
}

void IOSystem::openFile(const std::string &fname)
{
    filename_ = fname;

    // just create empty file;
    FILE* f=fopen(fname.c_str(), "w"); //points.txt
    fclose(f);
}

void IOSystem::savePointToFile(STOPT::POINT* pt, int n)
{
    FILE* f=fopen(filename_.c_str(), "a");

    for (int i = 0; i < n; i++) {
        fprintf(f, "Point %d: X=%d, Y=%d \n", i, pt[i].x, pt[i].y);
    }

    fclose(f);
}

void IOSystem::savePointToFile(const std::vector<std::vector<STOPT::POINT> *> *pt)
{
    FILE* f=fopen(filename_.c_str(), "a");

    int i = 0;
    std::vector< std::vector< STOPT::POINT >* >::const_iterator jj = pt->begin();
    for (; jj!=pt->end(); jj++) {

        std::vector< STOPT::POINT >::const_iterator kk = (*jj)->begin();
        for (; kk!=(*jj)->end(); kk++, i++) {

            fprintf(f, "Point %d: X=%d, Y=%d \n", i, (*kk).x, (*kk).y);
        }
    }

    fclose(f);
}

void IOSystem::savePointToFile(const std::vector<std::vector<std::vector<STOPT::POINT3D> *> *> *pt)
{
    FILE* f=fopen(filename_.c_str(), "a");

    int i = 0;
    std::vector< std::vector< std::vector< STOPT::POINT3D >* >* >::const_iterator ii = pt->begin();
    for (; ii!=pt->end(); ii++) {

        std::vector< std::vector< STOPT::POINT3D >* >::const_iterator jj = (*ii)->begin();
        for (; jj!=(*ii)->end(); jj++) {

            std::vector< STOPT::POINT3D >::const_iterator kk = (*jj)->begin();
            for (; kk!=(*jj)->end(); kk++, i++) {

                fprintf(f, "Point %d: X=%f, Y=%f, Z=%f\n", i, (*kk).x, (*kk).y, (*kk).z );
            }
        }
    }

    fclose(f);
}

void IOSystem::saveStatistic(int count, long cost, float time)
{

    FILE* f=fopen(filename_.c_str(), "a");

    fprintf(f, "==========================================================\n");
    fprintf(f, "Overall count: %d \n",    count );
    fprintf(f, "Overall cost: %ld \n",    cost  );
    fprintf(f, "Elapsed time: %f sec \n", time  );

    fclose(f);
}
