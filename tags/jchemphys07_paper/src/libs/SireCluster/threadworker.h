/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIRECLUSTER_THREADWORKER_H
#define SIRECLUSTER_THREADWORKER_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>

#include "processor.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

class SIRECLUSTER_EXPORT ThreadWorker : public WorkerBase, private QThread
{
public:
    ThreadWorker();

    ~ThreadWorker();

protected:
    virtual void calculate()=0;

    void runFG();
    void runBG();

    void waitUntilReady();

private:
    void run();

    /** Mutex that is used to control the running of
        the background thread of this processor */
    QMutex runmutex;

    /** Wait condition used to wait until we need to do something */
    QWaitCondition runwaiter;

    /** Whether or not to keep going */
    bool keepgoing;

    /** Whether or not a job still needs to start... */
    bool job_still_starting;
};

}

SIRE_END_HEADER

#endif
