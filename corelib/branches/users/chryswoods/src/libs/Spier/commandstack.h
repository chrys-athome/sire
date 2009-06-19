/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SPIER_COMMANDSTACK_H
#define SPIER_COMMANDSTACK_H

#include <QTime>

#include "command.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class CommandStack;
}

QDataStream& operator<<(QDataStream&, const Spier::CommandStack&);
QDataStream& operator>>(QDataStream&, Spier::CommandStack&);

namespace Spier
{

/** This class provides a stack of Spier::Command objects, thereby
    allowing a record of commands to be kept, and also so that
    an undo/redo framework exists
    
    @author Christopher Woods
*/
class SPIER_EXPORT CommandStack 
            : public SireBase::ConcreteProperty<CommandStack,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const CommandStack&);
friend QDataStream& ::operator>>(QDataStream&, CommandStack&);

public:
    CommandStack();
    
    CommandStack(const QByteArray &initial_state);
    
    CommandStack(const CommandStack &other);

    virtual ~CommandStack();
    
    CommandStack& operator=(const CommandStack &other);
    
    bool operator==(const CommandStack &other) const;
    bool operator!=(const CommandStack &other) const;
    
    static const char* typeName();
    
    virtual CommandStack* clone() const
    {
        return new CommandStack(*this);
    }
    
    virtual void undo();
    virtual void redo();
    
    virtual void scanTo(quint64 time);
    virtual void skipTo(quint64 time);
    
    virtual void saveCurrentState();
    
    virtual void execute(const Command &command);

    static const CommandStack& null();

private:
    /** A timer used to measure the time between commands */
    QTime command_timer;
    
    /** The stack of all of the commands that have been 
        performed */
    QList<CommandPtr> command_stack;
    
    /** The times at which each command was performed */
    QList<quint64> command_times;
    
    /** Saved states of the document - there must be at least
        on complete saved state - this allows any state of the
        document to be constructed */
    QMap<quint64,QByteArray> saved_states;
    
    /** Which command are we currently on? */
    quint64 current_command;

    /** Time tracker */
    quint64 time_marker;
};

typedef SireBase::PropPtr<CommandStack> CommandStackPtr;

}

//Q _ DECLARE_METATYPE( Spier::CommandStack )

//S I RE_EXPOSE_CLASS( Spier::CommandStack )

//S I RE_EXPOSE_PROPERTY( Spier::CommandStackPtr, Spier::CommandStack )

SIRE_END_HEADER

#endif
