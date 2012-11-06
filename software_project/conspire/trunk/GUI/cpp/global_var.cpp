
#include <QMap>
#include <QString>
#include "Conspire/GUI/uploadthread.h"
#include "Conspire/GUI/downloadthread.h"

static QMap<QString, UploadThread *> *uploadarray = NULL;
static QMap<QString, DownloadThread *> *downloadarray = NULL;

QMap<QString, UploadThread *> *GetUploadArray()
{
   if (!(uploadarray))
   {
      uploadarray = new QMap<QString, UploadThread *>();
   }
   return uploadarray;
}

QMap<QString, DownloadThread *> *GetDownloadArray()
{
   if (!(downloadarray))
   {
      downloadarray = new QMap<QString, DownloadThread *>();
   }
   return downloadarray;
}

void DestroyUploadArray()
{
   delete uploadarray;
   uploadarray = NULL;
}

void DestroyDownloadArray()
{
   delete downloadarray;
   downloadarray = NULL;
}
