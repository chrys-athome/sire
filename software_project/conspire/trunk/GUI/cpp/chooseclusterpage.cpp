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
#include "Conspire/GUI/clusterswidget.h"
#include "Conspire/GUI/chooseclusterpage.h"
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

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;

#include "config.h"

static QString install_dir = JOB_CLASSES_INSTALLATION_DIR;

void ChooseClusterPage::build()
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
    
    QLabel *label_table = new QLabel(Conspire::tr("Choose the cluster that you would like Conspire to use:"));
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
   
    QFile *xmlFile = new QFile(QString("%1/%2").arg(install_dir).arg("../acc_clusters.xml"));

    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
       emit( push( PagePointer( new ExceptionPage(
            Conspire::tr("Error opening job classes XML file"),
                        Conspire::file_error( Conspire::tr("Cannot open the "
                           "file \"%1/%2\".")
                              .arg(install_dir).arg("../acc_clusters.xml"), CODELOC ) ) ) ) );
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
           if (xmlReader->name() == "clusterdescription")
           {
              t_jobclassincludedirs.clear();
              t_jobclassid = QString(*(xmlReader->attributes().value("id").string()));
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
           if (xmlReader->name() == "clusterdescription")
           {
              ClustersWidget *this_jcw =
                 new ClustersWidget(numberof, all_jcw,
                                    t_jobclassid, t_jobclassname, t_jobclassdescription, t_jobclassicon,
                                    t_jobclassdirectory, t_jobclassxml, t_jobclassincludedirs);
              qlinear->addItem(this_jcw);
              all_jcw->append(this_jcw);
              connect(this_jcw, SIGNAL(push(PagePointer)), this, SIGNAL(push(PagePointer)));
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
                              .arg(install_dir).arg("../acc_clusters.xml"), CODELOC ) ) ) ) );
    }

    xmlReader->clear();
    xmlFile->close();
    delete xmlReader;
    delete xmlFile;
/*    
    ClustersWidget *none_jcw = new ClustersWidget("blah", "blah", "blah", "blah", "blah", "blah", QStringList("blah"));
    qlinear->addItem(none_jcw);
    all_jcw->append(none_jcw);
    connect(none_jcw, SIGNAL(push(PagePointer)), this, SIGNAL(push(PagePointer)));
    */
//    tableofworkstores = new QGraphicsWidget();
    qscene->addItem(tableofworkstores);
    connect(qscene, SIGNAL(mousePressEvent(QGraphicsSceneMouseEvent *)), tableofworkstores, SLOT(mousePressEvent(QGraphicsSceneMouseEvent *)));

    tableofworkstores->update();
    sub_rack->addWidget(qview);
    return_button = NULL;
    
    /*
    button = new Button(Conspire::tr("Refresh"));
    connect(button, SIGNAL(clicked()), this, SLOT(refreshWork()));
    sub_rack->addWidget(button);
    */

    return_button = new Button(Conspire::tr("Return"));
    connect(return_button, SIGNAL(clicked()), this, SIGNAL(pop()));
    sub_rack->addWidget(return_button);
 
    login_label = new QLabel();
    sub_rack->addWidget(login_label);

    stack->addWidget(sub_rack);

    rack->addWidget(stack);
    allUpdate();
}

/** Constructor */
ChooseClusterPage::ChooseClusterPage(Page *parent) : Page(parent)
{
    build();
}

/** Destructor */
ChooseClusterPage::~ChooseClusterPage()
{}

void ChooseClusterPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void ChooseClusterPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void ChooseClusterPage::paint(QPainter *painter, 
                        const QStyleOptionGraphicsItem *option, 
                        QWidget *widget)
{
    for (int i = 0; i < all_jcw->size(); i++)
    {
       ((ClustersWidget *)(all_jcw->at(i)))->update();
    }
    Page::paint(painter, option, widget);
}

void ChooseClusterPage::allUpdate()
{
    for (int i = 0; i < all_jcw->size(); i++)
    {
       ((ClustersWidget *)(all_jcw->at(i)))->update();
    }
    QCoreApplication::processEvents();
}
