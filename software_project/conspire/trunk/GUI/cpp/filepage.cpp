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

#include "Conspire/GUI/filepage.h"
#include "Conspire/GUI/exceptionpage.h"
#include "Conspire/GUI/widgetrack.h"
#include "Conspire/GUI/button.h"

#include "Conspire/values.h"
#include "Conspire/exceptions.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>

using namespace Conspire;

/** Constructor */
FilePage::FilePage(Page *parent) : ConfigPage(parent)
{
    build();
}

/** Constructor, passing in the option to edit, and the root key of that option */
FilePage::FilePage(Option option, QString root_key, Page *parent)
         : ConfigPage(parent)
{
    build();
    setOption(option, root_key);
}
   
/** Destructor */
FilePage::~FilePage()
{}

/** Return the option being viewed and edited */
Option FilePage::option() const
{
    return opt;
}

/** Return the root key of that option */
QString FilePage::rootKey() const
{
    return root_key;
}

/** Return the short key of the option (the key without the root_key) */
QString FilePage::shortKey() const
{
    if (opt.index() == 1)
        return opt.key();
    else
        return QString("%1[%2]").arg(opt.key()).arg(opt.index());
}

/** Return the full key of the option, including the root key */
QString FilePage::fullKey() const
{
    if (root_key.isEmpty())
        return shortKey();
    else
        return QString("%1.%2").arg(root_key, shortKey());
}

/** Build the widget */
void FilePage::build()
{
    setTitle("Unnamed FilePage");
    setDescription("Unnamed FilePage description");

    rack = new WidgetRack(this);

    value_edit = new QLineEdit();
    value_edit->setAlignment( ::Qt::AlignCenter );
    value_edit->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, 
                                           QSizePolicy::Expanding) );
    value_edit->setFont( QFont("LucidaGrande", 48) );

    connect(value_edit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    rack->addWidget(value_edit);

    browse_button = new Button(Conspire::tr("Browse..."));
    browse_button->setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum) );
    connect(browse_button, SIGNAL(clicked()), this, SLOT(browse()));
    
    rack->addWidget(browse_button);

    help_label = new QLabel();
    help_label->setAlignment( ::Qt::AlignLeft | ::Qt::AlignVCenter );
    help_label->setWordWrap(true);
    help_label->setFont( QFont("LucidaGrande", 14) );
    help_label->setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum) );
    rack->addWidget(help_label);
}

void FilePage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
    rack->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
}

void FilePage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
    rack->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
}

template <class T> const T& patch_convert_to_T(const Obj &obj)
{
	const Object *object = obj;
	const T *cast_object = dynamic_cast<const T*>(object);
	if (not cast_object) { throw invalid_cast(); }
	return *cast_object;
}

/** Function called when editing of the option is finished */
void FilePage::editingFinished()
{
    conspireDebug() << "EDITING FINISHED" << value_edit->text() << old_text;

    if (value_edit->text() == old_text)
        return;

    old_text = value_edit->text();

    //try to update the option locally first. This catches parse errors
    //before they propogate up the document
    try
    {
        Obj new_val = StringValue(value_edit->text());

        Options opts(opt);
        opts.setNestedValue(shortKey(),patch_convert_to_T<Value>(new_val));
        
        conspireDebug() << "emit( update(" << fullKey() << "," << value_edit->text() 
                        << " ) )";
        emit( update(fullKey(),new_val) );
        emit( pop() );
    }
    catch(const Conspire::Exception &e)
    {
        emit( push(PagePointer( new ExceptionPage( Conspire::tr(
                    "Something went wrong when updating the value of the option \"%1\""
                    "to \"%2\".")
                        .arg(fullKey(), value_edit->text()), e) ) ) );
                        
        value_edit->setText( opt.value().toString() );
        old_text = value_edit->text();
    }
}

void FilePage::browse()
{
    QString filename = QFileDialog::getOpenFileName(0, Conspire::tr("Choose a file..."), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
    
    if (not filename.isEmpty())
    {
        value_edit->setText(filename);
        this->editingFinished();
    }
}

/** Set the option to be edited */
void FilePage::setOption(Option option, QString key)
{
    opt = option;
    root_key = key;
    
    this->setDescription(option.description());
    help_label->setText(option.description());
    this->setTitle(fullKey());
    
    value_edit->setText(opt.value().toString());
    old_text = value_edit->text();
}

/** Slot called when the option has been updated */
void FilePage::reread(Options options)
{
    try
    {
        Option opt = options[fullKey()];
    
        QString text = opt.value().toString();
    
        if (text != old_text)
        {
            value_edit->setText(text);
            old_text = text;
        }
    }
    catch(...)
    {
        conspireDebug() << "ERROR IN REREADING OPTION...";
    }
    
    ConfigPage::reread(options);
}
