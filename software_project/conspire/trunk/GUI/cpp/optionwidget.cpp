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

#include "Conspire/GUI/optionwidget.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include <QLabel>
#include <QLineEdit>

using namespace Conspire;

/** Constructor */
OptionWidget::OptionWidget(QGraphicsItem *parent) : PageWidget(parent)
{
    build();
}

/** Constructor, passing in the option to edit, and the root key of that option */
OptionWidget::OptionWidget(Option option, QString root_key, QGraphicsItem *parent)
             : PageWidget(parent)
{
    build();
    setOption(option, root_key);
}
   
/** Destructor */
OptionWidget::~OptionWidget()
{}

/** Return the option being viewed and edited */
Option OptionWidget::option() const
{
    return opt;
}

/** Return the root key of that option */
QString OptionWidget::rootKey() const
{
    return root_key;
}

/** Build the widget */
void OptionWidget::build()
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(::Qt::Vertical, this);
    this->setLayout(l);
    
    help_box = new QLabel();
    key_box = new QLabel();
    
    QGraphicsProxyWidget *help_box_proxy = new QGraphicsProxyWidget(this);
    QGraphicsProxyWidget *key_box_proxy = new QGraphicsProxyWidget(this);
    
    help_box_proxy->setWidget(help_box);
    key_box_proxy->setWidget(key_box);
    
    l->addItem(key_box_proxy);
    l->addItem(help_box_proxy);
    
    value_edit = new QLineEdit();
    
    QGraphicsProxyWidget *value_edit_proxy = new QGraphicsProxyWidget(this);
    value_edit_proxy->setWidget(value_edit);
    
    l->addItem(value_edit_proxy);
}

/** Set the option to be edited */
void OptionWidget::setOption(Option option, QString key)
{
    opt = option;
    root_key = key;
    
    help_box->setText(opt.description());
    key_box->setText(opt.key());
    
    value_edit->setText(opt.value().toString());
}
