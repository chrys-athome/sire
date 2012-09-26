
#include <QGraphicsWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QSettings>

#include "Conspire/GUI/mainwindow.h"
#include "Conspire/GUI/workpacketwidget.h"
#include "Conspire/GUI/chooseclasspage.h"

#include "Conspire/GUI/exceptionpage.h"
#include "Conspire/GUI/configdocument.h"
#include "Conspire/GUI/workstorepage.h"

#include "Conspire/GUI/uploadthread.h"
#include "Conspire/GUI/downloadthread.h"
#include "Conspire/GUI/global_var.h"

#include "Acquire/acquire_client.h"

#define PIE_RADIUS 48.

using namespace Conspire;

WorkPacketWidget::WorkPacketWidget(const char *message, int col, int row,
   int iidx, QList<QGraphicsLayoutItem *> *iall_wpw, const char *iquuid,
   QGraphicsItem *parent, ::Qt::WindowFlags wFlags) : QGraphicsWidget(parent, wFlags)
{
   setFlag(QGraphicsItem::ItemStacksBehindParent, false);
   setFlag(QGraphicsItem::ItemIsSelectable, true);
   setFlag(QGraphicsItem::ItemIsFocusable, true);
   my_message = QString(message);
   my_width = 2*PIE_RADIUS;
   my_height = 2*PIE_RADIUS;
   all_wpw = iall_wpw;
   my_idx = iidx;
   setGeometry(col*my_width, row*my_height, my_width, my_height);
   if (iquuid)
      quuid = strdup(iquuid);
   else
      quuid = NULL;
   local_to_broker = 0.;
   broker_to_compute = 0.;
   computing = 0.;
   compute_to_broker = 0.;
   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   workstoreid = qsetter->value(QString(quuid) + "/workstoreid").toString();
   delete qsetter;
   connect(this, SIGNAL(clicked()), this, SLOT(modifyWork()));
}

void WorkPacketWidget::modifyWork()
{
   if (my_message == QString("Create new..."))
   {
      const char *hadclusters = AcquireListOfAccessibleClusters();
      if ((hadclusters == NULL) || (strlen(hadclusters) == 0))
      {
         conspireDebug() << "ERROR";
         emit( push( PagePointer( new ExceptionPage(
               Conspire::tr("Please add some accessible clusters before creating work"),
                           Conspire::file_error( Conspire::tr("Cannot make new work."), CODELOC ) ) ) ) );
         return;
      }
      emit( push( PagePointer(new ChooseClassPage())) );
   } else
   {
      emit( push( PagePointer(new WorkStorePage(workstoreid))) );
   }
}

void WorkPacketWidget::computeAndUpdateUpload()
{
   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   int uploadComplete = qsetter->value(QString(quuid) + "/uploadComplete").toInt();
   workstoreid = qsetter->value(QString(quuid) + "/workstoreid").toString();
   delete qsetter;
   if (uploadComplete)
   {
      local_to_broker = 1.;
   } else
   {
       if (GetUploadArray()->value(workstoreid))
       {
          if (GetUploadArray()->value(workstoreid)->isFinished())
          {
             local_to_broker = 1.;
          } else
          {
             local_to_broker = GetUploadArray()->value(workstoreid)->getBlockFraction();
             local_to_broker = (local_to_broker < 0.) ? 0. : ((local_to_broker > 1.) ? 1. : local_to_broker);
          }
       } else
       {
          local_to_broker = 0.;
       }
   }
}

void WorkPacketWidget::updateAmounts(float ibroker_to_compute,
                                     float icomputing,
                                     float icompute_to_broker)
{
   computeAndUpdateUpload();
   broker_to_compute = ibroker_to_compute;
   computing = icomputing;
   compute_to_broker = icompute_to_broker;
}

void WorkPacketWidget::updateAmounts()
{
   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   workstoreid = qsetter->value(QString(quuid) + "/workstoreid").toString();
   delete qsetter;
   computeAndUpdateUpload();
   char **store_ids = NULL;
   float *pct_b2c = NULL;
   float *pct_wrk = NULL;
   float *pct_c2b = NULL;
   int noofws = 0;
   int64_t bytesfree = 0;
   int retval = AcquireQueryAllWorkStatus(&store_ids, &pct_b2c, &pct_wrk, &pct_c2b, &noofws, &bytesfree);
   if ((retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS) || (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS_NO_WORK))
   {
      for (int i = 0; i < noofws; i++)
      {
         if (strcmp(store_ids[i], workstoreid.toAscii().constData()) == 0)
         {
            broker_to_compute = pct_b2c[i] / 100.;
            computing = pct_wrk[i] / 100.;
            compute_to_broker = pct_c2b[i] / 100.;
            break;
         }
      }
   }
}

void WorkPacketWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   int alpha = 255;
//   QPoint a = QCursor::pos();
//   QPointF ab = mapFromScene(mview->mapToScene(mview->mapFromGlobal(a)));
   painter->setRenderHint(QPainter::Antialiasing, true);

   QPen p = painter->pen();
   p.setColor(QColor(128, 128, 128, alpha));
   painter->setPen(p);
   painter->setBrush(QColor(0, 0, 0, 0));
   painter->drawChord(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                    16 * 0, 16 * 360);
   
   if (local_to_broker == 0.) goto addtext;
   
   painter->setPen(QColor(128, 64, 64, 255));
   painter->setBrush(QColor(128, 64, 64, 255));
   if (local_to_broker == 1.)
   {
      painter->drawChord(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                       16 * 0, (float)(16 * 360) * local_to_broker);
   } else
   {
      painter->drawPie(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                       16 * 90, -(float)(16 * 360) * local_to_broker);
   }

   if (broker_to_compute == 0.) goto addtext;

   painter->setPen(QColor(128, 128, 64, 255));
   painter->setBrush(QColor(128, 128, 64, 255));
   if (broker_to_compute == 1.)
   {
      painter->drawChord(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                       16 * 0, (float)(16 * 360) * broker_to_compute);
   } else
   {
      painter->drawPie(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                       16 * 90, -(float)(16 * 360) * broker_to_compute);
   }

   if (computing == 0.) goto addtext;

   painter->setPen(QColor(96, 192, 96, 255));
   painter->setBrush(QColor(96, 192, 96, 255));
   if (computing == 1.)
   {
      painter->drawChord(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                       16 * 0, (float)(16 * 360) * computing);
   } else
   {
      painter->drawPie(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                       16 * 90, -(float)(16 * 360) * computing);
   }

   if (compute_to_broker == 0.) goto addtext;
   
   painter->setPen(QColor(64, 128, 128, 255));
   painter->setBrush(QColor(64, 128, 128, 255));
   if (compute_to_broker == 1.)
   {
      painter->drawChord(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                     16 * 0, (float)(16 * 360) * compute_to_broker);
   } else
   {
      painter->drawPie(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                     16 * 90, -(float)(16 * 360) * compute_to_broker);
   }
   
addtext:
   QFont font("Times", 12);
   font.setStyleStrategy(QFont::ForceOutline);
   painter->setFont(font);
   painter->setPen(QColor(0, 0, 0, 255));
   painter->setBrush(QColor(0, 0, 0, 255));
   painter->drawText(0, 40, 100, 20, ::Qt::AlignHCenter, my_message, NULL);
   update();
}

QString WorkPacketWidget::getWorkStoreID()
{
   return workstoreid;
}

void WorkPacketWidget::updateText(QString ttext)
{
   my_message = ttext;
}

void WorkPacketWidget::updateUUID(QString iquuid)
{
   if (quuid)
   {
      free(quuid);
      quuid = NULL;
   }
   quuid = strdup(iquuid.toAscii().constData());
}

void WorkPacketWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   /*
   if (event->button() == ::Qt::LeftButton)
   {
      printf("left button release\n");
   }
   */
}

void WorkPacketWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   /*
   if (event->button() == ::Qt::RightButton)
   {
      printf("right button press\n");
   }
   */
   
   
   if (event->button() == ::Qt::LeftButton)
   {
      const QRectF widg_geom = this->geometry();
      if (widg_geom.contains(event->scenePos()))
      {
         modifyWork();
      } else
      {
         int nextidx = (my_idx + 1) % (all_wpw->size());
         ((WorkPacketWidget *)(all_wpw->at(nextidx)))->mousePressEvent(event);
      }

      //printf("left button press\n");
      //emit ( clicked() );
   }
}

