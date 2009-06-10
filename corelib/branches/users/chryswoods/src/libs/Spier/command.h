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

#ifndef SPIER_COMMAND_H
#define SPIER_COMMAND_H

#include "SireBase/property.h"

#include "renderview.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class Command;
class NullCommand;
}

QDataStream& operator<<(QDataStream&, const Spier::Command&);
QDataStream& operator>>(QDataStream&, Spier::Command&);

QDataStream& operator<<(QDataStream&, const Spier::NullCommand&);
QDataStream& operator>>(QDataStream&, Spier::NullCommand&);

namespace Spier
{

/** This is the base class of all commands. Spier works by passing
    in commands, each of which can be undone, and each of which
    records the time at which it is performed. This allows a 
    timeline of commands to be stored, together with a 
    history which can be rewound
    
    @author Christopher Woods
*/
class SPIER_EXPORT Command : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Command&);
friend QDataStream& ::operator>>(QDataStream&, Command&);

public:
    Command();
    
    Command(const Command &other);
    
    virtual ~Command();
    
    static const char* typeName()
    {
        return "Spier::Command";
    }
    
    virtual Command* clone() const=0;
    
    int nChildren() const;
    
    const Command& child(int i) const;

    virtual void operator()(RenderView &render_view) const=0;

    virtual void undo(RenderView &render_view) const=0;
    virtual void redo(RenderView &render_view) const=0;

    virtual bool mergeWith(const Command &other)=0;

protected:
    Command& operator=(const Command &other);
    
    bool operator==(const Command &other) const;
    bool operator!=(const Command &other) const;
};

/** This is a null command that does nothing */
class SPIER_EXPORT NullCommand : public SireBase::ConcreteProperty<NullCommand,Command>
{

friend QDataStream& ::operator<<(QDataStream&, const NullCommand&);
friend QDataStream& ::operator>>(QDataStream&, NullCommand&);

public:
    NullCommand();
    NullCommand(const NullCommand &other);
    
    ~NullCommand();
    
    NullCommand& operator=(const NullCommand &other);
    
    bool operator==(const NullCommand &other) const;
    bool operator!=(const NullCommand &other) const;
    
    static const char* typeName();
    
    void operator()(RenderView &render_view) const;

    void undo(RenderView &render_view) const;
    void redo(RenderView &render_view) const;

    bool mergeWith(const Command &other);
};

}

Q_DECLARE_METATYPE( Spier::NullCommand )

SIRE_EXPOSE_CLASS( Spier::Command )
SIRE_EXPOSE_CLASS( Spier::NullCommand )

SIRE_EXPOSE_PROPERTY( Spier::CommandPtr, Spier::Command )

SIRE_END_HEADER

#endif
