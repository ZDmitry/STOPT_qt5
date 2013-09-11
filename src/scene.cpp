/*
    This file is part of STOPT project.

    Copyright 2012-2013      by Dmitry Zagnoyko <hiroshidi@gmail.com>

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

#include "scene.h"

#include "analitycs/analitycs2d.h"
#include "analitycs/analitycs3d.h"

#include "shapes/gpcircle.h" //"bubble.h"
#include "qtlogo.h"

#include <QMouseEvent>
#include <QTime>

#include <math.h>
#include <stdlib.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLScene::GLScene(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    rf_ = nullptr;
    vm_ = STOPT::V_2D;

    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    xRot_ = 0;
    yRot_ = 0;
    zRot_ = 0;

    colGreen_  = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    colPurple_ = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    animationTimer.setSingleShot(true); //(false);
    connect(&animationTimer, SIGNAL(timeout()), this, SLOT(animate()));
    //animationTimer.start(25);

    setAutoFillBackground(false);
    setMinimumSize(200, 200);
}

void GLScene::initializeGL()
{
    glEnable(GL_MULTISAMPLE);

    logo = new QtLogo(this);
    logo->setColor(colGreen_.dark());
}

void GLScene::setupViewport(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

#ifdef QT_OPENGL_ES
    glOrthof(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#endif

    glMatrixMode(GL_MODELVIEW);
}

void GLScene::createBubbles(int number)
{
    for (int i = 0; i < number; ++i) {
        QPointF position(width()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
                        height()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))));
        qreal radius = qMin(width(), height())*(0.0125 + 0.0875*qrand()/(RAND_MAX+1.0));

        shapes_.append(new GPCircle(position, radius, i));
    }
}

GLScene::~GLScene()
{
    if ( rf_ != nullptr ) delete rf_;
    qDeleteAll(shapes_);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLScene::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot_)
        xRot_ = angle;
}

void GLScene::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot_)
        yRot_ = angle;
}

void GLScene::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot_)
        zRot_ = angle;
}

void GLScene::setVideoMode(STOPT::VMODE vm)
{
    vm_ = vm;
    if (rf_ != nullptr) rf_->setVideoMode(vm);
}

void GLScene::setData(Radiofield *data)
{
    rf_ = data;
    data->setVideoMode(vm_);
    data->generatePoints();

    int n = data->getCount();
    int radius = data->getRadius();

    STOPT::POINT* pt = ((Radiofield2D*)data)->getRawPoints();

    shapes_.clear();
    shapes_.reserve(n);

    for (int i = 0; i < n; ++i) {
        QPointF position(pt[i].x+10, pt[i].y-10);

        shapes_.append(new GPCircle(position, radius, i));
    }

    update();
}

void GLScene::mousePressEvent(QMouseEvent *event)
{
    lastPos_ = event->pos();

//    QPointF position(event->x(), event->y());
//    qreal radius = qMin(width(), height())*(0.0125 + 0.0875*qrand()/(RAND_MAX+1.0));
//    QPointF velocity(width()*0.0125*(-0.5 + qrand()/(RAND_MAX+1.0)),
//                    height()*0.0125*(-0.5 + qrand()/(RAND_MAX+1.0)));

//    bubbles.append(new Bubble(position, radius, velocity));

//    update();
}

void GLScene::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos_.x();
    int dy = event->y() - lastPos_.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot_ + 8 * dy);
        setYRotation(yRot_ + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot_ + 8 * dy);
        setZRotation(zRot_ + 8 * dx);
    }
    lastPos_ = event->pos();

    update();
}

// animation loop
void GLScene::animate()
{
//    QMutableListIterator<Shape*> iter(bubbles);

//    while (iter.hasNext()) {
//        Shape *bubble = iter.next();
//        bubble->move(rect());
//    }
    update();
}

void GLScene::paintEvent(QPaintEvent *event)
{
    makeCurrent();

    if ( vm_ == STOPT::V_3D ){
        render3d(event);
    }
    else if ( vm_ == STOPT::V_PROJ) {
        render3dProj(event);
    }
    else if ( vm_ == STOPT::V_2D ) {
        // painting preformed AFTER glOperations(...)
        render2d(event);
    }


}

// text
void GLScene::drawInstructions(QPainter *painter)
{
    QString text = tr("Click and drag with the left mouse button "
                      "to rotate the Qt logo.");
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(4, metrics.leading());

    QRect rect = metrics.boundingRect(0, 0, width() - 2*border, int(height()*0.125),
                                      Qt::AlignCenter | Qt::TextWordWrap, text);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                     QColor(0, 0, 0, 127));
    painter->setPen(Qt::white);
    painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                      QColor(0, 0, 0, 127));
    painter->drawText((width() - rect.width())/2, border,
                      rect.width(), rect.height(),
                      Qt::AlignCenter | Qt::TextWordWrap, text);
}

void GLScene::render2d(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.save();
    painter.fillRect(event->rect(),QBrush(colPurple_.dark()));
    foreach (Shape *shape, shapes_) {
        if (shape->rect().intersects(event->rect()))
            shape->draw(&painter);
    }
    //drawInstructions(&painter);
    painter.restore();
    painter.end();
}

void GLScene::render3d(QPaintEvent *event)
{
    Q_UNUSED(event)
    // prepare world matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // prepare scene
    qglClearColor(colPurple_.dark());
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);

    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // setup viewport accorting to aspect ratio
    qreal pixelRatio = devicePixelRatio();
    setupViewport(width() * pixelRatio, height() * pixelRatio);

    // load identity matrix
    // translate and rotate scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot_ / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot_ / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot_ / 16.0, 0.0, 0.0, 1.0);

    logo->draw();

    // rollback scene
    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // rollback world matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void GLScene::render3dProj(QPaintEvent *event)
{
    Q_UNUSED(event)
}

QSize GLScene::sizeHint() const
{
    return QSize(400, 400);
}

// keep actual aspect ratio
void GLScene::resizeGL(int width, int height)
{
    qreal pixelRatio = devicePixelRatio();
    setupViewport(width*pixelRatio, height*pixelRatio);
}

// fill scene elements on show event
void GLScene::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    createBubbles(20 - shapes_.count());
}
