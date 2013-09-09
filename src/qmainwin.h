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

#ifndef QMAINWIN_H
#define QMAINWIN_H

#include <QWidget>

namespace Ui {
class QMainWin;
}

class QMainWin : public QWidget
{
    Q_OBJECT

public:
    explicit QMainWin(QWidget *parent = 0);
    ~QMainWin();

    void clearFields(QWidget *tab);
    void disableVariantFields(QWidget *grpVariants, bool enabled);
    void enableROField(QWidget *obj, bool enable);


public slots:
    void videoModeSelect(int i);
    void interactionModeSelect(int s);

private:
    Ui::QMainWin *ui;

    static const QString RO_DISABLED_STYLE;
    static const QString RO_ENABLED_STYLE;
};

#endif // QMAINWIN_H
