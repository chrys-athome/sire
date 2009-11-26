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

#include "renderviewcommand.h"
#include "renderview.h"

#include "SireStream/datastream.h"

#include "SireError/errors.h"

using namespace Spier;
using namespace SireStream;

static const RegisterMetaType<RenderViewCommand> r_rvcommand( MAGIC_ONLY,
                                                    RenderViewCommand::typeName() );
                                                    
/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, 
                                     const RenderViewCommand &rvcommand)
{
    writeHeader(ds, r_rvcommand, 1);
    
    ds << static_cast<const Command&>(rvcommand);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, RenderViewCommand &rvcommand)
{
    VersionID v = readHeader(ds, r_rvcommand);
    
    if (v == 1)
    {
        ds >> static_cast<Command&>(rvcommand);
        rvcommand.render_view = 0;
    }
    else
        throw version_error( v, "1", r_rvcommand, CODELOC );
    
    return ds;
}

/** Constructor */
RenderViewCommand::RenderViewCommand(const QString &description) 
                  : Command(description), render_view(0)
{}

/** Copy constructor */
RenderViewCommand::RenderViewCommand(const RenderViewCommand &other)
                  : Command(other), render_view(other.render_view)
{}

/** Destructor */
RenderViewCommand::~RenderViewCommand()
{}

/** Copy assignment operator */
RenderViewCommand& RenderViewCommand::operator=(const RenderViewCommand &other)
{
    render_view = other.render_view;
    Command::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool RenderViewCommand::operator==(const RenderViewCommand &other) const
{
    return render_view == other.render_view and 
           Command::operator==(other);
}

/** Comparison operator */
bool RenderViewCommand::operator!=(const RenderViewCommand &other) const
{
    return render_view != other.render_view or 
           Command::operator!=(other);
}

/** Return the render view on which this command last operated

    \throw SireError::nullptr_error
*/
RenderView& RenderViewCommand::renderView()
{
    if (not render_view)
        throw SireError::nullptr_error( QObject::tr(
                "Cannot get the RenderView of a command that has not been actioned "
                "(%1).")
                    .arg(this->toString()), CODELOC );

    return *render_view;
}
    
/** Return a const reference to the view on which this command
    last operated
    
    \throw SireError::nullptr_error
*/
const RenderView& RenderViewCommand::renderView() const
{
    return const_cast<RenderViewCommand*>(this)->renderView();
}

/** Execute this command on the passed render view */
CommandPtr RenderViewCommand::operator()(RenderView &renderview) const
{
    CommandPtr c = *this;
    
    RenderViewCommand &execute_command = c.edit().asA<RenderViewCommand>();
    
    execute_command.render_view = &renderview;
    execute_command.execute();
    
    return c;
}

/** Make this command an orphan */
void RenderViewCommand::makeOrphan()
{
    render_view = 0;
}
