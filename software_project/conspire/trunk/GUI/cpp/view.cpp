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
#include "Conspire/values.h"
#include "Conspire/exceptions.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
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

OptionsView::OptionsView(QWidget *parent) : View(parent), views(0)
{}

void OptionsView::build(const Options &options)
{
    //remove all current children
    foreach (QObject *child, this->children())
    {
        delete child;
    }
    
    //remove the current layout
    delete this->layout();
    
    //remove the current list of children
    delete views;
    views = 0;
    
    //now create the new children views for the options
    this->setLayout( new QVBoxLayout(this) );
    
    QStringList keys = options.keys();
    
    bool can_add = false;
    
    foreach (QString key, keys)
    {
        Option opt = options[key];

        if (opt.hasValue() or (not opt.isOptional()))
        {
            if (not views)
                views = new QHash<QString,OptionView*>();
        
            OptionView *view = new OptionView(opt,this);
            this->layout()->addWidget(view);
            views->insert(key, view);
        }
        else
            can_add = true;
    }
    
    if (can_add)
        this->layout()->addWidget( new QPushButton("Add",this) );
}

void OptionsView::update(const Options &options)
{
    QStringList keys = options.keys();
    
    bool can_add = false;
    
    int i = 0;
    
    foreach (QString key, keys)
    {
        Option opt = options[key];
        
        if (opt.hasValue() or (not opt.isOptional()))
        {
            if (not views)
                views = new QHash<QString,OptionView*>();
                
            if (views->contains(key))
            {
                views->value(key)->update(opt);
                ++i;
            }
            else
            {
                OptionView *view = new OptionView(opt,this);
                dynamic_cast<QVBoxLayout*>(this->layout())->insertWidget(i,view);
                ++i;
                
                views->insert(key,view);
            }
        }
        else if (views and views->contains(key))
        {
            //this option has been deleted...
            delete views->value(key);
            views->remove(key);
        }
    }
}

OptionsView::OptionsView(const Options &options, QWidget *parent)
            : View(parent), views(0)
{
    this->build(options);
}

OptionsView::~OptionsView()
{
    delete views;
}

///////////
/////////// Implementation of "EntryView"
///////////

EntryView::EntryView(Obj value, int index, QWidget *parent) 
          : QWidget(parent), val(value), idx(index)
{}

EntryView::~EntryView()
{}

void EntryView::setValue(Obj new_value)
{
    if (val != new_value)
    {
        val = new_value;
        emit( edited(val) );
    }
}

Obj EntryView::value() const
{
    return val;
}

int EntryView::index() const
{
    return idx;
}

EntryView* EntryView::build(Option option, QWidget *parent)
{
    return new StringValueView(option, parent);
}

void EntryView::updateValue(Obj value)
{
    val = value;
}

///////////
/////////// Implementation of "EntryViewHolder"
///////////

EntryViewHolder::EntryViewHolder(QString label_text, EntryView *view, 
                                 bool can_add, bool can_delete,
                                 QWidget *parent)
                : QWidget(parent), v(view)
{
    this->setLayout( new QHBoxLayout(this) );
    
    label = new QLabel(label_text, this);
    layout()->addWidget(label);
    layout()->addWidget(view);
    
    if (can_delete)
    {
        QToolButton *b = new QToolButton(this);
        b->setText("-");
        connect(b, SIGNAL(clicked()), this, SLOT(clickedDelete()));
        layout()->addWidget(b);
    }
    
    if (can_add)
    {
        QToolButton *b = new QToolButton(this);
        b->setText("+");
        connect(b, SIGNAL(clicked()), this, SLOT(clickedAdd()));
        layout()->addWidget(b);
    }
}

EntryViewHolder::~EntryViewHolder()
{}

void EntryViewHolder::setLabel(QString label_text)
{
    label->setText(label_text);
}

EntryView* EntryViewHolder::view()
{
    return v;
}

///////////
/////////// Implementation of "EntryViewGroup"
///////////

EntryViewGroup::EntryViewGroup(Option option, QWidget *parent)
               : QWidget(parent)
{
    allow_multiple = option.allowMultiple();
    this->setLayout( new QHBoxLayout(this) );

    QPushButton *help_button = new QPushButton(QObject::tr("?"), this);
    connect(help_button, SIGNAL(clicked()), this, SLOT(showHelp()));
    this->layout()->addWidget(help_button);
    
    QWidget *holder = new QWidget(this);
    holder->setLayout( new QVBoxLayout() );
    this->layout()->addWidget(holder);

    views = new QHash<int,EntryViewHolder*>();

    //add each widget one above the other...
    if (allow_multiple)
    {
        QList<int> idxs = option.indiciesWithValue();
        
        qSort(idxs);

        bool can_add = true;
        bool can_delete = true;
        
        if (not option.isOptional() and idxs.count() == 1)
        {
            can_delete = false;
        }
        
        foreach (int idx, idxs)
        {
            QString label = option.key();
            
            if (idx != 1 or idxs.count() > 1)
            {
                label = QString("%1[%2]").arg(label).arg(idx);
            }
        
            EntryView *view = EntryView::build(option[idx], this);
            connect(view, SIGNAL(edited(Obj,int)), this, SLOT(valueChanged(Obj,int)));
            
            EntryViewHolder *holder = new EntryViewHolder(label, view, 
                                                          can_add, can_delete, this);

            views->insert(idx,holder);
            
            this->layout()->addWidget(holder);
        }
    }
    else
    {
        bool can_add = false;
        bool can_delete = option.isOptional();
    
        EntryView *view = EntryView::build(option,this);
        connect(view, SIGNAL(edited(Obj)), this, SLOT(valueChanged(Obj)));
        
        EntryViewHolder *holder = new EntryViewHolder(option.key(), view,
                                                      can_add, can_delete, this);
        
        views->insert(1,holder);
        this->layout()->addWidget(holder);
    }
}

EntryViewGroup::~EntryViewGroup()
{}

bool EntryViewGroup::allowMultiple() const
{
    return allow_multiple;
}

Obj EntryViewGroup::value() const
{
    if (views)
    {
        if (not views->isEmpty())
        {
            return views->constBegin().value()->view()->value();
        }
    }

    return None();
}

Obj EntryViewGroup::value(int index) const
{
    if (views)
    {
        EntryViewHolder *holder = views->value(index,0);
        
        if (holder)
            return holder->view()->value();
    }
    
    return None();
}

void EntryViewGroup::update(Option option)
{
    ///
}

void EntryViewGroup::valueChanged(Obj new_value)
{
    ///
}

void EntryViewGroup::valueChanged(Obj new_value, int index)
{
    ///
}

///////////
/////////// Implementation of "StringValueView"
///////////

StringValueView::StringValueView(Option option, QWidget *parent) 
                : EntryView(option.value(), option.index(), parent)
{
    this->setLayout( new QHBoxLayout() );
    
    edit = new QLineEdit(this);
    edit->setText( option.value().toString() );
    
    this->layout()->addWidget(edit);
    
    connect(edit, SIGNAL(returnPressed()), this, SLOT(textChanged()));
}

StringValueView::~StringValueView()
{}

void StringValueView::textChanged()
{
    try
    {
        StringValue val( edit->text() );
        this->setValue(val);
    }
    catch(const Conspire::Exception &e)
    {
        conspireDebug() << "EXCEPTION THROWN:";
        conspireDebug() << e.toString();
        QMessageBox::warning(this, "Conspire", e.why(), QMessageBox::Discard);
        edit->setText(this->value().toString());
    }
}

void StringValueView::update(Option option)
{
    Obj val = option.value();
    edit->setText( val.toString() );
    this->updateValue(val);
}

///////////
/////////// Implementation of "OptionView"
///////////

OptionView::OptionView(View *parent) : View(parent)
{}

void OptionView::build(const Option &option)
{
    //remove all current children
    foreach (QObject *child, this->children())
    {
        delete child;
    }
    
    //remove the current layout
    delete this->layout();

    //forget the current values
    k = QString::null;
    v = QString::null;
    help_text = QString::null;
    edit = 0;

    //disconnect all objects connected to signals
    //from this view
    this->disconnect();

    //rebuild everything
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

void OptionView::update(const Option &option)
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
    
    edit->setText(v);
}

OptionView::OptionView(const Option &option, View *parent)   
           : View(parent)
{
    this->build(option);
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
{
    this->setText( cmd.changedText() );
}
               
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
    
    view = new OptionsView(this);
    connect(view, SIGNAL(setOption(QString,QString)), 
            this, SLOT(updateOption(QString,QString)));
    this->layout()->addWidget(view);

    QPushButton *undo = new QPushButton("Undo");
    QPushButton *redo = new QPushButton("Redo");
    QPushButton *save = new QPushButton("Save");
    QPushButton *load = new QPushButton("Load");
    QPushButton *quit = new QPushButton("Quit");

    QLabel *undo_label = new QLabel(this);
    QLabel *redo_label = new QLabel(this);

    undo->setEnabled(false);
    redo->setEnabled(false);

    this->layout()->addWidget(undo);
    this->layout()->addWidget(undo_label);
    this->layout()->addWidget(redo);
    this->layout()->addWidget(redo_label);
    this->layout()->addWidget(save);
    this->layout()->addWidget(load);
    this->layout()->addWidget(quit);
    
    connect(undo, SIGNAL(clicked()), undo_stack, SLOT(undo()));
    connect(redo, SIGNAL(clicked()), undo_stack, SLOT(redo()));
    
    connect(undo_stack, SIGNAL(canUndoChanged(bool)), undo, SLOT(setEnabled(bool)));
    connect(undo_stack, SIGNAL(canRedoChanged(bool)), redo, SLOT(setEnabled(bool)));
    connect(undo_stack, SIGNAL(undoTextChanged(QString)),
            undo_label, SLOT(setText(QString)));
    connect(undo_stack, SIGNAL(redoTextChanged(QString)),
            redo_label, SLOT(setText(QString)));
    
    connect(save, SIGNAL(clicked()), this, SLOT(save()));
    connect(load, SIGNAL(clicked()), this, SLOT(load()));
    connect(quit, SIGNAL(clicked()), this, SLOT(quit()));
    
    view->build(opts);
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
    
        try
        {
            view->update(opts);
        }
        catch(...)
        {
            view->build(opts);
        }
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
    try
    {
        view->update(opts);
    }
    catch(...)
    {
        view->build(opts);
    }
}

void OptionsControl::redo(const OptionsCommand &command)
{
    this->applyCommand(command);
}

void OptionsControl::save()
{}

void OptionsControl::load()
{}

void OptionsControl::quit()
{
    QApplication::quit();
}
