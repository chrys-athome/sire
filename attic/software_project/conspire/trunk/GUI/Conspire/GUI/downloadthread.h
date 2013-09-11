

#ifndef __DOWNLOAD_THREAD_H
#define __DOWNLOAD_THREAD_H

#include <QRunnable>

class DownloadThread : public QRunnable
{
   public:
      DownloadThread(const char *istore_id, const char *iresults_id, const char *ioutputdirectory);
      ~DownloadThread();
      void run();
      int getCurrentBlock();
      float getBlockFraction();
      void cancelDownload();
      int isFinished();
   private:
      char *store_id;
      char *results_id;
      char *outputdirectory;
      
      int cancel;
      int finished;
      int currblock;
      int totalblocks;
};

#endif
