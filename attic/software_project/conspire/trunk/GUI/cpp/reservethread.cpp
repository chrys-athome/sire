
#include <string.h>
#include "Conspire/GUI/reservethread.h"
#include "Acquire/acquire_client.h"

#include <QDir>
#include <QFileInfo>

int64_t ReserveThread::totalOfDirectory(const char *directoryname)
{
   int64_t sofartot = 0;
   QDir *tdir = new QDir(directoryname);
   QFileInfoList fList = tdir->entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files, QDir::DirsFirst);
   for (int i = 0; i < fList.size(); i++)
   {
      QFileInfo file = fList.at(i);
      sofartot += (file.isDir()) ? totalOfDirectory(file.absoluteFilePath().toUtf8().constData()) : file.size();
   }
   delete tdir;
   return sofartot;
}

ReserveThread::ReserveThread(const char *ixmldescr, const char *iworkdirectory)
{
   if (ixmldescr)
      xmldescr = strdup(ixmldescr);
   else
      xmldescr = NULL;
   if (iworkdirectory)
      workdirectory = strdup(iworkdirectory);
   else
      workdirectory = NULL;
   directorysize = (iworkdirectory) ? totalOfDirectory(iworkdirectory) : 0;
   // The block size - this should probably be changed to reflect a default block size in the headers
   totalblocks = (directorysize / 1048576) + 1;
   currblock = 0;
   cancel = 0;
   finished = 0;
   remotesize = 0;
   store_id = NULL;
   setAutoDelete(false);
}

ReserveThread::~ReserveThread()
{
   if (cancel)
   {
      if (store_id)
      {
         int retval = AcquireDeleteWorkStore(store_id);
         if (retval != ACQUIRE_DELETE_WORK_STORE__SUCCESS)
         {
            printf("there was an error deleting the cancelled work store reserve process!\n");
         }
      }
   }
   if (xmldescr) free(xmldescr);
   xmldescr = NULL;
   if (workdirectory) free(workdirectory);
   workdirectory = NULL;
}

void ReserveThread::run()
{
   store_id = AcquireReserveWorkStore(xmldescr, workdirectory, &remotesize, &currblock, &cancel);
   finished = 1;
}

void ReserveThread::cancelReserve()
{
   cancel = 1;
}

int ReserveThread::getCurrentBlock()
{
   return currblock;
}

int ReserveThread::getTotalBlocks()
{
   return totalblocks;
}

float ReserveThread::getBlockFraction()
{
   return ((totalblocks == 0) ? 0. : (currblock / ((float)totalblocks)));
}

int ReserveThread::isFinished()
{
   return finished;
}

const char *ReserveThread::getFinalID()
{
   return store_id;
}

int64_t ReserveThread::getLocalUsedSpace()
{
   return directorysize;
}

int64_t ReserveThread::getRemoteUsedSpace()
{
   return remotesize;
}
