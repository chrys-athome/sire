
#include <stdio.h>
#include <string.h>
#include <QProcess>

#include "Conspire/GUI/downloadthread.h"
#include "Conspire/GUI/reservethread.h"
#include "Acquire/acquire_client.h"

DownloadThread::DownloadThread(const char *istore_id, const char *iresults_id, const char *ioutputdirectory)
{
   if (istore_id)
      store_id = strdup(istore_id);
   else
      store_id = NULL;
   if (iresults_id)
      results_id = strdup(iresults_id);
   else
      results_id = NULL;
   if (ioutputdirectory)
      outputdirectory = strdup(ioutputdirectory);
   else
      outputdirectory = NULL;
   
   cancel = 0;
   finished = 0;
   currblock = 0;
   setAutoDelete(false);
}

DownloadThread::~DownloadThread()
{
   if (store_id) free(store_id);
   store_id = NULL;
   if (results_id) free(results_id);
   results_id = NULL;
   if (outputdirectory) free(outputdirectory);
   outputdirectory = NULL;
}

void DownloadThread::run()
{
   AcquireDownloadFromWorkStore(store_id, results_id, outputdirectory, &currblock, &totalblocks, &cancel);
   finished = 1;
   printf("total size of results directory: %d\n", ReserveThread::totalOfDirectory(outputdirectory));

#ifdef Q_WS_MAC
   QProcess proc2;
   proc2.start("open", QStringList() << outputdirectory);
   proc2.waitForFinished(-1);
#endif
#ifdef Q_WS_X11
   QProcess proc2;
   proc2.start("xdg-open", QStringList() << outputdirectory);
   proc2.waitForFinished(-1);
#endif
}

int DownloadThread::getCurrentBlock()
{
   return currblock;
}

float DownloadThread::getBlockFraction()
{
   return ((totalblocks == 0) ? 0. : (currblock / ((float)totalblocks)));
}

void DownloadThread::cancelDownload()
{
   cancel = 1;
}

int DownloadThread::isFinished()
{
   return finished;
}
