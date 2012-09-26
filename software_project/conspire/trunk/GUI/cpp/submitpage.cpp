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

#include "Conspire/GUI/submitpage.h"
#include "Conspire/GUI/widgetrack.h"
#include "Conspire/GUI/button.h"
#include "Conspire/GUI/exceptionpage.h"
#include "Conspire/GUI/widgetstack.h"
#include "Conspire/GUI/uploadthread.h"
#include "Conspire/GUI/downloadthread.h"
#include "Conspire/GUI/global_var.h"
#include "Conspire/GUI/workstorepage.h"
#include "Conspire/GUI/mainwindow.h"
#include "Conspire/GUI/reservethread.h"

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
#include <QPixmap>
#include <QDesktopServices>
#include <QUuid>
#include <QSettings>
#include <QThreadPool>

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;

#include "config.h"

static QString install_dir = JOB_CLASSES_INSTALLATION_DIR;

void SubmitPage::build()
{
    job_id = -1;
    output_name = "results.tar.gz";
    draw_file_progress_bar = 0;
    num_bytes_expected = 0;
    num_bytes_transferred = 0;

    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);

    WidgetRack *rack = new WidgetRack(this);
    
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(::Qt::Vertical, this);
    
    this->setLayout(l);
    
    l->insertItem(0,rack);

    stack = new WidgetStack(this);
    
    WidgetRack *sub_rack = new WidgetRack(this);
    
    QLabel *label = new QLabel(Conspire::tr("When would you like the job to finish?"));
    sub_rack->addWidget(label);
    
    QComboBox *box = new QComboBox();
    box->addItem(Conspire::tr("As soon as possible!"));
    box->addItem(Conspire::tr("As soon as is practical."));
    box->addItem(Conspire::tr("Whenever is cheapest."));
    
    QGraphicsProxyWidget *box_proxy = new QGraphicsProxyWidget(this);
    box_proxy->setWidget(box);
    box_proxy->setZValue(100);
    
    sub_rack->addWidget(box_proxy);
    
    label = new QLabel(Conspire::tr("How energy efficient would you like to be?"));
    sub_rack->addWidget(label);
    
    box = new QComboBox();
    box->addItem(Conspire::tr("Use as little electricity as possible!"));
    box->addItem(Conspire::tr("Use as little electricity as is practical."));
    box->addItem(Conspire::tr("Burn as much electricity as is needed!"));

    box_proxy = new QGraphicsProxyWidget(this);
    box_proxy->setWidget(box);
    box_proxy->setZValue(100);

    sub_rack->addWidget(box_proxy);
    sub_rack->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    sub_rack->setZValue(100);
    
    stack->addWidget(sub_rack);
    
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
    
    rack->addWidget(stack);
    
    status_label = new QLabel(Conspire::tr("Click \"Start Job\" to run the job..."));
    rack->addWidget(status_label);

    progress_bar = new QProgressBar();
    progress_bar->setMinimum(0);
    progress_bar->setMaximum(100);
    rack->addWidget(progress_bar);
    
    button = new Button(Conspire::tr("Start Job"), this);
    rack->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(submit()));
}

/** Constructor */
SubmitPage::SubmitPage(Page *parent) : Page(parent)
{
    cancelled = 0;
    build();
}

/** Construct, passing in the options used to submit the job */
SubmitPage::SubmitPage(Options options, QString clas, Page *parent) 
           : Page(parent), opts(options), job_class(clas)
{
    cancelled = 0;
    build();
}

/** Destructor */
SubmitPage::~SubmitPage()
{}

void SubmitPage::cancellation()
{
   cancelled = 1;
   printf("cancellation triggered!\n");
}

void SubmitPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void SubmitPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void SubmitPage::paint(QPainter *painter, 
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

bool SubmitPage::removeDir(const QString &dirName)
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

void SubmitPage::cleanWorkstoreLocal(QString quuid)
{
   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   QString uploaddir = qsetter->value(quuid + "/uploaddir").toString();
   removeDir(uploaddir);
   qsetter->beginGroup(quuid);
   qsetter->remove("");
   qsetter->endGroup();
   delete qsetter;
}

void SubmitPage::allUpdate()
{
    status_label->update();
    progress_bar->update();
    QCoreApplication::processEvents();
}

void SubmitPage::submit()
{
    if (job_class.isEmpty())
        return;

    try
    {

    button->disconnect();
    button->hide();
    
    status_label->setText(Conspire::tr("Creating a new directory..."));
    progress_bar->setValue(5);

    allUpdate();

    conspireDebug() << "SUBMIT THE CALCULATION";

    stack->switchTo(1);

    //make a directory in which to stage the script
    QString overdatadir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
                    
    conspireDebug() << overdatadir;
    
    QUuid uuidgen;
    QString quuid = uuidgen.createUuid().toString().replace('{', ' ').replace('}', ' ').trimmed();
    
    QString datadir = overdatadir + "/input_" + quuid;
                
    QDir dir(datadir);
    if (not dir.mkpath("."))
        throw Siren::unavailable_resource(
                Conspire::tr("Cannot create a temporary directory! (%1)")
                        .arg(datadir), CODELOC );

    status_label->setText(Conspire::tr("Assembling the input files..."));
    progress_bar->setValue(10);
    allUpdate();
    
    Options submit_opts = opts;

    try
    {
        //go through the options and get all of the files - copy them into
        //the temporary directory
        QStringList all_keys = opts.keysAndIndiciesWithValue(true);
        
        double progress_value = 10;
        double progress_delta = all_keys.count() / 20.0;
        
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
                
                status_label->setText(Conspire::tr("Copying file \"%1\"...")
                                            .arg(opt.value().toString()));
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
                
                if (not f.copy(QString("%1/%2").arg(datadir,short_name)))
                    emit( push( PagePointer( new ExceptionPage(
                        Conspire::tr("Error in submission! Cannot copy the file "
                                     "\"%2\" which is required for option \"%1\".")
                                        .arg(opt.key(), opt.value().toString()),
                            Conspire::file_error( Conspire::tr("Cannot copy the "
                                "file \"%2\" which is specified for option \"%1\" "
                                "to the temporary directory \"%3\".")
                                    .arg(opt.key(), opt.value().toString(), datadir), 
                                        CODELOC ) ) ) ) );
            
                //update the options object so that it uses the local path for the file
                submit_opts = submit_opts.setNestedValue(key, short_name)
                                         .asA<Options>();
            }
        }
        
        status_label->setText( Conspire::tr("Creating a config file...") );
        progress_bar->setValue(30);
        allUpdate();
         
        //now write the configuration file into this directory
        QFile conf(QString("%1/config").arg(datadir));
        
        if (not conf.open(QIODevice::WriteOnly))
            emit( push( PagePointer( new ExceptionPage(
                Conspire::tr("Error in submission! Could not write the config file."),
                            Conspire::file_error( Conspire::tr("Cannot open the "
                                "config file \"%1/config\".")
                                    .arg(datadir), CODELOC ) ) ) ) );

        //write the config file
        {
            QTextStream ts(&conf);
            ts << submit_opts.toConfig(true);
            conf.close();
        }

        status_label->setText( Conspire::tr("Copying support files...") );
        progress_bar->setValue(35);
        allUpdate();

        //now copy all support files needed for this class of job
        //into the temporary file
        
        
        // HACK this needs to be updated to read the XML file!
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
                            
            if (not f.copy(QString("%1/%2").arg(datadir,f_name)))
                throw Conspire::file_error( Conspire::tr(
                        "Cannot copy class file \"%1\" to temp directory \"%2\".")
                            .arg(file, datadir), CODELOC );
        
        }
        
        status_label->setText(Conspire::tr("Recording directory for uploading..."));
        progress_bar->setValue(40);
        allUpdate();
        
        QSettings *qsetter = new QSettings("UoB", "AcquireClient");
        qsetter->setValue(quuid + "/uploaddir", datadir);
        delete qsetter;

        status_label->setText( Conspire::tr("Generating initial work descriptor...") );
        progress_bar->setValue(45);
        allUpdate();
        
        QString xmldescr = overdatadir + "/descr_" + quuid + ".xml";
        int retval = AcquireGenerateInitialXMLWorkDescriptor(xmldescr.toAscii().constData(), "Untitled work", "work/subwork/single", "single", 3600);
        
        if (retval == 0)
        {
                  emit( push( PagePointer( new ExceptionPage(
            Conspire::tr("Error in submission! Most likely could not write the XML descriptor "
                        "\"%1\" (or client state is corrupt).").arg(xmldescr),
                        Conspire::file_error( Conspire::tr("Cannot write the "
                           "file \"%1\".")
                              .arg(xmldescr), CODELOC ) ) ) ) );
        } else
        {
           status_label->setText(Conspire::tr("Recording XML descriptor filename..."));
           progress_bar->setValue(50);
           allUpdate();
         
           QSettings *qsetter = new QSettings("UoB", "AcquireClient");
           qsetter->setValue(quuid + "/descrxml", xmldescr);
           delete qsetter;
        }

        status_label->setText( Conspire::tr("Negotiating a remote work allocation...") );
        progress_bar->setValue(52);
        allUpdate();
        printf("total size of data directory: %d\n", ReserveThread::totalOfDirectory(datadir.toAscii().constData()));
        ReserveThread *rsrvth = new ReserveThread(xmldescr.toAscii().constData(), datadir.toAscii().constData());
        QThreadPool::globalInstance()->start(rsrvth);
        int lastblockscount = -1;
        while (!(rsrvth->isFinished()))
        {
           if (lastblockscount != rsrvth->getCurrentBlock())
           {
              if (cancelled) rsrvth->cancelReserve();
              status_label->setText( Conspire::tr("Compressing, encrypting and hashing block %1/%2...").arg(QString::number(rsrvth->getCurrentBlock()+1)).arg(QString::number(rsrvth->getTotalBlocks()+1)));
              progress_bar->setValue(52 + (95-52)*rsrvth->getBlockFraction());
              allUpdate();
              if (cancelled) rsrvth->cancelReserve();
              lastblockscount = rsrvth->getCurrentBlock();
           }
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        if ((cancelled) && (rsrvth->getFinalID() == NULL))
        {
           emit(pop(true));
           cleanWorkstoreLocal(quuid);
           return;
        }
        int blocks = rsrvth->getCurrentBlock();
        int64_t spaceused = rsrvth->getRemoteUsedSpace();
        int64_t localspaceused = rsrvth->getLocalUsedSpace();
        const char *workstore = rsrvth->getFinalID();
        int allinstances = 1;
        if (workstore)
        {
           QSettings *qsetter = new QSettings("UoB", "AcquireClient");
           qsetter->setValue(quuid + "/workstoreid", workstore);
           qsetter->setValue(quuid + "/remotespace", QString::number(spaceused));
           qsetter->setValue(quuid + "/localspace", QString::number(localspaceused));
           qsetter->setValue(quuid + "/blocks", QString::number(blocks));
           qsetter->setValue(quuid + "/instances", QString::number(allinstances));
           delete qsetter;
           
           status_label->setText( Conspire::tr("Done. Acquired work store ID.") );
           progress_bar->setValue(97);
           allUpdate();
           
           UploadThread *uploadthread = new UploadThread(workstore, datadir.toAscii().constData(), NULL, NULL, allinstances, 3600, blocks);
           GetUploadArray()->insert(workstore, uploadthread);
           QThreadPool::globalInstance()->start(uploadthread);
                      
           status_label->setText( Conspire::tr("Started upload") );
           progress_bar->setValue(100);
           allUpdate();
           
        }
        
        //emit( pop() );
        emit( pop2() );

/*        
        QSettings 
                  
        //now tar up this whole directory into a workpacket
        {
            status_label->setText( Conspire::tr("Constructing the workpacket...") );
            progress_bar->setValue(80);
            allUpdate();
    
            conspireDebug() << "Creating the workpacket...";
            QProcess proc;
            proc.setWorkingDirectory(datadir);
            
            QStringList args;

            QDir d(datadir);
            d.setFilter(QDir::Files);

            args << "-zLcvf" << "workpacket.tgz" << d.entryList();
            conspireDebug() << "tar" << args;
            
            proc.start("tar", args);
            
            proc.waitForFinished(-1);
            
            conspireDebug() << "STDOUT" << proc.readAllStandardOutput();
            conspireDebug() << "STDERR" << proc.readAllStandardError();
            
            conspireDebug() << "...workpacket created!";
        }
        
        status_label->setText( Conspire::tr("Sending the job to the cloud...") );
        progress_bar->setValue(85);
        allUpdate();
        
        QProcessEnvironment env;
        env.insert("PARENT_NODE", broker);
        env.insert("PARENT_NODE_PORT", "10000");
        env.insert("ISSUBMIT", "TRUE");
        env.insert("WORKNAME", "workpacket");
        env.insert("CLASSNAME", job_class);
        env.insert("FILENAME", QString("%1/workpacket.tgz").arg(datadir));
        
        QProcess proc;
        proc.setProcessEnvironment(env);
        
        proc.start("python", 
            QStringList(QString("%1/leafhead3.py").arg(install_dir)));
        
        progress_bar->setValue(90);
        allUpdate();
        
        
        proc.waitForFinished(-1);
        
        QByteArray out = proc.readAllStandardOutput();
        //QString out = lines.join("\n");
        QByteArray err = proc.readAllStandardError();
        
        conspireDebug() << "OUTPUT" << out;
        conspireDebug() << "ERROR" << err;
        
        conspireDebug() << "JOB SUBMITTED";
        
        if (proc.exitCode() != 0)
        {
            status_label->setText( Conspire::tr("SUBMISSION FAILURE!!!") );
            progress_bar->setValue(0);
            allUpdate();
        
            //something went wrong
            emit( push( PagePointer( new ExceptionPage("Error in submission!", 
                                            unavailable_resource("Could not submit!", 
                                                CODELOC) ) ) ) );
        }

        stack->switchTo(2);
        status_label->setText( Conspire::tr("Job is on the cloud!") );
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
        
        status_label->setText( Conspire::tr("Submitted the job "
               "to the cloud! JOB_ID = %1.").arg(job_id) );
        progress_bar->setValue(100);
        allUpdate();
        
        button->setText( Conspire::tr("Get Status") );
        connect(button, SIGNAL(clicked()), this, SLOT(query()));
        button->show();
    */
    }
    catch(...)
    {
//        dir.rmpath(".");
        throw;
    }
    
//    dir.rmpath(".");
    }
    catch(const Siren::Exception &e)
    {
        emit( push( PagePointer( new ExceptionPage(
            Conspire::tr("Error in submission!"), e) ) ) );
    }
}
