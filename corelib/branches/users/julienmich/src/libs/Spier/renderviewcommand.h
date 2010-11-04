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

class RVCommandStack;

/** This is the base class of all commands that are applied
    to a render view. These commands can only be
    called by committing them to a command stack
    (the RVCommandStack). This is to ensure that the
    undo history of the commands cannot become corrupted.
    
    @author Christopher Woods
*/
class SPIER_EXPORT RenderViewCommand : public Command
{

friend class RVCommandStack;
friend class RenderView;

friend QDataStream& ::operator<<(QDataStream&, const RenderViewCommand&);
friend QDataStream& ::operator>>(QDataStream&, RenderViewCommand&);

public:
    RenderViewCommand(const QString &description);
    
    RenderViewCommand(const RenderViewCommand &other);
    
    ~RenderViewCommand();
    
    static const char* typeName()
    {
        return "Spier::RenderViewCommand";
    }

protected:
    RenderViewCommand& operator=(const RenderViewCommand &other);
    
    bool operator==(const RenderViewCommand &other) const;
    bool operator!=(const RenderViewCommand &other) const;

    RenderView& renderView();
    
    const RenderView& renderView() const;
    
    CommandPtr operator()(RenderView &render_view) const;

    void makeOrphan();

    virtual void execute()=0;

private:
    /** Pointer to the render view on which this command operated.
        This is automatically set to '0' by the RVCommandStack
        if the RenderView is deleted */
    RenderView *render_view;
};

}

SIRE_EXPOSE_CLASS( Spier::RenderViewCommand )

SIRE_END_HEADER

#endif
