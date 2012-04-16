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

#include "Conspire/GUI/optionspage.h"
#include "Conspire/GUI/addpage.h"
#include "Conspire/GUI/optionpage.h"
#include "Conspire/GUI/exceptionpage.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include <QPushButton>
#include <QSignalMapper>

using namespace Conspire;

/** Constructor */
OptionsPage::OptionsPage(QGraphicsItem *parent) : ConfigPage(parent)
{
    build();
}

/** Construct, passing in a top-level options object */
OptionsPage::OptionsPage(Options options, QGraphicsItem *parent)
            : ConfigPage(parent)
{
    build();
    setOptions(options);
}

/** Construct, passing in an Opitons object that is a sub-options object,
    with root key 'root_key' */
OptionsPage::OptionsPage(Options options, QString root_key,
                         QGraphicsItem *parent)
            : ConfigPage(parent)
{
    build();
    setOptions(options, root_key);
}

/** Destructor */
OptionsPage::~OptionsPage()
{}

/** Return the options object being edited by this view */
Options OptionsPage::options() const
{
    return opts;
}

/** Return the root key of the options object being edited. This is
    null if this view is editing the top-level options object */
QString OptionsPage::rootKey() const
{
    return root_key;
}

/** This function is called when the options object is updated */
void OptionsPage::reread(Options options)
{
    if (not root_key.isNull())
    {
        try
        {
            Option opt = options.getNestedOption(root_key);
            options = opt.value().asA<Options>();
        }
        catch(...)
        {
            conspireDebug() << "CANNOT FIND" << root_key << "IN OPTIONS!";
            options = Options();
        }
    }
    
    //create one button for each option...
    opts = options;
    
    QStringList keys = opts.keysAndIndiciesWithValue();
    
    //make sure that there are as many buttons in the layout as options
    QGraphicsLinearLayout *l = dynamic_cast<QGraphicsLinearLayout*>(this->layout());
    
    if (not l)
    {
        conspireDebug() << "INVALID LAYOUT!!!";
        throw Conspire::program_bug( Conspire::tr(
                "How do we have an invalid layout???"), CODELOC );
    }
    
    if (l->count() > keys.count())
    {
        for (int i = l->count()-1; i >= keys.count(); --i)
        {
            QGraphicsLayoutItem *item = l->itemAt(i);
            l->removeAt(i);
            buttons.removeAt(i);
            delete item;
        }
    }
    else if (l->count() < keys.count())
    {
        for (int i = l->count(); i < keys.count(); ++i)
        {
            QPushButton *b = new QPushButton();
            b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            
            connect(b, SIGNAL(clicked()), mapper, SLOT(map()));
            buttons.append(b);
            QGraphicsProxyWidget *b_proxy = new QGraphicsProxyWidget(this);
            b_proxy->setWidget(b);
            l->insertItem(i, b_proxy);
        }
    }
    
    for (int i=0; i<keys.count(); ++i)
    {
        QAbstractButton *b = buttons[i];
        Option opt = opts.getNestedOption(keys[i]);
        
        if (opt.hasSubOptions())
        {
            b->setText( QString("%1 ----->").arg(keys[i]) );
        }
        else
        {
            b->setText( QString("%1 == %2").arg(keys[i], opt.value().toString()) );
        }
        
        mapper->removeMappings(b);
        mapper->setMapping(b, keys[i]);
    }
}

/** Slot called when one of the options is clicked */
void OptionsPage::clicked(const QString &key)
{
    try
    {
        //create a new OptionsWidget to display the suboptions
        conspireDebug() << "CLICKED" << key;
        Option opt = opts.getNestedOption(key);
        
        if (opt.hasSubOptions())
        {
            QString root = key;
                
            if (not root_key.isNull())
                root.prepend(".").prepend(root_key);

            PagePointer p(new OptionsPage(opt.value().asA<Options>(), root));
            
            emit( push(p) );
        }
        else
        {
            PagePointer p(new OptionPage(opt, root_key));
            
            emit( push(p) );
        }
    }
    catch(const Conspire::Exception &e)
    {
        conspireDebug() << "EXCEPTION" << e.toString();
        
        PagePointer p(new ExceptionPage( Conspire::tr(
                "An error occurred when you clicked on the option \"%1\".")
                    .arg(key), e ));
                    
        emit( push(p) );
    }
}

/** Actually build the widget */
void OptionsPage::build()
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout( ::Qt::Vertical, this );
    this->setLayout(l);
    
    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(const QString&)), this, SLOT(clicked(const QString&)));
}

/** Set the options and root_key used by this object */
void OptionsPage::setOptions(Options options, QString key)
{
    root_key = key;
    opts = Options();
    this->reread(options);
}
