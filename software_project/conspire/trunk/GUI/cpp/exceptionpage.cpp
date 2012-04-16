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

#include "Conspire/GUI/exceptionpage.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include <QLabel>
#include <QTextEdit>

using namespace Conspire;

/** Constructor */
ExceptionPage::ExceptionPage(QGraphicsItem *parent)
              : Page(parent)
{
    build();
}

/** Construct, passing in the error, and a description of what action was
    taken that caused the error */
ExceptionPage::ExceptionPage(QString description, const Exception &e,
                             QGraphicsItem *parent)
              : Page(parent)
{
    build();
    setException(description, e);
}
                
/** Destructor */
ExceptionPage::~ExceptionPage()
{}

/** Build the widget */
void ExceptionPage::build()
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(::Qt::Vertical, this);
    this->setLayout(l);
    
    short_error = new QLabel();
    long_error = new QTextEdit();
    long_error->setReadOnly(true);
    
    QGraphicsProxyWidget *short_error_proxy = new QGraphicsProxyWidget(this);
    QGraphicsProxyWidget *long_error_proxy = new QGraphicsProxyWidget(this);
    
    short_error_proxy->setWidget(short_error);
    long_error_proxy->setWidget(long_error);
    
    l->addItem(short_error_proxy);
    l->addItem(long_error_proxy);
}

void ExceptionPage::setException(QString description, const Exception &error)
{
    desc = description;
    e = error;
    
    short_error->setText(desc);
    long_error->setText(error.toString());
}
