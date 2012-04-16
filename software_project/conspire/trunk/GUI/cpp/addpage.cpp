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

#include "Conspire/option.h"

#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>

#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>

using namespace Conspire;

/** Constructor */
AddPage::AddPage(QGraphicsItem *parent) : ConfigPage(parent)
{
    build();
}

/** Constructor, setting the Options object that details the list
    of options that can be added */
AddPage::AddPage(Options options, QGraphicsItem *parent)
        : ConfigPage(parent)
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

    QGraphicsGridLayout *l = dynamic_cast<QGraphicsGridLayout*>(this->layout());
    
    if (not l)
    {
        l = new QGraphicsGridLayout(this);
        this->setLayout(l);
    }
    
    //remove all items in the view
    for (int i=0; i<l->count(); ++i)
    {
        QGraphicsLayoutItem *w = l->itemAt(i);
        l->removeAt(i);
        delete w;
    }
    
    //now add in one row for each addable option
    QStringList keys = options.addableKeys();
    
    int row = 0;
    
    foreach (QString key, keys)
    {
        Option option = options[key];
    
        QLabel *label = new QLabel(key);
        label->setWordWrap(true);
        label->setAlignment( ::Qt::AlignCenter );
        
        QGraphicsProxyWidget *label_proxy = new QGraphicsProxyWidget(this);
        label_proxy->setWidget(label);
        
        l->addItem(label_proxy, row, 0, ::Qt::AlignCenter);
        
        label = new QLabel(option.description());
        label->setWordWrap(true);
        label->setAlignment( ::Qt::AlignCenter );

        label_proxy = new QGraphicsProxyWidget(this);
        label_proxy->setWidget(label);
        
        l->addItem(label_proxy, row, 1, ::Qt::AlignCenter);
        
        QPushButton *button = new QPushButton(Conspire::tr("Add"));
        connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
        
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
            
            mapper->setMapping(button, QString("%1[%2]").arg(option.key())
                                                .arg(lowest_available));
        }
        else
            mapper->setMapping(button, option.key());
        
        QGraphicsProxyWidget *button_proxy = new QGraphicsProxyWidget(this);
        button_proxy->setWidget(button);
        
        l->addItem(button_proxy, row, 2, ::Qt::AlignCenter);
        
        row += 1;
    }
}

/** Update the options object */
void AddPage::reread(Options options)
{}

/** Build the widget */
void AddPage::build()
{
    QGraphicsGridLayout *l = new QGraphicsGridLayout(this);
    this->setLayout(l);
    
    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(const QString&)), this, SLOT(addOption(QString)));
}

void AddPage::addOption(QString option)
{
    deleteLater();
    emit( add(option) );
}
