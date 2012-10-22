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
#include <QTableWidget>
#include <QHeaderView>

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;

void WorkPage::refreshWork()
{
   printf("refresh work triggered\n");
   login_label->setText("");
   char **store_ids = NULL;
   float *pct_b2c = NULL;
   float *pct_wrk = NULL;
   float *pct_c2b = NULL;
   int noofws = 0;
   int64_t bytesfree = 0;
   int retval = AcquireQueryAllWorkStatus(&store_ids, &pct_b2c, &pct_wrk, &pct_c2b, &noofws, &bytesfree);
   if ((retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS) || (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS_NO_WORK))
   {
      if (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS) login_label->setText(QString("Success. %1 bytes free").arg(QString::number(bytesfree)));
      if (retval == ACQUIRE_QUERY_ALL_WORK__SUCCESS_NO_WORK) login_label->setText(QString("Success. No work found. %1 bytes free").arg(QString::number(bytesfree)));
      int row = 0;
      int col = 0;
      QTableWidgetItem *qttwi = tableofworkstores->item(row, col);
      while (qttwi != NULL)
      {
         tableofworkstores->removeCellWidget(row, col);
         delete qttwi;
         col++;
         if (col >= 3) { col = 0; row++; }
         qttwi = tableofworkstores->item(row, col);
      }
      row = 0;
      col = 0;
      for (int i = 0; i < noofws; i++)
      {
         qttwi = new QTableWidgetItem("Untitled work");
         qttwi->setData(::Qt::UserRole, QString(store_ids[i]));
         tableofworkstores->setItem(row, col, qttwi);
         col++;
         if (col >= 3) { col = 0; row++; }
      }
      QTableWidgetItem *new_item = new QTableWidgetItem("Create new...");
      tableofworkstores->setItem(row, col, new_item);
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

void WorkPage::modifyWork(int row, int col)
{
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
//      makeWork();
   } else
   {
      emit( push(new WorkStorePage(qwidget->data(::Qt::UserRole).toString())));
   }
}

void WorkPage::build()
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
    
    QLabel *label_table = new QLabel(Conspire::tr("Your work currently in the Conspire cloud:"));
    sub_rack->addWidget(label_table);

    tableofworkstores = new QTableWidget(3, 3);
    tableofworkstores->horizontalHeader()->hide();
    tableofworkstores->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tableofworkstores->verticalHeader()->hide();
    tableofworkstores->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    tableofworkstores->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    tableofworkstores->setFrameStyle(::QFrame::Sunken | ::QFrame::StyledPanel);

    QTableWidgetItem *new_item = new QTableWidgetItem("Create new...");
    tableofworkstores->setItem(0, 0, new_item);
    connect(tableofworkstores, SIGNAL(cellClicked(int, int)), this, SLOT(modifyWork(int, int)));    
    
    sub_rack->addWidget(tableofworkstores);
    
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
}

/** Constructor */
WorkPage::WorkPage(Page *parent) : Page(parent)
{
    build();
}

/** Destructor */
WorkPage::~WorkPage()
{}

void WorkPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void WorkPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void WorkPage::paint(QPainter *painter, 
                       const QStyleOptionGraphicsItem *option, 
                       QWidget *widget)
{
    tableofworkstores->repaint();
    Page::paint(painter, option, widget);
}

void WorkPage::allUpdate()
{
   
    QCoreApplication::processEvents();
}
