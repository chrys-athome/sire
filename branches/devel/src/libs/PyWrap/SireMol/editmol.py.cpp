
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

void export_EditMol()
{

    bp::class_< SireMol::EditMol >( "EditMol" )
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
        .def(
            "cutGroupsByNum"
            , (::QHash<SireMol::CutGroupNum,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::EditMol::cutGroupsByNum )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "cutGroupsByNum"
            , (::QHash<SireMol::CutGroupNum,SireMol::CutGroup> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResID> const & ) const)( &::SireMol::EditMol::cutGroupsByNum )
            , ( bp::arg("resids") )
            , bp::default_call_policies() )
        .def(
            "dihedral"
            , &::SireMol::EditMol::dihedral
            , ( bp::arg("dih") )
            , bp::default_call_policies() )
        .def(
            "getWeight"
            , &::SireMol::EditMol::getWeight
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("weightfunc") )
            , bp::default_call_policies() )
        .def(
            "improper"
            , &::SireMol::EditMol::improper
            , ( bp::arg("improper") )
            , bp::default_call_policies() )
        .def(
            "info"
            , &::SireMol::EditMol::info
            , bp::default_call_policies() )
        .def(
            "isEmpty"
            , (bool ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::isEmpty )
            , bp::default_call_policies() )
        .def(
            "isEmpty"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::isEmpty )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "isEmpty"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::isEmpty )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "isEmpty"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::isEmpty )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "isEmpty"
            , (bool ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::isEmpty )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (double ( ::SireMol::EditMol::* )( ::SireMol::Bond const & ) const)( &::SireMol::EditMol::measure )
            , ( bp::arg("bnd") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::EditMol::* )( ::SireMol::Angle const & ) const)( &::SireMol::EditMol::measure )
            , ( bp::arg("ang") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::EditMol::* )( ::SireMol::Dihedral const & ) const)( &::SireMol::EditMol::measure )
            , ( bp::arg("dih") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::EditMol::* )( ::SireMol::Improper const & ) const)( &::SireMol::EditMol::measure )
            , ( bp::arg("improper") )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::nAtoms )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::nAtoms )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::nAtoms )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::nAtoms )
            , ( bp::arg("id") )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::nAtoms )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "nBonds"
            , (int ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::nBonds )
            , bp::default_call_policies() )
        .def(
            "nBonds"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::nBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "nBonds"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::nBonds )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "nCutGroups"
            , &::SireMol::EditMol::nCutGroups
            , bp::default_call_policies() )
        .def(
            "nInterBonds"
            , (int ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::nInterBonds )
            , bp::default_call_policies() )
        .def(
            "nInterBonds"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::nInterBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "nInterBonds"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::nInterBonds )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "nIntraBonds"
            , (int ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::nIntraBonds )
            , bp::default_call_policies() )
        .def(
            "nIntraBonds"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::nIntraBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "nIntraBonds"
            , (int ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::nIntraBonds )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "nResidues"
            , &::SireMol::EditMol::nResidues
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireMol::EditMol::name
            , bp::default_call_policies() )
        .def( bp::self != bp::self )
        .def( bp::self == bp::self )
        .def(
            "__getitem__"
            , (::SireMol::CutGroup ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::CutGroup ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::EditRes ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::EditRes ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::AtomID ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::CGNumAtomID const & ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::ResNumAtomID const & ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::ResIDAtomID const & ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("resatomid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditMol::operator[] )
            , ( bp::arg("atm") )
            , bp::default_call_policies() )
        .def(
            "remove"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const & ) )( &::SireMol::EditMol::remove )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "remove"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Bond const & ) )( &::SireMol::EditMol::remove )
            , ( bp::arg("bond") )
            , bp::default_call_policies() )
        .def(
            "remove"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) )( &::SireMol::EditMol::remove )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "remove"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID ) )( &::SireMol::EditMol::remove )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "removeAllBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) )( &::SireMol::EditMol::removeAllBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "removeAllBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID ) )( &::SireMol::EditMol::removeAllBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "removeAllBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const & ) )( &::SireMol::EditMol::removeAllBonds )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "removeAllBonds"
            , (void ( ::SireMol::EditMol::* )(  ) )( &::SireMol::EditMol::removeAllBonds )
            , bp::default_call_policies() )
        .def(
            "removeAllInterBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) )( &::SireMol::EditMol::removeAllInterBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "removeAllInterBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID ) )( &::SireMol::EditMol::removeAllInterBonds )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "removeAllInterBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const & ) )( &::SireMol::EditMol::removeAllInterBonds )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "removeAllInterBonds"
            , (void ( ::SireMol::EditMol::* )(  ) )( &::SireMol::EditMol::removeAllInterBonds )
            , bp::default_call_policies() )
        .def(
            "removeAllIntraBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) )( &::SireMol::EditMol::removeAllIntraBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "removeAllIntraBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID ) )( &::SireMol::EditMol::removeAllIntraBonds )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "removeAllIntraBonds"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const & ) )( &::SireMol::EditMol::removeAllIntraBonds )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "removeAllIntraBonds"
            , (void ( ::SireMol::EditMol::* )(  ) )( &::SireMol::EditMol::removeAllIntraBonds )
            , bp::default_call_policies() )
        .def(
            "renumberResidue"
            , &::SireMol::EditMol::renumberResidue
            , ( bp::arg("resnum"), bp::arg("newresnum") )
            , bp::default_call_policies() )
        .def(
            "residue"
            , (::SireMol::EditRes ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::residue )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "residue"
            , (::SireMol::EditRes ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::residue )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "residue"
            , (::SireMol::EditRes ( ::SireMol::EditMol::* )( ::QString const & ) const)( &::SireMol::EditMol::residue )
            , ( bp::arg("resname") )
            , bp::default_call_policies() )
        .def(
            "residueID"
            , &::SireMol::EditMol::residueID
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "residueName"
            , (::QString ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::residueName )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "residueName"
            , (::QString ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::residueName )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "residueNames"
            , &::SireMol::EditMol::residueNames
            , bp::default_call_policies() )
        .def(
            "residueNumber"
            , (::SireMol::ResNum ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::residueNumber )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "residueNumber"
            , (::SireMol::ResNum ( ::SireMol::EditMol::* )( ::QString const & ) const)( &::SireMol::EditMol::residueNumber )
            , ( bp::arg("resname") )
            , bp::default_call_policies() )
        .def(
            "residueNumbers"
            , (::QVector<SireMol::ResNum> ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::residueNumbers )
            , bp::default_call_policies() )
        .def(
            "residueNumbers"
            , (::QVector<SireMol::ResNum> ( ::SireMol::EditMol::* )( ::QString const & ) const)( &::SireMol::EditMol::residueNumbers )
            , ( bp::arg("resnam") )
            , bp::default_call_policies() )
        .def(
            "residueNumbers"
            , (::QVector<SireMol::ResNum> ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditMol::residueNumbers )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "residueNumbers"
            , (::QVector<SireMol::ResNum> ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditMol::residueNumbers )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "residues"
            , (::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditMol::* )(  ) const)( &::SireMol::EditMol::residues )
            , bp::default_call_policies() )
        .def(
            "residues"
            , (::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::EditMol::residues )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditMol::* )( ::SireMol::ResNum ) const)( &::SireMol::EditMol::residuesBondedTo )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditMol::* )( ::SireMol::ResID ) const)( &::SireMol::EditMol::residuesBondedTo )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIDGroup const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("group"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("atom"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::QStringList const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resnum"), bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resnum"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resnums"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID,::QStringList const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resid"), bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resid"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResID> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resids"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgnum"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupNum> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgnums"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgid"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupID> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgids"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIDGroup const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("group"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("atom"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("atoms"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::QStringList const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resnum"), bp::arg("atoms"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resnum"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resnums"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID,::QStringList const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resid"), bp::arg("atoms"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resid"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResID> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("resids"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgnum"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupNum> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgnums"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgid"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupID> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgids"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Bond const &,double,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::set )
            , ( bp::arg("bond"), bp::arg("lgth"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Bond const &,double,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::set )
            , ( bp::arg("bond"), bp::arg("lgth"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::set )
            , ( bp::arg("angle"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::set )
            , ( bp::arg("angle"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::set )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::set )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::set )
            , ( bp::arg("improper"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::set )
            , ( bp::arg("improper"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setAll"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setAll"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditMol::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum,::SireVol::CoordGroup const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("cgnum"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::CutGroupNum,SireVol::CoordGroup> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID,::SireVol::CoordGroup const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("cgid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::CutGroupID,SireVol::CoordGroup> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QVector<SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum,::QVector<SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("cgnum"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::CutGroupNum,QVector<SireMaths::Vector> > const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID,::QVector<SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("cgid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::CutGroupID,QVector<SireMaths::Vector> > const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::QVector<SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("resnum"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::ResNum,QVector<SireMaths::Vector> > const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID,::QVector<SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("resid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::ResID,QVector<SireMaths::Vector> > const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("atom"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::AtomIndex,SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CGNumAtomID const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("cgatomid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::CGNumAtomID,SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CGAtomID const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("cgatomid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::CGAtomID,SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNumAtomID const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("resatomid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::ResNumAtomID,SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResIDAtomID const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("resatomid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditMol::* )( ::QHash<SireMol::ResIDAtomID,SireMaths::Vector> const & ) )( &::SireMol::EditMol::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setName"
            , &::SireMol::EditMol::setName
            , ( bp::arg("name") )
            , bp::default_call_policies() )
        .def(
            "setResidueName"
            , &::SireMol::EditMol::setResidueName
            , ( bp::arg("resnum"), bp::arg("name") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIDGroup const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("group"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("atom"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::QStringList const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("resnum"), bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResNum,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("resnum"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResNum> const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("resnums"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID,::QStringList const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("resid"), bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::ResID,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("resid"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::ResID> const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("resids"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupNum,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("cgnum"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupNum> const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("cgnums"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::SireMol::CutGroupID,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("cgid"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditMol::* )( ::QSet<SireMol::CutGroupID> const &,::SireMaths::Vector const & ) )( &::SireMol::EditMol::translate )
            , ( bp::arg("cgids"), bp::arg("delta") )
            , bp::default_call_policies() );
}

}
