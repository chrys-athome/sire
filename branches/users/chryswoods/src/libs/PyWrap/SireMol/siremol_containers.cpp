/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) <year>  <name of author>
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

#include <QVector>

#include "SirePy/convertlist.hpp"
#include "SirePy/convertdict.hpp"
#include "SirePy/convertset.hpp"

#include "ThirdParty/tuples.hpp"

#include "siremol_headers.h"

using namespace SireMol;
using namespace SirePy;

void register_SireMol_containers()
{
    register_list< QList<Molecule> >();
    register_list< QVector<Molecule> >();

    register_list< QList<EditMol> >();
    register_list< QVector<EditMol> >();

    register_list< QList<AtomIndex> >();
    register_set< QSet<AtomIndex> >();
}
