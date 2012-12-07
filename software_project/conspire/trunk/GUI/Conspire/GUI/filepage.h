#ifndef CONSPIRE_FILEPAGE_H
#define CONSPIRE_FILEPAGE_H
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

#include "Conspire/GUI/configpage.h"

#include "Conspire/option.h"

CONSPIRE_BEGIN_HEADER

class QLabel;
class QLineEdit;

namespace Conspire
{
    class WidgetRack;
    class Button;

    /** This page is used to change the value of a specific option */
    class CONSPIRE_EXPORT FilePage : public ConfigPage
    {
        Q_OBJECT
    
    public:
        FilePage(Page *parent=0);
        FilePage(QString *ibrowsedir, Option option, QString root_key,
                 Page *parent=0);
                     
        ~FilePage();
        
        Option option() const;
        QString rootKey() const;
        int index() const;
    
        QString shortKey() const;
        QString fullKey() const;
    
    public slots:
        void reread(Options options);
    
    protected:
        void resizeEvent(QGraphicsSceneResizeEvent *e);
        void moveEvent(QGraphicsSceneMoveEvent *e);
    
    private slots:
        void editingFinished();
        void browse();
    
    private:
        void build();
        void setOption(Option option, QString key);
        
        QString *browsedir;
        
        Option opt;
        QString root_key;
        
        QLabel *help_label;
        QLineEdit *value_edit;
        
        Button *browse_button;
        
        QString old_text;
        
        WidgetRack *rack;
    };

}

CONSPIRE_END_HEADER

#endif
