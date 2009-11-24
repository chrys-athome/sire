/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include <QVector>
#include <QPointer>
#include <QSharedMemory>
#include <QMutex>
#include <QProcess>

#include "sharedmemory.h"

#ifdef Q_OS_UNIX
    #include <signal.h>    // CONDITIONAL_INCLUDE
#endif

#include <QDebug>

typedef QVector< QPointer<QSharedMemory> > SharedMemoryRegistry;

Q_GLOBAL_STATIC( QMutex, registryMutex );
Q_GLOBAL_STATIC( SharedMemoryRegistry, sharedMemoryRegistry );

#ifdef Q_OS_UNIX

    volatile sig_atomic_t error_in_progress = 0;
     
    /** Signal handler called on fatal errors to ensure that
        any shared memory has been detached */
    void fatal_error_signal(int sig)
    {
        if (error_in_progress)
            raise(sig);
 
        error_in_progress = 1;

        // detach all shared memory
        SirenTest::detachAllSharedMemory();

        signal(sig, SIG_DFL);
        raise(sig);
    }
    
    void install_handler( int sig, struct sigaction *action )
    {
        sigemptyset( &(action->sa_mask) );
        action->sa_handler = fatal_error_signal;
        action->sa_flags = 0;
            
        if (sigaction(sig, action, 0) == -1)
        {
            qDebug() << "CANNOT SET THE HANDLER FOR SIGNAL" << sig;
            std::exit(-1);
        }
    }
#endif // Q_OS_UNIX
    
/** This code is called in 'abort' if an exception
    manages to escape! */
static void error_abort()
{
    SirenTest::detachAllSharedMemory();
}

typedef QList< QPointer<QProcess> > ProcessPointers;

Q_GLOBAL_STATIC( ProcessPointers, processPointers );

namespace SirenTest
{

/** This function installs signal handlers that will ensure that
    all shared memory is detached if this process exits uncleanly */
void SIREN_EXPORT installSignalHandlers()
{
    #ifdef Q_OS_UNIX
        //set a separate signal stack to be used
        // (so we can work around possible stack corruption)
        static stack_t signal_stack;

        if ((signal_stack.ss_sp = malloc(SIGSTKSZ)) == 0)
        {
            qDebug() << "CANNOT ALLOCATE SIGNAL STACK SPACE!";
            std::exit(-1);
        }
            
        signal_stack.ss_size = SIGSTKSZ;
        signal_stack.ss_flags = 0;
        
        if (sigaltstack(&signal_stack,(stack_t *)0) < 0)
        {
            qDebug() << "CANNOT SET THE SIGNAL STACK!";
            std::exit(-1);
        }

        static struct sigaction segv_action;
        static struct sigaction fpe_action;
        static struct sigaction term_action;
        static struct sigaction bus_action;
        static struct sigaction int_action;
        static struct sigaction abrt_action;

        ::install_handler(SIGSEGV, &segv_action);
        ::install_handler(SIGFPE, &fpe_action);
        ::install_handler(SIGTERM, &term_action);
        ::install_handler(SIGBUS, &bus_action);
        ::install_handler(SIGINT, &int_action);
        ::install_handler(SIGABRT, &abrt_action);
    #endif

    std::set_terminate( error_abort );
}

/** This function will ensure that all shared memory connections
    are detached - this is necessary on Unix as if we crash
    without detaching, then the shared memory is not released */
void SIREN_EXPORT detachAllSharedMemory()
{
    ProcessPointers *processes = processPointers();
    
    if (not processes->isEmpty())
    {
        for (ProcessPointers::const_iterator it = processes->constBegin();
             it != processes->constEnd();
             ++it)
        {
            if (not it->isNull())
            {
                if (it->data()->pid() != 0)
                {
                    //use SIGTERM on Unix as the child has a chance
                    //of handling this signal and cleaning up
                    it->data()->terminate();
                }
            }
        }
    }

    SharedMemoryRegistry *registry = sharedMemoryRegistry();
    
    if (not registry)
        return;
        
    const QPointer<QSharedMemory> *array = registry->constData();
        
    for (int i=0; i<registry->count(); ++i)
    {
        const QPointer<QSharedMemory> &pointer = array[i];
        
        if (not pointer.isNull())
        {
            pointer->unlock();
            pointer->detach();
        }
    }
}

/** Register a process that is running - this will be killed
    if this process exits */
void SIREN_EXPORT registerProcess(QProcess *process)
{
    processPointers()->append( QPointer<QProcess>(process) );
}

/** Register some shared memory - this will be detached if
    this process exits */
void SIREN_EXPORT registerSharedMemory(QSharedMemory *memory)
{
    sharedMemoryRegistry()->append( QPointer<QSharedMemory>(memory) );
}

}
