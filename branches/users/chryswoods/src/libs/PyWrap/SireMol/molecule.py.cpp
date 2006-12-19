
#include <Python.h>
#include "boost/python.hpp"

#include "SireMol/qhash_siremol.h"

#include <QHash>

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/atom.h"

#include "SireMol/editmol.h"
#include "SireMol/property.h"

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

void
SIREMOL_EXPORT
export_Molecule()
{
    bp::class_< SireMol::Molecule >( "Molecule" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireMol::Residue const & >(( bp::arg("residue") )) )
        .def(
            "ID"
            , &::SireMol::Molecule::ID
            , bp::default_call_policies() )
        .def(
            "addProperty"
            , (void ( ::SireMol::Molecule::* )( ::QString const &, ::SireMol::Property const & ) )( &::SireMol::Molecule::addProperty )
            , bp::default_call_policies() )
        .def(
            "addProperty"
            , (void ( ::SireMol::Molecule::* )( ::QString const &, ::SireMol::PropertyBase const & ) )( &::SireMol::Molecule::addProperty )
            , bp::default_call_policies() )
        .def(
            "addProperty"
            , (void ( ::SireMol::Molecule::* )( ::QString const &, ::QVariant const & ) )( &::SireMol::Molecule::addProperty )
            , bp::default_call_policies() )
        .def(
            "angle"
            , &::SireMol::Molecule::angle
            , ( bp::arg("ang") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Residue ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::at )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Residue ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::at )
            , ( bp::arg("i") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::CutGroup ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Molecule::at )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::Molecule::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::Molecule::at )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::Molecule::* )( ::SireMol::IDMolAtom const & ) const)( &::SireMol::Molecule::at )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::Molecule::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::Molecule::atom )
            , ( bp::arg("cgatmid") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::Molecule::* )( ::SireMol::IDMolAtom const & ) const)( &::SireMol::Molecule::atom )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "atomNames"
            , (::QStringList ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::atomNames )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "atomNames"
            , (::QStringList ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::atomNames )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::atoms )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::AtomID,SireMol::Atom> ( ::SireMol::Molecule::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("atomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::CGAtomID,SireMol::Atom> ( ::SireMol::Molecule::* )( ::QSet<SireMol::CGAtomID> const & ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("cgatomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::ResNumAtomID,SireMol::Atom> ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResNumAtomID> const & ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("resatomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::ResIDAtomID,SireMol::Atom> ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResIDAtomID> const & ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("resatomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::AtomIndex,SireMol::Atom> ( ::SireMol::Molecule::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("atoms") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::CutGroupID,QVector<SireMol::Atom> > ( ::SireMol::Molecule::* )( ::QSet<SireMol::CutGroupID> const & ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("cgids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::ResNum,QVector<SireMol::Atom> > ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::ResID,QVector<SireMol::Atom> > ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResID> const & ) const)( &::SireMol::Molecule::atoms )
            , ( bp::arg("resids") )
            , bp::default_call_policies() )
        .def(
            "bond"
            , &::SireMol::Molecule::bond
            , ( bp::arg("bnd") )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Bond const &,double,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Bond const &,double,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("angle"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("angle"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Bond const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Bond const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("improper"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::change )
            , ( bp::arg("improper"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "connectivity"
            , (::SireMol::MoleculeBonds ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::connectivity )
            , bp::default_call_policies() )
        .def(
            "connectivity"
            , (::SireMol::ResidueBonds ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::connectivity )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "connectivity"
            , (::SireMol::ResidueBonds ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::connectivity )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "coordGroup"
            , &::SireMol::Molecule::coordGroup
            , ( bp::arg("id") )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , (::QVector<SireVol::CoordGroup> ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::coordGroups )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::coordGroups )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::coordGroups )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::Molecule::* )( ::QSet<SireMol::CutGroupID> const & ) const)( &::SireMol::Molecule::coordGroups )
            , ( bp::arg("cgids") )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::Molecule::coordGroups )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , (::QHash<SireMol::CutGroupID,SireVol::CoordGroup> ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResID> const & ) const)( &::SireMol::Molecule::coordGroups )
            , ( bp::arg("resids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::Molecule::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::Molecule::* )( ::SireMol::IDMolAtom const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::coordinates )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::AtomID,SireMaths::Vector> ( ::SireMol::Molecule::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("atomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::CGAtomID,SireMaths::Vector> ( ::SireMol::Molecule::* )( ::QSet<SireMol::CGAtomID> const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("cgatomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::ResNumAtomID,SireMaths::Vector> ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResNumAtomID> const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("resatomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::ResIDAtomID,SireMaths::Vector> ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResIDAtomID> const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("resatomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::AtomIndex,SireMaths::Vector> ( ::SireMol::Molecule::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("atoms") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID ) )( &::SireMol::Molecule::coordinates )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::CutGroupID,QVector<SireMaths::Vector> > ( ::SireMol::Molecule::* )( ::QSet<SireMol::CutGroupID> const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("cgids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) )( &::SireMol::Molecule::coordinates )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::ResNum,QVector<SireMaths::Vector> > ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::Molecule::* )( ::SireMol::ResID ) )( &::SireMol::Molecule::coordinates )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::ResID,QVector<SireMaths::Vector> > ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResID> const & ) const)( &::SireMol::Molecule::coordinates )
            , ( bp::arg("resids") )
            , bp::default_call_policies() )
        .def(
            "cutGroup"
            , &::SireMol::Molecule::cutGroup
            , ( bp::arg("id") )
            , bp::default_call_policies() )
        .def(
            "cutGroups"
            , (::QVector<SireMol::CutGroup> ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::cutGroups )
            , bp::default_call_policies() )
        .def(
            "cutGroups"
            , (::QHash<SireMol::CutGroupID,SireMol::CutGroup> ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::cutGroups )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "cutGroups"
            , (::QHash<SireMol::CutGroupID,SireMol::CutGroup> ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::cutGroups )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "dihedral"
            , &::SireMol::Molecule::dihedral
            , ( bp::arg("dih") )
            , bp::default_call_policies() )
        .def(
            "edit"
            , &::SireMol::Molecule::edit
            , bp::default_call_policies() )
        .def(
            "getProperty"
            , &::SireMol::Molecule::getProperty
            , ( bp::arg("name") )
            , bp::return_value_policy<bp::copy_const_reference>() )
        .def(
            "getWeight"
            , &::SireMol::Molecule::getWeight
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("weightfunc") )
            , bp::default_call_policies() )
        .def(
            "improper"
            , &::SireMol::Molecule::improper
            , ( bp::arg("improper") )
            , bp::default_call_policies() )
        .def(
            "info"
            , &::SireMol::Molecule::info
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "isEmpty"
            , (bool ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::isEmpty )
            , bp::default_call_policies() )
        .def(
            "isEmpty"
            , (bool ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::isEmpty )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "isEmpty"
            , (bool ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::isEmpty )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "isEmpty"
            , (bool ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Molecule::isEmpty )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (double ( ::SireMol::Molecule::* )( ::SireMol::Bond const & ) const)( &::SireMol::Molecule::measure )
            , ( bp::arg("bnd") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::Molecule::* )( ::SireMol::Angle const & ) const)( &::SireMol::Molecule::measure )
            , ( bp::arg("ang") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::Molecule::* )( ::SireMol::Dihedral const & ) const)( &::SireMol::Molecule::measure )
            , ( bp::arg("dih") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::Molecule::* )( ::SireMol::Improper const & ) const)( &::SireMol::Molecule::measure )
            , ( bp::arg("improper") )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::nAtoms )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::nAtoms )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::nAtoms )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Molecule::nAtoms )
            , ( bp::arg("id") )
            , bp::default_call_policies() )
        .def(
            "nBonds"
            , (int ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::nBonds )
            , bp::default_call_policies() )
        .def(
            "nBonds"
            , (int ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::nBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "nBonds"
            , (int ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::nBonds )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "nCutGroups"
            , &::SireMol::Molecule::nCutGroups
            , bp::default_call_policies() )
        .def(
            "nInterBonds"
            , (int ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::nInterBonds )
            , bp::default_call_policies() )
        .def(
            "nInterBonds"
            , (int ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::nInterBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "nInterBonds"
            , (int ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::nInterBonds )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "nIntraBonds"
            , (int ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::nIntraBonds )
            , bp::default_call_policies() )
        .def(
            "nIntraBonds"
            , (int ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::nIntraBonds )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "nIntraBonds"
            , (int ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::nIntraBonds )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "nResidues"
            , &::SireMol::Molecule::nResidues
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireMol::Molecule::name
            , bp::default_call_policies() )
        .def( bp::self != bp::self )
        .def( bp::self == bp::self )
        .def(
            "__getitem__"
            , (::SireMol::CutGroup ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Molecule::operator[] )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Residue ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::operator[] )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Residue ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::operator[] )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::Molecule::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::Molecule::operator[] )
            , ( bp::arg("cgatomid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::Molecule::* )( ::SireMol::IDMolAtom const & ) const)( &::SireMol::Molecule::operator[] )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "properties"
            , &::SireMol::Molecule::properties
            , bp::return_value_policy<bp::copy_const_reference>() )
        .def(
            "residue"
            , (::SireMol::Residue ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::residue )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "residue"
            , (::SireMol::Residue ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::residue )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "residue"
            , (::SireMol::Residue ( ::SireMol::Molecule::* )( ::QString const & ) const)( &::SireMol::Molecule::residue )
            , ( bp::arg("resname") )
            , bp::default_call_policies() )
        .def(
            "residueName"
            , (::QString ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::residueName )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "residueName"
            , (::QString ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::residueName )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "residueNames"
            , &::SireMol::Molecule::residueNames
            , bp::default_call_policies() )
        .def(
            "residueNumber"
            , (::SireMol::ResNum ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::residueNumber )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "residueNumber"
            , (::SireMol::ResNum ( ::SireMol::Molecule::* )( ::QString const & ) const)( &::SireMol::Molecule::residueNumber )
            , ( bp::arg("resname") )
            , bp::default_call_policies() )
        .def(
            "residueNumbers"
            , (::QVector<SireMol::ResNum> ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::residueNumbers )
            , bp::default_call_policies() )
        .def(
            "residueNumbers"
            , (::QVector<SireMol::ResNum> ( ::SireMol::Molecule::* )( ::QString const & ) const)( &::SireMol::Molecule::residueNumbers )
            , ( bp::arg("resnam") )
            , bp::default_call_policies() )
        .def(
            "residueNumbers"
            , (::QVector<SireMol::ResNum> ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Molecule::residueNumbers )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "residues"
            , (::QHash<SireMol::ResNum,SireMol::Residue> ( ::SireMol::Molecule::* )(  ) const)( &::SireMol::Molecule::residues )
            , bp::default_call_policies() )
        .def(
            "residues"
            , (::QHash<SireMol::ResNum,SireMol::Residue> ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResNum> const & ) const)( &::SireMol::Molecule::residues )
            , ( bp::arg("resnums") )
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::Residue> ( ::SireMol::Molecule::* )( ::SireMol::ResNum ) const)( &::SireMol::Molecule::residuesBondedTo )
            , ( bp::arg("resnum") )
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::Residue> ( ::SireMol::Molecule::* )( ::SireMol::ResID ) const)( &::SireMol::Molecule::residuesBondedTo )
            , ( bp::arg("resid") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::AtomIDGroup const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("group"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::AtomIndex const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("atom"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResNum,::QStringList const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resnum"), bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResNum,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resnum"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResNum> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resnums"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResID,::QStringList const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resid"), bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResID,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resid"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResID> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resids"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("cgid"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::CutGroupID> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("cgids"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::AtomIDGroup const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("group"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::AtomIndex const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("atom"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("atoms"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResNum,::QStringList const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resnum"), bp::arg("atoms"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResNum,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resnum"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResNum> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resnums"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResID,::QStringList const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resid"), bp::arg("atoms"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResID,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resid"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResID> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("resids"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("cgid"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::CutGroupID> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::rotate )
            , ( bp::arg("cgids"), bp::arg("matrix"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Bond const &,double,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::set )
            , ( bp::arg("bond"), bp::arg("lgth"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Bond const &,double,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::set )
            , ( bp::arg("bond"), bp::arg("lgth"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::set )
            , ( bp::arg("angle"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::set )
            , ( bp::arg("angle"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::set )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::set )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::set )
            , ( bp::arg("improper"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::set )
            , ( bp::arg("improper"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setAll"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setAll"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Molecule::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID,::SireVol::CoordGroup const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("cgid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QVector<SireVol::CoordGroup> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QHash<SireMol::CutGroupID,SireVol::CoordGroup> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QVector<SireMaths::Vector> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID,::QVector<SireMaths::Vector> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("cgid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QHash<SireMol::CutGroupID,QVector<SireMaths::Vector> > const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResNum,::QVector<SireMaths::Vector> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("resnum"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QHash<SireMol::ResNum,QVector<SireMaths::Vector> > const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResID,::QVector<SireMaths::Vector> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("resid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QHash<SireMol::ResID,QVector<SireMaths::Vector> > const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("atom"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QHash<SireMol::AtomIndex,SireMaths::Vector> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::CGAtomID const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("cgatomid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QHash<SireMol::CGAtomID,SireMaths::Vector> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResNumAtomID const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("resatomid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QHash<SireMol::ResNumAtomID,SireMaths::Vector> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResIDAtomID const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("resatomid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Molecule::* )( ::QHash<SireMol::ResIDAtomID,SireMaths::Vector> const & ) )( &::SireMol::Molecule::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setNewID"
            , &::SireMol::Molecule::setNewID
            , bp::default_call_policies() )
        .def(
            "setProperty"
            , (void ( ::SireMol::Molecule::* )( ::QString const &, ::SireMol::Property const & ) )( &::SireMol::Molecule::setProperty )
            , bp::default_call_policies() )
        .def(
            "setProperty"
            , (void ( ::SireMol::Molecule::* )( ::QString const &, ::SireMol::PropertyBase const & ) )( &::SireMol::Molecule::setProperty )
            , bp::default_call_policies() )
        .def(
            "setProperty"
            , (void ( ::SireMol::Molecule::* )( ::QString const &, ::QVariant const & ) )( &::SireMol::Molecule::setProperty )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::AtomIDGroup const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("group"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("atom"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResNum,::QStringList const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("resnum"), bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResNum,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("resnum"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResNum> const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("resnums"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResID,::QStringList const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("resid"), bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::ResID,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("resid"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::ResID> const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("resids"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::SireMol::CutGroupID,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("cgid"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Molecule::* )( ::QSet<SireMol::CutGroupID> const &,::SireMaths::Vector const & ) )( &::SireMol::Molecule::translate )
            , ( bp::arg("cgids"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "version"
            , &::SireMol::Molecule::version
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() );

    bp::implicitly_convertible< SireMol::Residue const &, SireMol::Molecule >();

}

}
