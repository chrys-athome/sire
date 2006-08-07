#ifndef SPIER_MAINWINDOW_H
#define SPIER_MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QDir>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class Viewer;

/**
This is the Spier main window class, that holds the menu bar, viewer widget, dock window tools etc.
 
@author Christopher Woods
*/
class SPIER_EXPORT MainWindow : public QMainWindow
{

Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

public slots:
    void load();
    void load(const QString &filename);
    void save();

    void quit();

    void about();
    void aboutQt();
    
    void toggleFullScreen();
    
    void viewBuffer();
    void viewingBuffer(int id);
    
private:

    void initialise();

    /** The Viewer widget which holds the openGL display */
    Viewer *viewer;
    
    /** The action group used to toggle between different buffers */
    QActionGroup *buffergroup;

    /** The last opened directory */
    QDir lastdir;

};

}

SIRE_END_HEADER

#endif
