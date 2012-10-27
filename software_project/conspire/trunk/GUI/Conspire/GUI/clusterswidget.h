
#ifndef __CLUSTERS_WIDGET_H
#define __CLUSTERS_WIDGET_H

#include <QtGui/QGraphicsWidget>
#include <QtGui/QGraphicsLayoutItem>
#include <QtGui/QGraphicsSceneMouseEvent>

#include "Conspire/GUI/page.h"

class QString;

namespace Conspire
{

class ClustersWidget : public QGraphicsWidget
{
   Q_OBJECT
   public:
      ClustersWidget(int row, QList<QGraphicsLayoutItem *> *iall_clw,
                     QString iid, QString ifullname, QString ifulldescr, QString iiconpath,
                     QGraphicsItem *parent = 0, ::Qt::WindowFlags wFlags = 0);
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
   signals:
      void clicked();
      void push(PagePointer new_page);
   protected slots:
      void newClusterCredentials();
   protected:
      virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
      virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
   private:
      QString id;
      QString fullname;
      QString fulldescr;
      QString iconpath;
      QImage theicon;
      QList<QGraphicsLayoutItem *> *all_clw;
      int my_width;
      int my_height;
      int my_row;
};

}

#endif
