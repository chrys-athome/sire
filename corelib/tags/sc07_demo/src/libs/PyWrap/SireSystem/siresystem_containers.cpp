/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007   Christopher Woods
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

#include <Python.h>
#include <boost/python.hpp>

#include "SireSystem/qhash_siresystem.h"
#include "SireMol/qhash_siremol.h"

#include <QVector>
#include <QSet>

#include <boost/tuple/tuple.hpp>

#include "SirePy/convertlist.hpp"
#include "SirePy/convertdict.hpp"
#include "SirePy/convertset.hpp"
#include "SirePy/convertsharedpointer.hpp"

#include "ThirdParty/tuples.hpp"

#include "siresystem_headers.h"

using namespace SireSystem;
using namespace SirePy;

using boost::python::register_tuple;

void register_SireSystem_containers()
{
    register_container< SystemMonitor, SystemMonitorBase >();
    register_container< Move, MoveBase >();
    register_container< Moves, MovesBase >();

    register_list< QList<Move> >();

    register_tuple< boost::tuple<Move,double> >();
    
    register_list< QVector< boost::tuple<Move,double> > >();
    
    register_tuple< SireSystem::detail::AtomPairs >();

    #if QT_VERSION >= 0x402000

    register_dict< QHash<Symbol, SystemMonitor> >();
    
    register_dict< QHash<SireSystem::detail::AtomPairs,RDF> >();

    #else
    
    register_dict< QHash<Symbol, SystemMonitor>,
                   Symbol, SystemMonitor >();
    
    register_dict< QHash<SireSystem::detail::AtomPairs,RDF>,
                   SireSystem::detail::AtomPairs, RDF >();
    
    #endif    
}
