#ifndef SIREPY_PYTHONPARSER_H
#define SIREPY_PYTHONPARSER_H

#include <QString>
#include <QStringList>
#include <QReadWriteLock>

#include <boost/shared_ptr.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

class QTimer;

namespace SirePy
{

class PythonScript;

/** The status of the PythonParser */
enum PythonStatus{ PYTHON_IDLE,     //the parser is not currently doing anything
                   PYTHON_BUSY,     //the parser is currently processing
                   PYTHON_ERROR,    //the parser is in a broken or not running state!
                   PYTHON_EXITING}; //the parser is exiting

/**
This is a complete Python parser.
 
@author Christopher Woods
*/
class SIREPY_EXPORT PythonParser : public QObject
{
    Q_OBJECT
    
public:
    PythonParser(QObject *parent=0);
    ~PythonParser();

    /** Run the python script contained in 'scrpt' */
    void run(const boost::shared_ptr<PythonScript> &scrpt);
    /** Pause the interpreter! */
    void pause();
    /** Restart the interpreter (play, as in play/pause) */
    void play();

    /** Function to access the status of the PythonParser */
    PythonStatus status() const;

protected:
    /** Start (or restart) the Python interpreter */
    void start(const boost::shared_ptr<PythonScript> &script);
    /** Stop the Python interpreter */
    void stop();
    /** Part of the implementation of start() and stop() */
    void startPython();
    void stopPython();
    /** Set the status of the PythonParser */
    void setStatus(PythonStatus s);
    /** Run a block of python code */
    void runBlock(const QString &block);
    
    /** A mutex to protect access to the status of the parser */
    QReadWriteLock statlock;

    /** The status of the PythonParser */
    PythonStatus stat;

    /** The complete ran, running and to-be-run Python script */
    boost::shared_ptr<PythonScript> script;
        
    /** A timer used to check if there is any more python to run */
    QTimer *timer;
    
private slots:
    /** Check the python queue to see if there are any lines
        to parse */
    void checkQueue();

signals:
    /** Emmited whenever there is a parse error */
    void parseError();
    /** Emmited whenever the script has been fully parsed */
    void parseComplete();
    
};

}

SIRE_END_HEADER

#endif
