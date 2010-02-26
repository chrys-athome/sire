/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QUuid>

#include "thread.h"
#include "forages.h"

#include "Siren/errors.h"

#include <QDebug>

using namespace Siren;

Thread::Thread(QObject *parent) : QThread(parent), thread_is_starting(false)
{
    this->setTerminationEnabled(false);
}

Thread::Thread(QString name, QObject *parent)
       : QThread(parent), thread_name(name), thread_is_starting(false)
{
    this->setTerminationEnabled(false);
}

Thread::~Thread()
{
    if (not QThread::wait(10000))
    {
        //kill this thread by interupting for-ages
        Siren::end_for_ages(this);
        
        if (not QThread::wait(20000))
        {
            //still not dead - kill this thread
            QThread::setTerminationEnabled(true);
            QThread::terminate();
        }
    }
}

/** Start this thread - this blocks until the thread
    calls the 'signalStarted()' function - this ensures
    that the thread has fully initialised before control
    is resumed */
void Thread::start()
{
    MutexLocker lkr( &start_mutex );
    
    if (QThread::isRunning())
        return;
        
    thread_is_starting = true;
        
    QThread::start();
    
    start_waiter.wait( &start_mutex );
}

namespace Siren
{
    void SIREN_EXPORT init_rand(int ID)
    {
        #ifdef Q_OS_UNIX
            //QUuid *very annoyingly* calls qsrand using the current time.
            // THIS IS REALLY ANNOYING WHEN USING QUuid IN AN MPI PROGRAM!!!!

            //what is even more annoying, is that qsrand is a *per-thread*
            //seed, so this has only fixed this thread. Other threads
            //will use a seed of 1, so will all create the same sequence
            //of QUuids - the only way to fix this is for all new threads
            //to seed qsrand
    
            // However, it only calls qsrand on the first QUuid - I'll thus
            // call qsrand after the first...
            QUuid::createUuid();
        
            QFile f("/dev/urandom");
            
            if (f.open( QIODevice::ReadOnly ))
            {
                QDataStream ds;
                quint32 seed;
                ds >> seed;
        
                f.close();
        
                if (ID > 1 and ID < 10000)
                    qsrand(ID * seed);
                else
                    qsrand(seed);
            }
            else
            {
                throw Siren::unsupported( QObject::tr(
                        "/dev/urandom is not available on this platform!"), CODELOC );
            }

            //lets dispose of another QUuid while we're at it
            QUuid::createUuid();
        #endif
    }
}

void Thread::run()
{
    start_mutex.lock();

    int ID = register_this_thread();

    try
    {
        if (thread_name.isEmpty())
            setThreadString( QObject::tr("Thread{%1}").arg(ID) );
        else
            setThreadString( QString("%1{%2}").arg(thread_name).arg(ID) );

        Siren::init_rand(ID);
    
        this->threadMain();
    }
    catch(const Siren::exception &e)
    {
        printError(e.toString());
    }
    catch(const std::exception &e)
    {
        printError( std_exception(e).toString() );
    }
    catch(...)
    {
        printError( unknown_error( QObject::tr(
                        "Unknown error thrown in thread %1!").arg(ID), CODELOC )
                            .toString() );
    }
    
    unregister_this_thread();
    
    if (thread_is_starting)
    {
        thread_is_starting = false;
        start_waiter.wakeAll();
        start_mutex.unlock();
    }
}

/** Call this function to signal that this thread has now fully started */
void Thread::signalStarted()
{
    if (thread_is_starting)
    {
        thread_is_starting = false;
        start_waiter.wakeAll();
        start_mutex.unlock();
    }
}
