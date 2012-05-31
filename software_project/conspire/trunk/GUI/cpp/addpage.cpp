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

#include "Conspire/GUI/addpage.h"
#include "Conspire/GUI/optionbutton.h"
#include "Conspire/GUI/widgetrack.h"

#include "Conspire/option.h"

#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>

#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>

using namespace Conspire;

/** Constructor */
AddPage::AddPage(Page *parent) : ConfigPage(parent)
{
    build();
}

/** Constructor, setting the Options object that details the list
    of options that can be added */
AddPage::AddPage(Options options, Page *parent)
        : ConfigPage(parent)
{
    build();
    setOptions(options);
}

/** Constructor, setting the Options object that details the list
    of options that can be added, together with the root key of 
    any added options */
AddPage::AddPage(Options options, QString key, Page *parent)
        : ConfigPage(parent), root_key(key)
{
    build();
    setOptions(options);
}

/** Destructor */
AddPage::~AddPage()
{}

/** Set the Options object that details the list of options that can be added */
void AddPage::setOptions(Options options)
{
    conspireDebug() << options.addableKeys();
    
    QStringList keys = options.addableKeys();
    
    while (buttons.count() > keys.count())
    {
        buttons.takeLast()->deleteLater();
    }
    
    while (buttons.count() < keys.count())
    {
        OptionButton *button = new OptionButton(this);
        button_rack->addWidget(button);
        buttons.append(button);

        connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    }
    
    for (int i=0; i<keys.count(); ++i)
    {
        Option option = options[keys[i]];
    
        buttons[i]->setText(option.key());
        buttons[i]->setValue(option.description());
        
        mapper->removeMappings( buttons[i] );

        if (option.allowMultiple())
        {
            //find the lowest available index that does not have a value
            QList<int> idxs = option.indiciesWithValue();
            qSort(idxs);
            
            int lowest_available = 1;
            
            foreach(int idx, idxs)
            {
                if (idx == lowest_available)
                {
                    lowest_available += 1;
                }
                else
                    break;
            }
            
            mapper->setMapping(buttons[i], QString("%1[%2]").arg(option.key())
                                                            .arg(lowest_available));
        }
        else
            mapper->setMapping(buttons[i], keys[i]);
    }
}

/** Update the options object */
void AddPage::reread(Options options)
{
    if (root_key.isEmpty())
    {
        setOptions(options);
    }
    else
    {
        try
        {
            setOptions( options.getNestedValue(root_key).asA<Options>() );
        }
        catch(...)
        {
            setOptions( Options() );
        }
    }
    
    ConfigPage::reread(options);
}

/** Build the widget */
void AddPage::build()
{
    button_rack = new WidgetRack(::Qt::Vertical, this);

    button_rack->setGeometry( this->geometry() );
    button_rack->show();

    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(const QString&)), this, SLOT(addOption(QString)));
}


void AddPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    ConfigPage::resizeEvent(e);
    button_rack->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
}

void AddPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    ConfigPage::moveEvent(e);
    button_rack->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
}

void AddPage::paint(QPainter *painter, 
                        const QStyleOptionGraphicsItem *option, 
                        QWidget *widget)
{
    ConfigPage::paint(painter, option, widget);
}

void AddPage::addOption(QString option)
{
    if (root_key.isEmpty())
    {
        emit( add(option) );
    }
    else
    {
        emit( add(QString("%1.%2").arg(root_key,option)) );
    }
    
    emit( pop() );
}
