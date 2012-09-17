
#ifndef __UPLOAD_THREAD_H
#define __UPLOAD_THREAD_H

#include <QRunnable>

class UploadThread : public QRunnable
{
   public:
      UploadThread(const char *istore_id, const char *iinputdirectory, const char *iclusterid,
         const char *ipassword, int iinstances, int imaxtimesecs, int iblockstotal);
      ~UploadThread();
      void run();
      int getCurrentBlock();
      float getBlockFraction();
      void cancelUpload();
      int isFinished();
   private:
      char *store_id;
      char *inputdirectory;
      char *clusterid;
      char *password;
      int instances;
      int maxtimesecs;
      
      int cancel;
      int finished;
      int currblock;
      int totalblocks;
};

#endif
