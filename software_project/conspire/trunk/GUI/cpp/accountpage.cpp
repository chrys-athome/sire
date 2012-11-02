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

#include <vector>

#include "Conspire/GUI/accountpage.h"
#include "Conspire/GUI/userpage.h"
#include "Conspire/GUI/widgetrack.h"
#include "Conspire/GUI/button.h"
#include "Conspire/GUI/exceptionpage.h"
#include "Conspire/GUI/widgetstack.h"
#include "Conspire/GUI/chooseclusterpage.h"

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
#include <QXmlStreamReader>

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"
#include "config.h"

using namespace Conspire;

static QString install_dir = STATIC_INSTALL_DIR;

void AccountPage::addSSHAccount()
{
//   emit( push( PagePointer( new UserPage(1))));
   emit( push( PagePointer( new ChooseClusterPage())));
}

void AccountPage::refreshList()
{
   std::vector<QString> cluster_id_list;
   std::vector<QString> cluster_name_list;
       QFile *xmlFile = new QFile(QString("%1/clusterdata/acc_clusters.xml").arg(install_dir));

    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
       emit( push( PagePointer( new ExceptionPage(
            Conspire::tr("Error opening job classes XML file"),
                        Conspire::file_error( Conspire::tr("Cannot open the "
                           "file \"%1/clusterdata/acc_clusters.xml\".")
                              .arg(install_dir), CODELOC ) ) ) ) );
    }

    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);

    QString t_clusterid;
    QString t_clustername;
    
    while (!xmlReader->atEnd() && !xmlReader->hasError())
    {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if (token == QXmlStreamReader::StartDocument) continue;
        if (token == QXmlStreamReader::StartElement)
        {
           if (xmlReader->name() == "clusterdescription")
           {
              t_clusterid = QString((xmlReader->attributes().value("id")).toString());
              conspireDebug() << t_clusterid;
           }
           if (xmlReader->name() == "name") t_clustername = xmlReader->readElementText();
        }
        if (token == QXmlStreamReader::EndElement)
        {
           if (xmlReader->name() == "clusterdescription")
           {
              cluster_id_list.push_back(t_clusterid);
              cluster_name_list.push_back(t_clustername);
           }
        }
    }

    if (xmlReader->hasError())
    {
       emit( push( PagePointer( new ExceptionPage(
            Conspire::tr("Error in XML parsing."),
                        Conspire::file_error( Conspire::tr("Cannot open the "
                           "file \"%1/clusterdata/acc_clusters.xml\".")
                              .arg(install_dir), CODELOC ) ) ) ) );
    }

    xmlReader->clear();
    xmlFile->close();
    delete xmlReader;
    delete xmlFile;

   int items = clusterlist->count();
   for (int i = items; i--; )
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
      QString this_cluster_name = QString();
      QString this_cluster_id = QString(token);
      for (int i = 0; i < cluster_id_list.size(); i++)
      {
         if (this_cluster_id == cluster_id_list.at(i))
         {
            this_cluster_name = cluster_name_list.at(i);
         }
      }
      if (this_cluster_name.isEmpty())
      {
         this_cluster_name = QString("Unknown cluster (%1)").arg(this_cluster_id);
      }
      QListWidgetItem *wid = new QListWidgetItem(this_cluster_name);
      clusterlist->addItem(wid);
      haveany = 1;
      token = strtok_r(NULL, ",", &remainder);
      login_label->setText("Network OK.");
   }
   if (haveany == 0)
   {
      QListWidgetItem *none_item = new QListWidgetItem("None available...");
      clusterlist->addItem(none_item);
      login_label->setText("No clusters available");
   }
   AcquireClientClearResults();
}

void AccountPage::build()
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
    
    QLabel *intro_label = new QLabel(Conspire::tr("Cluster accounts using SSH must be added"
       " to Conspire in order to do work. Click here to add login details:"));
    intro_label->setWordWrap(true);
    intro_label->setMinimumSize(intro_label->sizeHint());
    sub_rack->addWidget(intro_label);
    
    modifybutton = new Button(Conspire::tr("Add..."));
    connect(modifybutton, SIGNAL(clicked()), this, SLOT(addSSHAccount()));
    sub_rack->addWidget(modifybutton);
    
    QLabel *clusters_avail = new QLabel(Conspire::tr("Clusters currently available"
       " to your Conspire account are listed below:"));
    clusters_avail->setWordWrap(true);
    clusters_avail->setMinimumSize(clusters_avail->sizeHint());
    sub_rack->addWidget(clusters_avail);
        
    clusterlist = new QListWidget();
    QListWidgetItem *none_item = new QListWidgetItem("None available...");
    clusterlist->addItem(none_item);
    sub_rack->addWidget(clusterlist);
    
    button = new Button(Conspire::tr("Refresh"));
    connect(button, SIGNAL(clicked()), this, SLOT(refreshList()));
    sub_rack->addWidget(button);
    
    return_button = new Button(Conspire::tr("Return"));
    connect(return_button, SIGNAL(clicked()), this, SIGNAL(pop()));
    sub_rack->addWidget(return_button);
    
    login_label = new QLabel("");
    sub_rack->addWidget(login_label);

    stack->addWidget(sub_rack);
    
    rack->addWidget(stack);
    refreshList();
}

/** Constructor */
AccountPage::AccountPage(Page *parent) : Page(parent)
{
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
}

void AccountPage::allUpdate()
{
   
    QCoreApplication::processEvents();
}
