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

#include <QVector>
#include <QSet>

#include <boost/tuple/tuple.hpp>

#include "Helpers/convertlist.hpp"
#include "Helpers/convertdict.hpp"
#include "Helpers/convertset.hpp"
#include "Helpers/tuples.hpp"
#include "Base/convertpackedarray.hpp"

#include "SireMol/atom.h"
#include "SireMol/atomidentifier.h"
#include "SireMol/cutgroup.h"
#include "SireMol/residue.h"
#include "SireMol/chain.h"
#include "SireMol/segment.h"
#include "SireMol/molecule.h"
#include "SireMol/atomselection.h"
#include "SireMol/moleculegroup.h"
#include "SireMol/moleculegroups.h"
#include "SireMol/mgnum.h"
#include "SireMol/mgname.h"
#include "SireMol/partialmolecule.h"

using namespace SireMol;

using boost::python::register_tuple;

void register_SireMol_containers()
{
    register_list< QList<AtomIdx> >();
    register_list< QVector<AtomIdx> >();

    register_list< QList<CGIdx> >();
    register_list< QList<ResIdx> >();
    register_list< QList<ChainIdx> >();
    register_list< QList<SegIdx> >();

    register_list< QList<MolNum> >();
    register_list< QList<MGNum> >();
    register_list< QList<MGName> >();

    register_list< QList<AtomSelection> >();
    register_list< QList<ViewsOfMol> >();
    register_list< QList<Molecule> >();
    register_list< QList<MoleculeGroup> >();

    register_list< QList< boost::tuple<AtomIdentifier,AtomIdentifier> > >();

    register_list< QVector< boost::tuple<MolNum,SireID::Index> > >(); 

    register_tuple< boost::tuple<AtomIdx,AtomIdx> >();
    register_tuple< boost::tuple<AtomIdx,AtomIdx,AtomIdx> >();
    register_tuple< boost::tuple<AtomIdx,AtomIdx,AtomIdx,AtomIdx> >();

    register_tuple< boost::tuple<AtomIdentifier,AtomIdentifier> >();

    register_tuple< boost::tuple<AtomSelection,AtomSelection> >();    

    register_tuple< boost::tuple<PartialMolecule,double> >();

    register_tuple< boost::tuple<MolNum,SireID::Index> >();

    #if QT_VERSION >= 0x402000
    register_dict< QHash<MolNum,Selector<Atom> >();
    register_dict< QHash<MolNum,Selector<CutGroup> >();
    register_dict< QHash<MolNum,Selector<Residue> >();
    register_dict< QHash<MolNum,Selector<Chain> >();
    register_dict< QHash<MolNum,Selector<Segment> >();

    register_set< QSet<AtomIdx> >();
    register_set< QSet<ResIdx> >();

    register_set< QSet<MolNum> >();
    register_set< QSet<MolName> >();

    #else
    register_dict< QHash< MolNum,Selector<Atom> >, MolNum, Selector<Atom> >();
    register_dict< QHash< MolNum,Selector<CutGroup> >, MolNum, Selector<CutGroup> >();
    register_dict< QHash< MolNum,Selector<Residue> >, MolNum, Selector<Residue> >();
    register_dict< QHash< MolNum,Selector<Chain> >, MolNum, Selector<Chain> >();
    register_dict< QHash< MolNum,Selector<Segment> >, MolNum, Selector<Segment> >();

    register_set< QSet<AtomIdx>, AtomIdx >();
    register_set< QSet<ResIdx>, ResIdx >();
    
    register_set< QSet<MolNum>, MolNum >();
    register_set< QSet<MolName>, MolName >();

    #endif    
}
