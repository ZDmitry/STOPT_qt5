PROJECT NAME: STOPT (Qt ver.)
================================================================================
- AUTHOR:   Zagnoyko Dmitry ( zagnoyko.dmitry@gmail.com )  
- LICENCE:  GPLv3    
- L18n:     EN
- TOOLS:    Qt 5.1.1, Qt Creator
- LIBS:     OpenGL, libGLU

DESCRIPTION:
--------------------------------------------------------------------------------
System for topological optimisation. Uses area dimensions and hotspots radius 
of coverage for calculating total area coverage. Analizes given variants of 
hotspots and finds best of them.

BUILD: 
--------------------------------------------------------------------------------
1. cd src/
2. qmake && make

FEATURES:
--------------------------------------------------------------------------------
- Result is count and cost for hotspots and overlap coeficient
- 2D and 3D modes: for flat area (uses Qt) and volume rooms (uses OpenGL)
- Manual (user puts hotspots) and automatic (program puts hotspots) modes
- Saves resulting array of points as plain text (*.txt)
- Save scene to image file (*.png)
