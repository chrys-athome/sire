
#include <QGraphicsWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QSettings>

#include "Conspire/GUI/mainwindow.h"
#include "Conspire/GUI/jobclasswidget.h"

#include "Conspire/GUI/exceptionpage.h"
#include "Conspire/GUI/configdocument.h"
#include "Conspire/GUI/workstorepage.h"

#include "Acquire/acquire_client.h"

#define CLASSDISP_WIDTH 325.
#define CLASSDISP_HEIGHT 100.

using namespace Conspire;

static QString install_dir 
                = "/home/benlong/conspire/job_classes";

JobClassWidget::JobClassWidget(int row, QList<QGraphicsLayoutItem *> *iall_jcw,
                               QString iid, QString ifullname, QString ifulldescr, QString iiconpath,
                               QString ijobdir, QString ijoboptions, QStringList ijoboptionsincludedirs,
                               QGraphicsItem *parent, ::Qt::WindowFlags wFlags)
{
   setFlag(QGraphicsItem::ItemStacksBehindParent, false);
   setFocusPolicy(::Qt::NoFocus);
   id = QString(iid);
   fullname = QString(ifullname);
   fulldescr = QString(ifulldescr);
   iconpath = QString(iiconpath);
   jobdir = QString(ijobdir);
   joboptions = QString(ijoboptions);
   joboptionsincludedirs = QStringList(ijoboptionsincludedirs);
   theicon.load(QString("%1/%2").arg(install_dir).arg(iconpath));
   all_jcw = iall_jcw;
   
   my_row = row;
   my_width = CLASSDISP_WIDTH;
   my_height = CLASSDISP_HEIGHT;
   setContentsMargins(0, 0, my_width, my_height);
   setGeometry(0, my_height*row, my_width, my_height);
   QSizeF qrsize = QSizeF(my_width, my_height);
   setMinimumSize(qrsize);
   setMaximumSize(qrsize);
   setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   connect(this, SIGNAL(clicked()), this, SLOT(newJob()));
}

void JobClassWidget::newJob()
{
   QStringList path;
   path << QString("%1/%2").arg(install_dir).arg(jobdir);
   
   printf("joboptions %s\n", joboptions.toAscii().constData());
   Options opts = Options::fromXMLFile(QString("%1").arg(joboptions), path);
   conspireDebug() << "PUSH CONFIGDOC";
   emit( push( PagePointer(new ConfigDocument(jobdir, opts))) );
   conspireDebug() << "PUSHED!";
}

void JobClassWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   int alpha = 255;
//   QPoint a = QCursor::pos();
//   QPointF ab = mapFromScene(mview->mapToScene(mview->mapFromGlobal(a)));
   painter->setRenderHint(QPainter::Antialiasing, true);
   

   QPen p = painter->pen();
   p.setColor(QColor(128, 128, 128, alpha));
   painter->setPen(p);
   painter->setBrush(QColor(0, 0, 0, 0));
   painter->drawRect(0, 0, my_width, my_height);
   painter->drawImage(QRect(0, 0, my_height, my_height), theicon);
//   painter->drawChord(0, my_height - 2*PIE_RADIUS, 2*PIE_RADIUS, 2*PIE_RADIUS,
//                    16 * 0, 16 * 360);
   
   
   QFont qfont1("Times", 10);
   qfont1.setWeight(QFont::DemiBold);
//   qfont1.setStyleStrategy(QFont::ForceOutline);
   painter->setFont(qfont1);
   painter->setPen(QColor(0, 0, 0, 255));
   painter->setBrush(QColor(0, 0, 0, 255));
   QRectF trecto = QRectF(my_height + 5, 0, my_width - (my_height + 5), 20);
   painter->drawText(trecto, ::Qt::TextWordWrap | ::Qt::AlignLeft, fullname, &trecto);
   QFont qfont2("Times", 10);
//   qfont2.setStyleStrategy(QFont::ForceOutline);
   painter->setFont(qfont2);
   painter->setPen(QColor(0, 0, 0, 255));
   painter->setBrush(QColor(0, 0, 0, 255));
   QRectF trect = QRectF(my_height + 5, 20, my_width - (my_height + 5), my_height - 20);
   painter->drawText(trect, ::Qt::TextWordWrap | ::Qt::AlignLeft, fulldescr, &trect);
   setContentsMargins(0, 0, my_width, my_height);
//   setGeometry(0, 0, my_width, my_height);
   update();
}

void JobClassWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   /*
   if (event->button() == ::Qt::LeftButton)
   {
      printf("left button release\n");
   }
   */
}

void JobClassWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   printf("clicked %s (%fx%f)\n", fullname.toAscii().constData(), event->scenePos().x(), event->scenePos().y());
   /*
   if (event->button() == ::Qt::LeftButton)
   {
      printf("clicked %s (%fx%f)\n", fullname.toAscii().constData(), event->pos().x(), event->pos().y());
   }
   */
   if (event->button() == ::Qt::LeftButton)
   {
      const QRectF widg_geom = this->geometry();
      if (widg_geom.contains(event->scenePos()))
      {
         newJob();
      } else
      {
         int nextidx = (my_row + 1) % (all_jcw->size());
         ((JobClassWidget *)(all_jcw->at(nextidx)))->mousePressEvent(event);
      }
      setFocusPolicy(::Qt::NoFocus);
      //printf("left button press\n");
      //emit ( clicked() );
   }
}
