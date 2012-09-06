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
#include "Conspire/GUI/userpage.h"
#include "Conspire/GUI/workpage.h"
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

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;

static QString install_dir 
                = "/home/benlong/conspire/job_classes";

static QString broker = "ssi-amrmmhd.epcc.ed.ac.uk";
//static QString broker = "127.0.0.1";

void UserPage::continueToWorkStores()
{
   emit( push( PagePointer( new WorkPage(1))));
}

void UserPage::modifyAccount()
{
   emit( push( PagePointer( new AccountPage(1))));
}

void UserPage::build()
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
    
       lineedit_host = new QLineEdit();
    if (usemode == 1)
    {
       QLabel *intro_label = new QLabel(Conspire::tr("For Acquire to push your work to a cluster, "
          "trusted machine logins must be added to your account. These are transparently appended "
          "to an encrypted remote keyring which can only be opened with your main password."));
       intro_label->setWordWrap(true);
       intro_label->adjustSize();
       sub_rack->addWidget(intro_label);
       QLabel *label_host = new QLabel(Conspire::tr("Machine to add to remote SSH keyring:"));
       label_host->setFocusPolicy(::Qt::NoFocus);
       sub_rack->addWidget(label_host);
       lineedit_host->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
       lineedit_host->setPlaceholderText("Hostname");
       sub_rack->addWidget(lineedit_host);
    }
    
    if (usemode == 0)
    {
       QLabel *hello_label_unknown = new QLabel(Conspire::tr("Hello. Please log in."));
       sub_rack->addWidget(hello_label_unknown);
    }
    QLabel *label_username = new QLabel(Conspire::tr("Username:"));
    label_username->setFocusPolicy(::Qt::NoFocus);
    sub_rack->addWidget(label_username);
    lineedit_username = new QLineEdit();
    lineedit_username->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    lineedit_username->setPlaceholderText("Username");
    lineedit_username->setFocusPolicy(::Qt::StrongFocus);
    sub_rack->addWidget(lineedit_username);

    QLabel *label_password = new QLabel(Conspire::tr("Password:"));
    label_password->setFocusPolicy(::Qt::NoFocus);
    sub_rack->addWidget(label_password);
    lineedit_password = new QLineEdit();
    lineedit_password->setEchoMode(QLineEdit::Password);
    lineedit_password->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    lineedit_password->setPlaceholderText("Password");
    lineedit_password->setFocusPolicy(::Qt::StrongFocus);
    connect(lineedit_password, SIGNAL(returnPressed()), this, SLOT(login()));
    sub_rack->addWidget(lineedit_password);

    login_label = new QLabel();
    sub_rack->addWidget(login_label);

    if (usemode == 1)
    {
        button = new Button(Conspire::tr("Add"), this);
        sub_rack->addWidget(button);
        connect(button, SIGNAL(clicked()), this, SLOT(sshadd()));
        lineedit_host->setFocus();
    }
    
    stack->addWidget(sub_rack);
    
    WidgetRack *sub_rack2 = new WidgetRack(this);
    
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    last_username = qsetter->value("preferences/lastUsername", QString("")).toString();
    delete qsetter;
    
    QLabel *hello_label_known = new QLabel(Conspire::tr("Hello %1. Please type your password.").arg(last_username));
    sub_rack2->addWidget(hello_label_known);
    QLabel *label_password_known = new QLabel(Conspire::tr("Password:"));
    sub_rack2->addWidget(label_password_known);
    lineedit_password_known = new QLineEdit();
    lineedit_password_known->setEchoMode(QLineEdit::Password);
    lineedit_password_known->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    lineedit_password_known->setPlaceholderText("Password");
    connect(lineedit_password_known, SIGNAL(returnPressed()), this, SLOT(login()));
    sub_rack2->addWidget(lineedit_password_known);
    QLabel *ifyouare_label_loggedin2 = new QLabel(Conspire::tr("If you are not %1,"
       " or would like to use another account, please use the 'Change user' button below").arg(last_username));
    ifyouare_label_loggedin2->setWordWrap(true);
    sub_rack2->addWidget(ifyouare_label_loggedin2);

    chgusr1_button = new Button(Conspire::tr("Change user..."));
    connect(chgusr1_button, SIGNAL(clicked()), this, SLOT(changeUser()));
    sub_rack2->addWidget(chgusr1_button);
    
    stack->addWidget(sub_rack2);
    
    WidgetRack *sub_rack3 = new WidgetRack(this);
    
    QLabel *hello_label_loggedin = new QLabel(Conspire::tr("Hello %1. You are logged in.").arg(last_username));
    sub_rack3->addWidget(hello_label_loggedin);
    QLabel *ifyouare_label_loggedin = new QLabel(Conspire::tr("If you are not %1,"
       " or would like to use another account, please use the 'Change user' button below").arg(last_username));
    ifyouare_label_loggedin->setWordWrap(true);
    sub_rack3->addWidget(ifyouare_label_loggedin);
    
    chgusr2_button = new Button(Conspire::tr("Change user..."));
    connect(chgusr2_button, SIGNAL(clicked()), this, SLOT(changeUser()));
    sub_rack3->addWidget(chgusr2_button);
    
    continuebutton = new Button(Conspire::tr("To work..."));
    connect(continuebutton, SIGNAL(clicked()), this, SLOT(continueToWorkStores()));
    sub_rack3->addWidget(continuebutton);
    
    modifybutton = new Button(Conspire::tr("Modify account..."));
    connect(modifybutton, SIGNAL(clicked()), this, SLOT(modifyAccount()));
    sub_rack3->addWidget(modifybutton);
    
    stack->addWidget(sub_rack3);
    
    /*
    QLabel *submitting_label = new QLabel("SUBMITTING THE JOB...");
    
    submitting_label->setScaledContents(true);
    submitting_label->setPixmap( QPixmap(":data.jpg") );
    submitting_label->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,
                                                 QSizePolicy::Expanding) );
    submitting_label->setMinimumSize( QSize(10,10) );
    
    stack->addWidget(submitting_label);
    
    QLabel *cloud_label = new QLabel("CLOUD");
    
    cloud_label->setScaledContents(true);
    cloud_label->setPixmap( QPixmap(":cloud.jpg") );
    cloud_label->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    cloud_label->setMinimumSize( QSize(10,10) );
    
    stack->addWidget(cloud_label);
    
    QLabel *emerald_label = new QLabel("EMERALD");
    
    emerald_label->setScaledContents(true);
    emerald_label->setPixmap( QPixmap(":cluster.jpg") );
    emerald_label->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    emerald_label->setMinimumSize( QSize(10,10) );
    
    stack->addWidget(emerald_label);
    */
    rack->addWidget(stack);
    
    submode = (strlen(last_username.toAscii().constData()) > 0);
    
    if (usemode == 0)
    {
       stack->switchTo(submode);
       if (submode)
       {
           button = new Button(Conspire::tr("Log in"), this);
           rack->addWidget(button);
           connect(button, SIGNAL(clicked()), this, SLOT(login()));
           lineedit_password_known->setFocus();
       } else
       {
           button = new Button(Conspire::tr("Log in"), this);
           rack->addWidget(button);
           connect(button, SIGNAL(clicked()), this, SLOT(login()));
           lineedit_username->setFocus();
       }
    }
    
    //QWidget::setTabOrder(lineedit_host, lineedit_username);
    //QWidget::setTabOrder(lineedit_username, lineedit_password);
}

/** Constructor */
UserPage::UserPage(int iusemode, Page *parent) : Page(parent)
{
//   usemode = AcquireClientIsInitialised();
    usemode = iusemode;
    build();
}

/** Destructor */
UserPage::~UserPage()
{}

void UserPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void UserPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void UserPage::paint(QPainter *painter, 
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

void UserPage::allUpdate()
{
    //status_label->update();
    //progress_bar->update();
    QCoreApplication::processEvents();
}

QString UserPage::addMachine(QString username, QString password, QString machinename, bool *loginsuccessful)
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

QString UserPage::loginUser(QString username, QString password, bool *loginsuccessful)
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

void UserPage::changeUser()
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

void UserPage::login()
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

void UserPage::sshadd()
{
    QString susername = lineedit_username->text();
    QString machinename = lineedit_host->text();
    QString spassword = lineedit_password->text();
    
    bool wasokay = FALSE;
    QString sshadd_retstring = addMachine(susername, spassword, machinename, &wasokay);

    if (wasokay) button->hide();
    login_label->setText(sshadd_retstring);
    allUpdate();
    if (wasokay) emit( pop() );
}

void UserPage::submit()
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

void UserPage::query()
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

void UserPage::getResults()
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
