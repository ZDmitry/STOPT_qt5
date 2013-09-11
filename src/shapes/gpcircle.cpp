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

#include "gpcircle.h"

// CIRCLE

GPCircle::GPCircle(const QPointF &position, qreal radius, int number)
    : Shape(position),
      radius_(radius),
      number_(number)
{
    updateBrush();
}

void GPCircle::move(const QRectF &boundRect)
{
    qreal leftOverflow   = pos_.x() - radius_ - boundRect.left();
    qreal rightOverflow  = pos_.x() + radius_ - boundRect.right();
    qreal topOverflow    = pos_.y() - radius_ - boundRect.top();
    qreal bottomOverflow = pos_.y() + radius_ - boundRect.bottom();

    if (leftOverflow < 0.0) {
        pos_.setX(pos_.x() - 2 * leftOverflow);
    } else if (rightOverflow > 0.0) {
        pos_.setX(pos_.x() - 2 * rightOverflow);
    }

    if (topOverflow < 0.0) {
        pos_.setY(pos_.y() - 2 * topOverflow);
    } else if (bottomOverflow > 0.0) {
        pos_.setY(pos_.y() - 2 * bottomOverflow);
    }
}

QRectF GPCircle::rect() const
{
    return QRectF(pos_.x() - radius_, pos_.y() - radius_,
                  2 * radius_, 2 * radius_);
}

void GPCircle::draw(QPainter *painter) const
{
    qreal ox = pos_.x() - radius_;
    qreal oy = pos_.y() - radius_;

    painter->save();
    painter->translate(ox, oy);
    painter->setBrush(brush_);
    painter->drawEllipse(0, 0, int(2*radius_), int(2*radius_));

    painter->setBrush(QBrush(Qt::black));

    //drawText preformance is very slow
    //painter->drawText((radius_- 4), (radius_ - 4), strNum_);
    painter->drawEllipse(radius_, radius_ , 2, 2);

    painter->restore();
}

void GPCircle::updateBrush()
{
    QRadialGradient grad(QPointF(radius_, radius_), radius_,
                         QPointF(radius_*0.5, radius_*0.5));

    grad.setColorAt(0, QColor(255, 255, 255, 255));
    grad.setColorAt(0.25, inColor_);
    grad.setColorAt(1, outColor_);

    brush_ = QBrush(grad);
}

GPCircle& GPCircle::operator=(const GPCircle& r)
{
    pos_.setX(0);
    pos_.setY(0);

    move( r.rect() );

    number_   = r.getNumber();
    radius_   = r.getRadius();

    int ri, g, b, a;

    ri = r.getInnerColor().red();
    g  = r.getInnerColor().green();
    b  = r.getInnerColor().blue();
    a  = r.getInnerColor().alpha();

    inColor_.setRgb(ri,g,b,a);

    ri = r.getOuterColor().red();
    g  = r.getOuterColor().green();
    b  = r.getOuterColor().blue();
    a  = r.getOuterColor().alpha();

    outColor_.setRgb(ri,g,b,a);

    updateBrush();

    return (*this);
}

void GPCircle::buildGeometry(int d, qreal s)
{
    Q_UNUSED(d)
    Q_UNUSED(s)
}
