
#include <Python.h>
#include "boost/python.hpp"

#include "SireMol/qhash_siremol.h"

#include <QHash>

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/atom.h"

#include "SireMol/editmol.h"

#include "SireMol/residueinfo.h"

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

void export_Residue()
{

    bp::class_< SireMol::Residue >( "Residue" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireMol::Molecule const &, SireMol::ResNum >(( bp::arg("mol"), bp::arg("resnum") )) )
        .def( bp::init< SireMol::Molecule const &, SireMol::ResID >(( bp::arg("mol"), bp::arg("resid") )) )
        .def( bp::init< SireMol::Molecule const &, QString const & >(( bp::arg("mol"), bp::arg("resname") )) )
        .def(
            "angle"
            , &::SireMol::Residue::angle
            , ( bp::arg("ang") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::Residue::* )( ::SireMol::AtomID ) const)( &::SireMol::Residue::at )
            , ( bp::arg("i") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::Residue::* )( ::QString const & ) const)( &::SireMol::Residue::at )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::Residue::at )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::CutGroup ( ::SireMol::Residue::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Residue::at )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::CutGroup ( ::SireMol::Residue::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::Residue::at )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::Residue::* )( ::SireMol::AtomID ) const)( &::SireMol::Residue::atom )
            , ( bp::arg("i") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::Residue::* )( ::QString const & ) const)( &::SireMol::Residue::atom )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::Residue::atom )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "atomNames"
            , &::SireMol::Residue::atomNames
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::Residue::* )(  ) const)( &::SireMol::Residue::atoms )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::AtomID,SireMol::Atom> ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::Residue::atoms )
            , ( bp::arg("atomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<QString,SireMol::Atom> ( ::SireMol::Residue::* )( ::QSet<QString> const & ) const)( &::SireMol::Residue::atoms )
            , ( bp::arg("atomnames") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::AtomIndex,SireMol::Atom> ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::Residue::atoms )
            , ( bp::arg("atms") )
            , bp::default_call_policies() )
        .def(
            "bond"
            , &::SireMol::Residue::bond
            , ( bp::arg("bnd") )
            , bp::default_call_policies() )
        .def(
            "bondedResidues"
            , &::SireMol::Residue::bondedResidues
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Bond const &,double,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Bond const &,double,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("angle"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("angle"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Bond const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Bond const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("improper"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::change )
            , ( bp::arg("improper"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "connectivity"
            , &::SireMol::Residue::connectivity
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::Residue::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Residue::contains )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::Residue::* )( ::SireMol::AtomID ) const)( &::SireMol::Residue::contains )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::Residue::* )( ::QString const & ) const)( &::SireMol::Residue::contains )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::Residue::contains )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::Residue::* )( ::SireMol::Bond const & ) const)( &::SireMol::Residue::contains )
            , ( bp::arg("bond") )
            , bp::default_call_policies() )
        .def(
            "coordGroup"
            , &::SireMol::Residue::coordGroup
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "coordGroups"
            , &::SireMol::Residue::coordGroups
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::Residue::* )( ::SireMol::AtomID ) const)( &::SireMol::Residue::coordinates )
            , ( bp::arg("i") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::Residue::* )( ::QString const & ) const)( &::SireMol::Residue::coordinates )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::Residue::coordinates )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::Residue::* )(  ) const)( &::SireMol::Residue::coordinates )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::AtomID,SireMaths::Vector> ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::Residue::coordinates )
            , ( bp::arg("atomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<QString,SireMaths::Vector> ( ::SireMol::Residue::* )( ::QSet<QString> const & ) const)( &::SireMol::Residue::coordinates )
            , ( bp::arg("atomnames") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::AtomIndex,SireMaths::Vector> ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::Residue::coordinates )
            , ( bp::arg("atms") )
            , bp::default_call_policies() )
        .def(
            "cutGroup"
            , &::SireMol::Residue::cutGroup
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "cutGroups"
            , &::SireMol::Residue::cutGroups
            , bp::default_call_policies() )
        .def(
            "dihedral"
            , &::SireMol::Residue::dihedral
            , ( bp::arg("dih") )
            , bp::default_call_policies() )
        .def(
            "getWeight"
            , (double ( ::SireMol::Residue::* )( ::QStringList const &,::QStringList const &,::SireMol::WeightFunction const & ) const)( &::SireMol::Residue::getWeight )
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("weightfunc") )
            , bp::default_call_policies() )
        .def(
            "getWeight"
            , (double ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomIndex> const &,::QSet<SireMol::AtomIndex> const &,::SireMol::WeightFunction const & ) const)( &::SireMol::Residue::getWeight )
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("weightfunc") )
            , bp::default_call_policies() )
        .def(
            "improper"
            , &::SireMol::Residue::improper
            , ( bp::arg("improper") )
            , bp::default_call_policies() )
        .def(
            "info"
            , &::SireMol::Residue::info
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "isEmpty"
            , &::SireMol::Residue::isEmpty
            , bp::default_call_policies() )
        .def(
            "measure"
            , (double ( ::SireMol::Residue::* )( ::SireMol::Bond const & ) const)( &::SireMol::Residue::measure )
            , ( bp::arg("bnd") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::Residue::* )( ::SireMol::Angle const & ) const)( &::SireMol::Residue::measure )
            , ( bp::arg("ang") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::Residue::* )( ::SireMol::Dihedral const & ) const)( &::SireMol::Residue::measure )
            , ( bp::arg("dih") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::Residue::* )( ::SireMol::Improper const & ) const)( &::SireMol::Residue::measure )
            , ( bp::arg("improper") )
            , bp::default_call_policies() )
        .def(
            "molecule"
            , &::SireMol::Residue::molecule
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::Residue::* )(  ) const)( &::SireMol::Residue::nAtoms )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::Residue::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Residue::nAtoms )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "nBonds"
            , &::SireMol::Residue::nBonds
            , bp::default_call_policies() )
        .def(
            "nCutGroups"
            , &::SireMol::Residue::nCutGroups
            , bp::default_call_policies() )
        .def(
            "nInterBonds"
            , &::SireMol::Residue::nInterBonds
            , bp::default_call_policies() )
        .def(
            "nIntraBonds"
            , &::SireMol::Residue::nIntraBonds
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireMol::Residue::name
            , bp::default_call_policies() )
        .def(
            "number"
            , &::SireMol::Residue::number
            , bp::default_call_policies() )
        .def( bp::self != bp::self )
        .def( bp::self == bp::self )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::Residue::* )( ::SireMol::AtomID ) const)( &::SireMol::Residue::operator[] )
            , ( bp::arg("i") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::Residue::* )( ::QString const & ) const)( &::SireMol::Residue::operator[] )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::Residue::operator[] )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::CutGroup ( ::SireMol::Residue::* )( ::SireMol::CutGroupID ) const)( &::SireMol::Residue::operator[] )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::CutGroup ( ::SireMol::Residue::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::Residue::operator[] )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "resName"
            , &::SireMol::Residue::resName
            , bp::default_call_policies() )
        .def(
            "resNum"
            , &::SireMol::Residue::resNum
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::Residue> ( ::SireMol::Residue::* )( ::SireMol::AtomID ) const)( &::SireMol::Residue::residuesBondedTo )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::Residue> ( ::SireMol::Residue::* )( ::QString const & ) const)( &::SireMol::Residue::residuesBondedTo )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::Residue> ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::Residue::residuesBondedTo )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::SireMol::AtomID,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atomid"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomID> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atomids"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::QString const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atom"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::QStringList const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atom"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::SireMol::AtomID,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atomid"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomID> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atomids"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::QString const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atom"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::QStringList const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atoms"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atom"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::rotate )
            , ( bp::arg("atoms"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Bond const &,double,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::set )
            , ( bp::arg("bond"), bp::arg("lgth"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Bond const &,double,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::set )
            , ( bp::arg("bond"), bp::arg("lgth"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::set )
            , ( bp::arg("angle"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::set )
            , ( bp::arg("angle"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::set )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::set )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::set )
            , ( bp::arg("improper"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::set )
            , ( bp::arg("improper"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setAll"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setAll"
            , (void ( ::SireMol::Residue::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::Residue::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Residue::* )( ::QVector<SireMaths::Vector> const & ) )( &::SireMol::Residue::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Residue::* )( ::SireMol::AtomID,::SireMaths::Vector const & ) )( &::SireMol::Residue::setCoordinates )
            , ( bp::arg("atomid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Residue::* )( ::QHash<SireMol::AtomID,SireMaths::Vector> const & ) )( &::SireMol::Residue::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Residue::* )( ::QString const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::setCoordinates )
            , ( bp::arg("atomname"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Residue::* )( ::QHash<QString,SireMaths::Vector> const & ) )( &::SireMol::Residue::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::setCoordinates )
            , ( bp::arg("atom"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::Residue::* )( ::QHash<SireMol::AtomIndex,SireMaths::Vector> const & ) )( &::SireMol::Residue::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Residue::* )( ::SireMaths::Vector const & ) )( &::SireMol::Residue::translate )
            , ( bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Residue::* )( ::SireMol::AtomID,::SireMaths::Vector const & ) )( &::SireMol::Residue::translate )
            , ( bp::arg("atomid"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomID> const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::translate )
            , ( bp::arg("atomids"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Residue::* )( ::QString const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::translate )
            , ( bp::arg("atom"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Residue::* )( ::QStringList const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::translate )
            , ( bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Residue::* )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::translate )
            , ( bp::arg("atom"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::Residue::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Vector const & ) )( &::SireMol::Residue::translate )
            , ( bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() );
}

}
