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

class QFrame;
class QAbstractButton;
class QSignalMapper;

namespace Conspire
{
    class WidgetRack;
    class Button;
    class OptionButton;

    /** This is a page that is used to view and edit a single page
        of options */
    class CONSPIRE_EXPORT OptionsPage : public ConfigPage
    {
        Q_OBJECT
    
    public:
        OptionsPage(Page *parent=0);
        OptionsPage(Options options, Page *parent=0);
        OptionsPage(Options options, QString root_key,
                    Page *parent=0);
        
        ~OptionsPage();
        
        Options options() const;
        
        QString rootKey() const;
    
    public slots:
        void reread(Options options);

    protected:
        void resizeEvent(QGraphicsSceneResizeEvent *e);
        void moveEvent(QGraphicsSceneMoveEvent *e);

        void paint(QPainter *painter, 
                   const QStyleOptionGraphicsItem *option, 
                   QWidget *widget);

    private slots:
        void clicked(const QString &key);
        void add();

    private:
        void build();
        void setOptions(Options options, QString root_key = QString::null);
        
        void pvt_reread(Options options);
        
        /** The options that can be edited by this page */
        Options opts;
        
        /** The root key of this options object */
        QString root_key;
        
        /** The rack used to hold everything in the widget */
        WidgetRack *rack;
        
        /** The rack used to hold all of the option buttons */
        WidgetRack *button_rack;
        
        /** The list of active buttons */
        QList<OptionButton*> buttons;
        
        /** The button that is used to add new options */
        Button *add_button;
        
        /** The signal mapper */
        QSignalMapper *mapper;
    };

}

CONSPIRE_END_HEADER

#endif