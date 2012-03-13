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
                dynamic_cast<QBoxLayout*>(this->layout())->insertWidget(i,view);
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

EntryView::EntryView(Obj value, int index, bool multiple, QWidget *parent) 
          : QWidget(parent), val(value), idx(index), allow_multiple(multiple)
{}

EntryView::~EntryView()
{}

void EntryView::setValue(Obj new_value)
{
    conspireDebug() << new_value.toString();

    if (val != new_value)
    {
        val = new_value;
 
        if (allow_multiple)
        {    
            conspireDebug() << "emit edited(" << new_value.toString() << ", " << idx << ")";
            emit( edited(val,idx) );
        }
        else
        {
            conspireDebug() << "emit edited(" << new_value.toString() << ")";
            emit( edited(val) );
        }
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

bool EntryView::allowMultiple() const
{
    return allow_multiple;
}

EntryView* EntryView::build(Option option, QWidget *parent)
{
    return new StringValueView(option, parent);
}

void EntryView::updateValue(Obj value)
{
    val = value;
}

void EntryView::add()
{
    emit( added(idx) );
}

void EntryView::remove()
{
    emit( removed(idx) );
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
    
    v->setParent(this);
    
    label = new QLabel(label_text, this);
    layout()->addWidget(label);
    layout()->addWidget(view);
    
    del_button = new QToolButton(this);
    del_button->setText("-");
    connect(del_button, SIGNAL(clicked()), view, SLOT(remove()));
    layout()->addWidget(del_button);
    
    if (not can_delete)
        del_button->hide();
        
    add_button = new QToolButton(this);
    add_button->setText("+");
    connect(add_button, SIGNAL(clicked()), view, SLOT(add()));
    layout()->addWidget(add_button);
    
    if (not can_add)
        add_button->hide();
}

EntryViewHolder::~EntryViewHolder()
{}

void EntryViewHolder::setLabel(QString label_text)
{
    label->setText(label_text);
}

void EntryViewHolder::update(bool can_add, bool can_delete)
{
    if (can_add)
        add_button->show();
    else
        add_button->hide();
        
    if (can_delete)
        del_button->show();
    else
        del_button->hide();
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
    help_text = option.description();
    this->layout()->addWidget(help_button);
    
    group_holder = new QWidget(this);
    group_holder->setLayout( new QVBoxLayout() );
    this->layout()->addWidget(group_holder);

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
            
            EntryViewHolder *holder = new EntryViewHolder(label, view, 
                                                          can_add, can_delete, this);

            connect(view, SIGNAL(edited(Obj,int)), this, SIGNAL(edited(Obj,int)));
            connect(view, SIGNAL(added(int)), this, SIGNAL(added(int)));
            connect(view, SIGNAL(removed(int)), this, SIGNAL(removed(int)));

            views->insert(idx,holder);
            
            group_holder->layout()->addWidget(holder);
        }
    }
    else
    {
        bool can_add = false;
        bool can_delete = option.isOptional();
    
        EntryView *view = EntryView::build(option,this);
        connect(view, SIGNAL(edited(Obj)), this, SIGNAL(edited(Obj)));
        
        EntryViewHolder *holder = new EntryViewHolder(option.key(), view,
                                                      can_add, can_delete, this);
        
        views->insert(1,holder);
        group_holder->layout()->addWidget(holder);
    }
}

void EntryViewGroup::update(Option option)
{
    if (not views)
        return;

    if (option.allowMultiple())
    {
        QList<int> idxs = option.indiciesWithValue();
        
        qSort(idxs);

        conspireDebug() << "VALID INDICIES ==" << idxs;

        bool can_add = true;
        bool can_delete = true;
        
        if (not option.isOptional() and idxs.count() == 1)
        {
            can_delete = false;
        }
        
        int i=0;
        
        foreach (int idx, idxs)
        {
            EntryViewHolder *holder = views->value(idx, 0);
            
            if (holder == 0)
            {
                QString label = option.key();
            
                if (idx != 1 or idxs.count() > 1)
                {
                    label = QString("%1[%2]").arg(label).arg(idx);
                }
        
                EntryView *view = EntryView::build(option[idx], this);
            
                holder = new EntryViewHolder(label, view, 
                                             can_add, can_delete, this);

                connect(view, SIGNAL(edited(Obj,int)), this, SIGNAL(edited(Obj,int)));
                connect(view, SIGNAL(added(int)), this, SIGNAL(added(int)));
                connect(view, SIGNAL(removed(int)), this, SIGNAL(removed(int)));

                views->insert(idx,holder);
            
                dynamic_cast<QBoxLayout*>(group_holder->layout())
                                                ->insertWidget(i, holder);
            }
            else
            {
                holder->update(can_add, can_delete);
                holder->view()->update(option[idx]);
                
                if (idx == 1 and idxs.count() > 1)
                {
                    holder->setLabel( QString("%1[1]").arg(option.key()) );
                }
            }
            
            i += 1;
        }
        
        //delete indicies which are no longer valid
        foreach (int idx, views->keys())
        {
            if (not idxs.contains(idx))
            {
                EntryViewHolder *holder = views->value(idx, 0);
                
                if (holder)
                {
                    delete holder;
                    views->remove(idx);
                }
            }
        }
    }
    else
    {
        bool can_add = false;
        bool can_delete = option.isOptional();

        EntryViewHolder *holder = views->value(1, 0);
        
        if (holder)
        {
            holder->update(can_add, can_delete);
            holder->view()->update(option);
        }
        else
        {
            EntryView *view = EntryView::build(option,this);
            connect(view, SIGNAL(edited(Obj)), this, SIGNAL(edited(Obj)));
            connect(view, SIGNAL(added(int)), this, SIGNAL(added(int)));
            connect(view, SIGNAL(removed(int)), this, SIGNAL(removed(int)));
            
            EntryViewHolder *holder = new EntryViewHolder(option.key(), view,
                                                          can_add, can_delete, this);
            
            views->insert(1,holder);
            group_holder->layout()->addWidget(holder);
        }
    }
}

EntryViewGroup::~EntryViewGroup()
{}

void EntryViewGroup::showHelp() const
{
    QMessageBox msgbox;
    msgbox.setText( help_text );
    msgbox.exec();
}

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

///////////
/////////// Implementation of "StringValueView"
///////////

StringValueView::StringValueView(Option option, QWidget *parent) 
                : EntryView(option.value(), option.index(), 
                            option.allowMultiple(), parent)
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

OptionView::OptionView(View *parent) : View(parent), editor(0)
{
    this->setLayout(new QHBoxLayout(this));
}

void OptionView::build(const Option &option)
{
    delete editor;
    editor = 0;
    k = QString::null;

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

    editor = new EntryViewGroup(option,this);
    this->layout()->addWidget(editor);

    connect(editor, SIGNAL(edited(Obj)), this, SLOT(edited(Obj)));
    connect(editor, SIGNAL(edited(Obj,int)), this, SLOT(edited(Obj,int)));
    connect(editor, SIGNAL(added(int)), this, SLOT(added(int)));
    connect(editor, SIGNAL(removed(int)), this, SLOT(removed(int)));

    View *root_node = this->rootNode();
    
    if (root_node)
    {
        connect(this, SIGNAL(setOption(QString,QString)),
                root_node, SIGNAL(setOption(QString,QString)));
                
        connect(this, SIGNAL(addOption(QString, int)),
                root_node, SIGNAL(addOption(QString, int)));
                
        connect(this, SIGNAL(removeOption(QString, int)),
                root_node, SIGNAL(removeOption(QString, int)));
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

    editor->update(option);
}

OptionView::OptionView(const Option &option, View *parent)   
           : View(parent), editor(0)
{
    this->setLayout( new QHBoxLayout(this) );
    this->build(option);
}

OptionView::~OptionView()
{}

QString OptionView::key() const
{
    return k;
}

void OptionView::edited(Obj value)
{
    conspireDebug() << "OptionView::edited(Obj)";
    emit( setOption(key(), value.toString()) );
}

void OptionView::edited(Obj value, int index)
{
    conspireDebug() << "OptionView::edited(Obj)";
    emit( setOption(QString("%1[%2]").arg(key()).arg(index), value.toString()) );
}

void OptionView::added(int index)
{
    conspireDebug() << "OptionView::added(" << key() << "," << index << ")";
    emit( addOption(key(), index) );
}

void OptionView::removed(int index)
{
    conspireDebug() << "OptionView::removed(" << key() << "," << index << ")";
    emit( removeOption(key(), index) );
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
            this, SLOT(setOption(QString,QString)));
            
    connect(view, SIGNAL(addOption(QString,int)),
            this, SLOT(addOption(QString,int)));
            
    connect(view, SIGNAL(removeOption(QString,int)),
            this, SLOT(removeOption(QString,int)));
            
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
    
        conspireDebug() << opts.toConfig();
    
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

void OptionsControl::setOption(QString key, QString value)
{
    conspireDebug() << "CHANGED:" << key << "=" << value;
    undo_stack->push( new OptionsUndoCommand(this, OptionsCommand(opts,key,value)) );
}

void OptionsControl::addOption(QString key, int index)
{
    QList<int> idxs = opts[key].indiciesWithValue();
    
    for (int i=1; i<=(idxs.count()+1); ++i)
    {
        if (not idxs.contains(i))
        {
            index = i;
            break;
        }
    }

    conspireDebug() << "Adding:" << key << index;
    opts = opts.addDefaultValue( QString("%1[%2]").arg(key).arg(index) )
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

void OptionsControl::removeOption(QString key, int index)
{
    conspireDebug() << "Remove:" << key << index;
    opts = opts.removeValue( QString("%1[%2]").arg(key).arg(index) ).asA<Options>();

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
