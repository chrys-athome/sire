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

#include <QVBoxLayout>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>

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
    this->setOptions(new_opts);
}

void OptionsWidget::remove(QString key)
{
    Options new_opts = opts.removeValue(key).asA<Options>();
    this->setOptions(new_opts);
}

void OptionsWidget::update(QString key, Obj object)
{
    Options new_opts = opts.setNestedValue(key, object.asA<Value>()).asA<Options>();
    this->setOptions(new_opts);
}

/** Internal function used to construct the widget */
void OptionsWidget::build()
{
    graphics_view = new QGraphicsView(this);
    
    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(graphics_view);    
    this->setLayout(l);

    graphics_scene = new QGraphicsScene(this);
    graphics_view->setScene(graphics_scene);
    
    QGraphicsWidget *form = new QGraphicsWidget();
    graphics_layout = new QGraphicsGridLayout();
    graphics_layout->setSpacing(0);
    form->setLayout(graphics_layout);
    graphics_scene->addItem(form);
    
    view = new OptionsEditView();
    graphics_layout->addItem(view, 0, 0);
    graphics_layout->setContentsMargins(0,0,0,0);
    
    connect(view, SIGNAL(add(QString)), this, SLOT(add(QString)));
    connect(view, SIGNAL(remove(QString)), this, SLOT(remove(QString)));
    connect(view, SIGNAL(update(QString,Obj)), this, SLOT(update(QString,Obj)));
}
