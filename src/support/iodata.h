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
#ifndef IOSYSTEM_H
#define IOSYSTEM_H

#include <vector>
#include "stoptdef.h"
//#include "shapes/shape.h"

class IOSystem {
public:
	IOSystem();

    //void  saveToEmfFile(OPENFILENAME &of, bool useBorder, const Figure* border, const vector<Figure>* pCircle)

    void	saveToTxtFile(const std::vector<STOPT::POINT>*, int);
    void	saveToTxtFile(const char* , const std::vector<STOPT::POINT>*, int);
    void	saveToTxtFile(const char* , const std::vector<STOPT::POINT>*, double, int);

    void    saveToTxtFile(const std::vector<STOPT::POINT3D>*, STOPT::VMODE,    int);
    void	saveToTxtFile(const char* , const  std::vector<STOPT::POINT3D>*, STOPT::VMODE , int);
    void	saveToTxtFile(const char* , const  std::vector<STOPT::POINT3D>*, double, STOPT::VMODE , int);
    void	saveToTxtFile(const char* , const  std::vector<STOPT::POINT3D>*, double, double, STOPT::VMODE, int);
};

#endif //IOSYSTEM_H
