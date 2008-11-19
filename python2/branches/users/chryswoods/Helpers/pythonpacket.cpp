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

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireStream;

static const RegisterMetaType<PythonPacket> r_pypacket;

/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds, const PythonPacket &pypacket)
{
    writeHeader(ds, r_pypacket, 1);

    SharedDataStream sds(ds);

    sds << pypacket.script_contents
        << static_cast<const WorkPacketBase&>(pypacket);

    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds, PythonPacket &pypacket)
{
    VersionID v = readHeader(ds, r_pypacket);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> pypacket.script_contents 
            >> static_cast<WorkPacketBase&>(pypacket);
    }
    else
        throw version_error(v, "1", r_pypacket, CODELOC);

    return ds;
}

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
PythonPacket::PythonPacket() : WorkPacketBase()
{}

/** Construct a PythonPacket that executes the python script 
    from the file 'filename' - note that this reads the contents
    of this file now - so any changes that are made to the file
    between constructing this PythonPacket and running this
    packet are ignored */
PythonPacket::PythonPacket(const QString &filename)
             : WorkPacketBase()
{
    QFile f(filename);

    if (not f.open( QIODevice::ReadOnly ))
    {
        throw SireError::file_error(f, CODELOC);
    }

    //read the entire file
    QTextStream ts(&f);

    script_contents = ts.readAll();
}

/** Copy constructor */
PythonPacket::PythonPacket(const PythonPacket &other)
             : WorkPacketBase(other),
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
        WorkPacketBase::operator=(other);
    }

    return *this;
}

/** Return whether or not the packet has finished */
bool PythonPacket::hasFinished() const
{
    return script_contents.isEmpty();
}

/** Run the script */
float PythonPacket::chunk()
{
    if (script_contents.isEmpty())
        return 1;

    //create a Python thread to run this code
    PyThreadStateHolder pythread;
    
    //we are now free to run our code
    qDebug() << "Running the script...";
    PyRun_SimpleString( script_contents.toAscii().constData() );
    qDebug() << "Script has finished";

    return 1;
}
