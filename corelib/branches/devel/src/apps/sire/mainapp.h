#ifndef MAINAPP_H
#define MAINAPP_H

#include <QObject>

#include "SirePy/pythonparser.h"

class QStringList;

/**
The main application object for Sire. 

This is the main application object for Sire. A copy of this objects 
is started on each node in the cluster.

The MainApp running on the master process can be controlled by python, and
houses the python script thread. The master process MainApp also 
acts as the network TCP server which is used to allow external
clients (commandline and gui) to connect and control the application.
 
@author Christopher Woods
*/
class MainApp : public QObject
{

Q_OBJECT

public:
    MainApp();
    virtual ~MainApp();

public slots:
    void runPython(const QStringList &files);

private slots:
    void shutdown();
        
private:
    void initialise();    
        
    /** Pointer to the python parser */
    SirePy::PythonParser *parser;
        
};

#endif
