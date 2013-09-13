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

#include "scene.h"
#include "analitycs/analitycs2d.h"
#include "analitycs/analitycs3d.h"
#include "support/iodata.h"

using namespace std;

const QString QMainWin::RO_DISABLED_STYLE = QString("background-color: rgb(221, 221, 221);");
const QString QMainWin::RO_ENABLED_STYLE  = QString("background-color: rgb(224, 251, 255);");

QMainWin::~QMainWin()
{
    delete ui;
}

QMainWin::QMainWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QMainWin)
{
    ui->setupUi(this);

    // pack 2d input fields of variant in QList
    {
    li2dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantRadius1,
                               ui->tb2dVariantPrice1));

    li2dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantRadius2,
                               ui->tb2dVariantPrice2));

    li2dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantRadius3,
                               ui->tb2dVariantPrice3));

    li2dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantRadius4,
                               ui->tb2dVariantPrice4));

    li2dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantRadius5,
                               ui->tb2dVariantPrice5));
    }

    // pack 2d output fields of variant in QList
    {
    li2dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantCost1,
                               ui->tb2dVariantOverlap1));

    li2dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantCost2,
                               ui->tb2dVariantOverlap2));

    li2dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantCost3,
                               ui->tb2dVariantOverlap3));

    li2dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantCost4,
                               ui->tb2dVariantOverlap4));

    li2dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb2dVariantCost5,
                               ui->tb2dVariantOverlap5));
    }

    // pack 2d chekbox of variant in QList
    {
        li2dVariantsBest.append(ui->chb2dVariants1);
        li2dVariantsBest.append(ui->chb2dVariants2);
        li2dVariantsBest.append(ui->chb2dVariants3);
        li2dVariantsBest.append(ui->chb2dVariants4);
        li2dVariantsBest.append(ui->chb2dVariants5);
    }

    // pack 3d input fields of variant in QList
    {
    li3dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantRadius1,
                               ui->tb3dVariantPrice1));

    li3dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantRadius2,
                               ui->tb3dVariantPrice2));

    li3dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantRadius3,
                               ui->tb3dVariantPrice3));

    li3dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantRadius4,
                               ui->tb3dVariantPrice4));

    li3dVariantsIn.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantRadius5,
                               ui->tb3dVariantPrice5));
    }

    // pack 3d output fields of variant in QList
    {
    li3dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantCost1,
                               ui->tb3dVariantOverlap1));

    li3dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantCost2,
                               ui->tb3dVariantOverlap2));

    li3dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantCost3,
                               ui->tb3dVariantOverlap3));

    li3dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantCost4,
                               ui->tb3dVariantOverlap4));

    li3dVariantsOut.append(QPair<QLineEdit*, QLineEdit*>(
                               ui->tb3dVariantCost5,
                               ui->tb3dVariantOverlap5));
    }

    // pack 3d chekbox of variant in QList
    {
        li3dVariantsBest.append(ui->chb3dVariants1);
        li3dVariantsBest.append(ui->chb3dVariants2);
        li3dVariantsBest.append(ui->chb3dVariants3);
        li3dVariantsBest.append(ui->chb3dVariants4);
        li3dVariantsBest.append(ui->chb3dVariants5);
    }


    // set mask for input fields
    QIntValidator *qiv = new QIntValidator ( 0, 999, this );
    // 3d mode fields
    ui->tb3dHeight->setValidator(qiv);
    connect(ui->tb3dHeight,SIGNAL(textChanged(QString)),this,SLOT(dimensionsPassed(QString)));
    ui->tb3dWidth->setValidator(qiv);
    connect(ui->tb3dWidth,SIGNAL(textChanged(QString)),this,SLOT(dimensionsPassed(QString)));
    ui->tb3dLength->setValidator(qiv);
    connect(ui->tb3dLength,SIGNAL(textChanged(QString)),this,SLOT(dimensionsPassed(QString)));

    QList<QLineEdit*> fields = ui->gb3dVariants->findChildren<QLineEdit*>();

    foreach(QLineEdit* item, fields) {
        if ( !(item->isReadOnly()) ) {
            item->setValidator(qiv);
            connect(item,SIGNAL(textChanged(QString)),this,SLOT(enableMoreVariants(QString)));
        }
    }

    // 2d mode fields
    ui->tb2dHeight->setValidator(qiv);
    connect(ui->tb2dHeight,SIGNAL(textChanged(QString)),this,SLOT(dimensionsPassed(QString)));
    ui->tb2dWidth->setValidator(qiv);
    connect(ui->tb2dWidth,SIGNAL(textChanged(QString)),this,SLOT(dimensionsPassed(QString)));

    fields = ui->gb2dVariants->findChildren<QLineEdit*>();

    foreach(QLineEdit* item, fields) {
        if ( !(item->isReadOnly()) ) {
            item->setValidator(qiv);
            connect(item,SIGNAL(textChanged(QString)),this,SLOT(enableMoreVariants(QString)));
        }
    }

    ui->gb2dInteractive->setVisible(false);
    ui->btnRender->setEnabled(false);

    // set current mode as 2d
    ui->twVideoMode->setCurrentIndex(0);
    videoModeSelect(0);

    connect(ui->twVideoMode, SIGNAL(currentChanged(int)), this, SLOT(videoModeSelect(int)));
    connect(ui->chb2dInteractive,SIGNAL(stateChanged(int)), this, SLOT(interactionModeSelect(int)));
    connect(ui->chb3dProjections, SIGNAL(stateChanged(int)), this, SLOT(projectionModeSelect(int)));

    connect(ui->tb2dInteractiveRadius,SIGNAL(textChanged(QString)), this, SLOT(readInteractiveData(QString)));
    connect(ui->tb2dInteractivePrice,SIGNAL(textChanged(QString)), this, SLOT(readInteractiveData(QString)));

    foreach(QCheckBox* chb, li2dVariantsBest) {
        chb->installEventFilter(this);
    }

    foreach(QCheckBox* chb, li3dVariantsBest) {
        chb->installEventFilter(this);
    }

    connect(ui->btnRender, SIGNAL(pressed()), this, SIGNAL(inputComplete()));


    connect(this, SIGNAL(inputComplete()), this, SLOT(prepareData()));

    //ui->gvScene->setViewport(new GLScene());
    // Scene connections
    connect(this, SIGNAL(videoModeChanged(STOPT::VMODE)), ui->glScene, SLOT(setVideoMode(STOPT::VMODE)));
    connect(this, SIGNAL(dataReady(Radiofield*)), ui->glScene, SLOT(setData(Radiofield*)));
    connect(ui->glScene, SIGNAL(pointFixed(int,bool)), this, SLOT(writeInteractiveResults(int,bool)));
}

void QMainWin::clearFields(QWidget* tab)
{
    QList<QLineEdit*> fields = tab->findChildren<QLineEdit*>();

    foreach(QLineEdit* item, fields) {
        item->clear();
    }
}

void QMainWin::enableFieldsGroup(QWidget *grpVariants, bool enabled)
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
void QMainWin::enableMoreVariants(QString)
{
    QListIterator< QPair<QLineEdit*, QLineEdit*> > it = li2dVariantsIn;
    QListIterator< QPair<QLineEdit*, QLineEdit*> > jt = li2dVariantsOut;
    QListIterator< QCheckBox* > kt = li2dVariantsBest;

    if (ui->twVideoMode->currentWidget() ==
             ui->tab3dMode) {

        it = li3dVariantsIn;
        jt = li3dVariantsOut;
        kt = li3dVariantsBest;
    }

    if (it.hasNext()) {
        ui->btnRender->setEnabled( !isInputPairEmpty(it.next()) );
    }
    if (jt.hasNext()) jt.next();

    while(it.hasNext() && jt.hasNext() && kt.hasNext()) {
        QPair<QLineEdit*, QLineEdit*> preIn  = it.peekPrevious();
        QPair<QLineEdit*, QLineEdit*> nelIn  = it.next();
        QPair<QLineEdit*, QLineEdit*> nelOut = jt.next();
        QCheckBox* nelBest = kt.next();

        if (isInputPairEmpty(preIn)) {

            enableInputPair(nelIn,false);
            enableInputPair(nelOut,false);
            nelBest->setEnabled(false);

            clearInputPair(nelIn);
            clearInputPair(nelOut);
            nelBest->setCheckState(Qt::Unchecked);
        }
        else {
            enableInputPair(nelIn,true);
            enableInputPair(nelOut,true);
            nelBest->setEnabled(true);
        }
    }
}

void QMainWin::dimensionsPassed(QString)
{
    if (ui->twVideoMode->currentWidget() ==
            ui->tab2dMode) {

        if (ui->tb2dHeight->text().isEmpty() ||
                ui->tb2dWidth->text().isEmpty()){

            enableFieldsGroup(ui->gb2dVariants,false);
            enableFieldsGroup(ui->gb2dInteractive,false);
            enableFieldsGroup(ui->gb2dOptimal,false);
            enableFieldsGroup(ui->gb2dOverlap,false);
            enableCheckedVariants(li2dVariantsBest, false);

            ui->pb2dCoverage->setEnabled(false);
            clearFields(ui->gb2dVariants);
        }
        else {
            //enableFieldsGroup(ui->gb2dVariants,true);
            ui->tb2dVariantRadius1->setEnabled(true);
            ui->tb2dVariantPrice1->setEnabled(true);
            enableROField(ui->tb2dVariantCost1,true);
            enableROField(ui->tb2dVariantOverlap1,true);

            enableFieldsGroup(ui->gb2dInteractive,true);
            enableFieldsGroup(ui->gb2dOptimal,true);
            enableFieldsGroup(ui->gb2dOverlap,true);

            ui->pb2dCoverage->setEnabled(true);

            int height = ui->tb2dHeight->text().toInt();
            int width  = ui->tb2dWidth->text().toInt();
            ui->tb2dCoverTask->setText( QString::number((height*width)) );
        }

    }
    else if (ui->twVideoMode->currentWidget() ==
                ui->tab3dMode) {

        if (ui->tb3dHeight->text().isEmpty() ||
                ui->tb3dWidth->text().isEmpty() ||
                ui->tb3dLength->text().isEmpty()){

            enableFieldsGroup(ui->gb3dVariants,false);
            enableFieldsGroup(ui->gb3dOptimal,false);
            enableFieldsGroup(ui->gb3dOverlap,false);
            enableCheckedVariants(li3dVariantsBest,false);

            ui->pb3dCoverage->setEnabled(false);
            clearFields(ui->gb3dVariants);
        }
        else {
            //enableFieldsGroup(ui->gb3dVariants,true);
            ui->tb3dVariantRadius1->setEnabled(true);
            ui->tb3dVariantPrice1->setEnabled(true);
            enableROField(ui->tb3dVariantCost1,true);
            enableROField(ui->tb3dVariantOverlap1,true);

            enableFieldsGroup(ui->gb3dOptimal,true);
            enableFieldsGroup(ui->gb3dOverlap,true);

            ui->pb3dCoverage->setEnabled(true);

            int height = ui->tb3dHeight->text().toInt();
            int width  = ui->tb3dWidth->text().toInt();
            int lenght = ui->tb3dLength->text().toInt();
            ui->tb3dCoverTask->setText( QString::number((height*width*lenght)) );
        }
    }
}

void QMainWin::prepareData()
{
    int n = 0;

    Radiofield* best = nullptr;

    QListIterator< QPair<QLineEdit*, QLineEdit*> > it = li2dVariantsIn;
    QListIterator< QPair<QLineEdit*, QLineEdit*> > jt = li2dVariantsOut;

    if (ui->twVideoMode->currentWidget() ==
            ui->tab2dMode) {

        n = numOfFilledVariants(li2dVariantsIn);

        int height = ui->tb2dHeight->text().toInt();
        int width  = ui->tb2dWidth->text().toInt();

        std::vector<Radiofield2D* >* rf = new std::vector<Radiofield2D* >();
        rf->reserve(n);

        for (int i=0; i<n; i++) {
            QPair<QLineEdit*, QLineEdit*> fl = it.next();
            QPair<QLineEdit*, QLineEdit*> sl = jt.next();

            int radius = fl.first->text().toInt();
            int price  = fl.second->text().toInt();

            // generate points for this variant
            rf->push_back( new Radiofield2D(width, height, radius, price) );
            Radiofield2D*& rfe = rf->at(i);
            // get calculated values:
            QString cost = QString::number(rfe->getTotalCost());
            sl.first->setText(cost);
            QString overlap = QString::number(rfe->effectiveArea());
            sl.second->setText(overlap);
        }

        int bestVariant = Radiofield2D::getOptimalVariant(rf);
        Radiofield2D*& rfb = rf->at(bestVariant);
        int bestCost    = rfb->getTotalCost();
        int bestCount   = rfb->getCount();
        int bestRadius  = rfb->getRadius();
        int bestSquare  = rfb->filledArea();

        double bestOverlap = rfb->effectiveArea();
        if ( bestOverlap > 1.f ) ui->pb2dCoverage->setValue(100);
        else {
            ui->pb2dCoverage->setValue( qRound(bestOverlap*100) );
        }
        ui->tb2dOverlap->setText(QString::number(bestOverlap));
        ui->tb2dSquare->setText(QString::number(bestSquare));

        clearCheckedVariants(li2dVariantsBest);
        li2dVariantsBest.at(bestVariant)->setCheckState(Qt::Checked);
        ui->tb2dOptimalCost->setText(QString::number(bestCost));
        ui->tb2dOptimalCount->setText(QString::number(bestCount));
        ui->tb2dOptimalRadius->setText(QString::number(bestRadius));

        best = new Radiofield2D();
        Radiofield* tmp  = rfb;
        rfb = dynamic_cast<Radiofield2D*>(best); best = tmp;
        tmp = nullptr;

        foreach(Radiofield2D* f, (*rf)) {
            delete f;
        }
        delete rf;
    // endif ( vm_ == V_2D)
    }

    if (ui->twVideoMode->currentWidget() ==
             ui->tab3dMode) {

        it = li3dVariantsIn;
        jt = li3dVariantsOut;

        n = numOfFilledVariants(li3dVariantsIn);

        int height = ui->tb3dHeight->text().toInt();
        int width  = ui->tb3dWidth->text().toInt();
        int lenght = ui->tb3dLength->text().toInt();

        std::vector<Radiofield3D* >* rf = new std::vector<Radiofield3D* >();
        rf->reserve(n);

        for (int i=0; i<n; i++) {
            QPair<QLineEdit*, QLineEdit*> fl = it.next();
            QPair<QLineEdit*, QLineEdit*> sl = jt.next();

            int radius = fl.first->text().toInt();
            int price  = fl.second->text().toInt();

            // generate points for this variant
            rf->push_back( new Radiofield3D(width, height, lenght, radius, price) );
            Radiofield3D*& rfe = rf->at(i);
            // get calculated values:
            QString cost = QString::number(rfe->getTotalCost());
            sl.first->setText(cost);
            QString overlap = QString::number(rfe->effectiveVolume());
            sl.second->setText(overlap);
        }

        int bestVariant = Radiofield3D::getOptimalVariant(rf);
        Radiofield3D*& rfb = rf->at(bestVariant);
        int bestCost    = rfb->getTotalCost();
        int bestCount   = rfb->getCount();
        int bestRadius  = rfb->getRadius();
        int bestVolume  = rfb->filledVolume();

        double bestOverlap = rfb->effectiveVolume();
        if ( bestOverlap > 1.f ) ui->pb3dCoverage->setValue(100);
        else {
            ui->pb3dCoverage->setValue( qRound(bestOverlap*100) );
        }
        ui->tb3dOverlap->setText(QString::number(bestOverlap));
        ui->tb3dVolume->setText(QString::number(bestVolume));

        clearCheckedVariants(li3dVariantsBest);
        li3dVariantsBest.at(bestVariant)->setCheckState(Qt::Checked);
        ui->tb3dOptimalCost->setText(QString::number(bestCost));
        ui->tb3dOptimalCount->setText(QString::number(bestCount));
        ui->tb3dOptimalRadius->setText(QString::number(bestRadius));

        best = new Radiofield3D();
        Radiofield* tmp  = rfb;
        rfb = dynamic_cast<Radiofield3D*>(best); best = tmp;
        tmp = nullptr;

        foreach(Radiofield3D* f, (*rf)) {
            delete f;
        }
        delete rf;

    // endif ( vm_ == V_3D)
    }

    //if ( best != nullptr ) delete best;

    emit dataReady(best);
}

void QMainWin::readInteractiveData(QString)
{
    if (ui->twVideoMode->currentWidget() ==
            ui->tab2dMode) {

        if (ui->tb2dInteractiveRadius->text().isEmpty() ||
                ui->tb2dInteractivePrice->text().isEmpty()) {

            ui->glScene->setInteractiveData(false, 0, 0, 0);

            ui->tb2dInteractiveCost->text().clear();
            ui->tb2dInteractiveCount->text().clear();

            ui->pb2dCoverage->setValue(0);
            ui->pb2dCoverage->setEnabled(false);
            clearFields(ui->gb2dOverlap);
            enableFieldsGroup(ui->gb2dOverlap,false);
        }
        else {
            ui->pb2dCoverage->setValue(0);
            ui->pb2dCoverage->setEnabled(true);
            clearFields(ui->gb2dOverlap);
            enableFieldsGroup(ui->gb2dOverlap,true);

            int radius = ui->tb2dInteractiveRadius->text().toInt();
            int height = ui->tb2dHeight->text().toInt();
            int width  = ui->tb2dWidth->text().toInt();

            ui->glScene->setInteractiveData(true, width, height, radius);
        }

    }
}

void QMainWin::writeInteractiveResults(int count, bool covered)
{
    int radius = ui->tb2dInteractiveRadius->text().toInt();
    int price  = ui->tb2dInteractivePrice->text().toInt();

    ui->tb2dInteractiveCount->setText(QString::number(count));

    int height = ui->tb2dHeight->text().toInt();
    int width  = ui->tb2dWidth->text().toInt();

    ui->tb2dCoverTask->setText(QString::number(width*height));

    hsPoints_.append(QPair<int, int>(radius, price));

    if (covered) {
        ui->pb2dCoverage->setValue(100);
    }

    QListIterator< QPair<int, int> > it = hsPoints_;

    qreal square = 0;
    qreal cost   = 0;

    while( it.hasNext() ) {
        QPair<int, int> p = it.next();
        square += (p.first*2)*(p.first*2);
        cost   += p.second;
    }

    ui->tb2dInteractiveCost->setText(QString::number(cost));
    ui->tb2dSquare->setText(QString::number(square));
    qreal overlap = square/(width*height);
    ui->tb2dOverlap->setText(QString::number(overlap));
}

void QMainWin::saveScene(QString &fname)
{
    ui->glScene->grabFrameBuffer().save(fname);
}

void QMainWin::saveData(QString &fname)
{

}

bool QMainWin::eventFilter(QObject *target, QEvent *event)
{
    // deny checked state by mouse press
    QListIterator< QCheckBox* > it = li2dVariantsBest;

    if (ui->twVideoMode->currentWidget() ==
            ui->tab2dMode)
    { ; }
    else if (ui->twVideoMode->currentWidget() ==
            ui->tab3dMode) {

        it = li3dVariantsBest;
    }

    while(it.hasNext()) {
        if ( target == it.next() ) {
            if (event->type() ==  QEvent::MouseButtonPress ||
                    event->type() == QEvent::MouseButtonRelease ) {
                return true;
            }
        }
    }


    return QWidget::eventFilter(target, event);
}

void QMainWin::enableROField(QWidget *obj, bool enable)
{
    if (enable)
        obj->setStyleSheet(RO_ENABLED_STYLE);
    else
        obj->setStyleSheet(RO_DISABLED_STYLE);

    obj->setEnabled(enable);
}

int QMainWin::numOfFilledVariants(QList<QPair<QLineEdit *, QLineEdit *> > li)
{
    int i=0;
    QListIterator< QPair<QLineEdit*, QLineEdit*> > it = li;

    while(it.hasNext()) {
         if (isInputPairEmpty(it.next())) break;
         i++;
    }

    return i;

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

        emit videoModeChanged(STOPT::V_2D);
    }
    else if (i == 1) {
        clearFields(ui->tab3dMode);
        ui->pb3dCoverage->setValue(0);
        ui->chb2dInteractive->setVisible(false);
        ui->chb3dProjections->setVisible(true);

        emit videoModeChanged(STOPT::V_3D);
    }

    dimensionsPassed(QString());
}

void QMainWin::interactionModeSelect(int s)
{
    if ( s == Qt::Checked) {
        ui->gb2dInteractive->setVisible(true);
        ui->gb2dVariants->setVisible(false);
        ui->btnRender->setEnabled(false);

        hsPoints_.clear();
        ui->glScene->setInteractiveMode(true);
    }
    else if ( s == Qt::Unchecked) {
        ui->gb2dInteractive->setVisible(false);
        ui->gb2dVariants->setVisible(true);
        ui->btnRender->setEnabled(true);

        hsPoints_.clear();
        ui->glScene->setInteractiveMode(false);
    }
}

void QMainWin::projectionModeSelect(int s)
{
    if (ui->twVideoMode->currentWidget() ==
            ui->tab3dMode) {

      if ( s == Qt::Checked) {

          emit videoModeChanged(STOPT::V_PROJ);
      }
      else if (s == Qt::Unchecked) {
          emit videoModeChanged(STOPT::V_3D);
      }
    }
}

void QMainWin::clearCheckedVariants(QList<QCheckBox *> cv)
{
    foreach(QCheckBox * cb, cv) {
        cb->setCheckState(Qt::Unchecked);
    }
}

void QMainWin::enableCheckedVariants(QList<QCheckBox *> cv, bool enable)
{
    foreach(QCheckBox * cb, cv) {
        cb->setEnabled(enable);
    }
}

int QMainWin::bestCheckedVariants(QList<QCheckBox *> cv)
{
    int i=0;
    QListIterator< QCheckBox * > it = cv;

    while(it.hasNext()) {
         if ( it.next()->isChecked() ) break;
         i++;
    }

    if ( i == 5 ) i = (-1);

    return i;
}

void QMainWin::clearInputPair(QPair<QLineEdit *, QLineEdit *> qp)
{
    qp.first->clear();
    qp.second->clear();
}

bool QMainWin::isInputPairEmpty(QPair<QLineEdit *, QLineEdit *> qp)
{
    return (qp.first->text().isEmpty() || qp.second->text().isEmpty());
}

void QMainWin::enableInputPair(QPair<QLineEdit *, QLineEdit *> qp, bool enabled)
{
    if (qp.first->isReadOnly()) {
        enableROField(qp.first, enabled);
        enableROField(qp.second, enabled);
    }
    else {
        qp.first->setEnabled(enabled);
        qp.second->setEnabled(enabled);
    }
}
