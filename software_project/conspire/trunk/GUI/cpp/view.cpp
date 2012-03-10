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
#include <QApplication>

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
            OptionView *view = new OptionView(opts[key], this);
            
            connect(view, SIGNAL(updatedOption(Option)), 
                    this, SLOT(childUpdated(Option)));

            connect(view, SIGNAL(updatedOption(Options)), 
                    this, SLOT(childUpdated(Options)));
        
            this->layout()->addWidget(view);
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

void OptionsView::childUpdated(Option option)
{
    Option old_option = opts[option.key()];
    
    if (old_option != option)
    {
        Options new_options = opts.update(option);
        emit( updatedOption(new_options) );
        opts = new_options;
    }
}

void OptionsView::childUpdated(Options options)
{
    throw Conspire::incomplete_code( Conspire::tr("Not written yet..."), CODELOC );
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

void OptionView::childUpdated(Option option)
{}

void OptionView::edited()
{
    try
    {
        //try to update the option
        Option newopt = opt.setNestedValue(opt.key(), edit->text()).asA<Option>();
        
        //ok - that was successful - now try to update the option in 
        //the complete options object...
        emit( updatedOption(newopt) );
        
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

///////////
/////////// Implementation of "OptionsControl"
///////////

OptionsControl::OptionsControl(QWidget *parent) : QWidget(parent)
{}

OptionsControl::OptionsControl(const Options &options, QWidget *parent)
               : QWidget(parent), opts(options)
{
    this->setLayout( new QVBoxLayout(this) );
    
    OptionsView *view = new OptionsView(opts, this);
    connect(view, SIGNAL(updatedOption(Options)), this, SLOT(updated(Options)));
    this->layout()->addWidget(view);

    QPushButton *undo = new QPushButton("Undo");
    QPushButton *redo = new QPushButton("Redo");
    QPushButton *save = new QPushButton("Save");
    QPushButton *load = new QPushButton("Load");
    QPushButton *quit = new QPushButton("Quit");

    this->layout()->addWidget(undo);
    this->layout()->addWidget(redo);
    this->layout()->addWidget(save);
    this->layout()->addWidget(load);
    this->layout()->addWidget(quit);
    
    connect(undo, SIGNAL(clicked()), this, SLOT(undo()));
    connect(redo, SIGNAL(clicked()), this, SLOT(redo()));
    connect(save, SIGNAL(clicked()), this, SLOT(save()));
    connect(load, SIGNAL(clicked()), this, SLOT(load()));
    connect(quit, SIGNAL(clicked()), this, SLOT(quit()));
}

OptionsControl::~OptionsControl()
{}

void OptionsControl::updated(Options options)
{
    opts = options;
    conspireDebug() << "Updated configuration!";
    conspireDebug() << opts.toConfig();
}

void OptionsControl::undo()
{}

void OptionsControl::redo()
{}

void OptionsControl::save()
{}

void OptionsControl::load()
{}

void OptionsControl::quit()
{
    QApplication::quit();
}
