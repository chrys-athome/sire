/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

//Include the Python header - needs to be before anything else
#include <Python.h>
//Include the boost.python header
#include <boost/python.hpp>

#include <QDebug>

//now include local module's headers...
#include "pythonparser.h"
#include "pythonerror.h"
#include "pythonscript.h"

//now include the python wrappers
#include "python2wrappers.h"

//finally global headers
#include <QReadWriteLock>
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

//will import the boost.python namespace - needs to be after all header files
using namespace boost::python;
using namespace SirePy;

//have file local handles for the __main__ module and its global namespace
handle<PyObject> mainmodule;
handle<PyObject> mainnamespace;

PythonParser::PythonParser(QObject *parent) : QObject(parent),timer(0)
{
    this->setStatus(PYTHON_ERROR);
}

PythonParser::~PythonParser()
{}

void PythonParser::run(const boost::shared_ptr<PythonScript> &scpt)
{
    this->start(scpt);
}

void PythonParser::start(const boost::shared_ptr<PythonScript> &scrpt)
{
    //stop python - this ensures ensures that python is not already running...
    this->stop();
    
    //now get a copy of the pointer to the script
    script = scrpt;

    if (not script)
        //there is no script, so there is nothing that we need to do
        return;
    
    //start the python interpretor
    startPython();

    //create a timer to periodically check to see if there are any python lines
    //to process
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(checkQueue()));

    //set the status of the interpreter to 'IDLE'
    this->setStatus(PYTHON_IDLE);
    timer->start();
}

void PythonParser::stop()
{
    //if there is no timer, then the parser has not yet been started
    if (not timer)
      return;

    this->setStatus(PYTHON_EXITING);
    
    //stop the timer
    timer->stop();
    delete timer;
    timer = 0;
    
    //shut down the python interpreter
    this->stopPython();
    
    //now forget about the script
    script.reset();
}

void PythonParser::checkQueue()
{
    //check the string queue to see if there are any lines of code to
    //process...
    if (not script)
    {
        qDebug() << tr("PythonParser is running despite it not having a script...");
        qDebug() << tr("Forcibly stopping the python interpreter!");
        this->stopPython();
    }
    else if (script->status() == SCRIPT_EMPTY)
    {
        timer->start(100);
    }
    else if (script->status() == SCRIPT_COMPLETE)
    {
        //tell the application that the script is complete
        qDebug() << tr("The python script has been completely parsed");
        emit parseComplete();
        
        this->pause();
    }
    else
    {
        //pull the next block off of the script
        QString block = script->nextBlock();

        //try to run this block
        try
        {
            this->runBlock(block);
            //restart the timer with '0' timeout so that we immediately
            //try to parse the next line
            timer->start(0);
        }
        catch(PythonError error)
        {
            //there was an error with this block
            qDebug() << tr("There was an error with the python. Pausing the interpreter!");
            script->setError(error);
            //pause the parser
            this->pause();

        }
        
        //if there was an error then emit a signal (we do it here because we 
        //don't want to emit a signal while we are doing exception handling
        if (not script->error().isEmpty())
        {
            //debug the PythonError
            script->error().debug();
            emit parseError();
        }
    }
}

void PythonParser::runBlock(const QString &statement)
{
    if (statement.isNull())
    {
        qDebug() << tr("Attempting to process a null statement!");
        return;
    }

    //if the python parser is not idle then we have a problem...
    if (this->status() != PYTHON_IDLE)
    {
        qDebug() << tr("The python parser is not idle!");
        throw "NotIdleException";
    }

    //process this line of python
    this->setStatus(PYTHON_BUSY);
    PyObject *output = PyRun_String(statement.toAscii().constData(),
                                    Py_file_input,
                                    mainnamespace.get(),mainnamespace.get());

    //the output will be 'None' if this was successful, otherwise
    //it will be a null pointer!
    if (output == 0)
    {
        qDebug() << tr("Python has thrown an exception!");
        script->debugScript();
        //construct a PythonError object using the Python API
        PythonError error = PythonError::fromPythonAPI();
        throw error;
    }

    this->setStatus(PYTHON_IDLE);
}

void PythonParser::startPython()
{
    if (Py_IsInitialized())
        stopPython();

    qDebug() << tr("Starting Python Parser");
    Py_SetProgramName(QCoreApplication::instance()->applicationName().toAscii().data());
    Py_Initialize();
    qDebug() << tr("argv[0] = '%1'").arg(Py_GetProgramName());
    qDebug() << tr("Started Python %1\n %2").arg(Py_GetVersion(),Py_GetCopyright());

    //get the handles to the __main__ module and global namespace
    mainmodule = handle<PyObject>(borrowed( PyImport_AddModule("__main__") ));
    mainnamespace = handle<PyObject>(borrowed( PyModule_GetDict(mainmodule.get()) ));

    //now initialise the statically compiled in modules
    //init_python_modules();
}

void PythonParser::stopPython()
{
    if (Py_IsInitialized())
    {
        qDebug() << tr("Closing python...");
        this->setStatus(PYTHON_ERROR);

        //clear the Python error indicator as we are no longer interested in it
        PyErr_Clear();
        
        //clear the script error indicator (this releases any held python objects)
        script->ignoreError();

        //now release the memory held in the namespaces
        mainnamespace.release();
        mainmodule.release();

        //close the Python interpretor
        Py_Finalize();
    }
}

PythonStatus PythonParser::status() const
{
    QReadLocker locker(const_cast<QReadWriteLock*>(&statlock));
    return stat;
}

void PythonParser::setStatus(PythonStatus st)
{
    QWriteLocker locker(const_cast<QReadWriteLock*>(&statlock));
    stat = st;
}

void PythonParser::pause()
{
    //just stop the timer! (and disconnect the signal)
    timer->stop();
    disconnect(timer,SIGNAL(timeout()),this,SLOT(checkQueue()));
}

void PythonParser::play()
{
    //reconnect the signal
    connect(timer,SIGNAL(timeout()),this,SLOT(checkQueue()));
    //restart the timer
    timer->start(0);
}
