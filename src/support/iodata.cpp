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

// STL
#include <algorithm>

using namespace std;
using namespace STOPT;

void IOSystem::saveToTxtFile(const vector<POINT>* pPt, int cost)
{
    saveToTxtFile("points.txt", pPt, cost);
}

void IOSystem::saveToTxtFile(const char* file, const vector<POINT>* pt, int cost)
{
    saveToTxtFile(file, pt, 0.f, cost);
}

void IOSystem::saveToTxtFile(const char* file, const vector<POINT>* pt, double t, int cost)
{
	FILE* f=fopen(file,"w"); //points.txt
    int i=0;
	// WARN: important to leave int i, counting should start from 0
    for (vector<POINT>::const_iterator it = pt->begin(); it != pt->end(); it++, ++i)
        fprintf(f, "Point %d: X=%d, Y=%d\n", i, (*it).x, (*it).y);
	fprintf(f,"==========================================================\n");
	//if (inter)
	//	GetWinInt(hICX1Wnd,&x);
	//else 
	//	GetWinInt(hCY0Wnd,&x);
    fprintf(f,"Overall cost: %d \n", cost);
	if (t!=0.f)
        fprintf(f,"Elapsed time: %f sec.", t);
	fclose(f);
}

void IOSystem::saveToTxtFile(const vector<POINT3D>* pt, VMODE vm, int cost)
{
    saveToTxtFile("points.txt", pt, 0.f, 0.f, vm, cost);
}

void IOSystem::saveToTxtFile(const char* file, const vector<POINT3D>* pt, VMODE vm, int cost)
{
    saveToTxtFile(file, pt, 0.f, 0.f, vm, cost);
}

void IOSystem::saveToTxtFile(const char* file, const vector<POINT3D>* pt, double t, VMODE vm, int cost)
{
    saveToTxtFile(file, pt, t, 0.f, vm, cost);
}

void IOSystem::saveToTxtFile(const char* file, const vector<POINT3D>* pt, double t, double tt, VMODE vm, int cost)
{
	int k=1;
	if (vm!=V_2D) k=(-1);

	FILE* f=fopen(file,"w");
    int i=0;
	// WARN: important to leave int i, counting should start from 0
    for (vector<POINT3D>::const_iterator it = pt->begin(); it != pt->end(); ++it, ++i)
        fprintf(f, "Point %d: X=%f, Y=%f, Z=%f\n", i, (*it).x, (*it).y*k, (*it).z);
	fprintf(f,"==========================================================\n");
	//if (inter)
	//	GetWinInt(hICX1Wnd,&x);
	//else 
	//	GetWinInt(hCY0Wnd,&x);
    fprintf(f,"Overall cost?count: %d \n",cost);
	if (t!=0.f)
		fprintf(f,"Elapsed time: %f sec. \n",t);
	if (tt!=0.f)
		fprintf(f,"Elapsed time (1): %f sec.",tt);
	fclose(f);
}

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

