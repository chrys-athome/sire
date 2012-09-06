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

#include "Conspire/GUI/accountpage.h"
#include "Conspire/GUI/workpage.h"
#include "Conspire/GUI/userpage.h"
#include "Conspire/GUI/widgetrack.h"
#include "Conspire/GUI/button.h"
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
#include <QListWidget>

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;

static QString install_dir 
                = "/home/benlong/conspire/job_classes";

static QString broker = "ssi-amrmmhd.epcc.ed.ac.uk";
//static QString broker = "127.0.0.1";

void AccountPage::continueToWorkStores()
{
   emit( push( PagePointer( new WorkPage(1))));
}

void AccountPage::addSSHAccount()
{
   emit( push( PagePointer( new UserPage(1))));
}

void AccountPage::refreshList()
{
   int items = clusterlist->count();
   for (int i = 0; i < items; i++)
   {
      QListWidgetItem *wid = clusterlist->item(i);
      clusterlist->removeItemWidget(wid);
      delete wid;
   }
   const char *listclust = AcquireListOfAccessibleClusters();
   char *remainder = NULL;
   const char *token = strtok_r((char *)listclust, ",", &remainder);
   int haveany = 0;
   while (token != NULL)
   {
      char buffer[512];
      const char *cname = NULL;
      if (strcmp(token, "pretend") == 0) cname = "My local testing cluster";
      if (strcmp(token, "bluecrystalp1") == 0) cname = "Bluecrystal Phase 1";
      if (strcmp(token, "bluecrystalp2") == 0) cname = "Bluecrystal Phase 2";
      if (cname)
      {
         sprintf(buffer, "%s (%s)", cname, token);
      } else
      {
         sprintf(buffer, "%s", token);
      }
      QListWidgetItem *wid = new QListWidgetItem(buffer);
      clusterlist->addItem(wid);
      haveany = 1;
      token = strtok_r(NULL, ",", &remainder);
   }
   if (haveany == 0)
   {
      QListWidgetItem *none_item = new QListWidgetItem("None available...");
      clusterlist->addItem(none_item);
   }
   AcquireClientClearResults();
}

void AccountPage::build()
{
   
    job_id = -1;
    output_name = "results.tar.gz";

    draw_file_progress_bar = 0;
    num_bytes_expected = 0;
    num_bytes_transferred = 0;

    //this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    
    WidgetRack *rack = new WidgetRack(this);
    
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(::Qt::Vertical, this);
    
    this->setLayout(l);
    
    l->insertItem(0,rack);

    stack = new WidgetStack(this);
    stack->setFocusPolicy(::Qt::NoFocus);
    
    WidgetRack *sub_rack = new WidgetRack(this);
    sub_rack->setFocusPolicy(::Qt::NoFocus);
    
    QLabel *intro_label = new QLabel(Conspire::tr("Remote machine accounts using SSH must be added"
       " to Acquire in order to do work. Click here to add an SSH login:"));
    intro_label->setWordWrap(true);
    sub_rack->addWidget(intro_label);
    
    modifybutton = new Button(Conspire::tr("Add..."));
    connect(modifybutton, SIGNAL(clicked()), this, SLOT(addSSHAccount()));
    sub_rack->addWidget(modifybutton);
    
    QLabel *clusters_avail = new QLabel(Conspire::tr("Clusters currently available"
       " to your account are listed below:"));
    clusters_avail->setWordWrap(true);
    sub_rack->addWidget(clusters_avail);
        
    clusterlist = new QListWidget();
    QListWidgetItem *none_item = new QListWidgetItem("None available...");
    clusterlist->addItem(none_item);
    sub_rack->addWidget(clusterlist);
    
    button = new Button(Conspire::tr("Refresh"));
    connect(button, SIGNAL(clicked()), this, SLOT(refreshList()));
    sub_rack->addWidget(button);
    
    login_label = new QLabel("");
    sub_rack->addWidget(login_label);

    stack->addWidget(sub_rack);
    
    rack->addWidget(stack);
    refreshList();
}

/** Constructor */
AccountPage::AccountPage(int iusemode, Page *parent) : Page(parent)
{
//   usemode = AcquireClientIsInitialised();
    usemode = iusemode;
    build();
}

/** Destructor */
AccountPage::~AccountPage()
{}

void AccountPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void AccountPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void AccountPage::paint(QPainter *painter, 
                       const QStyleOptionGraphicsItem *option, 
                       QWidget *widget)
{
    Page::paint(painter, option, widget);
    
    if (draw_file_progress_bar)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setRenderHint(QPainter::TextAntialiasing);

        painter->setOpacity(0.5);
        
        QRectF rect( 10, 10, this->geometry().width()-20, 
                     this->geometry().height()-20 );
                     
        double prog = (1.0*num_bytes_transferred) / num_bytes_expected;
        
        int span = int(360*16*prog);
        
        painter->drawPie(rect, -int(0.5*span), span);
    }
}

void AccountPage::allUpdate()
{
   
    //status_label->update();
    //progress_bar->update();
    QCoreApplication::processEvents();
}

QString AccountPage::addMachine(QString username, QString password, QString machinename, bool *loginsuccessful)
{
   QString userathost = QString("%1@%2").arg(username, machinename);
   char *failed_hosts_list = NULL;
   if (loginsuccessful) *loginsuccessful = FALSE;
   int retval = AcquireAddSSHHostToPool(userathost.toAscii().constData(), password.toAscii().constData(), &failed_hosts_list);
   switch (retval)
   {
      case ACQUIRE_USER_ADD_SSH__SSH_SUCCESS:
      {
         if (loginsuccessful) *loginsuccessful = TRUE;
         return "Successfuly added machine";
      }
      case ACQUIRE_USER_ADD_SSH__FINGERPRINT_UNTRUSTED:
         return "The fingerprint for this machine has changed, contact administrator";
      case ACQUIRE_USER_ADD_SSH__SSH_ROUTING_FAILED:
      {
         if ((failed_hosts_list == NULL) || (strlen(failed_hosts_list) <= 1))
         {
            return "Bad password, machine not trusted or could not find machine (connection failed)";
         } else
         {
            return QObject::tr("Could not reach machine, failed to connect to %1").arg(failed_hosts_list);
         }
      }
   }
}

QString AccountPage::loginUser(QString username, QString password, bool *loginsuccessful)
{
   int login_success = AcquireClientInit(DEFAULT_HOST, DEFAULT_PORT, username.toAscii().constData(),
                                         password.toAscii().constData(), "broker");
   if (login_success == ACQUIRE_USER_REGISTRATION_STATE__FINAL)
   {
      // Successful log in
      if (loginsuccessful) *loginsuccessful = TRUE;
      QSettings *qsetter = new QSettings("UoB", "AcquireClient");
      qsetter->setValue("preferences/lastUsername", username);
      delete qsetter;
      stack->switchTo(2);
      return "Log in successful";
   } else
   {
      // Failed to log in
      AcquireClientDestroy();
      if (loginsuccessful) *loginsuccessful = FALSE;
      return "Log in failed";
   }
}

void AccountPage::changeUser()
{
   button->show();
   login_label->setText("");
   lineedit_username->setText("");
   lineedit_password->setText("");
   if (AcquireClientIsInitialised())
   {
      AcquireClientDestroy();
   }
   submode = 0;
   stack->switchTo(0);
   lineedit_username->setFocus();
   allUpdate();
}

void AccountPage::login()
{
    QString username;
    QString password;
    if (submode)
    {
       username = last_username;
       password = lineedit_password_known->text();
    } else
    {
       username = lineedit_username->text();
       password = lineedit_password->text();
    }
    bool wasokay = FALSE;
    QString login_status = loginUser(username, password, &wasokay);

    if (wasokay)
    {
       button->hide();
       stack->switchTo(2);
    }
    login_label->setText(login_status);
    allUpdate();
}

void AccountPage::sshadd()
{
    QString susername = lineedit_username->text();
    QString machinename = lineedit_host->text();
    QString spassword = lineedit_password->text();
    
    bool wasokay = FALSE;
    QString sshadd_retstring = addMachine(susername, spassword, machinename, &wasokay);

    if (wasokay) button->hide();
    login_label->setText(sshadd_retstring);
    allUpdate();
}

void AccountPage::submit()
{
    if (job_class.isEmpty())
        return;

    try
    {

    button->disconnect();
    button->hide();
    
//    status_label->setText(Conspire::tr("Creating a temporary directory..."));
    progress_bar->setValue(5);

    allUpdate();

    conspireDebug() << "SUBMIT THE CALCULATION";

    stack->switchTo(1);

    //make a temporary directory in which to stage the script
    QString tmpdir;
    {
        //use QTemporaryFile to get a unique filename
        QTemporaryFile f;
        
        if (f.open())
            tmpdir = QFileInfo(f.fileName()).absoluteFilePath();
    }
    
    if (tmpdir.isEmpty())
        throw Siren::unavailable_resource( 
                Conspire::tr("Cannot create a temporary directory!"), CODELOC );
                
    conspireDebug() << tmpdir;
                
    QDir dir(tmpdir);
    if (not dir.mkpath("."))
        throw Siren::unavailable_resource(
                Conspire::tr("Cannot create a temporary directory! (%1)")
                        .arg(tmpdir), CODELOC );

//    status_label->setText(Conspire::tr("Assembling the input files..."));
    progress_bar->setValue(10);
    allUpdate();
    
    Options submit_opts = opts;

    try
    {
        //go through the options and get all of the files - copy them into
        //the temporary directory
        QStringList all_keys = opts.keysAndIndiciesWithValue(true);
        
        double progress_value = 10;
        double progress_delta = all_keys.count() / 40.0;
        
        foreach (QString key, all_keys)
        {
            Option opt = opts[key];
            
            progress_value += progress_delta;
            progress_bar->setValue(progress_value);
            allUpdate();
            
            if (opt.value().isA<FileValue>())
            {
                conspireDebug() << opt.key() << "is a file" << opt.value().toString();
                
                //copy this file to the tmp directory
                QFile f(opt.value().toString());
                
//                status_label->setText(Conspire::tr("Copying file \"%1\"...")
//                                            .arg(opt.value().toString()));
                allUpdate();
                
                if (not f.open(QIODevice::ReadOnly))
                    emit( push( PagePointer( new ExceptionPage(
                        Conspire::tr("Error in submission! Cannot find the file "
                                     "\"%2\" which is required for option \"%1\".")
                                        .arg(opt.key(), opt.value().toString()),
                            Conspire::file_error( Conspire::tr("Cannot open the "
                                "file \"%2\" which is specified for option \"%1\".")
                                    .arg(opt.key(), opt.value().toString()), CODELOC ) 
                                        ) ) ) );

                QString short_name = QFileInfo(f).fileName();
                
                if (not f.copy(QString("%1/%2").arg(tmpdir,short_name)))
                    emit( push( PagePointer( new ExceptionPage(
                        Conspire::tr("Error in submission! Cannot copy the file "
                                     "\"%2\" which is required for option \"%1\".")
                                        .arg(opt.key(), opt.value().toString()),
                            Conspire::file_error( Conspire::tr("Cannot copy the "
                                "file \"%2\" which is specified for option \"%1\" "
                                "to the temporary directory \"%3\".")
                                    .arg(opt.key(), opt.value().toString(), tmpdir), 
                                        CODELOC ) ) ) ) );
            
                //update the options object so that it uses the local path for the file
                submit_opts = submit_opts.setNestedValue(key, short_name)
                                         .asA<Options>();
            }
        }
        
//        status_label->setText( Conspire::tr("Creating a config file...") );
        progress_bar->setValue(50);
        allUpdate();
         
        //now write the configuration file into this directory
        QFile conf(QString("%1/config").arg(tmpdir));
        
        if (not conf.open(QIODevice::WriteOnly))
            emit( push( PagePointer( new ExceptionPage(
                Conspire::tr("Error in submission! Could not write the config file."),
                            Conspire::file_error( Conspire::tr("Cannot open the "
                                "config file \"%1/config\".")
                                    .arg(tmpdir), CODELOC ) ) ) ) );

        //write the config file
        {
            QTextStream ts(&conf);
            ts << submit_opts.toConfig(true);
            conf.close();
        }

//        status_label->setText( Conspire::tr("Copying support files...") );
        progress_bar->setValue(60);
        allUpdate();

        //now copy all support files needed for this class of job
        //into the temporary file
        QDir class_dir(QString("%1/%2").arg(install_dir,job_class));
        class_dir.setFilter(QDir::Files);
        
        if (not class_dir.exists())
        {
            emit( push( PagePointer( new ExceptionPage(
                Conspire::tr("Error in submission! Could not find the job class "
                             "directory \"%1\".").arg(class_dir.absolutePath()),
                            Conspire::file_error( Conspire::tr("Cannot open the "
                                "class file directory \"%1\".")
                                    .arg(class_dir.absolutePath()), CODELOC ) ) ) ) );
        }
        
        conspireDebug() << "Reading job class directory" << class_dir.absolutePath();
        
        foreach (QString file, class_dir.entryList())
        {
            QFile f(QString("%1/%2").arg(class_dir.absolutePath(),file));
            
            QString f_name = QFileInfo(f).fileName();
            
            conspireDebug() << "Copying file" << f_name;
            
            if (not f.open(QIODevice::ReadOnly))
                throw Conspire::file_error( Conspire::tr(
                        "Cannot open class file \"%1\".")
                            .arg(file), CODELOC );
                            
            if (not f.copy(QString("%1/%2").arg(tmpdir,f_name)))
                throw Conspire::file_error( Conspire::tr(
                        "Cannot copy class file \"%1\" to temp directory \"%2\".")
                            .arg(file, tmpdir), CODELOC );
        
        }
                  
        //now tar up this whole directory into a workpacket
        {
//            status_label->setText( Conspire::tr("Constructing the workpacket...") );
            progress_bar->setValue(80);
            allUpdate();
    
            conspireDebug() << "Creating the workpacket...";
            QProcess proc;
            proc.setWorkingDirectory(tmpdir);
            
            QStringList args;

            QDir d(tmpdir);
            d.setFilter(QDir::Files);

            args << "-zLcvf" << "workpacket.tgz" << d.entryList();
            conspireDebug() << "tar" << args;
            
            proc.start("tar", args);
            
            proc.waitForFinished(-1);
            
            conspireDebug() << "STDOUT" << proc.readAllStandardOutput();
            conspireDebug() << "STDERR" << proc.readAllStandardError();
            
            conspireDebug() << "...workpacket created!";
        }
        
//        status_label->setText( Conspire::tr("Sending the job to the cloud...") );
        progress_bar->setValue(85);
        allUpdate();
        
        QProcessEnvironment env;
        env.insert("PARENT_NODE", broker);
        env.insert("PARENT_NODE_PORT", "10000");
        env.insert("ISSUBMIT", "TRUE");
        env.insert("WORKNAME", "workpacket");
        env.insert("CLASSNAME", job_class);
        env.insert("FILENAME", QString("%1/workpacket.tgz").arg(tmpdir));
        
        QProcess proc;
        proc.setProcessEnvironment(env);
        
        proc.start("python", 
            QStringList(QString("%1/leafhead3.py").arg(install_dir)));
        
        progress_bar->setValue(90);
        allUpdate();
        
        /*QTextStream ts(&proc);
        QStringList lines;
        
        proc.closeWriteChannel();
        proc.waitForStarted(-1);
        
        draw_file_progress_bar = true;
        num_bytes_expected = QFileInfo(QString("%1/workpacket.tgz").arg(tmpdir)).size();
        num_bytes_transferred = 0;
        
        while (proc.state() == QProcess::Running)
        {
            if (proc.waitForReadyRead(100))
            {
                if (proc.canReadLine())
                {
                    QString line = ts.readLine();
                    conspireDebug() << "OUTPUT" << line;
                    lines.append(line);
                    
                    bool ok = false;
                    int completed = line.toInt(&ok);
                    
                    if (ok)
                    {
                        num_bytes_transferred = completed;
                        this->repaint();
                    }
                }
            }
        }
        
        draw_file_progress_bar = false;*/
        
        proc.waitForFinished(-1);
        
        QByteArray out = proc.readAllStandardOutput();
        //QString out = lines.join("\n");
        QByteArray err = proc.readAllStandardError();
        
        conspireDebug() << "OUTPUT" << out;
        conspireDebug() << "ERROR" << err;
        
        conspireDebug() << "JOB SUBMITTED";
        
        if (proc.exitCode() != 0)
        {
//            status_label->setText( Conspire::tr("SUBMISSION FAILURE!!!") );
            progress_bar->setValue(0);
            allUpdate();
        
            //something went wrong
            emit( push( PagePointer( new ExceptionPage("Error in submission!", 
                                            unavailable_resource("Could not submit!", 
                                                CODELOC) ) ) ) );
        }

        stack->switchTo(2);
//        status_label->setText( Conspire::tr("Job is on the cloud!") );
        progress_bar->setValue(95);
        allUpdate();

        //get the JOB_ID and pass it to the job running page
        QRegExp re("ID on BROKER: (\\d+)");

        int pos = re.indexIn(out);
        
        if (pos == -1)
            throw Conspire::unavailable_resource("ERROR IN PARSING RESULT OF SUBMISSION!",
                                                    CODELOC );

        conspireDebug() << "JOB_ID ==" << re.cap(1);
        job_id = re.cap(1).toInt();
        
//        status_label->setText( Conspire::tr("Submitted the job "
//               "to the cloud! JOB_ID = %1.").arg(job_id) );
        progress_bar->setValue(100);
        allUpdate();
        
        button->setText( Conspire::tr("Get Status") );
        connect(button, SIGNAL(clicked()), this, SLOT(query()));
        button->show();
    }
    catch(...)
    {
        dir.rmpath(".");
        throw;
    }
    
    dir.rmpath(".");
    }
    catch(const Siren::Exception &e)
    {
        emit( push( PagePointer( new ExceptionPage(
            Conspire::tr("Error in submission!"), e) ) ) );
    }
}

void AccountPage::query()
{
    button->hide();
    
//    status_label->setText( Conspire::tr("Querying the job...") );
    progress_bar->setValue(0);
    allUpdate();
    
    //do the query
    //PARENT_NODE=127.0.0.1 PARENT_NODE_PORT=10000 ISQUERY=TRUE WKPTID=2 python ./leafhead3.py
    QProcessEnvironment env;
    env.insert("PARENT_NODE", broker);
    env.insert("PARENT_NODE_PORT", "10000");
    env.insert("ISQUERY", "TRUE");
    env.insert("WKPTID", QString::number(job_id));
    
    QProcess proc;
    proc.setProcessEnvironment(env);
    
    proc.start("python", 
        QStringList(QString("%1/leafhead3.py").arg(install_dir)));
    
    progress_bar->setValue(50);
    allUpdate();
    
    proc.waitForFinished(-1);
    
    QByteArray out = proc.readAllStandardOutput();
    QByteArray err = proc.readAllStandardError();
    
    conspireDebug() << "OUTPUT" << out;
    conspireDebug() << "ERROR" << err;

//    status_label->setText( Conspire::tr("Got a response...") );
    progress_bar->setValue(50);
    allUpdate();

    //WORKPACKET ID 0 on BROKER is *******
    // where ****** is unallocated, allocated, running, completed, or non existant
    QRegExp re("WORKPACKET ID (\\d+) on BROKER is ([\\w\\s]+)");
    
    int pos = re.indexIn(out);
    
    if (pos == -1)
    {
//        status_label->setText( Conspire::tr("Strange? Couldn't get job status.") );
        progress_bar->setValue(0);
        allUpdate();
    }
    else
    {
        QString status = re.cap(2).simplified();

        if (status == "running")
        {
            stack->switchTo(3);
        }
    
//        status_label->setText( Conspire::tr("The job status is \"%1\".").arg(status) );
        progress_bar->setValue(100);
        allUpdate();
    
        if (status == "completed")
        {
            stack->switchTo(1);
            button->setText( Conspire::tr("Get Results") );
            button->disconnect();
            connect(button, SIGNAL(clicked()), this, SLOT(getResults()));
        }
    }
    
    button->show();
}

void AccountPage::getResults()
{
    //button->disconnect();
    button->hide();
    
//    status_label->setText( Conspire::tr("Getting the results...") );
    progress_bar->setValue(0);
    allUpdate();

    //do the query
    //PARENT_NODE=127.0.0.1 PARENT_NODE_PORT=10000 ISQUERY=TRUE WKPTID=2 python ./leafhead3.py
    QProcessEnvironment env;
    env.insert("PARENT_NODE", broker);
    env.insert("PARENT_NODE_PORT", "10000");
    env.insert("ISDOWNLOAD", "TRUE");
    env.insert("WKPTID", QString::number(job_id));
    env.insert("OUTFILENAME", output_name);

    QProcess proc;
    proc.setProcessEnvironment(env);
    
    proc.start("python", 
        QStringList(QString("%1/leafhead3.py").arg(install_dir)));
    
    progress_bar->setValue(50);
    allUpdate();
    
    proc.waitForFinished(-1);
    
    QByteArray out = proc.readAllStandardOutput();
    QByteArray err = proc.readAllStandardError();
    
    conspireDebug() << "OUTPUT" << out;
    conspireDebug() << "ERROR" << err;

//    status_label->setText( Conspire::tr("Downloaded results to %1").arg(output_name) );
    progress_bar->setValue(100);

    button->show();
    allUpdate();

    QProcess proc2;
    proc2.start("open", QStringList() << output_name);

    proc2.waitForFinished(-1);
}
