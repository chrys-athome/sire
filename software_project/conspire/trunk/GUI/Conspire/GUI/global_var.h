#ifndef __GLOBAL_VAR_FIX_H
#define __GLOBAL_VAR_FIX_H

class QVar;
class QString;
class UploadThread;
class DownloadThread;

QMap<QString, UploadThread *> *GetUploadArray();
QMap<QString, DownloadThread *> *GetDownloadArray();
void DestroyUploadArray();
void DestroyDownloadArray();

#endif
