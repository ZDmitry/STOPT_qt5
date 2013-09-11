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

#include "gpbox.h"

#include <cassert>

#include <QPen>
#include <QBrush>

// BOX

GPBox::GPBox(const QPointF &position, qreal width, qreal height)
    : Shape(position)
{
    box_.setTopLeft(position);
    box_.setHeight(height);
    box_.setWidth(width);
}

void GPBox::move(const QRectF &boundRect)
{
    qreal leftOverflow   = pos_.x() - box_.width()  - boundRect.left();
    qreal rightOverflow  = pos_.x() + box_.width()  - boundRect.right();
    qreal topOverflow    = pos_.y() - box_.height() - boundRect.top();
    qreal bottomOverflow = pos_.y() + box_.height() - boundRect.bottom();

    if (leftOverflow < 0.0) {
        pos_.setX(pos_.x() - 2 * leftOverflow);
        box_.setX(pos_.x());
    } else if (rightOverflow > 0.0) {
        pos_.setX(pos_.x() - 2 * rightOverflow);
        box_.setX(pos_.x());
    }

    if (topOverflow < 0.0) {
        pos_.setY(pos_.y() - 2 * topOverflow);
        box_.setY(pos_.y());
    } else if (bottomOverflow > 0.0) {
        pos_.setY(pos_.y() - 2 * bottomOverflow);
        box_.setY(pos_.y());
    }
}

QRectF GPBox::rect() const
{
    return box_;
}

void GPBox::draw(QPainter *painter) const
{
    painter->save();
    painter->setBackgroundMode(Qt::TransparentMode);
    painter->drawRect(box_);
    painter->restore();
}

void GPBox::updateBrush()
{
    brush_ = QBrush(inColor_);
}

void GPBox::buildGeometry(int d, qreal s)
{
    Q_UNUSED(d)
    Q_UNUSED(s)
}

