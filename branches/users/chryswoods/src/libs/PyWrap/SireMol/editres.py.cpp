
#include <Python.h>
#include "boost/python.hpp"

#include "SireMol/qhash_siremol.h"

#include <QHash>

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/atom.h"

#include "SireMol/editmol.h"
#include "SireMol/editres.h"

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

void export_EditRes()
{

    bp::class_< SireMol::EditRes >( "EditRes" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireMol::EditMol const &, SireMol::ResNum >(( bp::arg("editmol"), bp::arg("resnum") )) )
        .def( bp::init< SireMol::EditMol const &, SireMol::ResID >(( bp::arg("editmol"), bp::arg("resid") )) )
        .def( bp::init< SireMol::EditMol const &, QString const & >(( bp::arg("editmol"), bp::arg("resname") )) )
        .def(
            "add"
            , (void ( ::SireMol::EditRes::* )( ::QString const & ) )( &::SireMol::EditRes::add )
            , ( bp::arg("atm") )
            , bp::default_call_policies() )
        .def(
            "add"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Atom const & ) )( &::SireMol::EditRes::add )
            , ( bp::arg("atm") )
            , bp::default_call_policies() )
        .def(
            "add"
            , (void ( ::SireMol::EditRes::* )( ::QString const &,::SireMol::CutGroupNum ) )( &::SireMol::EditRes::add )
            , ( bp::arg("atm"), bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "add"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Atom const &,::SireMol::CutGroupNum ) )( &::SireMol::EditRes::add )
            , ( bp::arg("atm"), bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "add"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Bond const & ) )( &::SireMol::EditRes::add )
            , ( bp::arg("bond") )
            , bp::default_call_policies() )
        .def(
            "addAutoBonds"
            , (void ( ::SireMol::EditRes::* )(  ) )( &::SireMol::EditRes::addAutoBonds )
            , bp::default_call_policies() )
        .def(
            "addAutoBonds"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::BondAddingFunction const & ) )( &::SireMol::EditRes::addAutoBonds )
            , ( bp::arg("bondfunc") )
            , bp::default_call_policies() )
        .def(
            "addBond"
            , &::SireMol::EditRes::addBond
            , ( bp::arg("atmnam0"), bp::arg("atmnam1") )
            , bp::default_call_policies() )
        .def(
            "angle"
            , &::SireMol::EditRes::angle
            , ( bp::arg("ang") )
            , bp::default_call_policies() )
        .def(
            "applyTemplate"
            , &::SireMol::EditRes::applyTemplate
            , ( bp::arg("tmpl"), bp::arg("tmplfunc") )
            , bp::default_call_policies() )
        .def(
            "assertSameResidue"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditRes::assertSameResidue )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "assertSameResidue"
            , (void ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::EditRes::assertSameResidue )
            , ( bp::arg("atoms") )
            , bp::default_call_policies() )
        .def(
            "assertSameResidue"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditRes::assertSameResidue )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "assertSameResidue"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditRes::assertSameResidue )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "assertSameResidue"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Bond const & ) const)( &::SireMol::EditRes::assertSameResidue )
            , ( bp::arg("bond") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::EditRes::* )( ::SireMol::AtomID ) const)( &::SireMol::EditRes::at )
            , ( bp::arg("i") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::EditRes::* )( ::QString const & ) const)( &::SireMol::EditRes::at )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::Atom ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditRes::at )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::CutGroup ( ::SireMol::EditRes::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditRes::at )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "at"
            , (::SireMol::CutGroup ( ::SireMol::EditRes::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditRes::at )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditRes::* )( ::SireMol::AtomID ) const)( &::SireMol::EditRes::atom )
            , ( bp::arg("i") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditRes::* )( ::QString const & ) const)( &::SireMol::EditRes::atom )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "atom"
            , (::SireMol::Atom ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditRes::atom )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "atomNames"
            , &::SireMol::EditRes::atomNames
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::EditRes::* )(  ) const)( &::SireMol::EditRes::atoms )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::AtomID,SireMol::Atom> ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::EditRes::atoms )
            , ( bp::arg("atomids") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<QString,SireMol::Atom> ( ::SireMol::EditRes::* )( ::QSet<QString> const & ) const)( &::SireMol::EditRes::atoms )
            , ( bp::arg("atomnames") )
            , bp::default_call_policies() )
        .def(
            "atoms"
            , (::QHash<SireMol::AtomIndex,SireMol::Atom> ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::EditRes::atoms )
            , ( bp::arg("atms") )
            , bp::default_call_policies() )
        .def(
            "bond"
            , &::SireMol::EditRes::bond
            , ( bp::arg("bnd") )
            , bp::default_call_policies() )
        .def(
            "bondedResidues"
            , &::SireMol::EditRes::bondedResidues
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Bond const &,double,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Bond const &,double,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("angle"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("angle"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Bond const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Bond const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("bond"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("improper"), bp::arg("delta"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::change )
            , ( bp::arg("improper"), bp::arg("delta"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "connectivity"
            , &::SireMol::EditRes::connectivity
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditRes::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditRes::contains )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditRes::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditRes::contains )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditRes::* )( ::SireMol::AtomID ) const)( &::SireMol::EditRes::contains )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditRes::* )( ::QString const & ) const)( &::SireMol::EditRes::contains )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditRes::contains )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "contains"
            , (bool ( ::SireMol::EditRes::* )( ::SireMol::Bond const & ) const)( &::SireMol::EditRes::contains )
            , ( bp::arg("bond") )
            , bp::default_call_policies() )
        .def(
            "coordGroup"
            , (::SireVol::CoordGroup ( ::SireMol::EditRes::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditRes::coordGroup )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "coordGroup"
            , (::SireVol::CoordGroup ( ::SireMol::EditRes::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditRes::coordGroup )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "coordGroupsByID"
            , &::SireMol::EditRes::coordGroupsByID
            , bp::default_call_policies() )
        .def(
            "coordGroupsByNum"
            , &::SireMol::EditRes::coordGroupsByNum
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditRes::* )( ::SireMol::AtomID ) const)( &::SireMol::EditRes::coordinates )
            , ( bp::arg("i") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditRes::* )( ::QString const & ) const)( &::SireMol::EditRes::coordinates )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditRes::coordinates )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::EditRes::* )(  ) const)( &::SireMol::EditRes::coordinates )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::AtomID,SireMaths::Vector> ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::EditRes::coordinates )
            , ( bp::arg("atomids") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<QString,SireMaths::Vector> ( ::SireMol::EditRes::* )( ::QSet<QString> const & ) const)( &::SireMol::EditRes::coordinates )
            , ( bp::arg("atomnames") )
            , bp::default_call_policies() )
        .def(
            "coordinates"
            , (::QHash<SireMol::AtomIndex,SireMaths::Vector> ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::EditRes::coordinates )
            , ( bp::arg("atms") )
            , bp::default_call_policies() )
        .def(
            "cutGroup"
            , (::SireMol::CutGroup ( ::SireMol::EditRes::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditRes::cutGroup )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "cutGroup"
            , (::SireMol::CutGroup ( ::SireMol::EditRes::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditRes::cutGroup )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "cutGroupsByID"
            , &::SireMol::EditRes::cutGroupsByID
            , bp::default_call_policies() )
        .def(
            "cutGroupsByNum"
            , &::SireMol::EditRes::cutGroupsByNum
            , bp::default_call_policies() )
        .def(
            "dihedral"
            , &::SireMol::EditRes::dihedral
            , ( bp::arg("dih") )
            , bp::default_call_policies() )
        .def(
            "getWeight"
            , (double ( ::SireMol::EditRes::* )( ::QStringList const &,::QStringList const &,::SireMol::WeightFunction const & ) const)( &::SireMol::EditRes::getWeight )
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("weightfunc") )
            , bp::default_call_policies() )
        .def(
            "getWeight"
            , (double ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomIndex> const &,::QSet<SireMol::AtomIndex> const &,::SireMol::WeightFunction const & ) const)( &::SireMol::EditRes::getWeight )
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("weightfunc") )
            , bp::default_call_policies() )
        .def(
            "improper"
            , &::SireMol::EditRes::improper
            , ( bp::arg("improper") )
            , bp::default_call_policies() )
        .def(
            "info"
            , &::SireMol::EditRes::info
            , bp::default_call_policies() )
        .def(
            "isEmpty"
            , &::SireMol::EditRes::isEmpty
            , bp::default_call_policies() )
        .def(
            "measure"
            , (double ( ::SireMol::EditRes::* )( ::SireMol::Bond const & ) const)( &::SireMol::EditRes::measure )
            , ( bp::arg("bnd") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::EditRes::* )( ::SireMol::Angle const & ) const)( &::SireMol::EditRes::measure )
            , ( bp::arg("ang") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::EditRes::* )( ::SireMol::Dihedral const & ) const)( &::SireMol::EditRes::measure )
            , ( bp::arg("dih") )
            , bp::default_call_policies() )
        .def(
            "measure"
            , (::SireMaths::Angle ( ::SireMol::EditRes::* )( ::SireMol::Improper const & ) const)( &::SireMol::EditRes::measure )
            , ( bp::arg("improper") )
            , bp::default_call_policies() )
        .def(
            "molecule"
            , &::SireMol::EditRes::molecule
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::EditRes::* )(  ) const)( &::SireMol::EditRes::nAtoms )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::EditRes::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditRes::nAtoms )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "nAtoms"
            , (int ( ::SireMol::EditRes::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditRes::nAtoms )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "nBonds"
            , &::SireMol::EditRes::nBonds
            , bp::default_call_policies() )
        .def(
            "nCutGroups"
            , &::SireMol::EditRes::nCutGroups
            , bp::default_call_policies() )
        .def(
            "nInterBonds"
            , &::SireMol::EditRes::nInterBonds
            , bp::default_call_policies() )
        .def(
            "nIntraBonds"
            , &::SireMol::EditRes::nIntraBonds
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireMol::EditRes::name
            , bp::default_call_policies() )
        .def(
            "number"
            , &::SireMol::EditRes::number
            , bp::default_call_policies() )
        .def( bp::self != bp::self )
        .def( bp::self == bp::self )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::EditRes::* )( ::SireMol::AtomID ) const)( &::SireMol::EditRes::operator[] )
            , ( bp::arg("atomid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::EditRes::* )( ::QString const & ) const)( &::SireMol::EditRes::operator[] )
            , ( bp::arg("nam") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::Atom ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditRes::operator[] )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::CutGroup ( ::SireMol::EditRes::* )( ::SireMol::CutGroupID ) const)( &::SireMol::EditRes::operator[] )
            , ( bp::arg("cgid") )
            , bp::default_call_policies() )
        .def(
            "__getitem__"
            , (::SireMol::CutGroup ( ::SireMol::EditRes::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::EditRes::operator[] )
            , ( bp::arg("cgnum") )
            , bp::default_call_policies() )
        .def(
            "remove"
            , (void ( ::SireMol::EditRes::* )( ::QString const & ) )( &::SireMol::EditRes::remove )
            , ( bp::arg("atm") )
            , bp::default_call_policies() )
        .def(
            "remove"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const & ) )( &::SireMol::EditRes::remove )
            , ( bp::arg("atm") )
            , bp::default_call_policies() )
        .def(
            "remove"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Bond const & ) )( &::SireMol::EditRes::remove )
            , ( bp::arg("bond") )
            , bp::default_call_policies() )
        .def(
            "removeAllBonds"
            , (void ( ::SireMol::EditRes::* )( ::QString const & ) )( &::SireMol::EditRes::removeAllBonds )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "removeAllBonds"
            , (void ( ::SireMol::EditRes::* )(  ) )( &::SireMol::EditRes::removeAllBonds )
            , bp::default_call_policies() )
        .def(
            "removeBond"
            , &::SireMol::EditRes::removeBond
            , ( bp::arg("atmnam0"), bp::arg("atmnam1") )
            , bp::default_call_policies() )
        .def(
            "resName"
            , &::SireMol::EditRes::resName
            , bp::default_call_policies() )
        .def(
            "resNum"
            , &::SireMol::EditRes::resNum
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditRes::* )( ::SireMol::AtomID ) const)( &::SireMol::EditRes::residuesBondedTo )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditRes::* )( ::QString const & ) const)( &::SireMol::EditRes::residuesBondedTo )
            , ( bp::arg("atomname") )
            , bp::default_call_policies() )
        .def(
            "residuesBondedTo"
            , (::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::EditRes::residuesBondedTo )
            , ( bp::arg("atom") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomID,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atomid"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomID> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atomids"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::QString const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atom"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::QStringList const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atom"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atoms"), bp::arg("quat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomID,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atomid"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomID> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atomids"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::QString const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atom"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::QStringList const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atoms"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atom"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "rotate"
            , (void ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::rotate )
            , ( bp::arg("atoms"), bp::arg("rotmat"), bp::arg("point") )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Bond const &,double,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::set )
            , ( bp::arg("bond"), bp::arg("lgth"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Bond const &,double,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::set )
            , ( bp::arg("bond"), bp::arg("lgth"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::set )
            , ( bp::arg("angle"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Angle const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::set )
            , ( bp::arg("angle"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::set )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::set )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::set )
            , ( bp::arg("improper"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "set"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Improper const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::set )
            , ( bp::arg("improper"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setAll"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setAll"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) )( &::SireMol::EditRes::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditRes::* )( ::QVector<SireMaths::Vector> const & ) )( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomID,::SireMaths::Vector const & ) )( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("atomid"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditRes::* )( ::QHash<SireMol::AtomID,SireMaths::Vector> const & ) )( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditRes::* )( ::QString const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("atomname"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditRes::* )( ::QHash<QString,SireMaths::Vector> const & ) )( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("atom"), bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setCoordinates"
            , (void ( ::SireMol::EditRes::* )( ::QHash<SireMol::AtomIndex,SireMaths::Vector> const & ) )( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("newcoords") )
            , bp::default_call_policies() )
        .def(
            "setName"
            , &::SireMol::EditRes::setName
            , ( bp::arg("name") )
            , bp::default_call_policies() )
        .def(
            "setNumber"
            , &::SireMol::EditRes::setNumber
            , ( bp::arg("newnum") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditRes::* )( ::SireMaths::Vector const & ) )( &::SireMol::EditRes::translate )
            , ( bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomID,::SireMaths::Vector const & ) )( &::SireMol::EditRes::translate )
            , ( bp::arg("atomid"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomID> const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::translate )
            , ( bp::arg("atomids"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditRes::* )( ::QString const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::translate )
            , ( bp::arg("atom"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditRes::* )( ::QStringList const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::translate )
            , ( bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditRes::* )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::translate )
            , ( bp::arg("atom"), bp::arg("delta") )
            , bp::default_call_policies() )
        .def(
            "translate"
            , (void ( ::SireMol::EditRes::* )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Vector const & ) )( &::SireMol::EditRes::translate )
            , ( bp::arg("atoms"), bp::arg("delta") )
            , bp::default_call_policies() );
}

}
