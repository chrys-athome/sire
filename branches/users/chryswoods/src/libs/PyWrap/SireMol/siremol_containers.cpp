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

#include "SireMol/qhash_siremol.h"

#include <QVector>
#include <QSet>

#include <boost/tuple/tuple.hpp>

#include "SirePy/convertlist.hpp"
#include "SirePy/convertdict.hpp"
#include "SirePy/convertset.hpp"

#include "ThirdParty/tuples.hpp"

#include "siremol_headers.h"

using namespace SireMol;
using namespace SireMaths;
using namespace SirePy;

using boost::python::register_tuple;

void register_SireMol_containers()
{
    register_list< QList<Molecule> >();
    register_list< QVector<Molecule> >();

    register_list< QList<EditMol> >();
    register_list< QVector<EditMol> >();

    register_list< QList<EditRes> >();
    register_list< QVector<EditRes> >();

    register_list< QList<Residue> >();
    register_list< QVector<Residue> >();

    register_list< QList<Atom> >();
    register_list< QVector<Atom> >();

    register_list< QList<NewAtom> >();
    register_list< QVector<NewAtom> >();

    register_list< QList<AtomIndex> >();
    register_list< QVector<AtomIndex> >();

    register_list< QList<AtomInfo> >();
    register_list< QVector<AtomInfo> >();

    register_list< QList<Bond> >();
    register_list< QVector<Bond> >();
    
    register_list< QList<ResidueBonds> >();
    register_list< QVector<ResidueBonds> >();

    register_list< QList<ResNum> >();
    register_list< QVector<ResNum> >();
    
    register_list< QList<CutGroup> >();
    register_list< QVector<CutGroup> >();

    register_list< QList<CGAtomID> >();
    register_list< QVector<CGAtomID> >();

    register_list< QVector<MoleculeGroup> >();
    register_list< QList<MoleculeGroup> >();

    register_tuple< boost::tuple<QString,ResNum> >();
    register_tuple< boost::tuple<AtomIndex,AtomIndex> >();
    register_tuple< boost::tuple<AtomIndex,AtomIndex,AtomIndex> >();
    register_tuple< boost::tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex> >();
    
    register_tuple< boost::tuple<CutGroupID,AtomID> >();
    register_tuple< boost::tuple<CutGroupNum,AtomID> >();
    register_tuple< boost::tuple<ResNum,AtomID> >();
    register_tuple< boost::tuple<ResID,AtomID> >();
    register_tuple< boost::tuple<ResNum,QString> >();
    register_tuple< boost::tuple<ResID,QString> >();
    
    #if QT_VERSION >= 0x402000
    register_set< QSet<AtomIndex> >();
    register_set< QSet<MoleculeID> >();
    register_set< QSet<MoleculeGroupID> >();

    register_set< QSet<CutGroupID> >();
    register_set< QSet<CutGroupNum> >();

    register_set< QSet<ResNum> >();
    register_set< QSet<ResID> >();

    register_set< QSet<AtomID> >();

    register_dict< QHash< ResNum, QVector<Atom> > >();
    register_dict< QHash< ResID, QVector<Atom> > >();
    register_dict< QHash< CutGroupID, QVector<Atom> > >();
    register_dict< QHash< CutGroupNum, QVector<Atom> > >();

    register_dict< QHash< ResNum, QVector<AtomInfo> > >();
    register_dict< QHash< ResID, QVector<AtomInfo> > >();
    register_dict< QHash< CutGroupID, QVector<AtomInfo> > >();
    register_dict< QHash< CutGroupNum, QVector<AtomInfo> > >();

    register_dict< QHash< ResNum, QVector<Vector> > >();
    register_dict< QHash< ResID, QVector<Vector> > >();
    register_dict< QHash< CutGroupID, QVector<Vector> > >();
    register_dict< QHash< CutGroupNum, QVector<Vector> > >();
    
    register_dict< QHash<MoleculeID,Molecule> >();
    register_dict< QHash<MoleculeGroupID,MoleculeGroup> >();

    register_dict< QHash< CutGroupID, QSet<AtomID> > >();
    
    register_dict< QHash< CutGroupID, AtomInfoGroup> >();
    register_dict< QHash< CutGroupNum, AtomInfoGroup> >();

    register_dict< QHash<ResNum,EditRes> >();
    register_dict< QHash<ResID,EditRes> >();

    register_dict< QHash<ResNum,Residue> >();
    register_dict< QHash<ResID,Residue> >();

    register_dict< QHash<CutGroupID,CutGroup> >();
    register_dict< QHash<CutGroupNum,CutGroup> >();

    register_dict< QHash<AtomID,Atom> >();
    register_dict< QHash<CGAtomID,Atom> >();
    register_dict< QHash<CGNumAtomID,Atom> >();
    register_dict< QHash<ResNumAtomID,Atom> >();
    register_dict< QHash<ResIDAtomID,Atom> >();
    register_dict< QHash<AtomIndex,Atom> >();

    register_dict< QHash<AtomID,AtomInfo> >();
    register_dict< QHash<CGAtomID,AtomInfo> >();
    register_dict< QHash<CGNumAtomID,AtomInfo> >();
    register_dict< QHash<ResNumAtomID,AtomInfo> >();
    register_dict< QHash<ResIDAtomID,AtomInfo> >();
    register_dict< QHash<AtomIndex,AtomInfo> >();

    register_dict< QHash<AtomID,Vector> >();
    register_dict< QHash<CGAtomID,Vector> >();
    register_dict< QHash<CGNumAtomID,Vector> >();
    register_dict< QHash<ResNumAtomID,Vector> >();
    register_dict< QHash<ResIDAtomID,Vector> >();
    register_dict< QHash<AtomIndex,Atom> >();

    register_dict< QHash<QString,Atom> >();

    #else
    register_set< QSet<AtomIndex>, AtomIndex >();
    register_set< QSet<MoleculeID>, MoleculeID >();
    register_set< QSet<MoleculeGroupID>, MoleculeGroupID >();

    register_set< QSet<CutGroupID>, CutGroupID >();
    register_set< QSet<CutGroupNum>, CutGroupNum >();

    register_set< QSet<ResNum>, ResNum >();
    register_set< QSet<ResID>, ResID >();

    register_set< QSet<AtomID>, AtomID >();

    register_dict< QHash< ResNum, QVector<Atom> >, 
                                    ResNum, QVector<Atom> >();
    register_dict< QHash< ResID, QVector<Atom> >, 
                                    ResID, QVector<Atom> >();
    register_dict< QHash< CutGroupID, QVector<Atom> >, 
                                    CutGroupID, QVector<Atom> >();
    register_dict< QHash< CutGroupNum, QVector<Atom> >,
                                    CutGroupNum, QVector<Atom> >();

    register_dict< QHash< ResNum, QVector<AtomInfo> >,
                                    ResNum, QVector<AtomInfo> >();
    register_dict< QHash< ResID, QVector<AtomInfo> >,
                                    ResID, QVector<AtomInfo> >();
    register_dict< QHash< CutGroupID, QVector<AtomInfo> >,
                                    CutGroupID, QVector<AtomInfo> >();
    register_dict< QHash< CutGroupNum, QVector<AtomInfo> >,
                                    CutGroupNum, QVector<AtomInfo> >();

    register_dict< QHash< ResNum, QVector<Vector> >,
                                    ResNum, QVector<Vector> >();
    register_dict< QHash< ResID, QVector<Vector> >,
                                    ResID, QVector<Vector> >();
    register_dict< QHash< CutGroupID, QVector<Vector> >,
                                    CutGroupID, QVector<Vector> >();
    register_dict< QHash< CutGroupNum, QVector<Vector> >,
                                    CutGroupNum, QVector<Vector> >();
    
    register_dict< QHash<MoleculeID,Molecule>, MoleculeID, Molecule >();
    register_dict< QHash<MoleculeGroupID,MoleculeGroup>,
                                MoleculeGroupID, MoleculeGroup >();

    register_dict< QHash< CutGroupID, QSet<AtomID> >,
                                CutGroupID, QSet<AtomID> >();
    
    register_dict< QHash< CutGroupID, AtomInfoGroup>,
                                CutGroupID, AtomInfoGroup >();
    register_dict< QHash< CutGroupNum, AtomInfoGroup>,
                                CutGroupNum, AtomInfoGroup >();

    register_dict< QHash<ResNum,EditRes>, ResNum, EditRes >();
    register_dict< QHash<ResID,EditRes>, ResID, EditRes >();

    register_dict< QHash<ResNum,Residue>, ResNum, Residue >();
    register_dict< QHash<ResID,Residue>, ResID, Residue >();

    register_dict< QHash<CutGroupID,CutGroup>, CutGroupID, CutGroup >();
    register_dict< QHash<CutGroupNum,CutGroup>, CutGroupNum, CutGroup >();

    register_dict< QHash<AtomID,Atom>, AtomID, Atom >();
    register_dict< QHash<CGAtomID,Atom>, CGAtomID, Atom >();
    register_dict< QHash<CGNumAtomID,Atom>, CGNumAtomID, Atom >();
    register_dict< QHash<ResNumAtomID,Atom>, ResNumAtomID, Atom >();
    register_dict< QHash<ResIDAtomID,Atom>, ResIDAtomID, Atom >();
    register_dict< QHash<AtomIndex,Atom>, AtomIndex, Atom >();

    register_dict< QHash<AtomID,AtomInfo>, AtomID, AtomInfo >();
    register_dict< QHash<CGAtomID,AtomInfo>, CGAtomID, AtomInfo >();
    register_dict< QHash<CGNumAtomID,AtomInfo>, CGNumAtomID, AtomInfo >();
    register_dict< QHash<ResNumAtomID,AtomInfo>, ResNumAtomID, AtomInfo >();
    register_dict< QHash<ResIDAtomID,AtomInfo>, ResIDAtomID, AtomInfo >();
    register_dict< QHash<AtomIndex,AtomInfo>, AtomIndex, AtomInfo >();

    register_dict< QHash<AtomID,Vector>, AtomID, Vector >();
    register_dict< QHash<CGAtomID,Vector>, CGAtomID, Vector >();
    register_dict< QHash<CGNumAtomID,Vector>, CGNumAtomID, Vector >();
    register_dict< QHash<ResNumAtomID,Vector>, ResNumAtomID, Vector >();
    register_dict< QHash<ResIDAtomID,Vector>, ResIDAtomID, Vector >();
    register_dict< QHash<AtomIndex,Vector>, AtomIndex, Vector >();

    register_dict< QHash<QString,Atom>, QString, Atom >();
    
    #endif
}
