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

#include "Conspire/GUI/workstorepage.h"
#include "Conspire/GUI/workpage.h"
#include "Conspire/GUI/userpage.h"
#include "Conspire/GUI/widgetrack.h"
#include "Conspire/GUI/button.h"
#include "Conspire/GUI/exceptionpage.h"
#include "Conspire/GUI/widgetstack.h"

#include "Conspire/GUI/downloadthread.h"
#include "Conspire/GUI/uploadthread.h"
#include "Conspire/GUI/mainwindow.h"

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
#include <QListWidget>
#include <QThreadPool>
#include <QFileDialog>

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;


void WorkStorePage::downloadItem(QListWidgetItem *titem)
{
   if (downloadarray->value(workstoreid))
   {
      if (downloadarray->value(workstoreid)->isFinished())
      {
         DownloadThread *dth = downloadarray->take(workstoreid);
         if (dth) delete dth;
      } else
      {
         login_label->setText("Cannot download until previous download finished!");
         return;
      }
   }
      
   char *downloadid = strdup(titem->data(::Qt::UserRole).toString().toAscii().constData());
   printf("Item clicked %s\n", downloadid);
   QString thisDir = QFileDialog::getExistingDirectory(NULL, Conspire::tr("Select directory in which to download results"));
   if (thisDir.isEmpty()) return;
   DownloadThread *downloadthread = new DownloadThread(workstoreid.toAscii().constData(), downloadid,
      thisDir.toAscii().constData());
   downloadarray->insert(workstoreid, downloadthread);
   QThreadPool::globalInstance()->start(downloadthread);
   free(downloadid);
}

void WorkStorePage::restartUpload()
{
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QString uploaddir = qsetter->value(quuid + "/uploaddir").toString();
    int blocks = qsetter->value(quuid + "/blocks").toInt();
    UploadThread *uploadthread = new UploadThread(workstoreid.toAscii().constData(), uploaddir.toAscii().constData(), NULL, NULL, 1, 3600, blocks);
    uploadarray->insert(workstoreid, uploadthread);
    QThreadPool::globalInstance()->start(uploadthread);
    modifybutton->setText(Conspire::tr("Refresh"));
    disconnect(modifybutton, SIGNAL(clicked()));
    connect(modifybutton, SIGNAL(clicked()), this, SLOT(refreshTimes()));
}

void WorkStorePage::refreshTimes()
{
   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   int instances = qsetter->value(quuid + "/instances").toInt();
   delete qsetter;
   
   int *resultsarray = (int *)malloc(sizeof(int)*instances);
   int retval = AcquireAreResultsFromWorkStoreReady(instances, resultsarray, workstoreid.toAscii().constData());
   switch (retval)
   {
      case -2:
         login_label->setText("Access denied");
         free(resultsarray);
         return;
      case -1:
         login_label->setText("Unknown error (connection problem?)");
         free(resultsarray);
         return;
      case 0:
         login_label->setText("No results yet available");
         free(resultsarray);
         return;
      default: break;
   }

   int items = instancelist->count();
   for (int i = 0; i < items; i++)
   {
      QListWidgetItem *wid = instancelist->item(i);
      instancelist->removeItemWidget(wid);
      delete wid;
   }
   
   int haveany = 0;
   for (int i = 0; i < instances; i++)
   {
      if (resultsarray[i])
      {
         const char *downloadid = AcquireGetResultIDFromNumberWhenReady(i, workstoreid.toAscii().constData());
         if (downloadid)
         {
            QString fmtinst = QString("Slot %1 ready for download").arg(QString::number(i));
            QString sngleinst = QString("Click here to download");
            QListWidgetItem *wid = new QListWidgetItem((instances == 1) ? sngleinst : fmtinst);
            wid->setData(::Qt::UserRole, QString(downloadid));
            instancelist->addItem(wid);
            haveany++;
         }
      }
   }
   if (haveany == 0)
   {
      QListWidgetItem *none_item = new QListWidgetItem("None yet available...");
      instancelist->addItem(none_item);
      login_label->setText("No results yet available");
   } else
   {
      login_label->setText(QString("%1 set(s) of results ready for download").arg(QString::number(haveany)));
   }
   AcquireClientClearResults();
   free(resultsarray);
   allUpdate();
}

bool WorkStorePage::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);
 
    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }
 
            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
 
    return result;
}

void WorkStorePage::expungeWorkStore()
{
   // need to shut down any download / upload threads running on this work store first!!!!!
   int localsetfound = 0;
   UploadThread *uthread = uploadarray->value(workstoreid);
   if (uthread)
   {
      uthread->cancelUpload();
      while (!(uthread->isFinished())) sleep(0.1);
      delete uthread;
   }
   DownloadThread *dthread = downloadarray->value(workstoreid);
   if (dthread)
   {
      dthread->cancelDownload();
      while (!(dthread->isFinished())) sleep(0.1);
      delete dthread;
   }
   int retval = AcquireDeleteWorkStore(workstoreid.toAscii().constData());
   if (retval == ACQUIRE_DELETE_WORK_STORE__SUCCESS)
   {
      QSettings *qsetter = new QSettings("UoB", "AcquireClient");
      QStringList groups = qsetter->childGroups();
      for (int i = 0; i < groups.size(); i++)
      {
         QString qstr = qsetter->value(groups.at(i) + "/workstoreid").toString();
         if (qstr == workstoreid)
         {
            QString uploaddir = qsetter->value(quuid + "/uploaddir").toString();
            removeDir(uploaddir);
            qsetter->beginGroup(groups.at(i));
            qsetter->remove("");
            qsetter->endGroup();
            localsetfound = 1;
         }
      }
      delete qsetter;
      if (localsetfound)
      {
         login_label->setText("Successfully deleted");
      } else
      {
         login_label->setText("Remote deleted, but could not find local work data");
         printf("warning: Remote data deleted, but could not find local work data\n");
      }
      emit( pop() );
   } else
   {
      if (retval == ACQUIRE_DELETE_WORK_STORE__ACCESS_DENIED)
         login_label->setText("Access denied");
      if (retval == ACQUIRE_DELETE_WORK_STORE__UNKNOWN_ERROR)
         login_label->setText("Unknown error (connection failed?)");
   }
}

void WorkStorePage::build()
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
    
    QLabel *intro_label = new QLabel(Conspire::tr("Click to download results from the finished instances listed below:"));
    intro_label->setWordWrap(true);
    sub_rack->addWidget(intro_label);
    
    //AcquireAreResultsFromWorkStoreReady(const int nresultsarray, int *resultsarray, const char *store_id)
    instancelist = new QListWidget();
    QListWidgetItem *none_item = new QListWidgetItem("None yet available...");
    instancelist->addItem(none_item);
    sub_rack->addWidget(instancelist);
    connect(instancelist, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(downloadItem(QListWidgetItem *)));
    
    if (uploadarray->value(workstoreid))
       if (uploadarray->value(workstoreid)->isFinished())
       {
          QSettings *qsetter = new QSettings("UoB", "AcquireClient");
          qsetter->setValue(quuid + "/uploadComplete", 1);
          delete qsetter;
          delete uploadarray->take(workstoreid);
       }
    
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    if (qsetter->value(quuid + "/uploadComplete").toInt() == 1)
    {
        modifybutton = new Button(Conspire::tr("Refresh"));
        connect(modifybutton, SIGNAL(clicked()), this, SLOT(refreshTimes()));
        sub_rack->addWidget(modifybutton);
    } else
    {
        if (uploadarray->value(workstoreid) == NULL)
        {
           modifybutton = new Button(Conspire::tr("Resume upload"));
           connect(modifybutton, SIGNAL(clicked()), this, SLOT(restartUpload()));
           sub_rack->addWidget(modifybutton);
        } else
        {
           modifybutton = new Button(Conspire::tr("Refresh"));
           connect(modifybutton, SIGNAL(clicked()), this, SLOT(refreshTimes()));
           sub_rack->addWidget(modifybutton);
        }
    }
    delete qsetter;
        
    button = new Button(Conspire::tr("Expunge"));
    connect(button, SIGNAL(clicked()), this, SLOT(expungeWorkStore()));
    sub_rack->addWidget(button);
    
    return_button = new Button(Conspire::tr("Return"));
    connect(return_button, SIGNAL(clicked()), this, SIGNAL(pop()));
    sub_rack->addWidget(return_button);
    
    login_label = new QLabel("");
    sub_rack->addWidget(login_label);

    stack->addWidget(sub_rack);
    
    rack->addWidget(stack);
    
    if (modifybutton->text() == Conspire::tr("Refresh"))
       refreshTimes();
}

/** Constructor */
WorkStorePage::WorkStorePage(QString iworkstoreid, Page *parent) : Page(parent)
{
    workstoreid = QString(iworkstoreid);
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QStringList groups = qsetter->childGroups();
    for (int i = 0; i < groups.size(); i++)
    {
       QString qstr = qsetter->value(groups.at(i) + "/workstoreid").toString();
       if (qstr == workstoreid)
       {
          quuid = groups.at(i);
       }
    }
    delete qsetter;
    build();
}

/** Destructor */
WorkStorePage::~WorkStorePage()
{}

void WorkStorePage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void WorkStorePage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void WorkStorePage::paint(QPainter *painter, 
                       const QStyleOptionGraphicsItem *option, 
                       QWidget *widget)
{
    Page::paint(painter, option, widget);
}

void WorkStorePage::allUpdate()
{
   
    QCoreApplication::processEvents();
}
