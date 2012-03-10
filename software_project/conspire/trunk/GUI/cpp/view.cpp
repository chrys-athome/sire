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

#include "Conspire/GUI/view.h"
#include "Conspire/option.h"
#include "Conspire/exceptions.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>

using namespace Conspire;

///////////
/////////// Implementation of "View"
///////////

/** Constructor */
View::View(QWidget *parent) : QLabel(parent)
{}


/** Construct from the passed set of Options */
View::View(const Options &options, QWidget *parent) : QLabel(parent)
{
    this->setTextFormat( ::Qt::PlainText );
    this->setText( options.toString() );
}

/** Destructor */
View::~View()
{}

///////////
/////////// Implementation of "OptionsView"
///////////

OptionsView::OptionsView(QWidget *parent) : QWidget(parent)
{}

OptionsView::OptionsView(const Options &options, QWidget *parent)
            : QWidget(parent), opts(options)
{
    this->setLayout( new QVBoxLayout(this) );
    
    Qt::StringList keys = opts.keys();
    
    bool can_add = false;
    
    foreach (Qt::String key, keys)
    {
        Option opt = opts[key];

        if (opt.hasValue() or (not opt.isOptional()))
        {
            this->layout()->addWidget( new OptionView( opts[key], this ) );
            
            if (opt.allowMultiple())
                can_add = true;
        }
        else
            can_add = true;
    }
    
    if (can_add)
        this->layout()->addWidget( new QPushButton("Add",this) );
}

OptionsView::~OptionsView()
{}

Options OptionsView::options() const
{
    return opts;
}

///////////
/////////// Implementation of "OptionView"
///////////

OptionView::OptionView(QWidget *parent) : QWidget(parent)
{}

OptionView::OptionView(const Option &option, QWidget *parent)   
           : QWidget(parent), opt(option)
{
    this->setLayout( new QHBoxLayout(this) );
    
    QPushButton *b = new QPushButton("?", this);
    connect(b, SIGNAL(clicked()), this, SLOT(helpClicked()));
    this->layout()->addWidget(b);

    this->layout()->addWidget( new QLabel(option.key(),this) );

    edit = new QLineEdit(this);
    
    edit->setText( opt.value().toString() );
    this->layout()->addWidget(edit);
    
    connect(edit, SIGNAL(returnPressed()), this, SLOT(edited()));
}

OptionView::~OptionView()
{}

Option OptionView::option() const
{
    return opt;
}

void OptionView::helpClicked() const
{
    QMessageBox msgbox;
    msgbox.setText( opt.description() );
    msgbox.exec();
}

void OptionView::edited()
{
    try
    {
        Option newopt = opt.setNestedValue(opt.key(), edit->text()).asA<Option>();
        opt = newopt;
        edit->setText(opt.value().toString());
    }
    catch(const Conspire::Exception &e)
    {
        conspireDebug() << "EXCEPTION THROWN:";
        conspireDebug() << e.toString();
        QMessageBox::warning(this, "Conspire", e.why(), QMessageBox::Discard);
        edit->setText( opt.value().toString() );
    }
}
