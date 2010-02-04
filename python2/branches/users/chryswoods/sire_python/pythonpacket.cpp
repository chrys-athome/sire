/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

/////////////
///////////// This code is based heavily on the instructions provided
///////////// in Linux Journal (http://www.linuxjournal.com/article/3641)
/////////////
/////////////       Thanks :-)
/////////////

#include <Python.h>

#include <QFile>
#include <QTextStream>

#include "pythonpacket.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

#include <QDebug>

using namespace SireCluster;
using namespace Siren;

static const RegisterObject<PythonPacket> r_pypacket;

/** Small RAII class to hold a Python thread state object */
class PyThreadStateHolder
{
public:
    PyThreadStateHolder() : my_state(0), is_locked(false)
    {
        //get the global lock
        PyEval_AcquireLock();
        
        is_locked = true;
 
        //create a thread state object for this thread
        my_state = Py_NewInterpreter();

        // swap in my thread state
        PyThreadState_Swap(my_state);
    }

    ~PyThreadStateHolder()
    {
        if (my_state)
        {
            if (not is_locked)
                // grab the lock
                PyEval_AcquireLock();
            
            // make sure the thread state is swapped out of the interpreter
            PyThreadState_Swap(my_state);
        
            // delete my thread state object
            Py_EndInterpreter(my_state);
            
            // release the lock
            PyEval_ReleaseLock();
        }
        else if (is_locked)
        {
            PyEval_ReleaseLock();
            is_locked = false;
        }
    }

private:
    /** Pointer to the thread-local state of the python interpreter */
    PyThreadState *my_state;
    
    /** Whether or not we hold the python mutex */
    bool is_locked;
};

/** Null constructor */
PythonPacket::PythonPacket() : Implements<PythonPacket,WorkPacket>()
{}

/** Construct a PythonPacket that executes the python script 
    from the file 'filename' - note that this reads the contents
    of this file now - so any changes that are made to the file
    between constructing this PythonPacket and running this
    packet are ignored */
PythonPacket::PythonPacket(const QString &filename)
             : Implements<PythonPacket,WorkPacket>()
{
    QFile f(filename);

    if (not f.open( QIODevice::ReadOnly ))
    {
        throw Siren::file_error(f, CODELOC);
    }

    //read the entire file
    QTextStream ts(&f);

    script_contents = ts.readAll();
}

/** Copy constructor */
PythonPacket::PythonPacket(const PythonPacket &other)
             : Implements<PythonPacket,WorkPacket>(other),
               script_contents(other.script_contents)
{}

/** Destructor */
PythonPacket::~PythonPacket()
{}

/** Copy assignment operator */
PythonPacket& PythonPacket::operator=(const PythonPacket &other)
{
    if (this != &other)
    {
        script_contents = other.script_contents;
        super::operator=(other);
    }

    return *this;
}

/** Comparison operator */
bool PythonPacket::operator==(const PythonPacket &other) const
{
    return script_contents == other.script_contents and
           super::operator==(other);
}

/** Comparison operator */
bool PythonPacket::operator!=(const PythonPacket &other) const
{
    return not PythonPacket::operator==(other);
}

uint PythonPacket::hashCode() const
{
    return qHash(PythonPacket::typeName()) + qHash(script_contents);
}

QString PythonPacket::toString() const
{
    if (script_contents.isEmpty())
        return QObject::tr("PythonPacket::null");

    return QObject::tr("#PythonPacket:\n%1").arg(script_contents);
}

void PythonPacket::stream(Stream &s)
{
    s.assertVersion<PythonPacket>(1);

    Schema schema = s.item<PythonPacket>();

    schema.data("script_contents") & script_contents;

    super::stream( schema.base() );
}

/** Return whether or not the packet has finished */
bool PythonPacket::hasFinished() const
{
    return script_contents.isEmpty();
}

/** Run the script */
WorkPacketPtr PythonPacket::chunk() const
{
    if (script_contents.isEmpty())
        return *this;

    //create a Python thread to run this code
    PyThreadStateHolder pythread;
    
    QByteArray argv_string = "sire_python";

    char *argv = argv_string.data();

    PySys_SetArgv(0, &argv);

    //we are now free to run our code
    PyRun_SimpleString( script_contents.toAscii().constData() );
    
    PythonPacket next_state(*this);
    next_state.setProgress(100);
    next_state.script_contents.clear();
    
    return next_state;
}
