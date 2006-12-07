
#include <QDebug>

#include "threadworker.h"

using namespace SireCluster;

/** Constructor */
ThreadWorker::ThreadWorker()
             : WorkerBase(), QThread(),
               runmutex(QMutex::NonRecursive),
               keepgoing(true), job_still_starting(false)
{
    //start the background thread...

    //lock the run mutex...
    QMutexLocker lkr(&runmutex);

    //start the background thread - this will block
    //on the runmutex
    this->start();

    //sleep on the runwaiter - this will unblock the
    //started thread
    runwaiter.wait(&runmutex);

    //the running thread will have woken us up, and will have
    //released the runmutex when it entered its main run loop

    //at this point we can now be sure that the background thread
    //is waiting on runwaiter in its main loop
}

/** Destructor */
ThreadWorker::~ThreadWorker()
{
    //we need to terminate the thread
    QMutexLocker lkr(&runmutex);

    //can now set keepgoing to false as the main thread is not running
    keepgoing = false;

    //wake the main thread - it will block as we are holding runmutex
    runwaiter.wakeOne();

    //wait on runmutex - this will block until the background thread
    //exits
    runwaiter.wait(&runmutex);

    //the background thread should now have finished

    //now just wait until the thread has exited, just to be sure...
    QThread::wait();
}

/** What is run by the background thread... */
void ThreadWorker::run()
{
    qDebug() << "ThreadWorker::run()";

    try
    {
        //lock the run mutex..
        QMutexLocker lkr(&runmutex);

        //we want to keep going (for the moment)
        keepgoing = true;

        //wake a thread waiting for me...
        runwaiter.wakeOne();

        do
        {
            //wait until we are asked to do something...
            runwaiter.wait(&runmutex);

            //ok, the job has now started
            job_still_starting = false;

            if (not keepgoing)
                break;

            qDebug() << "Loop!";
            this->calculate();

            //wake a thread waiting for us to finish doing something
            runwaiter.wakeOne();

        } while( true );
    }
    catch(...)
    {}

    //wake all threads that are waiting, to prevent deadlock
    runwaiter.wakeAll();

    qDebug() << "ThreadWorker::end of run()";
}

/** Run the job in the background */
void ThreadWorker::runBG()
{
    qDebug() << "ThreadWorker::runBG()";

    //lock the runmutex - this blocks until the
    //background thread is ready to perform a calculation
    QMutexLocker lkr(&runmutex);

    //does a job still need to start?
    if (job_still_starting)
    {
        //we need to wait until that last job has finished...
        runwaiter.wait(&runmutex);
    }

    //ok, record that the job is now starting
    job_still_starting = true;

    //wake the background thread - the background thread will
    //block until we release the runmutex
    runwaiter.wakeOne();

    qDebug() << "....done!";
}

/** Run the job in the foreground - this will block until the
    job has finished */
void ThreadWorker::runFG()
{
    this->calculate();
}

/** Wait until the job has finished - this blocks until the
    background thread is no longer processing a calculation */
void ThreadWorker::waitUntilReady()
{
    QMutexLocker lkr(&runmutex);

    if (job_still_starting)
    {
        //ok, the job is still waiting to start, we need to wait for it...
        runwaiter.wait(&runmutex);
    }
}
