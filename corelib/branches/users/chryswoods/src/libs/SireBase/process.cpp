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

#include <QList>
#include <QTime>

#include "process.h"

#include "Siren/mutex.h"
#include "Siren/hanref.h"
#include "Siren/errors.h"

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <QDebug>

#ifndef Q_OS_UNIX
    #error This file can only be compiled on a UNIX-like machine!
#endif

using namespace SireBase;
using namespace Siren;

namespace SireBase
{
namespace detail
{

/** Private implementation of Process */
class ProcessData
{
public:
    ProcessData() : pid(-1), is_running(false), is_error(false), was_killed(false)
    {}
    
    ~ProcessData()
    {}

    /** The command being run */
    QString command;
    
    /** The arguments to the command */
    QStringList arguments;
    
    /** The process ID of the process */
    pid_t pid;
   
    /** Whether or not the process is running */
    bool is_running;
    
    /** Whether or not the job exited with an error */
    bool is_error;
    
    /** Whether or not the job was killed */
    bool was_killed;
};

} // end of namespace detail
} // end of namespace SireBase

using namespace SireBase::detail;

Q_GLOBAL_STATIC( QList<WeakHandle>, processRegistry );
Q_GLOBAL_STATIC( Mutex, registryMutex );

static const RegisterHandle<Process> r_process;

/** Null constructor */
Process::Process() : ImplementsHandle< Process, Handles<ProcessData> >()
{}

/** Copy constructor */
Process::Process(const Process &other)
        : ImplementsHandle< Process, Handles<ProcessData> >(other)
{}

/** Destructor */
Process::~Process()
{
    if (this->unique())
    {
        this->kill();
    }
}

uint Process::hashCode() const
{
    if (isNull())
        return qHash( Process::typeName() );
    else
        return qHash( Process::typeName() ) + qHash( resource().command );
}

QString Process::toString() const
{
    if (isNull())
        return QObject::tr("Process::null");
   
    else if (const_cast<Process*>(this)->isRunning())
        return QObject::tr("Process( %1 [running] )")
                    .arg( resource().command );

    else if (const_cast<Process*>(this)->wasKilled())
        return QObject::tr("Process( %1 [killed!] )")
                    .arg( resource().command );

    else if (const_cast<Process*>(this)->isError())
        return QObject::tr("Process( %1 [error!] )")
                    .arg( resource().command );

    else
        return QObject::tr("Process( %1 [finished] )")
                    .arg( resource().command );
}

/** Copy assignment operator */
Process& Process::operator=(const Process &other)
{
    Handles<ProcessData>::operator=(other);
    return *this;
}

/** Comparison operator */
bool Process::operator==(const Process &other) const
{
    return Handles<ProcessData>::operator==(other);
}

/** Comparison operator */
bool Process::operator!=(const Process &other) const
{
    return not Process::operator==(other);
}

/** Internal function used to clean up a running job that has 
    just finished - only call this function if you are
    holding the lock */
void Process::cleanUpJob(int status, int child_exit_status)
{
    if (isNull())
        return;
        
    if (WEXITSTATUS(child_exit_status) != 0)
    {
        //something went wrong with the job
        resource().is_error = true;
    }

    if (WIFSIGNALED(child_exit_status))
    {
        if (WTERMSIG(child_exit_status) == SIGKILL or
            WTERMSIG(child_exit_status) == SIGHUP)
        {
            resource().was_killed = true;
            resource().is_error = true;
        }
        else
        {
            resource().was_killed = false;
            resource().is_error = true;
        }
    }
    else if (WIFSTOPPED(child_exit_status))
    {
        if (WSTOPSIG(child_exit_status) == SIGKILL or
            WSTOPSIG(child_exit_status) == SIGHUP)
        {
            resource().was_killed = true;
            resource().is_error = true;
        }
        else
        {
            resource().was_killed = false;
            resource().is_error = true;
        }
    }
    
    //make sure that all of the child processes have finished
    //by killing the child's process group
    killpg(resource().pid, SIGKILL);
    
    resource().is_running = false;
}

/** From the return value in 'child_exit_status' work out 
    and return whether or not the child process is still running */
static bool processRunning(int child_exit_status)
{
    if (WIFEXITED(child_exit_status) == 0)
    {
        if (WIFSIGNALED(child_exit_status) != 0 or
            WIFSTOPPED(child_exit_status) != 0)
        {
            //the job was killed or stopped by a signal
            return false;
        }
        else
            return true;
    }
    
    //the job has exited normally
    return false;
}

/** Wait until the process has finished */
void Process::wait()
{
    if (isNull())
        return;
        
    HandleLocker lkr(*this);
    
    if (not resource().is_running)
        return;
    
    int child_exit_status;
    int status = waitpid(resource().pid, &child_exit_status, 0);
    
    if (status == -1)
    {
        qDebug() << "waitpid exited with status -1!" << strerror(errno);
        return;
    }
    else if (status != resource().pid)
    {
        qDebug() << "waitpid exited with the wrong PID (" << status
                 << "vs." << resource().pid << ")" << strerror(errno);
        return;
    }
    
    if ( processRunning(child_exit_status) )
    {
        qDebug() << "child process has not finished!";
    }
    else
    {
        this->cleanUpJob(status, child_exit_status);
    }
}

/** Wait until the process has finished, or until 'ms' milliseconds have passed.
    This returns whether or not the process has finished */
bool Process::wait(int ms)
{
    if (isNull())
        return true;
        
    QTime t;
    t.start();
    
    if (this->tryLock(ms))
    {
        if (not resource().is_running)
        {
            this->unlock();
            return true;
        }
    
        try
        {
            int child_exit_status;
            int status;
        
            while (t.elapsed() < ms)
            {
                status = waitpid(resource().pid, &child_exit_status, WNOHANG);

                if (status == 0)
                {
                    //we can't get the status - the job may still be running
                    ::sleep(1);
                    continue;
                }
                else if (status == -1)
                {
                    qDebug() << "waitpid exited with status -1!" << strerror(errno);
                    return true;
                }
                else if (status != resource().pid)
                {
                    qDebug() << "waitpid exited with the wrong PID (" << status
                             << "vs." << resource().pid << ")" << strerror(errno);
                    return true;
                }
                
                if ( processRunning(child_exit_status) )
                {
                    //the job is still running - wait a 1s then
                    //try again
                    ::sleep(1);
                }
                else
                {
                    //the job has finished - process the finished job
                    this->cleanUpJob(status, child_exit_status);
                    this->unlock();
                    return true;
                }
            }
            
            this->unlock();
            return false;
        }
        catch(...)
        {
            this->unlock();
            return true;
        }
    }
    else
        return false;
}

/** Run the command 'command' with the arguments 'arguments', and
    return a Process object that can be used to query and control the 
    job */
Process Process::run(const QString &command,  const QStringList &arguments)
{
    //fork to run the command
    pid_t pid = fork();
    
    if (pid == -1)
    {
        throw Siren::unsupported( QObject::tr(
            "It is not possible to use fork on this platform. Running "
            "the external command \"%1\" is therefore not possible.")
                .arg(command), CODELOC );
    }
    else if (pid == 0)
    {
        //this is the child process!

        //move this child (and all of its children) into a new 
        //process ID group - the new group will have the same
        //process ID as this child
        setpgrp();
        
        QByteArray cmd = command.toAscii();
        QList<QByteArray> args;
        char* char_args[ arguments.count() + 2 ];
        
        char_args[0] = cmd.data();
        
        for (int i=0; i<arguments.count(); ++i)
        {
            args.append( arguments[i].toAscii() );
            char_args[i+1] = args[i].data();
        }
        
        char_args[arguments.count()+1] = '\0';
       
        //now run the command
        int status = execvp( char_args[0], char_args );
        
        if (status != 0)
        {
            qDebug() << "Problem starting the command" << command 
                     << "with arguments" << arguments.join(" ")
                     << ". Status ==" << status << ", error =" 
                     << strerror(errno);
                     
            exit(-1);
        }
        
        exit(0);
    }
    else
    {
        //parent
        Process p;
        
        p.setResource( new ProcessData() );
        p.resource().pid = pid;
        
        p.resource().is_running = true;

        p.resource().command = command;
        p.resource().arguments = arguments;

        //record this process in the list of running processes
        MutexLocker lkr( registryMutex() );
        processRegistry()->append( WeakHandle(p) );

        return p;
    }
    
    return Process();
}
           
/** Run the command 'command' and return a Process object that can be
    used to monitor the command */        
Process Process::run(const QString &command)
{
    return Process::run(command, QStringList());
}

/** Run the command 'command' with the solitary argument 'arg' */
Process Process::run(const QString &command, const QString &arg)
{
    QStringList args;
    args.append(arg);
    
    return Process::run(command, args);
}

/** Kill this process */
void Process::kill()
{
    if (isNull())
        return;
    
    //kill the job
    if (resource().is_running)
    {
        qDebug() << "Killing job " << resource().command.toAscii().constData() 
                 << resource().arguments.join(" ").toAscii().constData();
        killpg(resource().pid, SIGKILL);
    }
    
    //now wait for it to finish
    this->wait();
}

/** Use this function to kill all of the jobs that are currently running */
void Process::killAll()
{
    MutexLocker lkr( registryMutex() );
    
    QList<WeakHandle> &process_list = *(processRegistry());

    for (QList<WeakHandle>::iterator it = process_list.begin();
         it != process_list.end();
         ++it)
    {
        HanRef handle = it->lock();
        
        if (handle.isA<Process>())
            handle.asA<Process>().kill();
    }
    
    process_list.clear();
}

/** Return whether or not the job is running */
bool Process::isRunning()
{
    if (isNull())
        return false;
        
    HandleLocker lkr(*this);
    return resource().is_running;
}

/** Return whether or not this process has finished running */
bool Process::hasFinished()
{
    if (isNull())
        return true;
        
    HandleLocker lkr(*this);
    return not resource().is_running;
}

/** Return whether or not the process exited in error */
bool Process::isError()
{
    if (isNull())
        return false;

    HandleLocker lkr(*this);
    return resource().is_error;
}

/** Return whether or not the process was killed */
bool Process::wasKilled()
{
    if (isNull())
        return false;

    HandleLocker lkr(*this);
    return resource().was_killed;
}
