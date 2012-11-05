
#ifndef __WORK_PACKET_WIDGET_H
#define __WORK_PACKET_WIDGET_H

#include <QList>
#include <QtGui/QGraphicsWidget>
#include <QtGui/QGraphicsLayoutItem>
#include <QtGui/QGraphicsSceneMouseEvent>

#include "Conspire/GUI/page.h"

class QString;

namespace Conspire
{

class WorkPacketWidget : public QGraphicsWidget
//QGraphicsLayoutItem
{
   Q_OBJECT
   public:
      WorkPacketWidget(const char *message, int col, int row,
         int iidx, QList<QGraphicsLayoutItem *> *iall_wpw, const char *iquuid,
         QGraphicsItem *parent = 0, ::Qt::WindowFlags wFlags = 0);
      void computeAndUpdateUpload();
      void computeAndUpdateDownload();
      void updateAmounts();
      void updateAmounts(float ibroker_to_compute,
                         float icomputing,
                         float icompute_to_broker);
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
      QString getWorkStoreID();
      void updateText(QString ttext);
      void updateUUID(QString iquuid);
   signals:
      void clicked();
      void push(PagePointer new_page);
   protected slots:
      void modifyWork();
   protected:
      virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
      virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
   private:
      char *quuid;
      int my_width;
      int my_height;
      int my_idx;
      
      QList<QGraphicsLayoutItem *> *all_wpw;

      QString my_message;
      QString my_name;
      QImage theicon;

      float local_to_broker;
      float broker_to_compute;
      float computing;
      float compute_to_broker;
      float broker_to_local;
      
      QString workstoreid;
};

}

#endif
