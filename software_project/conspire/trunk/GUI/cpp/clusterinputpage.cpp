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

#include "Conspire/GUI/clusterinputpage.h"
#include "Conspire/GUI/workpage.h"
#include "Conspire/GUI/userpage.h"
#include "Conspire/GUI/widgetrack.h"
#include "Conspire/GUI/button.h"
#include "Conspire/GUI/exceptionpage.h"
#include "Conspire/GUI/widgetstack.h"

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

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;

void ClusterInputPage::browseForKey()
{
   QString homepath = QDir::homePath();
   QString sshpath = QString("%1/%2").arg(homepath).arg(".ssh");
   if (!(QDir(sshpath).exists())) sshpath = QString();

   QString keyfile = QFileDialog::getOpenFileName(NULL, Conspire::tr("Select the file containing the private key."), sshpath, QString(), NULL, QFileDialog::DontUseNativeDialog);
   if (keyfile.isEmpty()) return;
   quuid = QString(keyfile);
   login_label->setText(QString("Selected keyfile '%1'.").arg(quuid));
}

void ClusterInputPage::tryClusterAdd()
{
   QString machinename;
   if (strcmp(workstoreid.toAscii().constData(), "emerald") == 0)
   {
      machinename = QString("emerald.einfrastructuresouth.ac.uk");
   }
   QString username = lineedit_username->text();
   QString password = lineedit_password->text();
   // try to add the cluster by doing the manipulations.
   if ((machinename.toAscii().constData() == NULL) || (strlen(machinename.toAscii().constData()) == 0))
   {
      login_label->setText(QString("No machine hostname specified"));
      return;
   }
   if ((username.toAscii().constData() == NULL) || (strlen(username.toAscii().constData()) == 0))
   {
      login_label->setText(QString("No username specified"));
      return;
   }
   QString userathost = QString("%1@%2").arg(username, machinename);
   char *failed_hosts_list = NULL;
//   if (loginsuccessful) *loginsuccessful = FALSE;
   int retval = AcquireAddSSHHostToPool(userathost.toAscii().constData(), password.toAscii().constData(), &failed_hosts_list, (char *)((quuid.isEmpty()) ? NULL : quuid.toAscii().constData()));
   switch (retval)
   {
      case ACQUIRE_USER_ADD_SSH__SSH_SUCCESS:
      {
//         if (loginsuccessful) *loginsuccessful = TRUE;
         login_label->setText(QString("Successfuly added machine"));
         emit ( pop(true) );
         return;
      }
      case ACQUIRE_USER_ADD_SSH__FINGERPRINT_UNTRUSTED:
         login_label->setText(QString("The fingerprint for this machine has changed, contact administrator"));
         return;
      case ACQUIRE_USER_ADD_SSH__SSH_ROUTING_FAILED:
      {
         if ((failed_hosts_list == NULL) || (strlen(failed_hosts_list) <= 1))
         {
            login_label->setText(QString("Bad password, machine not trusted or could not find machine (connection failed)"));
            return;
         } else
         {
            login_label->setText(QString(QObject::tr("Could not reach machine, failed to connect to %1").arg(failed_hosts_list)));
            return;
         }
      }
   }
   login_label->setText(QString("Failed to verify with machine"));
}

void ClusterInputPage::downloadItem(QListWidgetItem *titem)
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

void ClusterInputPage::restartUpload()
{
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QString uploaddir = qsetter->value(quuid + "/uploaddir").toString();
    int blocks = qsetter->value(quuid + "/blocks").toInt();
    UploadThread *uploadthread = new UploadThread(workstoreid.toAscii().constData(), uploaddir.toAscii().constData(), NULL, NULL, 1, 3600, blocks);
    GetUploadArray()->insert(workstoreid, uploadthread);
    QThreadPool::globalInstance()->start(uploadthread);
    modifybutton->setText(Conspire::tr("Refresh"));
    disconnect(modifybutton, SIGNAL(clicked()));
    connect(modifybutton, SIGNAL(clicked()), this, SLOT(refreshTimes()));
}

void ClusterInputPage::refreshTimes()
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

bool ClusterInputPage::removeDir(const QString &dirName)
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

void ClusterInputPage::expungeWorkStore()
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

void ClusterInputPage::build()
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
    
    if (strcmp(workstoreid.toAscii().constData(), "emerald") == 0)
    {
        QLabel *intro_label = new QLabel(Conspire::tr("This page adds logins for gateways needed for Emerald."));
        intro_label->setWordWrap(true);
        sub_rack->addWidget(intro_label);

        QLabel *intro_label2 = new QLabel(Conspire::tr("Username:"));
        intro_label2->setWordWrap(true);
        sub_rack->addWidget(intro_label2);

        lineedit_username = new QLineEdit();
        lineedit_username->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
        lineedit_username->setPlaceholderText("Username");
        lineedit_username->setFocusPolicy(::Qt::StrongFocus);
        sub_rack->addWidget(lineedit_username);
        
        modifybutton = new Button(Conspire::tr("Browse for key"));
        connect(modifybutton, SIGNAL(clicked()), this, SLOT(browseForKey()));
        sub_rack->addWidget(modifybutton);
        
        QLabel *intro_label3 = new QLabel(Conspire::tr("Password (for either key decryption or the account):"));
        intro_label3->setWordWrap(true);
        sub_rack->addWidget(intro_label3);
        
        lineedit_password = new QLineEdit();
        lineedit_password->setEchoMode(QLineEdit::Password);
        lineedit_password->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
        lineedit_password->setPlaceholderText("Password");
        lineedit_password->setFocusPolicy(::Qt::StrongFocus);
        sub_rack->addWidget(lineedit_password);
        
        return_button = new Button(Conspire::tr("Add cluster"));
        connect(return_button, SIGNAL(clicked()), this, SLOT(tryClusterAdd()));
        sub_rack->addWidget(return_button);
    }
        
    //AcquireAreResultsFromWorkStoreReady(const int nresultsarray, int *resultsarray, const char *store_id)
    
    login_label = new QLabel("");
    sub_rack->addWidget(login_label);

    stack->addWidget(sub_rack);
    
    rack->addWidget(stack);
    
    if (modifybutton->text() == Conspire::tr("Refresh"))
       refreshTimes();
}

/** Constructor */
ClusterInputPage::ClusterInputPage(QString iclusterid, Page *parent) : Page(parent)
{
    workstoreid = QString(iclusterid);
    build();
}

/** Destructor */
ClusterInputPage::~ClusterInputPage()
{}

void ClusterInputPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void ClusterInputPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void ClusterInputPage::paint(QPainter *painter, 
                       const QStyleOptionGraphicsItem *option, 
                       QWidget *widget)
{
    Page::paint(painter, option, widget);
}

void ClusterInputPage::allUpdate()
{
   
    QCoreApplication::processEvents();
}
