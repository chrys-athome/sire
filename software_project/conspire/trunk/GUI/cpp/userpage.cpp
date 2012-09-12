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

#include "Conspire/GUI/accountpage.h"
#include "Conspire/GUI/userpage.h"
#include "Conspire/GUI/workpage.h"
#include "Conspire/GUI/widgetrack.h"
#include "Conspire/GUI/button.h"
#include "Conspire/GUI/exceptionpage.h"
#include "Conspire/GUI/widgetstack.h"

#include "Conspire/option.h"
#include "Conspire/values.h"

#include "Conspire/conspire.hpp"

#include <QCoreApplication>

#include <QPainter>
#include <QRectF>

#include <QProcess>
#include <QProcessEnvironment>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QRegExp>
#include <QComboBox>
#include <QProgressBar>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QSettings>

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include "Acquire/acquire_client.h"

using namespace Conspire;

static QString install_dir 
                = "/home/benlong/conspire/job_classes";

static QString broker = "ssi-amrmmhd.epcc.ed.ac.uk";
//static QString broker = "127.0.0.1";

void UserPage::continueToWorkStores()
{
   emit( push( PagePointer( new WorkPage())));
}

void UserPage::modifyAccount()
{
   emit( push( PagePointer( new AccountPage())));
}

void UserPage::build()
{

    //this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    
    WidgetRack *rack = new WidgetRack(this);
    
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(::Qt::Vertical, this);
    
    this->setLayout(l);
    
    l->insertItem(0,rack);

    stack = new WidgetStack(this);
    stack->setFocusPolicy(::Qt::NoFocus);
    
    WidgetRack *sub_rack = new WidgetRack(this);
    sub_rack->setFocusPolicy(::Qt::NoFocus);
    
       lineedit_host = new QLineEdit();
    if (usemode == 1)
    {
       QLabel *intro_label = new QLabel(Conspire::tr("For Acquire to push your work to a cluster, "
          "trusted machine logins must be added to your account. These are transparently appended "
          "to an encrypted remote keyring which can only be opened with your main password."));
       intro_label->setWordWrap(true);
       intro_label->adjustSize();
       sub_rack->addWidget(intro_label);
       QLabel *label_host = new QLabel(Conspire::tr("Machine to add to remote SSH keyring:"));
       label_host->setFocusPolicy(::Qt::NoFocus);
       sub_rack->addWidget(label_host);
       lineedit_host->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
       lineedit_host->setPlaceholderText("Hostname");
       sub_rack->addWidget(lineedit_host);
    }
    
    if (usemode == 0)
    {
       QLabel *hello_label_unknown = new QLabel(Conspire::tr("Hello. Please log in."));
       sub_rack->addWidget(hello_label_unknown);
    }
    QLabel *label_username = new QLabel(Conspire::tr("Username:"));
    label_username->setFocusPolicy(::Qt::NoFocus);
    sub_rack->addWidget(label_username);
    lineedit_username = new QLineEdit();
    lineedit_username->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    lineedit_username->setPlaceholderText("Username");
    lineedit_username->setFocusPolicy(::Qt::StrongFocus);
    sub_rack->addWidget(lineedit_username);

    QLabel *label_password = new QLabel(Conspire::tr("Password:"));
    label_password->setFocusPolicy(::Qt::NoFocus);
    sub_rack->addWidget(label_password);
    lineedit_password = new QLineEdit();
    lineedit_password->setEchoMode(QLineEdit::Password);
    lineedit_password->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    lineedit_password->setPlaceholderText("Password");
    lineedit_password->setFocusPolicy(::Qt::StrongFocus);
    connect(lineedit_password, SIGNAL(returnPressed()), this, SLOT(login()));
    sub_rack->addWidget(lineedit_password);

    if (usemode == 1)
    {
        button = new Button(Conspire::tr("Add"), this);
        sub_rack->addWidget(button);
        connect(button, SIGNAL(clicked()), this, SLOT(sshadd()));
        lineedit_host->setFocus();
        return_button = new Button(Conspire::tr("Cancel"));
        connect(return_button, SIGNAL(clicked()), this, SIGNAL(pop()));
        sub_rack->addWidget(return_button);
    }

    login_label = new QLabel();
    sub_rack->addWidget(login_label);
    
    stack->addWidget(sub_rack);
    
    WidgetRack *sub_rack2 = new WidgetRack(this);
    
    QSettings *qsetter = new QSettings("UoB", "AcquireClient");
    last_username = qsetter->value("preferences/lastUsername", QString("")).toString();
    delete qsetter;
    
    QLabel *hello_label_known = new QLabel(Conspire::tr("Hello %1. Please type your password.").arg(last_username));
    sub_rack2->addWidget(hello_label_known);
    QLabel *label_password_known = new QLabel(Conspire::tr("Password:"));
    sub_rack2->addWidget(label_password_known);
    lineedit_password_known = new QLineEdit();
    lineedit_password_known->setEchoMode(QLineEdit::Password);
    lineedit_password_known->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    lineedit_password_known->setPlaceholderText("Password");
    connect(lineedit_password_known, SIGNAL(returnPressed()), this, SLOT(login()));
    sub_rack2->addWidget(lineedit_password_known);
    QLabel *ifyouare_label_loggedin2 = new QLabel(Conspire::tr("If you are not %1,"
       " or would like to use another account, please use the 'Change user' button below").arg(last_username));
    ifyouare_label_loggedin2->setWordWrap(true);
    sub_rack2->addWidget(ifyouare_label_loggedin2);

    chgusr1_button = new Button(Conspire::tr("Change user..."));
    connect(chgusr1_button, SIGNAL(clicked()), this, SLOT(changeUser()));
    sub_rack2->addWidget(chgusr1_button);
    
    stack->addWidget(sub_rack2);
    
    WidgetRack *sub_rack3 = new WidgetRack(this);
    
    QLabel *hello_label_loggedin = new QLabel(Conspire::tr("Hello %1. You are logged in.").arg(last_username));
    sub_rack3->addWidget(hello_label_loggedin);
    QLabel *ifyouare_label_loggedin = new QLabel(Conspire::tr("If you are not %1,"
       " or would like to use another account, please use the 'Change user' button below").arg(last_username));
    ifyouare_label_loggedin->setWordWrap(true);
    sub_rack3->addWidget(ifyouare_label_loggedin);
    
    chgusr2_button = new Button(Conspire::tr("Change user..."));
    connect(chgusr2_button, SIGNAL(clicked()), this, SLOT(changeUser()));
    sub_rack3->addWidget(chgusr2_button);
    
    continuebutton = new Button(Conspire::tr("To work..."));
    connect(continuebutton, SIGNAL(clicked()), this, SLOT(continueToWorkStores()));
    sub_rack3->addWidget(continuebutton);
    
    modifybutton = new Button(Conspire::tr("Modify account..."));
    connect(modifybutton, SIGNAL(clicked()), this, SLOT(modifyAccount()));
    sub_rack3->addWidget(modifybutton);
    
    stack->addWidget(sub_rack3);
    

    rack->addWidget(stack);
    
    submode = (strlen(last_username.toAscii().constData()) > 0);
    
    if (usemode == 0)
    {
       stack->switchTo(submode);
       if (submode)
       {
           button = new Button(Conspire::tr("Log in"), this);
           rack->addWidget(button);
           connect(button, SIGNAL(clicked()), this, SLOT(login()));
           lineedit_password_known->setFocus();
       } else
       {
           button = new Button(Conspire::tr("Log in"), this);
           rack->addWidget(button);
           connect(button, SIGNAL(clicked()), this, SLOT(login()));
           lineedit_username->setFocus();
       }
    }
}

/** Constructor */
UserPage::UserPage(int iusemode, Page *parent) : Page(parent)
{
    usemode = iusemode;
    build();
}

/** Destructor */
UserPage::~UserPage()
{}

void UserPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void UserPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void UserPage::paint(QPainter *painter, 
                       const QStyleOptionGraphicsItem *option, 
                       QWidget *widget)
{
    Page::paint(painter, option, widget);

}

void UserPage::allUpdate()
{
    //status_label->update();
    //progress_bar->update();
    QCoreApplication::processEvents();
}

QString UserPage::addMachine(QString username, QString password, QString machinename, bool *loginsuccessful)
{
   if ((machinename.toAscii().constData() == NULL) || (strlen(machinename.toAscii().constData()) == 0))
      return "No machine hostname specified";
   if ((username.toAscii().constData() == NULL) || (strlen(username.toAscii().constData()) == 0))
      return "No username specified";
   QString userathost = QString("%1@%2").arg(username, machinename);
   char *failed_hosts_list = NULL;
   if (loginsuccessful) *loginsuccessful = FALSE;
   int retval = AcquireAddSSHHostToPool(userathost.toAscii().constData(), password.toAscii().constData(), &failed_hosts_list);
   switch (retval)
   {
      case ACQUIRE_USER_ADD_SSH__SSH_SUCCESS:
      {
         if (loginsuccessful) *loginsuccessful = TRUE;
         return "Successfuly added machine";
      }
      case ACQUIRE_USER_ADD_SSH__FINGERPRINT_UNTRUSTED:
         return "The fingerprint for this machine has changed, contact administrator";
      case ACQUIRE_USER_ADD_SSH__SSH_ROUTING_FAILED:
      {
         if ((failed_hosts_list == NULL) || (strlen(failed_hosts_list) <= 1))
         {
            return "Bad password, machine not trusted or could not find machine (connection failed)";
         } else
         {
            return QObject::tr("Could not reach machine, failed to connect to %1").arg(failed_hosts_list);
         }
      }
   }
   return "Failed to verify with machine";
}

QString UserPage::loginUser(QString username, QString password, bool *loginsuccessful)
{
   int login_success = AcquireClientInit(DEFAULT_HOST, DEFAULT_PORT, username.toAscii().constData(),
                                         password.toAscii().constData(), "broker");
   if (login_success == ACQUIRE_USER_REGISTRATION_STATE__FINAL)
   {
      // Successful log in
      if (loginsuccessful) *loginsuccessful = TRUE;
      QSettings *qsetter = new QSettings("UoB", "AcquireClient");
      qsetter->setValue("preferences/lastUsername", username);
      delete qsetter;
      stack->switchTo(2);
      return "Log in successful";
   } else
   {
      // Failed to log in
      AcquireClientDestroy();
      if (loginsuccessful) *loginsuccessful = FALSE;
      return "Log in failed";
   }
}

void UserPage::changeUser()
{
   button->show();
   login_label->setText("");
   lineedit_username->setText("");
   lineedit_password->setText("");
   if (AcquireClientIsInitialised())
   {
      AcquireClientDestroy();
   }
   submode = 0;
   stack->switchTo(0);
   lineedit_username->setFocus();
   allUpdate();
}

void UserPage::login()
{
    QString username;
    QString password;
    if (submode)
    {
       username = last_username;
       password = lineedit_password_known->text();
    } else
    {
       username = lineedit_username->text();
       password = lineedit_password->text();
    }
    bool wasokay = FALSE;
    QString login_status = loginUser(username, password, &wasokay);

    if (wasokay)
    {
       button->hide();
       stack->switchTo(2);
    }
    login_label->setText(login_status);
    allUpdate();
}

void UserPage::sshadd()
{
    QString susername = lineedit_username->text();
    QString machinename = lineedit_host->text();
    QString spassword = lineedit_password->text();
    
    bool wasokay = FALSE;
    QString sshadd_retstring = addMachine(susername, spassword, machinename, &wasokay);

    if (wasokay) button->hide();
    login_label->setText(sshadd_retstring);
    allUpdate();
    if (wasokay) emit( pop() );
}