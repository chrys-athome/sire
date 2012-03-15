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

#include "Conspire/GUI/optionswidget.h"
#include "Conspire/GUI/optionseditview.h"
#include "Conspire/GUI/optionscommand.h"

#include <QVBoxLayout>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QUndoStack>
#include <QLabel>
#include <QPushButton>

using namespace Conspire;

/** Constructor */
OptionsWidget::OptionsWidget(QWidget *parent) : QWidget(parent)
{
    build();
}

/** Construct to edit the passed options */
OptionsWidget::OptionsWidget(Options options, QWidget *parent)
              : QWidget(parent)
{
    build();
    setOptions(options);
}

/** Destructor */
OptionsWidget::~OptionsWidget()
{}

/** Return the current state of the options being edited */
Options OptionsWidget::options() const
{
    return opts;
}

/** Set the options object to be edited */
void OptionsWidget::setOptions(Options options)
{
    if (options == opts)
        return;

    conspireDebug() << "SETTING OPTIONS";
    view->setOptions(options);
    opts = options;
    conspireDebug() << "ALL DONE :-)";
}

void OptionsWidget::add(QString key)
{
    Options new_opts = opts.addDefaultValue(key).asA<Options>();
    undo_stack->push( new OptionsAddCommand(this, key) );
}

void OptionsWidget::remove(QString key)
{
    Options new_opts = opts.removeValue(key).asA<Options>();
    undo_stack->push( new OptionsRemoveCommand(this, key) );
}

void OptionsWidget::update(QString key, Obj object)
{
    Options new_opts = opts.setNestedValue(key, object.asA<Value>()).asA<Options>();
    undo_stack->push( new OptionsUpdateCommand(this, key, object.asA<Value>()) );
}

void OptionsWidget::resizeEvent(QResizeEvent *event)
{
    if (event)
    {
        form->setGeometry(0, 0, event->size().width()-75, event->size().height()-75);
    }
}

/** Internal function used to construct the widget */
void OptionsWidget::build()
{
    this->setMinimumSize(600,400);

    graphics_view = new QGraphicsView(this);
    
    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(graphics_view);    
    this->setLayout(l);

    graphics_scene = new QGraphicsScene(this);
    graphics_view->setScene(graphics_scene);
    
    form = new QGraphicsWidget();
    graphics_layout = new QGraphicsGridLayout();
    graphics_layout->setSpacing(0);
    form->setLayout(graphics_layout);
    graphics_scene->addItem(form);
    
    undo_stack = new QUndoStack(this);
    QPushButton *undo_button = new QPushButton( QObject::tr("Undo") );
    QPushButton *redo_button = new QPushButton( QObject::tr("Redo") );
    
    undo_button->setEnabled(false);
    redo_button->setEnabled(false);
    
    undo_button->setSizePolicy( QSizePolicy(QSizePolicy::Minimum,
                                            QSizePolicy::Minimum) );
    redo_button->setSizePolicy( QSizePolicy(QSizePolicy::Minimum,
                                            QSizePolicy::Minimum) );
    
    QGraphicsProxyWidget *undo_button_proxy = new QGraphicsProxyWidget(form);
    undo_button_proxy->setWidget(undo_button);
    QGraphicsProxyWidget *redo_button_proxy = new QGraphicsProxyWidget(form);
    redo_button_proxy->setWidget(redo_button);
    
    graphics_layout->addItem( undo_button_proxy, 1, 0, 1, 1 );
    graphics_layout->addItem( redo_button_proxy, 2, 0, 1, 1 );
    
    connect(undo_button, SIGNAL(clicked()), undo_stack, SLOT(undo()));
    connect(redo_button, SIGNAL(clicked()), undo_stack, SLOT(redo()));
    connect(undo_stack, SIGNAL(canUndoChanged(bool)), 
            undo_button, SLOT(setEnabled(bool)));
    connect(undo_stack, SIGNAL(canRedoChanged(bool)),
            redo_button, SLOT(setEnabled(bool)));
            
    QLabel *undo_label = new QLabel();
    QLabel *redo_label = new QLabel();
    
    undo_label->setWordWrap(true);
    redo_label->setWordWrap(true);
    undo_label->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Minimum) );
    redo_label->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Minimum) );
    
    QGraphicsProxyWidget *undo_label_proxy = new QGraphicsProxyWidget(form);
    undo_label_proxy->setWidget(undo_label);
    QGraphicsProxyWidget *redo_label_proxy = new QGraphicsProxyWidget(form);
    redo_label_proxy->setWidget(redo_label);
    
    graphics_layout->addItem( undo_label_proxy, 1, 1, 1, 1 );
    graphics_layout->addItem( redo_label_proxy, 2, 1, 1, 1 );
    
    connect(undo_stack, SIGNAL(undoTextChanged(QString)),
            undo_label, SLOT(setText(QString)));
    connect(undo_stack, SIGNAL(redoTextChanged(QString)),
            redo_label, SLOT(setText(QString)));
    
    view = new OptionsEditView();
    graphics_layout->addItem(view, 0, 0, 1, 2);
    
    graphics_layout->setColumnStretchFactor(0, 1);
    graphics_layout->setColumnStretchFactor(1, 5);
    
    graphics_layout->setContentsMargins(0,0,0,0);
    
    connect(view, SIGNAL(add(QString)), this, SLOT(add(QString)));
    connect(view, SIGNAL(remove(QString)), this, SLOT(remove(QString)));
    connect(view, SIGNAL(update(QString,Obj)), this, SLOT(update(QString,Obj)));
}
