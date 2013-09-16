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
class OGLBox;

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

    float getScale() { return scale_; }
    void  setScale(float s) { scale_ = s; }

    void  setInteractiveData(bool ready, qreal width, qreal height, qreal radius);
    void  setProjectionMode(bool enable );
    void  setInteractiveMode(bool enable );

    void  saveData(const QString &fname);


signals:
    void pointFixed(int count, qreal covered);


public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void setVideoMode(STOPT::VMODE vm);
    void setData(Radiofield* data);


protected:
    void initializeGL();
//  void paintGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void showEvent(QShowEvent *event);


private slots:
    void animate();


private:
    void  createBox(qreal width, qreal height, qreal length);
    void  createRandHotspots(int number);
    void  drawHotspots();
    void  drawInstructions(QPainter &painter, const QString &text);
    void  setupViewport(int width, int height);

    void  render2d(QPaintEvent *event, QPainter &painter);
    void  render3d();
    void  render3dProj(QPaintEvent *event, QPainter &painter);

    qreal filledAreaPercent();

    bool  projections_;
    bool  interactive_;
    bool  interDataReady_;
    qreal interRadius_;

    STOPT::VMODE vm_;
    Radiofield*  rf_;

    GLuint object;
    int    xRot_;
    int    yRot_;
    int    zRot_;
    float  scale_;
    QPoint lastPos_;

    QColor colGreen_;
    QColor colPurple_;

    Shape*        box_;
    QList<Shape*> shapes_;
    QTimer        animationTimer;


    static const int START_SHIFT_POS2D = 20;
};

#endif // SCENE_H
