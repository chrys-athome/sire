// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ResidueInfo.pypp.hpp"
#include "boost/python.hpp"
#include "siremol_headers.h"
#include "SireMol/moleculedata.h"
#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/triangle.h"
#include "SireMaths/line.h"
#include "SireMaths/torsion.h"
#include "SireVol/space.h"

namespace bp = boost::python;

SireMol::ResidueInfo __copy__(const SireMol::ResidueInfo &other){ return SireMol::ResidueInfo(other); }

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_ResidueInfo_class(){

    bp::class_< SireMol::ResidueInfo >( "ResidueInfo" )    
        .def( bp::init< >() )    
        .def( 
            "assertAtomExists"
            , (void ( ::SireMol::ResidueInfo::* )( ::QString const & ) const)( &::SireMol::ResidueInfo::assertAtomExists )
            , ( bp::arg("atomname") ) )    
        .def( 
            "assertAtomExists"
            , (void ( ::SireMol::ResidueInfo::* )( ::SireMol::AtomID ) const)( &::SireMol::ResidueInfo::assertAtomExists )
            , ( bp::arg("atomid") ) )    
        .def( 
            "assertNAtoms"
            , &::SireMol::ResidueInfo::assertNAtoms
            , ( bp::arg("n") ) )    
        .def( 
            "assertSameResidue"
            , (void ( ::SireMol::ResidueInfo::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::ResidueInfo::assertSameResidue )
            , ( bp::arg("atom") ) )    
        .def( 
            "assertSameResidue"
            , (void ( ::SireMol::ResidueInfo::* )( ::QSet<SireMol::AtomIndex> const & ) const)( &::SireMol::ResidueInfo::assertSameResidue )
            , ( bp::arg("atoms") ) )    
        .def( 
            "assertSameResidue"
            , (void ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupID ) const)( &::SireMol::ResidueInfo::assertSameResidue )
            , ( bp::arg("cgid") ) )    
        .def( 
            "assertSameResidue"
            , (void ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::ResidueInfo::assertSameResidue )
            , ( bp::arg("cgnum") ) )    
        .def( 
            "at"
            , (::SireMol::CGAtomID const & ( ::SireMol::ResidueInfo::* )( ::SireMol::AtomID ) const)( &::SireMol::ResidueInfo::at )
            , ( bp::arg("atmid") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "at"
            , (::SireMol::CGAtomID const & ( ::SireMol::ResidueInfo::* )( ::QString const & ) const)( &::SireMol::ResidueInfo::at )
            , ( bp::arg("atmname") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "at"
            , (::SireMol::AtomInfoGroup const & ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupID ) const)( &::SireMol::ResidueInfo::at )
            , ( bp::arg("cgid") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "at"
            , (::SireMol::AtomInfoGroup const & ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::ResidueInfo::at )
            , ( bp::arg("cgnum") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "atom"
            , (::SireMol::AtomInfo const & ( ::SireMol::ResidueInfo::* )( ::SireMol::AtomID ) const)( &::SireMol::ResidueInfo::atom )
            , ( bp::arg("i") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "atom"
            , (::SireMol::AtomInfo const & ( ::SireMol::ResidueInfo::* )( ::QString const & ) const)( &::SireMol::ResidueInfo::atom )
            , ( bp::arg("atmname") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "atom"
            , (::SireMol::AtomInfo const & ( ::SireMol::ResidueInfo::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::ResidueInfo::atom )
            , ( bp::arg("cgid") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "atom"
            , (::SireMol::AtomInfo const & ( ::SireMol::ResidueInfo::* )( ::SireMol::CGNumAtomID const & ) const)( &::SireMol::ResidueInfo::atom )
            , ( bp::arg("cgid") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "atomGroup"
            , (::SireMol::AtomInfoGroup const & ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupID ) const)( &::SireMol::ResidueInfo::atomGroup )
            , ( bp::arg("cgid") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "atomGroup"
            , (::SireMol::AtomInfoGroup const & ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::ResidueInfo::atomGroup )
            , ( bp::arg("cgnum") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "atomGroups"
            , (::QHash<SireMol::CutGroupID,SireMol::AtomInfoGroup> ( ::SireMol::ResidueInfo::* )(  ) const)( &::SireMol::ResidueInfo::atomGroups ) )    
        .def( 
            "atomGroups"
            , (::QHash<SireMol::CutGroupID,SireMol::AtomInfoGroup> ( ::SireMol::ResidueInfo::* )( ::QSet<SireMol::CutGroupID> const & ) const)( &::SireMol::ResidueInfo::atomGroups )
            , ( bp::arg("cgids") ) )    
        .def( 
            "atomGroups"
            , (::QHash<SireMol::CutGroupNum,SireMol::AtomInfoGroup> ( ::SireMol::ResidueInfo::* )( ::QSet<SireMol::CutGroupNum> const & ) const)( &::SireMol::ResidueInfo::atomGroups )
            , ( bp::arg("cgnums") ) )    
        .def( 
            "atomName"
            , &::SireMol::ResidueInfo::atomName
            , ( bp::arg("atomid") ) )    
        .def( 
            "atomNames"
            , (::QHash<SireMol::AtomID,QString> ( ::SireMol::ResidueInfo::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::ResidueInfo::atomNames )
            , ( bp::arg("atomids") ) )    
        .def( 
            "atomNames"
            , (::QStringList ( ::SireMol::ResidueInfo::* )(  ) const)( &::SireMol::ResidueInfo::atomNames ) )    
        .def( 
            "atoms"
            , (::QVector<SireMol::AtomInfo> ( ::SireMol::ResidueInfo::* )(  ) const)( &::SireMol::ResidueInfo::atoms ) )    
        .def( 
            "atoms"
            , (::QHash<SireMol::AtomID,SireMol::AtomInfo> ( ::SireMol::ResidueInfo::* )( ::QSet<SireMol::AtomID> const & ) const)( &::SireMol::ResidueInfo::atoms )
            , ( bp::arg("idxs") ) )    
        .def( 
            "atoms"
            , (::QHash<SireMol::CGAtomID,SireMol::AtomInfo> ( ::SireMol::ResidueInfo::* )( ::QSet<SireMol::CGAtomID> const & ) const)( &::SireMol::ResidueInfo::atoms )
            , ( bp::arg("cgids") ) )    
        .def( 
            "atoms"
            , (::QHash<SireMol::CGNumAtomID,SireMol::AtomInfo> ( ::SireMol::ResidueInfo::* )( ::QSet<SireMol::CGNumAtomID> const & ) const)( &::SireMol::ResidueInfo::atoms )
            , ( bp::arg("cgids") ) )    
        .def( 
            "atoms"
            , (::QHash<QString,SireMol::AtomInfo> ( ::SireMol::ResidueInfo::* )( ::QSet<QString> const & ) const)( &::SireMol::ResidueInfo::atoms )
            , ( bp::arg("atms") ) )    
        .def( 
            "atoms"
            , (::QVector<SireMol::AtomInfo> ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupID ) const)( &::SireMol::ResidueInfo::atoms )
            , ( bp::arg("cgid") ) )    
        .def( 
            "atoms"
            , (::QVector<SireMol::AtomInfo> ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::ResidueInfo::atoms )
            , ( bp::arg("cgnum") ) )    
        .def( 
            "atoms"
            , (::QHash<SireMol::CutGroupID,QVector<SireMol::AtomInfo> > ( ::SireMol::ResidueInfo::* )( ::QSet<SireMol::CutGroupID> const & ) const)( &::SireMol::ResidueInfo::atoms )
            , ( bp::arg("cgids") ) )    
        .def( 
            "atoms"
            , (::QHash<SireMol::CutGroupNum,QVector<SireMol::AtomInfo> > ( ::SireMol::ResidueInfo::* )( ::QSet<SireMol::CutGroupNum> const & ) const)( &::SireMol::ResidueInfo::atoms )
            , ( bp::arg("cgnums") ) )    
        .def( 
            "contains"
            , (bool ( ::SireMol::ResidueInfo::* )( ::QString const & ) const)( &::SireMol::ResidueInfo::contains )
            , ( bp::arg("atmname") ) )    
        .def( 
            "contains"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::AtomIndex const & ) const)( &::SireMol::ResidueInfo::contains )
            , ( bp::arg("atm") ) )    
        .def( 
            "contains"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::AtomID ) const)( &::SireMol::ResidueInfo::contains )
            , ( bp::arg("atm") ) )    
        .def( 
            "contains"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupID ) const)( &::SireMol::ResidueInfo::contains )
            , ( bp::arg("cgid") ) )    
        .def( 
            "contains"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupID,::SireMol::AtomID ) const)( &::SireMol::ResidueInfo::contains )
            , ( bp::arg("cgid"), bp::arg("atomid") ) )    
        .def( 
            "contains"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::CGAtomID const & ) const)( &::SireMol::ResidueInfo::contains )
            , ( bp::arg("cgid") ) )    
        .def( 
            "contains"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::ResidueInfo::contains )
            , ( bp::arg("cgnum") ) )    
        .def( 
            "contains"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupNum,::SireMol::AtomID ) const)( &::SireMol::ResidueInfo::contains )
            , ( bp::arg("cgnum"), bp::arg("atomid") ) )    
        .def( 
            "contains"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::CGNumAtomID const & ) const)( &::SireMol::ResidueInfo::contains )
            , ( bp::arg("cgid") ) )    
        .def( 
            "cutGroupIDs"
            , &::SireMol::ResidueInfo::cutGroupIDs
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "cutGroupNums"
            , &::SireMol::ResidueInfo::cutGroupNums )    
        .def( 
            "indicies"
            , &::SireMol::ResidueInfo::indicies
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "isEmpty"
            , (bool ( ::SireMol::ResidueInfo::* )(  ) const)( &::SireMol::ResidueInfo::isEmpty ) )    
        .def( 
            "isEmpty"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupID ) const)( &::SireMol::ResidueInfo::isEmpty )
            , ( bp::arg("cgid") ) )    
        .def( 
            "isEmpty"
            , (bool ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::ResidueInfo::isEmpty )
            , ( bp::arg("cgnum") ) )    
        .def( 
            "nAtoms"
            , (int ( ::SireMol::ResidueInfo::* )(  ) const)( &::SireMol::ResidueInfo::nAtoms ) )    
        .def( 
            "nAtoms"
            , (int ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupID ) const)( &::SireMol::ResidueInfo::nAtoms )
            , ( bp::arg("cgid") ) )    
        .def( 
            "nAtoms"
            , (int ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::ResidueInfo::nAtoms )
            , ( bp::arg("cgnum") ) )    
        .def( 
            "nCutGroups"
            , &::SireMol::ResidueInfo::nCutGroups )    
        .def( 
            "name"
            , &::SireMol::ResidueInfo::name )    
        .def( 
            "number"
            , &::SireMol::ResidueInfo::number )    
        .def( bp::self != bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "__getitem__"
            , (::SireMol::CGAtomID const & ( ::SireMol::ResidueInfo::* )( ::SireMol::AtomID ) const)( &::SireMol::ResidueInfo::operator[] )
            , ( bp::arg("atmid") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "__getitem__"
            , (::SireMol::CGAtomID const & ( ::SireMol::ResidueInfo::* )( ::QString const & ) const)( &::SireMol::ResidueInfo::operator[] )
            , ( bp::arg("atmname") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "__getitem__"
            , (::SireMol::AtomInfoGroup const & ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupID ) const)( &::SireMol::ResidueInfo::operator[] )
            , ( bp::arg("cgid") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "__getitem__"
            , (::SireMol::AtomInfoGroup const & ( ::SireMol::ResidueInfo::* )( ::SireMol::CutGroupNum ) const)( &::SireMol::ResidueInfo::operator[] )
            , ( bp::arg("cgnum") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "resName"
            , &::SireMol::ResidueInfo::resName )    
        .def( 
            "resNum"
            , &::SireMol::ResidueInfo::resNum )    
        .def( 
            "toString"
            , &::SireMol::ResidueInfo::toString )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMol::ResidueInfo >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMol::ResidueInfo >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireMol::ResidueInfo > );

}
