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
#include "Conspire/GUI/jobclasswidget.h"
#include "Conspire/GUI/chooseclasspage.h"
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
#include <QXmlStreamReader>
#include <QUuid>

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;

#include "config.h"

static QString install_dir = STATIC_INSTALL_DIR;

void ChooseClassPage::selectJobClass()
{
   QSettings *qsetter = new QSettings("UoB", "AcquireClient");
   QString t_jobclass = qsetter->value(quuid + "/jobclass").toString();
   delete qsetter;
   if (t_jobclass.isEmpty())
   {
       login_label->setText("<font color='Red'><b><center>Please select a job class to create.</center></b></font>");
       return;
   }
   if (lineedit_jobname->text().isEmpty())
   {
       login_label->setText("<font color='Red'><b><center>Please name your new job.</center></b></font>");
       return;
   }
   qsetter = new QSettings("UoB", "AcquireClient");
   qsetter->setValue(quuid + "/jobname", lineedit_jobname->text());
   qsetter->setValue(quuid + "/user", QString(AcquireClientGetUsername()));
   delete qsetter;
   

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
    QString t_jobclassname;
    QString t_jobclassdirectory;
    QString t_jobclassxml;
    QStringList t_jobclassincludedirs;
    
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
           if (xmlReader->name() == "name") t_jobclassname = xmlReader->readElementText();
           if (xmlReader->name() == "directory") t_jobclassdirectory = xmlReader->readElementText();
           if (xmlReader->name() == "optionsxml") t_jobclassxml = xmlReader->readElementText();
           if (xmlReader->name() == "optionsincludedirs") t_jobclassincludedirs << xmlReader->readElementText();
        }
        if (token == QXmlStreamReader::EndElement)
        {
           if (xmlReader->name() == "jobclassdescription")
           {
               if (t_jobclass == t_jobclassid)
                  break;
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
    Options opts = Options::fromXMLFile(QString("%1").arg(t_jobclassxml), path);

    qDebug() << "Looking for a default config file...";
    QFileInfo default_config( QString("%1/job_classes/%2/default_config").arg(install_dir,t_jobclassdirectory) );

    qDebug() << default_config.absoluteFilePath();

    if (default_config.exists())
    {
        qDebug() << "Adding in options from the default configuration file...";
        opts = opts.fromConfigFile( default_config.absoluteFilePath() );
    }

    conspireDebug() << "PUSH CONFIGDOC";
    emit( push( PagePointer(new ConfigDocument(t_jobclassdirectory, opts, quuid)), true) );
    conspireDebug() << "PUSHED!";
}

void ChooseClassPage::deleteNewJob()
{
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    QString t_jobclass = qsetter->value(quuid + "/jobclass").toString();
    QString t_jobname = qsetter->value(quuid + "/jobname").toString();
    delete qsetter;
    if (t_jobname.isEmpty())
    {
        QSettings *qsetter = new QSettings("UoB", "AcquireClient");
        qsetter->beginGroup(quuid);
        qsetter->remove("");
        qsetter->endGroup();
        delete qsetter;
    }
    emit( pop(true) );
}

void ChooseClassPage::build(QString clstext)
{
    //this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    if (quuid.isEmpty())
    {
       QUuid uuidgen;
       quuid = uuidgen.createUuid().toString().replace('{', ' ').replace('}', ' ').trimmed();
    }
    
    WidgetRack *rack = new WidgetRack(this);
    
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(::Qt::Vertical, this);
    
    this->setLayout(l);
    
    l->insertItem(0,rack);

    stack = new WidgetStack(this);
    stack->setFocusPolicy(::Qt::NoFocus);
    
    WidgetRack *sub_rack = new WidgetRack(this);
    sub_rack->setFocusPolicy(::Qt::NoFocus);

    WidgetRack *sub_sub_rack = new WidgetRack(::Qt::Horizontal, this);
    QLabel *label_jobname = new QLabel("Job name: ");
    sub_sub_rack->addWidget(label_jobname);
    lineedit_jobname = new QLineEdit();
    lineedit_jobname->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    lineedit_jobname->setPlaceholderText("Give this job a name");
    lineedit_jobname->setFocusPolicy(::Qt::StrongFocus);
    if (not clstext.isEmpty())
       lineedit_jobname->setText(clstext);
    sub_sub_rack->addWidget(lineedit_jobname);
    sub_rack->addWidget(sub_sub_rack);
    connect(lineedit_jobname, SIGNAL(textChanged(QString)), this, SLOT(saveNewJobName(QString)));
    
    QLabel *label_table = new QLabel(Conspire::tr("Choose the job that you would like to do:"));
    sub_rack->addWidget(label_table);
    
    int numberof = 0;
    
    all_jcw = new QList<QGraphicsLayoutItem *>();

    qscene = new QGraphicsScene();
    qview = new QGraphicsView(qscene);
    qview->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    qview->setAutoFillBackground(true);

    qlinear = new QGraphicsLinearLayout(::Qt::Vertical);
    qlinear->setSpacing(0);
    tableofworkstores = new QGraphicsWidget();
    tableofworkstores->setLayout(qlinear);
   
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
    QString t_jobclassname;
    QString t_jobclassdescription;
    QString t_jobclassicon;
    QString t_jobclassdirectory;
    QString t_jobclassxml;
    QStringList t_jobclassincludedirs;
    
    while (!xmlReader->atEnd() && !xmlReader->hasError())
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if (token == QXmlStreamReader::StartDocument) continue;
        if (token == QXmlStreamReader::StartElement)
        {
           if (xmlReader->name() == "jobclassdescription")
           {
              t_jobclassincludedirs.clear();
              t_jobclassid = xmlReader->attributes().value("id").toString();
           }
           if (xmlReader->name() == "name") t_jobclassname = xmlReader->readElementText();
           if (xmlReader->name() == "description") t_jobclassdescription = xmlReader->readElementText();
           if (xmlReader->name() == "icon") t_jobclassicon = xmlReader->readElementText();
           if (xmlReader->name() == "directory") t_jobclassdirectory = xmlReader->readElementText();
           if (xmlReader->name() == "optionsxml") t_jobclassxml = xmlReader->readElementText();
           if (xmlReader->name() == "optionsincludedirs") t_jobclassincludedirs << xmlReader->readElementText();
        }
        if (token == QXmlStreamReader::EndElement)
        {
           if (xmlReader->name() == "jobclassdescription")
           {
              JobClassWidget *this_jcw =
                 new JobClassWidget(numberof, all_jcw,
                                    t_jobclassid, t_jobclassname, t_jobclassdescription, t_jobclassicon,
                                    t_jobclassdirectory, t_jobclassxml, t_jobclassincludedirs, quuid);
              qlinear->addItem(this_jcw);
              all_jcw->append(this_jcw);
              connect(this_jcw, SIGNAL(push(PagePointer, bool)), this, SIGNAL(push(PagePointer, bool)));
              numberof++;
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
/*    
    JobClassWidget *none_jcw = new JobClassWidget("blah", "blah", "blah", "blah", "blah", "blah", QStringList("blah"));
    qlinear->addItem(none_jcw);
    all_jcw->append(none_jcw);
    connect(none_jcw, SIGNAL(push(PagePointer)), this, SIGNAL(push(PagePointer)));
    */
//    tableofworkstores = new QGraphicsWidget();
    qscene->addItem(tableofworkstores);
    connect(qscene, SIGNAL(mousePressEvent(QGraphicsSceneMouseEvent *)), tableofworkstores, SLOT(mousePressEvent(QGraphicsSceneMouseEvent *)));

    tableofworkstores->update();
    QGraphicsProxyWidget *view_proxy = new QGraphicsProxyWidget(this);
    view_proxy->setWidget(qview);
    view_proxy->setZValue(-100);
    sub_rack->addWidget(view_proxy);
    
    sub_rack->addWidget(qview);
    return_button = NULL;
    
    login_label = new QLabel();
    sub_rack->addWidget(login_label);
    
    button = new Button(Conspire::tr("Create"));
    connect(button, SIGNAL(clicked()), this, SLOT(selectJobClass()));
    sub_rack->addWidget(button);

    return_button = new Button(Conspire::tr("Cancel"));
    connect(return_button, SIGNAL(clicked()), this, SLOT(deleteNewJob()));
    sub_rack->addWidget(return_button);
 
    stack->addWidget(sub_rack);

    rack->addWidget(stack);
    allUpdate();
}

/** Constructor */
ChooseClassPage::ChooseClassPage(QString clstext, QString iquuid, Page *parent) : Page(parent)
{
    quuid = iquuid;
    build(clstext);
}

/** Constructor */
ChooseClassPage::ChooseClassPage(Page *parent) : Page(parent)
{
    build(QString());
}

/** Destructor */
ChooseClassPage::~ChooseClassPage()
{}

void ChooseClassPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void ChooseClassPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void ChooseClassPage::paint(QPainter *painter, 
                        const QStyleOptionGraphicsItem *option, 
                        QWidget *widget)
{
    for (int i = 0; i < all_jcw->size(); i++)
    {
       ((JobClassWidget *)(all_jcw->at(i)))->update();
    }
    Page::paint(painter, option, widget);
}

void ChooseClassPage::allUpdate()
{
    for (int i = 0; i < all_jcw->size(); i++)
    {
       ((JobClassWidget *)(all_jcw->at(i)))->update();
    }
    QCoreApplication::processEvents();
}
