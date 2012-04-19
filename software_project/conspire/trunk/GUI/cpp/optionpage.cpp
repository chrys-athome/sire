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

#include "Conspire/GUI/optionpage.h"
#include "Conspire/GUI/exceptionpage.h"

#include "Conspire/values.h"
#include "Conspire/exceptions.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include <QLabel>
#include <QLineEdit>

using namespace Conspire;

/** Constructor */
OptionPage::OptionPage(Page *parent) : ConfigPage(parent)
{
    build();
}

/** Constructor, passing in the option to edit, and the root key of that option */
OptionPage::OptionPage(Option option, QString root_key, Page *parent)
           : ConfigPage(parent)
{
    build();
    setOption(option, root_key);
}
   
/** Destructor */
OptionPage::~OptionPage()
{}

/** Return the option being viewed and edited */
Option OptionPage::option() const
{
    return opt;
}

/** Return the root key of that option */
QString OptionPage::rootKey() const
{
    return root_key;
}

/** Build the widget */
void OptionPage::build()
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
    connect(value_edit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
    
    QGraphicsProxyWidget *value_edit_proxy = new QGraphicsProxyWidget(this);
    value_edit_proxy->setWidget(value_edit);
    
    l->addItem(value_edit_proxy);
}

/** Function called when editing of the option is finished */
void OptionPage::editingFinished()
{
    if (value_edit->text() == old_text)
        return;

    //try to update the option locally first. This catches parse errors
    //before they propogate up the document
    try
    {
        Obj new_val = StringValue(value_edit->text());

        Options opts(opt);
        opts.setNestedValue(opt.key(),new_val.asA<Value>());
        
        //this worked, so now set the global value
        QString key = opt.key();
    
        if (not root_key.isEmpty())
        {
            key = QString("%1.%2").arg(root_key, key);
        }
    
        emit( update(key,new_val) );
        emit( pop() );
    }
    catch(const Conspire::Exception &e)
    {
        emit( push(PagePointer( new ExceptionPage( Conspire::tr(
                    "Something went wrong when updating the value of the option \"%1\""
                    "to \"%2\".")
                        .arg(opt.key(), value_edit->text()), e) ) ) );
                        
        value_edit->setText( opt.value().toString() );
        old_text = value_edit->text();
    }
}

/** Set the option to be edited */
void OptionPage::setOption(Option option, QString key)
{
    opt = option;
    root_key = key;
    
    help_box->setText(opt.description());
    key_box->setText(opt.key());
    
    value_edit->setText(opt.value().toString());
    old_text = value_edit->text();
}

/** Slot called when the option has been updated */
void OptionPage::reread(Options options)
{
    Option opt = options[root_key];
    
    QString text = opt.value().toString();
    
    if (text != old_text)
    {
        value_edit->setText(text);
        old_text = text;
    }
}
