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

GPBox::GPBox(const QPointF &position, const QPointF &dimensions)
    : Shape(position)
{
    box_.setTopLeft(position);
    box_.setWidth(dimensions.x());
    box_.setHeight(dimensions.y());
}

void GPBox::move(const QPointF &pt)
{
    box_.setX(pt.x());
    pos_.setX(pt.x());

    box_.setY(pt.y());
    pos_.setY(pt.y());
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

void GPBox::setDimensions(const QPointF &dims)
{
    box_.setWidth(dims.x());
    box_.setHeight(dims.y());
}

QPointF GPBox::getDimensions()
{
    return QPointF(box_.width(), box_.height());
}

void GPBox::updateBrush()
{
    brush_ = QBrush(outColor_);
}

