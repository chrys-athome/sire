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

#include "command.h"
#include "renderview.h"

#include "SireStream/datastream.h"

using namespace Spier;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of Command
//////////

static const RegisterMetaType<Command> r_command( MAGIC_ONLY, Command::typeName() );

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const Command &command)
{
    writeHeader(ds, r_command, 1);
    
    ds << command.description_text
       << static_cast<const Property&>(command);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, Command &command)
{
    VersionID v = readHeader(ds, r_command);
    
    if (v == 1)
    {
        ds >> command.description_text
           >> static_cast<Property&>(command);
    }
    else
        throw version_error( v, "1", r_command, CODELOC );
        
    return ds;
}

/** Construct using the passed description */
Command::Command(const QString &description)
        : Property(), description_text(description)
{}

/** Copy constructor */
Command::Command(const Command &other)
        : Property(other), description_text(other.description_text)
{}

/** Destructor */
Command::~Command()
{}

/** Copy assignment operator */
Command& Command::operator=(const Command &other)
{
    description_text = other.description_text;
    Property::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool Command::operator==(const Command &other) const
{
    return description_text == other.description_text and
           Property::operator==(other);
}

/** Comparison operator */
bool Command::operator!=(const Command &other) const
{
    return description_text != other.description_text or
           Property::operator!=(other);
}

/** Return the short string used to describe this 
    command to the user */
const QString& Command::text() const
{
    return description_text;
}

Q_GLOBAL_STATIC( NullCommand, nullCommand )

const NullCommand& Command::null()
{
    return *(nullCommand());
}

//////////
////////// Implementation of NullCommand
//////////

static const RegisterMetaType<NullCommand> r_nullcommand;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const NullCommand &nullcommand)
{
    writeHeader(ds, r_nullcommand, 1);
    
    ds << static_cast<const Command&>(nullcommand);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, NullCommand &nullcommand)
{
    VersionID v = readHeader(ds, r_nullcommand);
    
    if (v == 1)
    {
        ds >> static_cast<Command&>(nullcommand);
    }
    else
        throw version_error( v, "1", r_nullcommand, CODELOC );
        
    return ds;
}

/** Constructor */
NullCommand::NullCommand() 
            : ConcreteProperty<NullCommand,Command>( QObject::tr("NULL") )
{}

/** Copy constructor */
NullCommand::NullCommand(const NullCommand &other)
            : ConcreteProperty<NullCommand,Command>(other)
{}

/** Destructor */
NullCommand::~NullCommand()
{}

/** Copy assignment operator */
NullCommand& NullCommand::operator=(const NullCommand &other)
{
    Command::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullCommand::operator==(const NullCommand&) const
{
    return true;
}

/** Comparison operator */
bool NullCommand::operator!=(const NullCommand&) const
{
    return false;
}

const char* NullCommand::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullCommand>() );
}

/** Null command does nothing */
void NullCommand::operator()(RenderView&) const
{}

/** Undoing nothing does nothing */
void NullCommand::undo(RenderView&) const
{}

/** Redoing nothing does nothing */
void NullCommand::redo(RenderView&) const
{}

/** Merging a NullCommand with another command returns a command */
CommandPtr NullCommand::mergeWith(const Command &command)
{
    return command;
}
