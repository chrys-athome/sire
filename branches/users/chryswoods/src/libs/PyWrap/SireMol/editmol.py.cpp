/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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
#include "boost/python.hpp"

#include "SireMol/qhash_siremol.h"

#include <QHash>

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/atom.h"

#include "SireMol/editmol.h"
#include "SireMol/editres.h"

#include "SireMol/atomidgroup.h"

#include "SireMol/bond.h"
#include "SireMol/angle.h"
#include "SireMol/dihedral.h"
#include "SireMol/improper.h"

#include "SireMol/cutgroup.h"

#include "SireMol/cgatomid.h"
#include "SireMol/cgnumatomid.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/residatomid.h"

#include "SireMol/bondaddingfunction.h"
#include "SireMol/templatefunction.h"
#include "SireMol/weightfunction.h"

#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/angle.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

namespace bp = boost::python;

namespace SireMol
{

void export_EditMol2(bp::class_<SireMol::EditMol> &editmol_wrapper);

void  
SIREMOL_EXPORT
export_EditMol()
{

    bp::class_< SireMol::EditMol > editmol_wrapper( "EditMol" );

    editmol_wrapper
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< QString const & >(( bp::arg("name") )) )
        .def( bp::init< QString const &, SireMol::CuttingFunction const & >(( bp::arg("name"), bp::arg("cuttingfunc") )) )
        .def( bp::init< SireMol::CuttingFunction const & >(( bp::arg("cuttingfunc") )) )
        .def( bp::init< SireMol::EditRes const & >(( bp::arg("other") )) )
        .def(
            "add"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Atom const & ) )( &::SireMol::EditMol::add )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "add"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Atom const &,::SireMol::CutGroupNum ) )( &::SireMol::EditMol::add )
            , ( bp::arg("atom"), bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "add"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Bond const & ) )( &::SireMol::EditMol::add )
            , ( bp::arg("bond") )
            , bp::default_call_policies() )
        .def(
            "add"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::QString const & ) )( &::SireMol::EditMol::add )
            , ( bp::arg("resnum"), bp::arg("resnam") )
            , bp::default_call_policies() )
        .def(
            "addAutoBonds"
            , (void ( ::SireMol::EditMol::* )(  ) )( &::SireMol::EditMol::addAutoBonds )
            , bp::default_call_policies() )
        .def(
            "addAutoBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::BondAddingFunction & ) )( &::SireMol::EditMol::addAutoBonds )
            , ( bp::arg("bondfunc") )
            , bp::default_call_policies() )
        .def(
            "angle"
            , &::SireMol::EditMol::angle
            , ( bp::arg("ang") )
            , bp::default_call_policies() )
        .def(
            "applyTemplate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::EditRes const &,::SireMol::ResNum,::SireMol::TemplateFunction const & ) )( &::SireMol::EditMol::applyTemplate )
            , ( bp::arg("tmpl"), bp::arg("resnum"), bp::arg("tmplfunc") )
            , bp::default_call_policies() )
        .def(
            "applyTemplate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::EditRes const &,::SireMol::ResID,::SireMol::TemplateFunction const & ) )( &::SireMol::EditMol::applyTemplate )
            , ( bp::arg("tmpl"), bp::arg("resid"), bp::arg("tmplfunc") )
            , bp::default_call_policies() )
        .def(
            "applyTemplate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::EditMol const &,::SireMol::TemplateFunction const & ) )( &::SireMol::EditMol::applyTemplate )
            , ( bp::arg("tmpl"), bp::arg("tmplfunc") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::EditRes ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::EditRes ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("i") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::CutGroup ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::CutGroup ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::AtomID ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::ResNumAtomID const & ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::ResIDAtomID const & ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::CGNumAtomID const & ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditMol::at )
            , ( bp::arg("atm") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::AtomID ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID,::SireMol::AtomID ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("cgid"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum,::SireMol::AtomID ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("cgnum"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("cgatmid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::CGNumAtomID const & ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("cgatmid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::SireMol::AtomID ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("resnum"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::ResNumAtomID const & ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::ResID,::SireMol::AtomID ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("resid"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::ResIDAtomID const & ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("atm") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::QString const & ) const)( &::SireMol::EditMol::atom )
            , ( bp::arg("resnum"), bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "atomNames"
            , (::QStringList ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::atomNames )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "atomNames"
            , (::QStringList ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::atomNames )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::atoms )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::AtomID,SireMol::Atom> ( ::SireMol::EditMol::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("atomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::CGAtomID,SireMol::Atom> ( ::SireMol::EditMol::* )( ::QSet<SireMol::CGAtomID> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("cgatomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::CGNumAtomID,SireMol::Atom> ( ::SireMol::EditMol::* )( ::QSet<SireMol::CGNumAtomID> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("cgatomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::ResNumAtomID,SireMol::Atom> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNumAtomID> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("resatomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::ResIDAtomID,SireMol::Atom> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResIDAtomID> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("resatomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::AtomIndex,SireMol::Atom> ( ::SireMol::EditMol::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("atoms") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::CutGroupID,QVector<SireMol::Atom> > ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupID> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("cgids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::CutGroupNum,QVector<SireMol::Atom> > ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupNum> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("cgnums") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::ResNum,QVector<SireMol::Atom> > ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::ResID,QVector<SireMol::Atom> > ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResID> const & ) const)( &::SireMol::EditMol::atoms )
            , ( bp::arg("resids") )
            , bp::default_call_policies() )
        .def(
            "bond"
            , &::SireMol::EditMol::bond
            , ( bp::arg("bnd") )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Bond const &,double,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Bond const &,double,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("angle"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("angle"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Bond const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Bond const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("improper"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::change )
            , ( bp::arg("improper"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "clean"
            , &::SireMol::EditMol::clean
            , bp::default_call_policies() )
        .def(
            "commit"
            , &::SireMol::EditMol::commit
            , bp::default_call_policies() )
        .def(
            "connectivity"
            , (::SireMol::MoleculeBonds ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::connectivity )
            , bp::default_call_policies() )
        .def(
            "connectivity"
            , (::SireMol::ResidueBonds ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::connectivity )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "connectivity"
            , (::SireMol::ResidueBonds ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::connectivity )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::QString const & ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("resnum"), bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("atm") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID,::SireMol::AtomID ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("cgid"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum,::SireMol::AtomID ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("cgnum"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::CGNumAtomID const & ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::SireMol::AtomID ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("resnum"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::ResNumAtomID const & ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::ResID,::SireMol::AtomID ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("resid"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::ResIDAtomID const & ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::Bond const & ) const)( &::SireMol::EditMol::contains )
            , ( bp::arg("bond") )
            , bp::default_call_policies() )
        .def(
            "coordGroup"
            , (::SireVol::CoordGroup ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::coordGroup )
            , ( bp::arg("id") )
            , bp::default_call_policies() )
        .def(
            "coordGroup"
            , (::SireVol::CoordGroup ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::coordGroup )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , (::QVector<SireVol::CoordGroup> ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::coordGroups )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupID> const & ) const)( &::SireMol::EditMol::coordGroups )
            , ( bp::arg("cgids") )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , (::QHash<SireMol::CutGroupNum,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupNum> const & ) const)( &::SireMol::EditMol::coordGroups )
            , ( bp::arg("cgnums") )
            , bp::default_call_policies() )
        .def(
            "coordGroupsByID"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::coordGroupsByID )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "coordGroupsByID"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::coordGroupsByID )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "coordGroupsByID"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::EditMol::coordGroupsByID )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "coordGroupsByID"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResID> const & ) const)( &::SireMol::EditMol::coordGroupsByID )
            , ( bp::arg("resids") )
            , bp::default_call_policies() )
        .def(
            "coordGroupsByNum"
            , (::QHash<SireMol::CutGroupNum,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::coordGroupsByNum )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "coordGroupsByNum"
            , (::QHash<SireMol::CutGroupNum,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::coordGroupsByNum )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "coordGroupsByNum"
            , (::QHash<SireMol::CutGroupNum,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::EditMol::coordGroupsByNum )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "coordGroupsByNum"
            , (::QHash<SireMol::CutGroupNum,SireVol::CoordGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResID> const & ) const)( &::SireMol::EditMol::coordGroupsByNum )
            , ( bp::arg("resids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::AtomID ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID,::SireMol::AtomID ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgid"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum,::SireMol::AtomID ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgnum"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::CGNumAtomID const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::SireMol::AtomID ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resnum"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::ResNumAtomID const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::ResID,::SireMol::AtomID ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resid"), bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::ResIDAtomID const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("atm") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::QString const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resnum"), bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::coordinates )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::AtomID,SireMaths::Vector> ( ::SireMol::EditMol::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("atomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::CGAtomID,SireMaths::Vector> ( ::SireMol::EditMol::* )( ::QSet<SireMol::CGAtomID> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgatomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::CGNumAtomID,SireMaths::Vector> ( ::SireMol::EditMol::* )( ::QSet<SireMol::CGNumAtomID> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgatomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::ResNumAtomID,SireMaths::Vector> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNumAtomID> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resatomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::ResIDAtomID,SireMaths::Vector> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResIDAtomID> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resatomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::AtomIndex,SireMaths::Vector> ( ::SireMol::EditMol::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("atoms") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::CutGroupID,QVector<SireMaths::Vector> > ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupID> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::CutGroupNum,QVector<SireMaths::Vector> > ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupNum> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("cgnums") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::ResNum,QVector<SireMaths::Vector> > ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::ResID,QVector<SireMaths::Vector> > ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResID> const & ) const)( &::SireMol::EditMol::coordinates )
            , ( bp::arg("resids") )
            , bp::default_call_policies() )
        .def(
            "cutGroup"
            , (::SireMol::CutGroup ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::cutGroup )
            , ( bp::arg("id") )
            , bp::default_call_policies() )
        .def(
            "cutGroup"
            , (::SireMol::CutGroup ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::cutGroup )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "cutGroups"
            , (::QVector<SireMol::CutGroup> ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::cutGroups )
            , bp::default_call_policies() )
        .def(
            "cutGroups"
            , (::QHash<SireMol::CutGroupID,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupID> const & ) const)( &::SireMol::EditMol::cutGroups )
            , ( bp::arg("cgids") )
            , bp::default_call_policies() )
        .def(
            "cutGroups"
            , (::QHash<SireMol::CutGroupNum,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupNum> const & ) const)( &::SireMol::EditMol::cutGroups )
            , ( bp::arg("cgnums") )
            , bp::default_call_policies() )
        .def(
            "cutGroupsByID"
            , (::QHash<SireMol::CutGroupID,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::cutGroupsByID )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "cutGroupsByID"
            , (::QHash<SireMol::CutGroupID,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::cutGroupsByID )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "cutGroupsByID"
            , (::QHash<SireMol::CutGroupID,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::EditMol::cutGroupsByID )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "cutGroupsByID"
            , (::QHash<SireMol::CutGroupID,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResID> const & ) const)( &::SireMol::EditMol::cutGroupsByID )
            , ( bp::arg("resids") )
            , bp::default_call_policies() )
        .def(
            "cutGroupsByNum"
            , (::QHash<SireMol::CutGroupNum,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::cutGroupsByNum )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "cutGroupsByNum"
            , (::QHash<SireMol::CutGroupNum,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::cutGroupsByNum )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
      ;
      
      export_EditMol2(editmol_wrapper);
}

}
