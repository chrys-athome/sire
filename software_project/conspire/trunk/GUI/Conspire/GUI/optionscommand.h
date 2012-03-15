#ifndef CONSPIRE_OPTIONSCOMMAND_H
#define CONSPIRE_OPTIONSCOMMAND_H
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

#include "Conspire/conspire.h"

#include <QUndoCommand>
#include <QPointer>

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    class OptionsWidget;

    /** This is the base class of all Options commands, which
        are used to describe the editing of an Options object */
    class CONSPIRE_EXPORT OptionsCommand : public QUndoCommand
    {
    public:
        OptionsCommand(OptionsCommand *parent=0);
        OptionsCommand(OptionsWidget *widget, OptionsCommand *parent=0);

        virtual ~OptionsCommand();
        
        void undo();
        void redo();

    protected:
        Options oldState() const;
        virtual Options newState() const=0;
        
    private:
        QPointer<OptionsWidget> w;
        
        /** The state of the Options object just before this
            command was performed */
        Options *old_state;
    };

    /** This command is used to set an option value */
    class CONSPIRE_EXPORT OptionsUpdateCommand : public OptionsCommand
    {
    public:
        OptionsUpdateCommand(OptionsCommand *parent=0);
        OptionsUpdateCommand(OptionsWidget *widget, OptionsCommand *parent=0);
        
        OptionsUpdateCommand(OptionsWidget *widget,
                             QString key, 
                             const Value &new_value,
                             OptionsCommand *parent=0);
                             
        ~OptionsUpdateCommand();
    
    protected:
        Options newState() const;
        
    private:
        /** The key to be updated */
        QString k;
        
        /** The new value for this key */
        Obj val;
    };

    /** This command is used to add a new option value */
    class CONSPIRE_EXPORT OptionsAddCommand : public OptionsCommand
    {
    public:
        OptionsAddCommand(OptionsCommand *parent=0);
        OptionsAddCommand(OptionsWidget *widget, OptionsCommand *parent=0);
        
        OptionsAddCommand(OptionsWidget *widget,
                          QString new_key,
                          OptionsCommand *parent=0);
                          
        ~OptionsAddCommand();
        
    protected:
        Options newState() const;
        
    private:
        /** The new key to add */
        QString k;
    }; 
    
    /** This command is used to remove an option value */
    class CONSPIRE_EXPORT OptionsRemoveCommand : public OptionsCommand
    {
    public:
        OptionsRemoveCommand(OptionsCommand *parent=0);
        OptionsRemoveCommand(OptionsWidget *widget, OptionsCommand *parent=0);
        
        OptionsRemoveCommand(OptionsWidget *widget,
                             QString take_key,
                             OptionsCommand *parent=0);
                             
        ~OptionsRemoveCommand();
        
    protected:
        Options newState() const;
        
    private:
        /** The key to be removed */
        QString k;
    };
}

CONSPIRE_END_HEADER

#endif
