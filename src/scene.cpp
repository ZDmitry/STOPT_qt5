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

#include "shapes/gpbox.h"
#include "shapes/gpcircle.h"

#include "shapes/oglbox.h"
#include "shapes/oglsphere.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QTime>

#include <QVector3D>

#include <math.h>
#include <stdlib.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


static inline GLfloat gradToRad(GLfloat a)
{
    return (a * M_PI / 180); // dgr -> rad
}


GLScene::GLScene(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    rf_  = nullptr;
    box_ = nullptr;
    vm_  = STOPT::V_2D;

    interactive_    = false;
    interDataReady_ = false;
    interRadius_    = 0;

    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    xRot_  = 0;
    yRot_  = 0;
    zRot_  = 0;
    scale_ = 0.0025; //1.0;

    colGreen_  = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    colPurple_ = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    animationTimer.setSingleShot(true); //(false);
    connect(&animationTimer, SIGNAL(timeout()), this, SLOT(animate()));
    //animationTimer.start(25);

    setAutoFillBackground(false);
    setMinimumSize(200, 200);

    //createBox(100,200,0);
    //createRandHotspots(20);
}

void GLScene::initializeGL()
{
    glEnable(GL_MULTISAMPLE);
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

GLScene::~GLScene()
{
    if ( rf_ != nullptr   ) {
        delete rf_;
        rf_ = nullptr;
    }
    if ( box_ != nullptr  ) {
        delete box_;
        box_ = nullptr;
    }
    if ( !shapes_.empty() ) qDeleteAll(shapes_);
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

    update();
}

void GLScene::setData(Radiofield *data)
{
    rf_ = data;
    data->setVideoMode(vm_);
    data->generatePoints();

    int n = data->getCount();
    int radius = data->getRadius();

    if ( !shapes_.empty() ) {
        qDeleteAll(shapes_);
        shapes_.clear();
    }
    shapes_.reserve(n);

    if ( vm_ == STOPT::V_2D ) {

        int width  = data->getWidth();
        int height = data->getHeight();

        createBox(width, height, 0);

        Radiofield2D* rfd = (dynamic_cast<Radiofield2D*>(data));
        if (rfd) {

            STOPT::POINT* pt = rfd->getRawPoints();

            for (int i = 0; i < n; ++i) {
                QPointF position(pt[i].x+START_SHIFT_POS2D, pt[i].y+START_SHIFT_POS2D);

                shapes_.append(new GPCircle(position, radius, i));
            }
        }
    }

    if ( vm_ == STOPT::V_3D ) {

        OGLBox* box3d = (dynamic_cast<OGLBox*>(box_));
        if (box3d) {
            const QVector3D* boxDims = box3d->dimensions();

            qreal width  = boxDims->x();
            qreal height = boxDims->y();
            qreal lenght = boxDims->z();

            createBox(width, height, lenght);
        }

        Radiofield3D* rfd = (dynamic_cast<Radiofield3D*>(data));
        if (rfd) {

            const std::vector< std::vector< std::vector< STOPT::POINT3D >* >* >* pt = rfd->getPoints();

            std::vector< std::vector< std::vector< STOPT::POINT3D >* >* >::const_iterator ii = pt->begin();
            for (; ii!=pt->end(); ii++) {

                std::vector< std::vector< STOPT::POINT3D >* >::const_iterator jj = (*ii)->begin();
                for (; jj!=(*ii)->end(); jj++) {

                    std::vector< STOPT::POINT3D >::const_iterator kk = (*jj)->begin();
                    for (; kk!=(*jj)->end(); kk++) {

                        QVector3D position( scale_ * (*kk).x, scale_ * (*kk).y, scale_ * (*kk).z );

                        shapes_.append(new OGLSphere(position, scale_ * radius));
                    }
                }
            }

        // endif (rfd)
        }

     // endif ( vm_ == STOPT::V_3D )
    }

    update();
}

void GLScene::setInteractiveMode(bool enable)
{
    interactive_ = enable;

    if ( !shapes_.empty() ) {
        qDeleteAll(shapes_);
        shapes_.clear();
    }
}


void GLScene::createBox(qreal width, qreal height, qreal length)
{
    if ( box_ != nullptr) {
        delete box_;
        box_ = nullptr;
    }

    if (vm_ == STOPT::V_2D) {

        QPointF pos(START_SHIFT_POS2D, START_SHIFT_POS2D);
        QPointF dims(width, height);

        box_ = new GPBox(pos,dims);
        box_->setOuterColor(colGreen_.dark());
    }

    if (vm_ == STOPT::V_3D) {

        QVector3D pos(0,0,0);
        //QVector3D dims(scale_ * height, scale_ * width, scale_ * length);
        QVector3D dims(width, height, length);

        box_ = new OGLBox(pos,dims);
        box_->setOuterColor(colGreen_.dark());
    }
}

void GLScene::createRandHotspots(int number)
{
    if ( !shapes_.empty() ) {
        qDeleteAll(shapes_);
        shapes_.clear();
    }
    shapes_.reserve(number);

    if (vm_ == STOPT::V_2D) {

        for (int i = 0; i < number; ++i) {
            QPointF position( width() *(0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
                              height()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0)))  );

            qreal radius = qMin(width(), height())*(0.0125 + 0.0875*qrand()/(RAND_MAX+1.0));

            shapes_.append(new GPCircle(position, radius, i));
        }
    }

    if (vm_ == STOPT::V_3D) {

        for (int i = 0; i < number; ++i) {
            QVector3D position( scale_ * width()  * (0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
                                scale_ * height() * (0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
                                scale_ * width()  * (0.1 + (0.8*qrand()/(RAND_MAX+1.0)))  );

            qreal radius = scale_ * qMin(width(), height())*(0.0125 + 0.0875*qrand()/(RAND_MAX+1.0));

            shapes_.append(new OGLSphere(position, radius));
        }
    }

}

void GLScene::mousePressEvent(QMouseEvent *event)
{
    lastPos_ = event->pos();

    if ( (event->buttons() & Qt::LeftButton) &&
           vm_ == STOPT::V_2D && interactive_ ) {

        if ( interDataReady_ && box_ != nullptr) {

            QPointF position(event->x(), event->y());

            if ( box_->rect().contains(position) ) {

                shapes_.append(new GPCircle(position, interRadius_, 0));
                update();
            }
        }
        else {
            QMessageBox::warning(this, tr("STOPT Interactive Mode"),
                             tr("You should fill radius and cost for hotspots!"),
                             QMessageBox::Ok);
        }
    }
}

void GLScene::mouseMoveEvent(QMouseEvent *event)
{
    if ( vm_ == STOPT::V_3D ) {

        int dx = event->x() - lastPos_.x();
        int dy = event->y() - lastPos_.y();

        if ( event->buttons() & Qt::LeftButton ) {
            setXRotation(xRot_ + 8 * dy);
            setYRotation(yRot_ + 8 * dx);
        } else if ( event->buttons() & Qt::RightButton ) {
            setXRotation(xRot_ + 8 * dy);
            setZRotation(zRot_ + 8 * dx);
        }
        lastPos_ = event->pos();

    }

    if ( (event->buttons() & Qt::LeftButton) &&
            vm_ == STOPT::V_2D && interactive_ ) {

        if ( !shapes_.empty() && box_ != nullptr ) {

            QPointF dest(event->x(), event->y());
            if ( box_->rect().contains(dest) ) {
                shapes_.last()->move(dest);
            }
        }
    }

    update();
}

void GLScene::mouseReleaseEvent(QMouseEvent *event)
{
    if ( !(event->buttons() & Qt::LeftButton) &&
            vm_ == STOPT::V_2D && interactive_  &&
            interDataReady_ ) {

        if ( !shapes_.empty() && box_ != nullptr ) {

            QPointF dest(event->x(), event->y());
            if ( box_->rect().contains(dest) ) {
                shapes_.last()->move(dest);

                emit pointFixed(shapes_.size(), isBoxFilled());
                update();
            }
        }
    }
}

// animation loop
void GLScene::animate()
{
//    QMutableListIterator<Shape*> iter(shapes_);

//    while (iter.hasNext()) {
//        Shape *shape = iter.next();
//        shape->move(rect());
//    }
    update();
}

void GLScene::paintEvent(QPaintEvent *event)
{
    makeCurrent();

    Q_UNUSED(event)
    // prepare world matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();


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

    // no glScalef,
    // use pos and dims implication instead
    // glScalef(scale_, scale_, scale_);

    if ( vm_ == STOPT::V_3D ){
        render3d();
    }

    if ( vm_ == STOPT::V_PROJ) {
        render3dProj();
    }

    // rollback scene
    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // rollback world matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glFlush();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    render2d(event, painter);

    painter.end();
}

// text
void GLScene::drawInstructions(QPainter *painter)
{
    QString text = tr("Fill 'Dimensions' and at least one 'Variants' fields"
                      "and push 'Render' to see result.\n"
                      "Swich between 2D and 3D mode.\n"
                      "Use mouse in 2D 'Interactive mode' to put points manualy\n"
                      "or to rotate scene in 3D mode.");
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(4, metrics.leading());

    QRect rect = metrics.boundingRect(0, 0, width() - 2*border, int(height()*0.125),
                                      Qt::AlignCenter | Qt::TextWordWrap, text);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                     QColor(0, 0, 0, 67));
    painter->setPen(Qt::white);
    painter->fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                      QColor(0, 0, 0, 67));
    painter->drawText((width() - rect.width())/2, border,
                      rect.width(), rect.height(),
                      Qt::AlignCenter | Qt::TextWordWrap, text);
}

void GLScene::render2d(QPaintEvent *event, QPainter &painter)
{
    painter.save();
    //painter.fillRect(event->rect(),QBrush(colPurple_.dark()));

    if ( vm_ == STOPT::V_2D) {

        foreach (Shape *shape, shapes_) {
            if (shape->rect().intersects(event->rect()))
                shape->draw(&painter);
        }
        if ( box_ != nullptr ) box_->draw(&painter);
    }

    if ( shapes_.empty() ) drawInstructions(&painter);

    painter.restore();
}

void GLScene::drawHotspots()
{
    if ( box_ != nullptr ) {

        OGLBox* box3d = (dynamic_cast<OGLBox*>(box_));
        if (box3d) {

            const QVector3D* boxDims = box3d->dimensions();
            // Translate to Front Left Top Box corner
            glPushMatrix();
            glTranslated( -(boxDims->x()) /* sqrt(3.f)*/, (boxDims->y()/2.f) / sqrt(3.f), -(boxDims->z()/2.f)  /* sqrt(3.f)*/ );

            foreach (Shape *shape, shapes_) {
                    shape->draw();
            }

            glPopMatrix();
        }
    }

}

void GLScene::render3d()
{
    //ENABLE TRANSPARENCY:
    //ALPHA
    glEnable(GL_ALPHA_TEST);
    //BLENDING
    glEnable(GL_BLEND);
    //Most effective func are:
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
    //glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glBlendFunc(GL_ONE, GL_ONE); // best fit with light

    if ( box_ != nullptr ) box_->draw();
    drawHotspots();

    // disable transparency
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void GLScene::render3dProj()
{

}

bool GLScene::isBoxFilled()
{
    bool b = false;
    if ( vm_ == STOPT::V_2D && !shapes_.empty() &&
         box_ != nullptr ) {

        QRegion rgRF( START_SHIFT_POS2D, START_SHIFT_POS2D, 1, 1);
        foreach( Shape* s, shapes_) {
            rgRF = rgRF.united(s->rect().toRect());
        }

        QRegion rgBox(box_->rect().toRect());
        rgBox = rgBox.subtracted(rgRF);
        b = rgBox.isEmpty();
    }
    return b;
}

QSize GLScene::sizeHint() const
{
    return QSize(400, 400);
}

void GLScene::setInteractiveData(bool ready, qreal width, qreal height, qreal radius)
{
    interDataReady_ = ready;
    interRadius_    = radius;

    if (ready) {
        createBox(width, height, 0);
        update();
    }
    else {
        if ( box_ != nullptr ) {
            delete box_;
            box_ = nullptr;
        }
    }
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
}
