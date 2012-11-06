
#include <QGraphicsWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QSettings>
#include <QXmlStreamReader>

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

#include "config.h"

#define PIE_RADIUS 48.

using namespace Conspire;

static QString install_dir = STATIC_INSTALL_DIR;

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
   {
      quuid = strdup(iquuid);
      tryReloadImage();
   } else
      quuid = NULL;
   local_to_broker = 0.;
   broker_to_compute = 0.;
   computing = 0.;
   compute_to_broker = 0.;
   broker_to_local = 0.;
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
      emit( push( PagePointer(new ChooseClassPage(QString(), QString())), false) );
   } else
   {
      emit( push( PagePointer(new WorkStorePage(workstoreid, QString(quuid))), false) );
   }
}

void WorkPacketWidget::computeAndUpdateUpload()
{
   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   int uploadComplete = qsetter->value(QString(quuid) + "/uploadComplete").toInt();
   workstoreid = qsetter->value(QString(quuid) + "/workstoreid").toString();
   my_name = qsetter->value(QString(quuid) + "/jobname").toString();
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

void WorkPacketWidget::computeAndUpdateDownload()
{
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    workstoreid = qsetter->value(QString(quuid) + "/workstoreid").toString();
//    my_name = qsetter->value(QString(quuid) + "/jobname").toString();
    delete qsetter;
    if (GetDownloadArray()->value(workstoreid))
    {
        broker_to_local = GetDownloadArray()->value(workstoreid)->getBlockFraction();
        broker_to_local = (broker_to_local < 0.) ? 0. : ((broker_to_local > 1.) ? 1. : broker_to_local);
    } else
    {
        broker_to_local = 0.;
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
   computeAndUpdateDownload();
}

void WorkPacketWidget::updateAmounts()
{
   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   workstoreid = qsetter->value(QString(quuid) + "/workstoreid").toString();
   my_name = qsetter->value(QString(quuid) + "/jobname").toString();
   QString my_class = qsetter->value(QString(quuid) + "/jobclass").toString();
   delete qsetter;
   tryReloadImage();
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
    unsigned char manycolours[] = { 0xFF, 0xFF, 0xFF,
                                    0x7F, 0x3F, 0x3F,
                                    0x7F, 0x7F, 0x3F,
                                    0x5F, 0x7F, 0x5F,
                                    0x3F, 0x7F, 0x7F,
                                    0x5F, 0x5F, 0x7F,
                                    0x7F, 0x3F, 0x7F };
   int alpha = 192;
   int stage = 0;
   stage = (local_to_broker == 1.) ? 1 : stage;
   stage = (broker_to_compute == 1.) ? 2 : stage;
   stage = (computing == 1.) ? 3 : stage;
   stage = (compute_to_broker == 1.) ? 4 : stage;
   stage = (broker_to_local == 1.) ? 5 : stage;
   
   float stage_progress = 0.;
   stage_progress = (stage == 0) ? local_to_broker : stage_progress;
   stage_progress = (stage == 1) ? broker_to_compute : stage_progress;
   stage_progress = (stage == 2) ? computing : stage_progress;
   stage_progress = (stage == 3) ? compute_to_broker : stage_progress;
   stage_progress = (stage == 4) ? broker_to_local : stage_progress;
//   printf("%d %f\n", stage, stage_progress);
   painter->setRenderHint(QPainter::Antialiasing, true);
   // Draw the background image
   painter->drawImage(QRect(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS), theicon);
   // Draw the faint grey outline
   QPen p = painter->pen();
   p.setColor(QColor(128, 128, 128, alpha));
   painter->setPen(p);
   painter->setBrush(QColor(0, 0, 0, 0));
   painter->drawChord(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                    16 * 0, 16 * 360);
   if (stage_progress == 0.)
   {
      painter->setPen(QColor(manycolours[stage*3+0],
                             manycolours[stage*3+1],
                             manycolours[stage*3+2],
                             alpha));
      painter->setBrush(QColor(manycolours[stage*3+0],
                               manycolours[stage*3+1],
                               manycolours[stage*3+2],
                               alpha));
      painter->drawChord(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                       16 * 0, (float)(16 * 360));
   } else
   {
      painter->setPen(QColor(manycolours[stage*3+3],
                             manycolours[stage*3+4],
                             manycolours[stage*3+5],
                             alpha));
      painter->setBrush(QColor(manycolours[stage*3+3],
                               manycolours[stage*3+4],
                               manycolours[stage*3+5],
                               alpha));
      painter->drawPie(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                       16 * 90, -(float)(16 * 360) * stage_progress);
      painter->setPen(QColor(manycolours[stage*3+0],
                             manycolours[stage*3+1],
                             manycolours[stage*3+2],
                             alpha));
      painter->setBrush(QColor(manycolours[stage*3+0],
                               manycolours[stage*3+1],
                               manycolours[stage*3+2],
                               alpha));
      painter->drawPie(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
                       16 * 90 - (float)(16 * 360) * stage_progress, -(float)(16 * 360) * (1. - stage_progress));
   }
   
   
addtext:

   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   workstoreid = qsetter->value(QString(quuid) + "/workstoreid").toString();
   my_name = qsetter->value(QString(quuid) + "/jobname").toString();
   delete qsetter;
   int movedown = 0;
   if (not my_name.isEmpty())
   {
      movedown = 15;
      QFont font("Times", 12);
      font.setStyleStrategy(QFont::ForceOutline);
      painter->setFont(font);
      painter->setPen(QColor(0, 0, 0, 255));
      painter->setBrush(QColor(0, 0, 0, 255));
      painter->drawText(0, 40 - movedown, 100, 20, ::Qt::AlignHCenter, my_name, NULL);
   }
   if ((stage == 0) && (stage_progress > 0.)) my_message = QString("Uploading...");
   if (stage == 1) my_message = QString("Waiting...");
   if ((stage == 1) && (stage_progress > 0.)) my_message = QString("Sending...");
   if (stage == 2) my_message = QString("Starting...");
   if ((stage == 2) && (stage_progress > 0.)) my_message = QString("Computing...");
   if (stage == 3) my_message = QString("Computed.");
   if ((stage == 3) && (stage_progress > 0.)) my_message = QString("Receiving...");
   if (stage == 4) my_message = QString("Complete.");
   if ((stage == 4) && (stage_progress > 0.)) my_message = QString("Downloading...");
   if (stage == 5) my_message = QString("Downloaded.");
   QFont font("Times", 12);
   font.setStyleStrategy(QFont::ForceOutline);
   painter->setFont(font);
   painter->setPen(QColor(0, 0, 0, 255));
   painter->setBrush(QColor(0, 0, 0, 255));
   painter->drawText(0, 40 + movedown, 100, 20, ::Qt::AlignHCenter, my_message, NULL);
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

void WorkPacketWidget::tryReloadImage()
{
    // Load a new QImage using the appropriate job class
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QString jobclass = qsetter->value(QString(quuid) + "/jobclass").toString();
    delete qsetter;
    if ((not jobclass.isEmpty()) && (theicon.isNull()))
    {
        QFile *xmlFile = new QFile(QString("%1/job_classes/job_classes.xml").arg(install_dir));

        if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
        emit( push( PagePointer( new ExceptionPage(
                Conspire::tr("Error opening job classes XML file"),
                            Conspire::file_error( Conspire::tr("Cannot open the "
                            "file \"%1/%2\".")
                                .arg(install_dir).arg("job_classes.xml"), CODELOC ) ) ) ) );
        }

        QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);

        QString t_jobclassid;
        
        while (!xmlReader->atEnd() && !xmlReader->hasError())
        {
            QXmlStreamReader::TokenType token = xmlReader->readNext();
            if (token == QXmlStreamReader::StartDocument) continue;
            if (token == QXmlStreamReader::StartElement)
            {
                if (xmlReader->name() == "jobclassdescription")
                {
                    t_jobclassid = xmlReader->attributes().value("id").toString();
                }
                if (xmlReader->name() == "icon")
                {
                    if (t_jobclassid == jobclass)
                        theicon.load(QString("%1/job_classes/%2").arg(install_dir).arg(xmlReader->readElementText()));
                }
            }
        }

        if (xmlReader->hasError())
        {
        emit( push( PagePointer( new ExceptionPage(
                Conspire::tr("Error in XML parsing."),
                            Conspire::file_error( Conspire::tr("Cannot open the "
                            "file \"%1/%2\".")
                                .arg(install_dir).arg("job_classes.xml"), CODELOC ) ) ) ) );
        }

        xmlReader->clear();
        xmlFile->close();
        delete xmlReader;
        delete xmlFile;
    }
}

void WorkPacketWidget::updateUUID(QString iquuid)
{
   if (iquuid.isEmpty())
   {
       quuid = NULL;
       return;
   }
   if ((!quuid) || (iquuid != QString(quuid)))
   {
       tryReloadImage();
   }
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

void WorkPacketWidget::specialCountedMousePress(int idx, QGraphicsSceneMouseEvent *event)
{
   if (event->button() == ::Qt::LeftButton)
   {
      const QRectF widg_geom = this->geometry();
      if (widg_geom.contains(event->scenePos()))
      {
         modifyWork();
      }
      //printf("left button press\n");
      //emit ( clicked() );
   }
   if ((idx+1) >= all_wpw->size()) return;
   ((WorkPacketWidget *)(all_wpw->at(idx+1)))->specialCountedMousePress(idx+1, event);
}

void WorkPacketWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Should add a check to make this stop after a
    // certain number of jumps (so, all_wpw->size())
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
         ((WorkPacketWidget *)(all_wpw->at(0)))->specialCountedMousePress(0, event);
      }

      //printf("left button press\n");
      //emit ( clicked() );
   }
}
