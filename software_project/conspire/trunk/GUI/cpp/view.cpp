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
#include <QUndoStack>

using namespace Conspire;

///////////
/////////// Implementation of "View"
///////////

/** Constructor */
View::View(QWidget *parent) : QWidget(parent)
{}

/** Destructor */
View::~View()
{}

View* View::parentView()
{
    QObject *p = this->parent();
    
    if (p)
    {
        View *parent = dynamic_cast<View*>(p);
        
        if (parent)
            return parent;
    }
    
    return 0;
}

const View* View::parentView() const
{
    const QObject *p = this->parent();
    
    if (p)
    {
        const View *parent = dynamic_cast<const View*>(p);
        
        if (parent)
            return parent;
    }
    
    return 0;
}

View* View::rootNode()
{
    View *parent = this->parentView();
    
    if (parent)
        return parent->rootNode();
    else
        return this;
}

const View* View::rootNode() const
{
    const View *parent = this->parentView();
    
    if (parent)
        return parent->rootNode();
    else
        return this;
}

QString View::rootKey() const
{
    const View *parent = this->parentView();
    
    if (parent)
        return parent->key();
    else
        return QString::null;
}

QString View::key() const
{
    const View *parent = this->parentView();
    
    if (parent)
        return parent->key();
    else
        return QString::null;
}

void View::updateOption(QString key, QString value)
{
    emit( setOption(key,value) );
}

///////////
/////////// Implementation of "OptionsView"
///////////

OptionsView::OptionsView(QWidget *parent) : View(parent)
{}

OptionsView::OptionsView(const Options &options, QWidget *parent)
            : View(parent)
{
    this->setLayout( new QVBoxLayout(this) );
    
    Qt::StringList keys = options.keys();
    
    bool can_add = false;
    
    foreach (Qt::String key, keys)
    {
        Option opt = options[key];

        if (opt.hasValue() or (not opt.isOptional()))
        {
            OptionView *view = new OptionView(opt,this);
            this->layout()->addWidget(view);
            views.insert(key, view);
        }
        else
            can_add = true;
    }
    
    if (can_add)
        this->layout()->addWidget( new QPushButton("Add",this) );
}

OptionsView::~OptionsView()
{}

///////////
/////////// Implementation of "OptionView"
///////////

OptionView::OptionView(View *parent) : View(parent)
{}

OptionView::OptionView(const Option &option, View *parent)   
           : View(parent)
{
    QString root_key = this->rootKey();
    
    if (not root_key.isEmpty())
    {
        k = QString("%1.%2").arg(root_key, option.key());
    }
    else
    {
        k = option.key();
    }

    v = option.value().toString();

    help_text = option.description();

    this->setLayout( new QHBoxLayout(this) );
    
    QPushButton *b = new QPushButton("?", this);
    connect(b, SIGNAL(clicked()), this, SLOT(helpClicked()));
    this->layout()->addWidget(b);

    this->layout()->addWidget( new QLabel(option.key(),this) );

    edit = new QLineEdit(this);
    
    edit->setText(v);
    this->layout()->addWidget(edit);
    
    connect(edit, SIGNAL(returnPressed()), this, SLOT(edited()));

    View *root_node = this->rootNode();
    
    if (root_node)
    {
        connect(this, SIGNAL(setOption(QString,QString)),
                root_node, SLOT(updateOption(QString,QString)));
    }
}

OptionView::~OptionView()
{}

void OptionView::helpClicked() const
{
    QMessageBox msgbox;
    msgbox.setText( help_text );
    msgbox.exec();
}

QString OptionView::key() const
{
    return k;
}

void OptionView::edited()
{
    try
    {
        emit( setOption(key(), edit->text()) );
    }
    catch(const Conspire::Exception &e)
    {
        conspireDebug() << "EXCEPTION THROWN:";
        conspireDebug() << e.toString();
        QMessageBox::warning(this, "Conspire", e.why(), QMessageBox::Discard);
        edit->setText(v);
    }
}

///////////
/////////// Implementation of "OptionsCommand"
///////////

OptionsCommand::OptionsCommand()
{}

OptionsCommand::OptionsCommand(Options oldstate, QString key, QString value)
               : old_state(oldstate), k(key), v(value)
{}

OptionsCommand::OptionsCommand(const OptionsCommand &other)
               : old_state(other.old_state), k(other.k), v(other.v)
{}

OptionsCommand::~OptionsCommand()
{}

OptionsCommand& OptionsCommand::operator=(const OptionsCommand &other)
{
    old_state = other.old_state;
    k = other.k;
    v = other.v;
}

QString OptionsCommand::key() const
{
    return k;
}

QString OptionsCommand::oldValue() const
{
    return old_state.getNestedValue(key()).toString();
}

QString OptionsCommand::newValue() const
{
    return v;
}

QString OptionsCommand::changedText() const
{
    return QString("Set \"%1\" equal to \"%2\"")
                .arg(key(), newValue());
}

Options OptionsCommand::oldState() const
{
    return old_state;
}

///////////
/////////// Implementation of "OptionsUndoCommand"
///////////

OptionsUndoCommand::OptionsUndoCommand() : QUndoCommand(), control(0)
{}

OptionsUndoCommand::OptionsUndoCommand(OptionsControl *parent,
                                       OptionsCommand command)
                   : control(parent), cmd(command)
{}
               
OptionsUndoCommand::~OptionsUndoCommand()
{}

void OptionsUndoCommand::redo()
{
    if (control)
        control->redo(cmd);
}

void OptionsUndoCommand::undo()
{
    if (control)
        control->undo(cmd);
}

QString OptionsUndoCommand::text() const
{
    return cmd.changedText();
}

QString OptionsUndoCommand::actionText() const
{
    return text();
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
    
    undo_stack = new QUndoStack(this);
    
    OptionsView *view = new OptionsView(opts, this);
    connect(view, SIGNAL(setOption(QString,QString)), 
            this, SLOT(updateOption(QString,QString)));
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

void OptionsControl::applyCommand(const OptionsCommand &command)
{
    if (opts == command.oldState())
    {
        conspireDebug() << "APPLY" << command.key() << command.newValue();

        opts = opts.setNestedValue(command.key(), command.newValue())
                   .asA<Options>();
    }
}

void OptionsControl::updateOption(QString key, QString value)
{
    conspireDebug() << "CHANGED:" << key << "=" << value;
    undo_stack->push( new OptionsUndoCommand(this, OptionsCommand(opts,key,value)) );
}

void OptionsControl::undo(const OptionsCommand &command)
{
    conspireDebug() << "REVERT" << command.key() << command.oldValue();
    opts = command.oldState();

    //need to propogate this down...
}

void OptionsControl::redo(const OptionsCommand &command)
{
    this->applyCommand(command);
}

void OptionsControl::undo()
{
    conspireDebug() << "UNDO:" << undo_stack->undoText();
    undo_stack->undo();
}

void OptionsControl::redo()
{
    conspireDebug() << "REDO:" << undo_stack->redoText();
    undo_stack->redo();
}

void OptionsControl::save()
{}

void OptionsControl::load()
{}

void OptionsControl::quit()
{
    QApplication::quit();
}
