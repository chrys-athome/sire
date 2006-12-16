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
