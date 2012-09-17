
#include <stdio.h>
#include <string.h>
#include <QString>
#include <QStringList>
#include <QSettings>
#include "Conspire/GUI/uploadthread.h"
#include "Acquire/acquire_client.h"

UploadThread::UploadThread(const char *istore_id, const char *iinputdirectory, const char *iclusterid,
   const char *ipassword, int iinstances, int imaxtimesecs, int iblockstotal)
{
   if (istore_id)
      store_id = strdup(istore_id);
   else
      store_id = NULL;
   if (iinputdirectory)
      inputdirectory = strdup(iinputdirectory);
   else
      inputdirectory = NULL;
   if (iclusterid)
      clusterid = strdup(iclusterid);
   else
      clusterid = NULL;
   if (ipassword)
      password = strdup(ipassword);
   else
      password = NULL;
   maxtimesecs = imaxtimesecs;
   instances = iinstances;
   totalblocks = iblockstotal;
   cancel = 0;
   finished = 0;
   currblock = 0;
   setAutoDelete(false);
}

UploadThread::~UploadThread()
{
   if (store_id) free(store_id);
   store_id = NULL;
   if (inputdirectory) free(inputdirectory);
   inputdirectory = NULL;
   if (clusterid) free(clusterid);
   clusterid = NULL;
   if (password)
   {
      memset(password, 0, strlen(password));
      free(password);
   }
   password = NULL;
}

void UploadThread::run()
{
   AcquireUploadToWorkStore(store_id, inputdirectory, clusterid, password, instances, maxtimesecs, &currblock, &cancel);
   QString quuid;
   QString workstoreid = QString(store_id);
   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   QStringList groups = qsetter->childGroups();
   for (int i = 0; i < groups.size(); i++)
   {
      QString qstr = qsetter->value(groups.at(i) + "/workstoreid").toString();
      if (qstr == workstoreid)
      {
         quuid = groups.at(i);
      }
   }
   qsetter->setValue(quuid + "/uploadComplete", 1);
   delete qsetter;
   finished = 1;
}

int UploadThread::getCurrentBlock()
{
   return currblock;
}

float UploadThread::getBlockFraction()
{
   float fraction = ((totalblocks == 0) ? 0. : (currblock / ((float)totalblocks)));
   printf("upload block %d/%d, completion %2.2f\n", currblock, totalblocks, fraction * 100.);
   return fraction;
}

void UploadThread::cancelUpload()
{
   cancel = 1;
}

int UploadThread::isFinished()
{
   return finished;
}
