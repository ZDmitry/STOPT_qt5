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

#include "qmainwin.h"
#include "ui_qmainwin.h"

const QString QMainWin::RO_DISABLED_STYLE = QString("background-color: rgb(221, 221, 221);");
const QString QMainWin::RO_ENABLED_STYLE  = QString("background-color: rgb(224, 251, 255);");

QMainWin::QMainWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QMainWin)
{
    ui->setupUi(this);

    // set mask for input fields
    QIntValidator *qiv = new QIntValidator ( 0, 65535, this );
    // 3d mode fields
    ui->tb3dHeight->setValidator(qiv);
    ui->tb3dWidth->setValidator(qiv);
    ui->tb3dLength->setValidator(qiv);

    QList<QLineEdit*> fields = ui->gb3dVariants->findChildren<QLineEdit*>();

    foreach(QLineEdit* item, fields) {
        if ( !(item->isReadOnly()) ) {
            item->setValidator(qiv);
            item->setEnabled(false);
        }
        else
            enableROField(item, false);
    }

    ui->tb3dVariantRadius1->setEnabled(true);
    ui->tb3dVariantPrice1->setEnabled(true);
    enableROField(ui->tb3dVariantCost1,true);
    enableROField(ui->tb3dVariantOverlap1,true);

    // 2d mode fields
    ui->tb2dHeight->setValidator(qiv);
    ui->tb2dWidth->setValidator(qiv);

    fields = ui->gb2dVariants->findChildren<QLineEdit*>();

    foreach(QLineEdit* item, fields) {
        if ( !(item->isReadOnly()) ) {
            item->setValidator(qiv);
            item->setEnabled(false);
        }
        else
            enableROField(item, false);
    }

    ui->tb2dVariantRadius1->setEnabled(true);
    ui->tb2dVariantPrice1->setEnabled(true);
    enableROField(ui->tb2dVariantCost1,true);
    enableROField(ui->tb2dVariantOverlap1,true);

    ui->gb2dInteractive->setVisible(false);

    // set current mode as 2d
    ui->twVideoMode->setCurrentIndex(0);
    videoModeSelect(0);

    connect(ui->twVideoMode, SIGNAL(currentChanged(int)), this, SLOT(videoModeSelect(int)));
    connect(ui->chb2dInteractive,SIGNAL(stateChanged(int)), this, SLOT(interactionModeSelect(int)));
}

void QMainWin::clearFields(QWidget* tab)
{
    QList<QLineEdit*> fields = tab->findChildren<QLineEdit*>();

    foreach(QLineEdit* item, fields) {
        item->clear();
    }
}

void QMainWin::disableVariantFields(QWidget *grpVariants, bool enabled)
{
    QList<QLineEdit*> fields = grpVariants->findChildren<QLineEdit*>();

    foreach(QLineEdit* item, fields) {
        if ( !(item->isReadOnly()) ) {
            item->setEnabled(enabled);
        }
        else
            enableROField(item, enabled);
    }
}

void QMainWin::enableROField(QWidget *obj, bool enable)
{
    if (enable)
        obj->setStyleSheet(RO_ENABLED_STYLE);
    else
        obj->setStyleSheet(RO_DISABLED_STYLE);

    obj->setEnabled(enable);
}

void QMainWin::videoModeSelect(int i)
{
    ui->chb2dInteractive->setCheckState(Qt::Unchecked);
    ui->chb3dProjections->setCheckState(Qt::Unchecked);

    if ( i == 0) {
        clearFields(ui->tab2dMode);
        ui->pb2dCoverage->setValue(0);
        ui->chb2dInteractive->setVisible(true);
        ui->chb3dProjections->setVisible(false);
    }
    else if (i == 1) {
        clearFields(ui->tab3dMode);
        ui->pb3dCoverage->setValue(0);
        ui->chb2dInteractive->setVisible(false);
        ui->chb3dProjections->setVisible(true);
    }
}

void QMainWin::interactionModeSelect(int s)
{
    if ( s == Qt::Checked) {
        ui->gb2dInteractive->setVisible(true);
        ui->gb2dVariants->setVisible(false);
    }
    else if ( s == Qt::Unchecked) {
        ui->gb2dInteractive->setVisible(false);
        ui->gb2dVariants->setVisible(true);
    }
}

QMainWin::~QMainWin()
{
    delete ui;
}
