#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QKeySequence>
#include <QCloseEvent>

#include "winmain.h"
#include "qmainwin.h"

WinMain::WinMain() :
    QMainWindow()
{
    body_ = new QMainWin;
    setCentralWidget(body_);

    createActions();
    createMenus();
    createToolBars();

    setUnifiedTitleAndToolBarOnMac(true);
}

void WinMain::closeEvent(QCloseEvent *e)
{
    if (unsavedChanges()) {
        e->accept();
    } else {
        e->ignore();
    }
}

bool WinMain::unsavedChanges()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("STOPT"),
                 tr("Do you want to save your work?"),
                 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (ret == QMessageBox::Yes) {
        saveScene();
        saveData();
        return true;
    }
    else if (ret == QMessageBox::No)
        return true;
    //else if (ret == QMessageBox::Cancel)
    return false;
}

bool WinMain::saveScene()
{
    QString fname;
    QString title  = "Save scene as image";
    QString filter = "Portable Network Graphics (*.png);";

    bool res = saveFile(fname, title, filter);
    if (res) {
        body_->saveScene(fname);
    }

    return res;
}

bool WinMain::saveData()
{
    QString fname;
    QString title  = "Save data as text";
    QString filter = "Plain text (*.txt);";

    bool res = saveFile(fname, title, filter);
    if (res) {
        body_->saveData(fname);
    }

    return res;
}

void WinMain::about()
{
    QMessageBox::about(this, tr("About..."),
             tr("The <b>STOPT</b> is application for topological<br>"
                "optimization that uses Qt5 and OpenGL.<br><br>"
                "Made by ZDmitry ( <a href='mailto:hiroshidi@gmail.com'>hiroshidi@gmail.com</a> )"));
}

void WinMain::createActions()
{
    actSaveScene_ = new QAction(QIcon(":/images/save.png"), tr("Save &Scene"), this);
    actSaveScene_->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    actSaveScene_->setStatusTip(tr("Save scene to image file"));
    connect(actSaveScene_, SIGNAL(triggered()), this, SLOT(saveScene()));

    actSaveData_ = new QAction(QIcon(":/images/new.png"), tr("Save &Data"), this);
    actSaveData_->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    actSaveData_->setStatusTip(tr("Save data array to text file"));
    connect(actSaveData_, SIGNAL(triggered()), this, SLOT(saveData()));

    actAbout_ = new QAction(tr("About"), this);
    actAbout_->setStatusTip(tr("Save scene to image file"));
    connect(actAbout_, SIGNAL(triggered()), this, SLOT(about()));

    actAboutQt_ = new QAction(tr("About Qt"), this);
    actAboutQt_->setStatusTip(tr("Show the Qt library's About box"));
    connect(actAboutQt_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    actExit_ = new QAction(tr("E&xit"), this);
    actExit_->setShortcuts(QKeySequence::Quit);
    actExit_->setStatusTip(tr("Exit the application"));
    connect(actExit_, SIGNAL(triggered()), this, SLOT(close()));
}

void WinMain::createMenus()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(actSaveScene_);
    fileMenu_->addAction(actSaveData_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(actExit_);

    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(actAbout_);
    helpMenu_->addAction(actAboutQt_);
}

void WinMain::createToolBars()
{
    fileToolBar_ = addToolBar(tr("File"));
    fileToolBar_->addAction(actSaveScene_);
    fileToolBar_->addAction(actSaveData_);
}

bool WinMain::saveFile(QString &fileName, QString &title, QString &filter)
{
    QString fname = QFileDialog::getSaveFileName(this, title,
                                                    qApp->applicationDirPath(),
                                                    filter);

    if (fname.isEmpty())
        return false;

    fileName = fname;
    return true;
}
