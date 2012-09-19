

#ifndef __RESERVE_THREAD_H
#define __RESERVE_THREAD_H

#include <QRunnable>

/*
int64_t spaceused = 0;
int blocks = 0;
const char *workstore = AcquireReserveWorkStore(xmldescr.toAscii().constData(), datadir.toAscii().constData(), &spaceused, &blocks);
*/

class ReserveThread : public QRunnable
{
   public:
      ReserveThread(const char *ixmldescr, const char *iworkdirectory);
      ~ReserveThread();
      static int64_t totalOfDirectory(const char *directoryname);
      void run();
      int getCurrentBlock();
      int getTotalBlocks();
      float getBlockFraction();
      int isFinished();
      const char *getFinalID();
      int64_t getLocalUsedSpace();
      int64_t getRemoteUsedSpace();
      void cancelReserve();
   private:
      char *xmldescr;
      char *workdirectory;
      const char *store_id;

      int64_t directorysize;
      int64_t remotesize;
      int finished;
      int cancel;
      int currblock;
      int totalblocks;
};

#endif
