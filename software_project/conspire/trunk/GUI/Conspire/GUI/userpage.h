#ifndef CONSPIRE_USERPAGE_H
#define CONSPIRE_USERPAGE_H
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

namespace Conspire
{
    class Button;
    class WidgetStack;

    /** This is the page used to submit a job to Acquire */
    class CONSPIRE_EXPORT UserPage : public Page
    {
        Q_OBJECT
        
    public:
        UserPage(int usemode, Page *parent=0);
        
        ~UserPage();

    protected slots:
        void login();
        void sshadd();
        void changeUser();
        void continueToWorkStores();
        void modifyAccount();
        
    protected:
       QString addMachine(QString username, QString password, QString machinename, bool *loginsuccessful);
       QString loginUser(QString username, QString password, bool *wassuccessful);
        void resizeEvent(QGraphicsSceneResizeEvent *e);
        void moveEvent(QGraphicsSceneMoveEvent *e);

        void paint(QPainter *painter, 
                   const QStyleOptionGraphicsItem *option, 
                   QWidget *widget);

    private:
        void build();
        void allUpdate();
        
        /** The mode in which the page is to be used **/
        int usemode;
        int submode;
        
        /** The options for the job to submit */
        Options opts;
        QLabel *ifyouare_label_loggedin2;
        QLabel *ifyouare_label_loggedin;
        QLabel *hello_label_loggedin;
        
        /** The stack holding the different pages during
            the job run process */
        WidgetStack *stack;

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
        
        /** The button used to change user */
        Button *button_knownuser;
        
        /** The button used to change user */
        Button *chgusr1_button;
        /** The button used to change user */
        Button *chgusr2_button;
        
        Button *continuebutton;
        
        Button *modifybutton;
        
        Button *return_button;

        /** The last username that was logged in */
        QString last_username;
    };

}

CONSPIRE_END_HEADER

#endif
