/********************************************\
  *
  *  Conspire
  *
  *  Copyright (C) 2012  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include <math.h>
#include "Conspire/GUI/workstorepage.h"
#include "Conspire/GUI/workpacketwidget.h"
#include "Conspire/GUI/newworkpage.h"
#include "Conspire/GUI/optionspage.h"
#include "Conspire/GUI/widgetrack.h"
#include "Conspire/GUI/button.h"
#include "Conspire/GUI/configdocument.h"
#include "Conspire/GUI/exceptionpage.h"
#include "Conspire/GUI/widgetstack.h"

#include "Conspire/option.h"
#include "Conspire/values.h"

#include "Conspire/conspire.hpp"

#include <QCoreApplication>

#include <QPainter>
#include <QRectF>

#include <QProcess>
#include <QProcessEnvironment>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QRegExp>
#include <QComboBox>
#include <QProgressBar>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QSettings>
#include <QHeaderView>
#include <QGraphicsView>
#include <QGraphicsGridLayout>
#include <QTimer>

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;

inline QString getDataAmount(uint64_t data)
{
   double a = log10((double)data);
   int l = int(floor(a));
   switch (l)
   {
      case 0:
      case 1:
      case 2:
         return QString("%1 bytes").arg(QString::number(data));
      default: break;
   }
   const char ms[] = { 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y', '\0' };
   double data2 = data;
   int count = 0;
   while (ms[count] != '\0')
   {
      data2 = ((uint64_t)floor(data2)) / 1024.0;
      a = log10(data2);
      l = int(floor(a));
      switch (l)
      {
         case -1:
         case 0:
            return QString("%1%2").arg(QString::number(data2, 'f', 2)).arg(QString::fromAscii(&(ms[count]), 1));
         case 1:
            return QString("%1%2").arg(QString::number(data2, 'f', 1)).arg(QString::fromAscii(&(ms[count]), 1));
         case 2:
            return QString("%1%2").arg(QString::number(int(data2))).arg(QString::fromAscii(&(ms[count]), 1));
         default: break;
      }
      count++;
   }
   return QString("Lots of space");
}

void NewWorkPage::refreshWorkPartial()
{
    printf("refresh work triggered (partial)\n");

    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QStringList groups = qsetter->childGroups();
    int widgets_count_by_saves = groups.size();
    for (int i = 0; i < groups.size(); i++)
    {
       widgets_count_by_saves -= (int)(QString(groups.at(i)) == QString("preferences"));
       QString qstr = qsetter->value(groups.at(i) + "/user").toString();
       if (QString(groups.at(i)) != QString("preferences"))
          widgets_count_by_saves -= (int)(QString(AcquireClientGetUsername()) != qstr);
    }
    delete qsetter;
    
    if (widgets_count_by_saves != ((int)(all_wpw->size()) - 1))
    {
        printf("%d widgets_count_by_saves, %d widgets_existing_minus_createnew\n", widgets_count_by_saves, ((int)(all_wpw->size()) - 1));
        refreshWorkFull();
        return;
    }
    
    char **store_ids = NULL;
    float *pct_b2c = NULL;
    float *pct_wrk = NULL;
    float *pct_c2b = NULL;
    int noofws = 0;
    int64_t bytesfree = 0;
    
    int retval = AcquireQueryAllWorkStatus(&store_ids, &pct_b2c, &pct_wrk, &pct_c2b, &noofws, &bytesfree);
    if ((retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS) || (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS_NO_WORK))
    {
        if (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS) login_label->setText(QString("Network OK. %1 free on Conspire.").arg(getDataAmount(bytesfree)));
        if (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS_NO_WORK) login_label->setText(QString("Network OK. No active jobs. %1 free on Conspire.").arg(getDataAmount(bytesfree)));
        QSettings *qsetter = new QSettings("UoB", "AcquireClient");
        QStringList groups = qsetter->childGroups();
        int i = 0;
        for (int j = 0; j < groups.size(); j++)
        {
            if ((QString(groups.at(j)) != QString("preferences")) && ((QString(AcquireClientGetUsername()) == (qsetter->value(groups.at(j) + "/user").toString()))))
            {
                int k;
                ((WorkPacketWidget *)(all_wpw->at(i)))->updateUUID(groups.at(j).toAscii().constData());
                QString qstr = qsetter->value(groups.at(j) + "/workstoreid").toString();
                for (k = 0; k < noofws; k++)
                {
                    if (qstr == QString(store_ids[k]))
                    {
                        if (pct_b2c[k] == 100.)
                        {
                            char **workdata_ids = NULL;
                            char *stage_ids = NULL;
                            float *stage_pcts = NULL;
                            int noofwds = 0;
                            int retval2 = AcquireQueryAllWorkDataStatus(store_ids[k], &workdata_ids, &stage_ids, &stage_pcts, &noofwds);
                            if (retval2 == ACQUIRE_QUERY_ALL_WORK__SUCCESS)
                            {
                                float amt_b2c = 0.;
                                float amt_prg = 0.;
                                float amt_c2b = 0.;
                                for (int l = 0; l < noofwds; l++)
                                {
                                    printf("%s\t%s: %c-%f\n", store_ids[k], workdata_ids[l], stage_ids[l], stage_pcts[l]);
                                    switch (stage_ids[l])
                                    {
                                        case 'G': amt_c2b += (stage_pcts[l] / ((float)noofwds)); break;
                                        case 'F': amt_prg += (stage_pcts[l] / ((float)noofwds)); break;
                                        case 'E': amt_b2c += (stage_pcts[l] / ((float)noofwds)); break;
                                        default: break;
                                    }
                                    switch (stage_ids[l])
                                    {
                                        case 'H': amt_c2b += (100. / ((float)noofwds));
                                        case 'G': amt_prg += (100. / ((float)noofwds));
                                        case 'F': amt_b2c += (100. / ((float)noofwds));
                                        default: break;
                                    }
                                }
                                ((WorkPacketWidget *)(all_wpw->at(i)))->updateText("Waiting...");
                                if (amt_b2c > 0.) ((WorkPacketWidget *)(all_wpw->at(i)))->updateText("Sending...");
                                if (amt_b2c == 100.) ((WorkPacketWidget *)(all_wpw->at(i)))->updateText("Computing...");
                                if (amt_prg == 100.) ((WorkPacketWidget *)(all_wpw->at(i)))->updateText("Computed...");
                                if (amt_c2b > 0.) ((WorkPacketWidget *)(all_wpw->at(i)))->updateText("Receiving...");
                                ((WorkPacketWidget *)(all_wpw->at(i)))->updateAmounts(amt_b2c / 100., amt_prg / 100., amt_c2b / 100.);
                            } else
                            {
                                printf("acquire query all work data failed (code %d)\n", retval2);
                            }
                        } else
                        {
                            ((WorkPacketWidget *)(all_wpw->at(i)))->updateText("Uploading...");
                        }
                        if (pct_c2b[i] == 100.) ((WorkPacketWidget *)(all_wpw->at(i)))->updateText("Complete.");
                        ((WorkPacketWidget *)(all_wpw->at(i)))->computeAndUpdateUpload();
                    }
                }
                i++;
            }
        }
        ((WorkPacketWidget *)(all_wpw->at(all_wpw->size() - 1)))->updateText("Create new...");
        ((WorkPacketWidget *)(all_wpw->at(all_wpw->size() - 1)))->updateAmounts(0., 0., 0.);
        delete qsetter;
    } else
    {
        switch (retval)
        {
            case ACQUIRE_QUERY_ALL_WORK__PARSE_ERROR:
                login_label->setText("Error parsing server reply");
            case ACQUIRE_QUERY_ALL_WORK__UNKNOWN_ERROR:
                login_label->setText("Error - Network may be down");
            case ACQUIRE_QUERY_ALL_WORK__ACCESS_DENIED:
                login_label->setText("Access to work states denied");
            default:
                login_label->setText("Error");
        }
    }
    AcquireClientClearResults();
    allUpdate();
}

void NewWorkPage::refreshWorkFull()
{
    printf("refresh work triggered (full)\n");
    for (int i = all_wpw->size(); i--; )
    {
        qgrid->removeItem(all_wpw->at(i));
        delete all_wpw->at(i);
    }
    all_wpw->clear();
    
    int row = 0;
    int col = 0;
    
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QStringList groups = qsetter->childGroups();
    int i = 0;
    for (int j = 0; j < groups.size(); j++)
    {
        if (QString(groups.at(j)) != QString("preferences"))
        {
            WorkPacketWidget *t_wpw =
                new WorkPacketWidget("", 0, 0, i, all_wpw, groups.at(j).toAscii().constData());
            qgrid->setRowFixedHeight(row, 100);
            qgrid->setColumnFixedWidth(col, 100);
            qgrid->addItem(t_wpw, row, col, 1, 1);
            col++;
            if (col >= 3) { row++; col = 0; }
            connect(t_wpw, SIGNAL(push(PagePointer)), this, SIGNAL(push(PagePointer)));
            all_wpw->append(t_wpw);
            i++;
        }
    }
    delete qsetter;
    WorkPacketWidget *t_wpw =
        new WorkPacketWidget("Create new...", 0, 0, i, all_wpw, NULL);
    qgrid->setRowFixedHeight(row, 100);
    qgrid->setColumnFixedWidth(col, 100);
    qgrid->addItem(t_wpw, row, col, 1, 1);
    col++;
    if (col >= 3) { row++; col = 0; }
    connect(t_wpw, SIGNAL(push(PagePointer)), this, SIGNAL(push(PagePointer)));
    all_wpw->append(t_wpw);
    i++;
}

void NewWorkPage::refreshWork()
{
    login_label->setText("");
    refreshWorkPartial();
    return;
    /*
    
    char **store_ids = NULL;
    float *pct_b2c = NULL;
    float *pct_wrk = NULL;
    float *pct_c2b = NULL;
    int noofws = 0;
    int64_t bytesfree = 0;
   
    int row = 0;
    int col = 0;
   
    for (int i = all_wpw->size(); i--; )
    {
        qgrid->removeItem(all_wpw->at(i));
        delete all_wpw->at(i);
    }
    all_wpw->clear();
      
    int retval = AcquireQueryAllWorkStatus(&store_ids, &pct_b2c, &pct_wrk, &pct_c2b, &noofws, &bytesfree);
    if ((retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS) || (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS_NO_WORK))
    {
        if (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS) login_label->setText(QString("Network OK. %1 free on Conspire.").arg(getDataAmount(bytesfree)));
        if (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS_NO_WORK) login_label->setText(QString("Network OK. No active jobs. %1 free on Conspire.").arg(getDataAmount(bytesfree)));

        for (int i = 0; i < (noofws); i++)
        {
            WorkPacketWidget *t_wpw = new WorkPacketWidget("", 0, 0, i, all_wpw, NULL);
            qgrid->setRowFixedHeight(row, 100);
            qgrid->setColumnFixedWidth(col, 100);
            qgrid->addItem(t_wpw, row, col, 1, 1);
            col++;
            if (col >= 3) { row++; col = 0; }
            connect(t_wpw, SIGNAL(push(PagePointer)), this, SIGNAL(push(PagePointer)));
            all_wpw->append(t_wpw);
        }
        int numberofwpw = noofws;
      
        QSettings *qsetter = new QSettings("UoB", "AcquireClient");
        QStringList groups = qsetter->childGroups();
        for (int i = 0; i < groups.size(); i++)
        {
            QString qstr = qsetter->value(groups.at(i) + "/workstoreid").toString();
            if ((qstr.isEmpty()) && (QString(groups.at(i)) != QString("preferences")))
            {
                conspireDebug() << groups.at(i);
                WorkPacketWidget *t_wpw =
                    new WorkPacketWidget("", 0, 0, numberofwpw, all_wpw, groups.at(i).toAscii().constData());
                qgrid->setRowFixedHeight(row, 100);
                qgrid->setColumnFixedWidth(col, 100);
                qgrid->addItem(t_wpw, row, col, 1, 1);
                col++;
                if (col >= 3) { row++; col = 0; }
                connect(t_wpw, SIGNAL(push(PagePointer)), this, SIGNAL(push(PagePointer)));
                all_wpw->append(t_wpw);
                numberofwpw++;
            }
        }
        delete qsetter;
        WorkPacketWidget *t_wpw =
            new WorkPacketWidget("Create new...", 0, 0, numberofwpw, all_wpw, NULL);
        qgrid->setRowFixedHeight(row, 100);
        qgrid->setColumnFixedWidth(col, 100);
        qgrid->addItem(t_wpw, row, col, 1, 1);
        col++;
        if (col >= 3) { row++; col = 0; }
        connect(t_wpw, SIGNAL(push(PagePointer)), this, SIGNAL(push(PagePointer)));
        all_wpw->append(t_wpw);
        numberofwpw++;
      
      for (int i = 0; i < noofws; i++)
      {
         QString quuid;
         QString workstoreid = QString(store_ids[i]);
         QSettings *qsetter = new QSettings("UoB", "AcquireClient");
         QStringList groups = qsetter->childGroups();
         for (int j = 0; j < groups.size(); j++)
         {
            QString qstr = qsetter->value(groups.at(j) + "/workstoreid").toString();
            if (qstr == workstoreid)
            {
               quuid = groups.at(j);
            }
         }
         delete qsetter;
         int thiswpwno = numberofwpw - noofws + i;
         ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->updateUUID(quuid);
         if (pct_b2c[i] == 100.)
         {
            char **workdata_ids = NULL;
            char *stage_ids = NULL;
            float *stage_pcts = NULL;
            int noofwds = 0;
            int retval2 = AcquireQueryAllWorkDataStatus(store_ids[i], &workdata_ids, &stage_ids, &stage_pcts, &noofwds);
            if (retval2 == ACQUIRE_QUERY_ALL_WORK__SUCCESS)
            {
               float amt_b2c = 0.;
               float amt_prg = 0.;
               float amt_c2b = 0.;
               for (int l = 0; l < noofwds; l++)
               {
                  printf("%s\t%s: %c-%f\n", store_ids[i], workdata_ids[l], stage_ids[l], stage_pcts[l]);
                  switch (stage_ids[l])
                  {
                     case 'G': amt_c2b += (stage_pcts[l] / ((float)noofwds)); break;
                     case 'F': amt_prg += (stage_pcts[l] / ((float)noofwds)); break;
                     case 'E': amt_b2c += (stage_pcts[l] / ((float)noofwds)); break;
                     default: break;
                  }
                  switch (stage_ids[l])
                  {
                     case 'H': amt_c2b += (100. / ((float)noofwds));
                     case 'G': amt_prg += (100. / ((float)noofwds));
                     case 'F': amt_b2c += (100. / ((float)noofwds));
                     default: break;
                  }
               }
               ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->updateText("Waiting...");
               if (amt_b2c > 0.) ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->updateText("Sending...");
               if (amt_b2c == 100.) ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->updateText("Computing...");
               if (amt_prg == 100.) ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->updateText("Computed...");
               if (amt_c2b > 0.) ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->updateText("Receiving...");
               ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->updateAmounts(amt_b2c / 100., amt_prg / 100., amt_c2b / 100.);
            } else
            {
               printf("acquire query all work data failed (code %d)\n", retval2);
            }
         } else
         {
            ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->updateText("Uploading...");
         }
         if (pct_c2b[i] == 100.) ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->updateText("Complete.");
         ((WorkPacketWidget *)(all_wpw->at(thiswpwno)))->computeAndUpdateUpload();
      }
      ((WorkPacketWidget *)(all_wpw->at(all_wpw->size() - 1)))->updateText("Create new...");
      ((WorkPacketWidget *)(all_wpw->at(all_wpw->size() - 1)))->updateAmounts(0., 0., 0.);
   } else
   {
      switch (retval)
      {
         case ACQUIRE_QUERY_ALL_WORK__PARSE_ERROR:
            login_label->setText("Error parsing server reply");
         case ACQUIRE_QUERY_ALL_WORK__UNKNOWN_ERROR:
            login_label->setText("Error - Network may be down");
         case ACQUIRE_QUERY_ALL_WORK__ACCESS_DENIED:
            login_label->setText("Access to work states denied");
         default:
            login_label->setText("Error");
      }
   }
   AcquireClientClearResults();
   allUpdate();
   */
}

void NewWorkPage::modifyWork(int row, int col)
{
   
   /*
   QTableWidgetItem *qwidget = tableofworkstores->item(row, col);
   if (qwidget == NULL) return;
   if (QString("Create new...") == qwidget->text())
   {
      const char *hadclusters = AcquireListOfAccessibleClusters();
      if ((hadclusters == NULL) || (strlen(hadclusters) == 0))
      {
         login_label->setText("Please add some accessible clusters before creating work");
         return;
      }
      makeWork();
   } else
   {
      emit( push(new WorkStorePage(qwidget->data(::Qt::UserRole).toString())));
   }
   */
}

void NewWorkPage::build()
{
    //this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    
    WidgetRack *rack = new WidgetRack(this);
    
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(::Qt::Vertical, this);
    
    this->setLayout(l);
    
    l->insertItem(0,rack);

    stack = new WidgetStack(this);
    stack->setFocusPolicy(::Qt::NoFocus);
    
    WidgetRack *sub_rack = new WidgetRack(this);
    sub_rack->setFocusPolicy(::Qt::NoFocus);
    
    QLabel *label_table = new QLabel(Conspire::tr("Jobs that Conspire is currently tracking:"));
    sub_rack->addWidget(label_table);
    
    all_wpw = new QList<QGraphicsLayoutItem *>();

    qscene = new QGraphicsScene();
    qview = new QGraphicsView(qscene);
    qview->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    qview->setAutoFillBackground(true);

    qgrid = new QGraphicsGridLayout();
    qgrid->setRowFixedHeight(0, 100);
    qgrid->setColumnFixedWidth(0, 100);
    WorkPacketWidget *none_wpw = new WorkPacketWidget("Create new...", 0, 0, 0, all_wpw, NULL);
    qgrid->addItem(none_wpw, 0, 0, 1, 1);
    all_wpw->append(none_wpw);
    connect(none_wpw, SIGNAL(push(PagePointer)), this, SIGNAL(push(PagePointer)));
//    tableofworkstores = new QGraphicsWidget();
    tableofworkstores = new QGraphicsWidget();
    tableofworkstores->setLayout(qgrid);
    qscene->addItem(tableofworkstores);
    connect(qscene, SIGNAL(mousePressEvent(QGraphicsSceneMouseEvent *)), tableofworkstores, SLOT(mousePressEvent(QGraphicsSceneMouseEvent *)));

    sub_rack->addWidget(qview);
    return_button = NULL;

    button = new Button(Conspire::tr("Refresh"));
    connect(button, SIGNAL(clicked()), this, SLOT(refreshWork()));
    sub_rack->addWidget(button);

    return_button = new Button(Conspire::tr("Return"));
    connect(return_button, SIGNAL(clicked()), this, SIGNAL(pop()));
    sub_rack->addWidget(return_button);
 
    login_label = new QLabel();
    sub_rack->addWidget(login_label);

    stack->addWidget(sub_rack);

    rack->addWidget(stack);
    refreshWork();
    refreshtimer = new QTimer(this);
    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(refreshWork()));
    refreshtimer->start(1000);
}

/** Constructor */
NewWorkPage::NewWorkPage(Page *parent) : Page(parent)
{
    build();
}

/** Destructor */
NewWorkPage::~NewWorkPage()
{}

void NewWorkPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void NewWorkPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void NewWorkPage::paint(QPainter *painter, 
                        const QStyleOptionGraphicsItem *option, 
                        QWidget *widget)
{
    for (int i = 0; i < all_wpw->size(); i++)
    {
       ((WorkPacketWidget *)(all_wpw->at(i)))->update();
    }
    Page::paint(painter, option, widget);
}

void NewWorkPage::allUpdate()
{
    QCoreApplication::processEvents();
}
