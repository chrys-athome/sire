#ifndef CONSPIRE_CHOOSECLUSTERPAGE_H
#define CONSPIRE_CHOOSECLUSTERPAGE_H
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

#include "Conspire/GUI/page.h"

#include "Conspire/option.h"

CONSPIRE_BEGIN_HEADER

class QLabel;
class QLineEdit;
class QProgressBar;
class QTableWidget;
class QGraphicsView;
class QGraphicsWidget;
class QGraphicsLinearLayout;
class QTimer;

namespace Conspire
{
    class Button;
    class WidgetStack;

    /** This is the page used to submit a job to Acquire */
    class CONSPIRE_EXPORT ChooseClusterPage : public Page
    {
        Q_OBJECT
        
    public:
        ChooseClusterPage(Page *parent=0);
        
        ~ChooseClusterPage();

    protected slots:
        
    protected:
        void resizeEvent(QGraphicsSceneResizeEvent *e);
        void moveEvent(QGraphicsSceneMoveEvent *e);

        void paint(QPainter *painter, 
                   const QStyleOptionGraphicsItem *option, 
                   QWidget *widget);

    private:
        void build();
        void allUpdate();
                
        /** The options for the job to submit */
        Options opts;
        
        /** The stack holding the different pages during
            the job run process */
        WidgetStack *stack;
        
        QGraphicsView *qview;
        QGraphicsScene *qscene;
        QList<QGraphicsLayoutItem *> *all_jcw;
        QGraphicsLinearLayout *qlinear;
        QGraphicsWidget *tableofworkstores;

        /** Text editor for host */
        QLineEdit *lineedit_host;

        /** Text editor for username */
        QLineEdit *lineedit_username;

        /** Text editor for password */
        QLineEdit *lineedit_password;

        /** Text editor for password */
        QLineEdit *lineedit_password_known;
        
        /** The status label for logging in */
        QLabel *login_label;
                
        /** The button used to submit the job */
        Button *button;
        
        QTimer *refreshtimer;
                        
        Button *return_button;

    };

}

CONSPIRE_END_HEADER

#endif
