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

#ifndef SCENE_H
#define SCENE_H

#include "support/stoptdef.h"

#include <QGLWidget>
#include <QTimer>

class Radiofield;
class Shape;
class QtLogo;

class GLScene : public QGLWidget
{
    Q_OBJECT

public:
    GLScene(QWidget *parent = 0);
    ~GLScene();

    QSize sizeHint() const;
    int xRotation() const { return xRot_; }
    int yRotation() const { return yRot_; }
    int zRotation() const { return zRot_; }

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void setVideoMode(STOPT::VMODE vm);
    void setData(Radiofield* data);

protected:
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void animate();

private:
    void createBubbles(int number);
    void drawInstructions(QPainter *painter);
    void setupViewport(int width, int height);

    void render2d(QPaintEvent *event);
    void render3d(QPaintEvent *event);
    void render3dProj(QPaintEvent *event);

    STOPT::VMODE vm_;
    Radiofield*  rf_;

    //GLuint object;
    int xRot_;
    int yRot_;
    int zRot_;
    QPoint lastPos_;

    QColor colGreen_;
    QColor colPurple_;

    QtLogo* logo;
    QList<Shape*> shapes_;
    QTimer animationTimer;
};

#endif // SCENE_H
