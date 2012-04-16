#ifndef CONSPIRE_OPTIONSPAGE_H
#define CONSPIRE_OPTIONSPAGE_H
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

#include "Conspire/option.h"

#include "Conspire/GUI/configpage.h"

CONSPIRE_BEGIN_HEADER

class QAbstractButton;
class QSignalMapper;

namespace Conspire
{
    /** This is a page that is used to view and edit a single page
        of options */
    class CONSPIRE_EXPORT OptionsPage : public ConfigPage
    {
        Q_OBJECT
    
    public:
        OptionsPage(QGraphicsItem *parent=0);
        OptionsPage(Options options, QGraphicsItem *parent=0);
        OptionsPage(Options options, QString root_key,
                    QGraphicsItem *parent=0);
        
        ~OptionsPage();
        
        Options options() const;
        
        QString rootKey() const;
    
    public slots:
        void reread(Options options);

    private slots:
        void clicked(const QString &key);

    private:
        void build();
        void setOptions(Options options, QString root_key = QString::null);
        
        /** The options that can be edited by this page */
        Options opts;
        
        /** The root key of this options object */
        QString root_key;
        
        /** The list of active buttons */
        QList<QAbstractButton*> buttons;
        
        /** The signal mapper */
        QSignalMapper *mapper;
    };

}

CONSPIRE_END_HEADER

#endif