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

#ifndef SPIER_RENDERVIEWCOMMAND_H
#define SPIER_RENDERVIEWCOMMAND_H

#include "command.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class RenderViewCommand;
}

QDataStream& operator<<(QDataStream&, const Spier::RenderViewCommand&);
QDataStream& operator>>(QDataStream&, Spier::RenderViewCommand&);

namespace Spier
{

/** This is the base class of all commands that are applied
    to a render view
    
    @author Christopher Woods
*/
class SPIER_EXPORT RenderViewCommand : public Command
{

friend QDataStream& ::operator<<(QDataStream&, const RenderViewCommand&);
friend QDataStream& ::operator>>(QDataStream&, RenderViewCommand&);

public:
    RenderViewCommand();
    
    RenderViewCommand(const RenderViewCommand &other);
    
    ~RenderViewCommand();
    
    static const char* typeName()
    {
        return "Spier::RenderViewCommand";
    }
    
    virtual CommandPtr operator()(RenderView &render_view) const=0;

protected:
    RenderViewCommand(RenderView &render_view);
    
    RenderViewCommand& operator=(const RenderViewCommand &other);
    
    bool operator==(const RenderViewCommand &other) const;
    bool operator!=(const RenderViewCommand &other) const;

    RenderView& renderView();
    
    const RenderView& renderView() const;

private:
    /** Guarded pointer to the render view that is being
        operated on by this command */
    QPointer<RenderView> render_view;
};

}

SIRE_EXPOSE_CLASS( Spier::RenderViewCommand )

SIRE_END_HEADER

#endif
