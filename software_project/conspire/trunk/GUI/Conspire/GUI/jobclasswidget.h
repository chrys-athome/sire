
#ifndef __JOB_CLASS_WIDGET_H
#define __JOB_CLASS_WIDGET_H

#include <QtGui/QGraphicsWidget>
#include <QtGui/QGraphicsLayoutItem>
#include <QtGui/QGraphicsSceneMouseEvent>

#include "Conspire/GUI/page.h"

class QString;

namespace Conspire
{

class JobClassWidget : public QGraphicsWidget
{
   Q_OBJECT
   public:
      JobClassWidget(int row, QList<QGraphicsLayoutItem *> *iall_jcw,
                     QString iid, QString ifullname, QString ifulldescr, QString iiconpath,
                     QString ijobdir, QString ijoboptions, QStringList ijoboptionsincludedirs,
                     QString iquuid,
                     QGraphicsItem *parent = 0, ::Qt::WindowFlags wFlags = 0);
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
   signals:
      void clicked();
      void push(PagePointer new_page);
   protected slots:
      void newJob();
   protected:
      virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
      virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
   private:
      QString id;
      QString fullname;
      QString fulldescr;
      QString iconpath;
      QString jobdir;
      QString joboptions;
      QString quuid;
      QStringList joboptionsincludedirs;
      QImage theicon;
      QList<QGraphicsLayoutItem *> *all_jcw;
      int my_width;
      int my_height;
      int my_row;
};

}

#endif
