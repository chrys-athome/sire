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
#include "Conspire/GUI/configdocument.h"
#include "Conspire/GUI/chooseclasspage.h"

#include "Conspire/GUI/downloadthread.h"
#include "Conspire/GUI/uploadthread.h"
#include "Conspire/GUI/global_var.h"

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
#include <QXmlStreamReader>

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"
#include "config.h"

static QString install_dir = STATIC_INSTALL_DIR;

using namespace Conspire;

void WorkStorePage::downloadItem(QListWidgetItem *titem)
{
   if (GetDownloadArray()->value(workstoreid))
   {
      if (GetDownloadArray()->value(workstoreid)->isFinished())
      {
         DownloadThread *dth = GetDownloadArray()->take(workstoreid);
         if (dth) delete dth;
      } else
      {
         login_label->setText("Cannot download until previous download finished!");
         return;
      }
   }
      
   char *downloadid = strdup(titem->data(::Qt::UserRole).toString().toAscii().constData());
   printf("Item clicked %s\n", downloadid);
   
   QString thisDir;
   int successfully_chosen_a_directory = 0;
   while (!successfully_chosen_a_directory)
   {
      thisDir = QFileDialog::getExistingDirectory(NULL, Conspire::tr("Select directory in which to download results"), QString(), QFileDialog::DontUseNativeDialog);
      if (thisDir.isEmpty()) return;

      QDir resultsdir( QString("%1/results").arg(thisDir) );
      
      int i = 1;
      while (resultsdir.exists())
      {
         i += 1;
         resultsdir = QDir( QString("%1/results%2").arg(thisDir).arg(i) );
      }

      successfully_chosen_a_directory = resultsdir.mkpath( resultsdir.absolutePath() );
      thisDir = resultsdir.absolutePath();
   }
   
   DownloadThread *downloadthread = new DownloadThread(workstoreid.toAscii().constData(), downloadid,
      thisDir.toAscii().constData());
   GetDownloadArray()->insert(workstoreid, downloadthread);
   QThreadPool::globalInstance()->start(downloadthread);
   free(downloadid);
}

void WorkStorePage::editWorkStore()
{
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QString jobname = qsetter->value(quuid + "/jobname").toString();
    QString jobclass = qsetter->value(quuid + "/jobclass").toString();
    QString saveddata = qsetter->value(quuid + "/saveddata").toString();
    QString uploaddir = qsetter->value(quuid + "/uploaddir").toString();
    delete qsetter;
    if (not uploaddir.isEmpty())
    {
        UploadThread *uploadthread = new UploadThread(workstoreid.toAscii().constData(), uploaddir.toAscii().constData(), NULL, NULL, 1, 3600, NULL);
        GetUploadArray()->insert(workstoreid, uploadthread);
        QThreadPool::globalInstance()->start(uploadthread);
        emit( pop(true));
        return;
    }
    if (not saveddata.isEmpty())
    {
        QFile file(saveddata);
        if (file.exists())
        {
           Options toptions;
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
            QString t_jobclassdirectory;
            QString t_jobclassxml;
            QString t_jobclassincludedirs;
            
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
                    if (xmlReader->name() == "directory") t_jobclassdirectory = xmlReader->readElementText();
                    if (xmlReader->name() == "optionsxml") t_jobclassxml = xmlReader->readElementText();
                    if (xmlReader->name() == "optionsincludedirs") t_jobclassincludedirs = xmlReader->readElementText();
                }
                if (token == QXmlStreamReader::EndElement)
                {
                    if (xmlReader->name() == "jobclassdescription")
                    {
                       if (t_jobclassid == jobclass) break;
//                       conspireDebug() << t_jobclassid << jobclass;
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
                
                
        QStringList path;
        path << QString("%1/job_classes/%2").arg(install_dir).arg(t_jobclassdirectory);
        
        printf("joboptions %s\n", t_jobclassxml.toAscii().constData());
        toptions = Options::fromXMLFile(QString("%1").arg(t_jobclassxml), path);

        qDebug() << "Looking for a default config file...";
        QFileInfo default_config( QString("%1/job_classes/%2/default_config").arg(install_dir,t_jobclassdirectory) );

        qDebug() << default_config.absoluteFilePath();
            
        toptions = toptions.fromConfigFile(saveddata);
        
           emit(push( PagePointer( new ConfigDocument(jobclass, toptions, quuid) ), true));
           return;
        }
    }
    if (not jobclass.isEmpty())
    {
       Options toptions;
       emit(push( PagePointer( new ConfigDocument(jobclass, toptions, quuid) ), true));
       return;
    }
    if (not jobname.isEmpty())
    {
       emit(push( PagePointer( new ChooseClassPage(jobname, quuid) ), true));
       return;
    }
    emit(push( PagePointer( new ChooseClassPage() )));
}

void WorkStorePage::restartUpload()
{
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QString uploaddir = qsetter->value(quuid + "/uploaddir").toString();
    int blocks = qsetter->value(quuid + "/blocks").toInt();
    delete qsetter;
    UploadThread *uploadthread = new UploadThread(workstoreid.toAscii().constData(), uploaddir.toAscii().constData(), NULL, NULL, 1, 3600, blocks);
    GetUploadArray()->insert(workstoreid, uploadthread);
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
   if (not workstoreid.isEmpty())
   {
      // need to shut down any download / upload threads running on this work store first!!!!!
      int localsetfound = 0;
      UploadThread *uthread = GetUploadArray()->value(workstoreid);
      if (uthread)
      {
         uthread->cancelUpload();
         while (!(uthread->isFinished())) sleep(0.1);
         delete uthread;
      }
      DownloadThread *dthread = GetDownloadArray()->value(workstoreid);
      if (dthread)
      {
         dthread->cancelDownload();
         while (!(dthread->isFinished())) sleep(0.1);
         delete dthread;
      }
      int retval = AcquireDeleteWorkStore(workstoreid.toAscii().constData());
      // HACK to allow deletion of non-broker work
      if (1)//(retval == ACQUIRE_DELETE_WORK_STORE__SUCCESS)
      {
         QSettings *qsetter = new QSettings("UoB", "AcquireClient");
         QStringList groups = qsetter->childGroups();
         for (int i = 0; i < groups.size(); i++)
         {
            QString qstr = qsetter->value(groups.at(i) + "/workstoreid").toString();
            if (qstr == workstoreid)
            {
               QString jobfile = qsetter->value(quuid + "/saveddata").toString();
               if (not jobfile.isEmpty())
               {
                  QFile file(jobfile);
                  file.remove();
               }
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
         conspireDebug() << QString("delete workstore");
         emit( pop() );
      } else
      {
         if (retval == ACQUIRE_DELETE_WORK_STORE__ACCESS_DENIED)
            login_label->setText("Access denied");
         if (retval == ACQUIRE_DELETE_WORK_STORE__UNKNOWN_ERROR)
            login_label->setText("Unknown error (connection failed?)");
      }
   } else
   {
      if (not quuid.isEmpty())
      {
         printf("my quuid %s\n", quuid.toAscii().constData());
         QSettings *qsetter = new QSettings("UoB", "AcquireClient");
         QStringList groups = qsetter->childGroups();
         for (int i = 0; i < groups.size(); i++)
         {
            if (groups.at(i) == quuid)
            {
               QString jobfile = qsetter->value(quuid + "/saveddata").toString();
               if (not jobfile.isEmpty())
               {
                  QFile file(jobfile);
                  file.remove();
               }
               qsetter->beginGroup(groups.at(i));
               qsetter->remove("");
               qsetter->endGroup();
            }
         }
         delete qsetter;
         conspireDebug() << QString("delete localstore") << quuid;
         emit( pop() );
      }
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
    
    if (GetUploadArray()->value(workstoreid))
       if (GetUploadArray()->value(workstoreid)->isFinished())
       {
          QSettings *qsetter = new QSettings("UoB", "AcquireClient");
          qsetter->setValue(quuid + "/uploadComplete", 1);
          delete qsetter;
          delete GetUploadArray()->take(workstoreid);
       }
    
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    if (qsetter->value(quuid + "/uploadComplete").toInt() == 1)
    {
        modifybutton = new Button(Conspire::tr("Refresh"));
        connect(modifybutton, SIGNAL(clicked()), this, SLOT(refreshTimes()));
        sub_rack->addWidget(modifybutton);
    } else
    {
        if (GetUploadArray()->value(workstoreid) == NULL)
        {
           modifybutton = new Button(Conspire::tr("Edit"));
           connect(modifybutton, SIGNAL(clicked()), this, SLOT(editWorkStore()));
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
WorkStorePage::WorkStorePage(QString iworkstoreid, QString iquuid, Page *parent) : Page(parent)
{
    quuid = iquuid;
    workstoreid = QString(iworkstoreid);
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QStringList groups = qsetter->childGroups();
    if (not workstoreid.isEmpty())
    {
        for (int i = 0; i < groups.size(); i++)
        {
            QString qstr = qsetter->value(groups.at(i) + "/workstoreid").toString();
            if (qstr == workstoreid)
            {
                quuid = groups.at(i);
            }
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
