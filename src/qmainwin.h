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

#include <QMainWindow>
#include "support/stoptdef.h"

namespace Ui {
class QMainWin;
}

// forward declaration
class QCheckBox;
class QLineEdit;

class GLScene;
class Radiofield;

class QMainWin : public QWidget
{
    Q_OBJECT

public:
    explicit QMainWin(QWidget* parent = 0);
    ~QMainWin();

    void clearCheckedVariants(QList< QCheckBox* > cv);
    void enableCheckedVariants(QList< QCheckBox* > cv, bool enable);
    int  bestCheckedVariants(QList< QCheckBox* > cv);

    void clearInputPair(QPair<QLineEdit *, QLineEdit *> qp);
    bool isInputPairEmpty(QPair<QLineEdit *, QLineEdit *> qp);
    void enableInputPair(QPair<QLineEdit *, QLineEdit *> qp, bool enabled);

    void clearFields(QWidget *tab);
    void enableFieldsGroup(QWidget *grpVariants, bool enabled);
    void enableROField(QWidget *obj, bool enable);

    int  numOfFilledVariants(QList<QPair<QLineEdit *, QLineEdit *> > li);

    virtual bool eventFilter(QObject *target, QEvent *event);

signals:
    void videoModeChanged(STOPT::VMODE);
    void inputComplete();  // input completed, can fill data
    void dataReady(Radiofield*);      // data filled, can render

public slots:
    void videoModeSelect(int i);
    void interactionModeSelect(int s);
    void projectionModeSelect(int s);
    void enableMoreVariants(QString);
    void dimensionsPassed(QString);

    void prepareData();
    void readInteractiveData(QString);
    void writeInteractiveResults(int count, qreal covered);
    void saveScene(QString &fname);
    void saveData(QString &fname);

private:
    Ui::QMainWin *ui;
    //GLScene* glScene

    QList< QPair<int, int> > hsPoints_;

    // first - radius, second - price
    QList< QPair<QLineEdit*, QLineEdit*> > li2dVariantsIn;
    QList< QPair<QLineEdit*, QLineEdit*> > li3dVariantsIn;

    // first - cost, second - overlap
    QList< QPair<QLineEdit*, QLineEdit*> > li2dVariantsOut;
    QList< QPair<QLineEdit*, QLineEdit*> > li3dVariantsOut;

    QList< QCheckBox* > li2dVariantsBest;
    QList< QCheckBox* > li3dVariantsBest;

    static const QString RO_DISABLED_STYLE;
    static const QString RO_ENABLED_STYLE;
    static const int     MAX_VARIANT_COUNT = 5;
};

#endif // QMAINWIN_H
