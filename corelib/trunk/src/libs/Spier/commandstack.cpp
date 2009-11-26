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

/*
#include "commandstack.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Spier;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<CommandStack> r_cmdstack;
*/
/** Seralise to a binary datastream */
/*QDataStream SPIER_EXPORT &operator<<(QDataStream &ds,
                                     const CommandStack &cmdstack)
{
    writeHeader(ds, r_cmdstack, CODELOC);
    
    SharedDataStream sds(ds);
    
    //get the current time
    quint64 time_mark = cmdstack.command_timer.elapsed();
    
    sds << cmdstack.command_stack
        << cmdstack.command_times
        << cmdstack.saved_states
        << cmdstack.current_command
        << time_mark
        << static_cast<const Property&>(cmdstack);
        
    return ds;
}
*/