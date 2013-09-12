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

#ifndef WINMAIN_H
#define WINMAIN_H

#include <QMainWindow>

class QMainWin;

class WinMain : public QMainWindow
{
    Q_OBJECT

public:
    WinMain();
    //~WinMain();

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    bool unsavedChanges();
    bool saveScene();
    bool saveData();
    void about();

private:
    void createActions();
    void createMenus();
    void createToolBars();

    bool saveFile(QString &fileName, QString &title, QString &filter);

    QMainWin *body_;

    QMenu    *fileMenu_;
    QMenu    *helpMenu_;

    QToolBar *fileToolBar_;

    QAction  *actSaveScene_;
    QAction  *actSaveData_;
    QAction  *actAbout_;
    QAction  *actAboutQt_;
    QAction  *actExit_;

};

#endif // WINMAIN_H
