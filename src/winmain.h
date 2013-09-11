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
