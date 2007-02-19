// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "CutGroup.pypp.hpp"
#include "boost/python.hpp"
#include "siremol_headers.h"
#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/triangle.h"
#include "SireMaths/line.h"
#include "SireMaths/torsion.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_CutGroup_class(){

    bp::class_< SireMol::CutGroup >( "CutGroup" )    
        .def( bp::init< >() )    
        .def( bp::init< QVector<SireMol::Atom> >(( bp::arg("atoms") )) )    
        .def( bp::init< QList<SireMol::Atom> >(( bp::arg("atoms") )) )    
        .def( bp::init< SireMol::AtomInfoGroup const &, SireVol::CoordGroup const & >(( bp::arg("atominfos"), bp::arg("coords") )) )    
        .def( bp::init< SireMol::AtomInfoGroup const &, QVector<SireMaths::Vector> const & >(( bp::arg("atominfos"), bp::arg("coords") )) )    
        .def( bp::init< SireMol::AtomInfoGroup const & >(( bp::arg("atominfos") )) )    
        .def( 
            "at"
            , &::SireMol::CutGroup::at
            , ( bp::arg("i") ) )    
        .def( 
            "atom"
            , &::SireMol::CutGroup::atom
            , ( bp::arg("i") ) )    
        .def( 
            "atomGroup"
            , &::SireMol::CutGroup::atomGroup
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::CutGroup::* )(  ) const)( &::SireMol::CutGroup::atoms ) )    
        .def( 
            "atoms"
            , (::QVector<SireMol::Atom> ( ::SireMol::CutGroup::* )( ::SireMol::AtomID,::SireMol::AtomID ) const)( &::SireMol::CutGroup::atoms )
            , ( bp::arg("strt"), bp::arg("end") ) )    
        .def( 
            "coordGroup"
            , &::SireMol::CutGroup::coordGroup
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::CutGroup::* )(  ) const)( &::SireMol::CutGroup::coordinates ) )    
        .def( 
            "coordinates"
            , (::QVector<SireMaths::Vector> ( ::SireMol::CutGroup::* )( ::SireMol::AtomID,::SireMol::AtomID ) const)( &::SireMol::CutGroup::coordinates )
            , ( bp::arg("strt"), bp::arg("end") ) )    
        .def( 
            "coordinates"
            , (::SireMaths::Vector ( ::SireMol::CutGroup::* )( ::SireMol::AtomID ) const)( &::SireMol::CutGroup::coordinates )
            , ( bp::arg("i") ) )    
        .def( 
            "isEmpty"
            , &::SireMol::CutGroup::isEmpty )    
        .def( 
            "nAtoms"
            , &::SireMol::CutGroup::nAtoms )    
        .def( bp::self != bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "__getitem__"
            , &::SireMol::CutGroup::operator[]
            , ( bp::arg("i") ) )    
        .def( 
            "rotate"
            , (void ( ::SireMol::CutGroup::* )( ::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireMol::CutGroup::rotate )
            , ( bp::arg("quat"), bp::arg("point") ) )    
        .def( 
            "rotate"
            , (void ( ::SireMol::CutGroup::* )( ::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireMol::CutGroup::rotate )
            , ( bp::arg("rotmat"), bp::arg("point") ) )    
        .def( 
            "setCoordGroup"
            , &::SireMol::CutGroup::setCoordGroup
            , ( bp::arg("newcoords") ) )    
        .def( 
            "setCoordinates"
            , &::SireMol::CutGroup::setCoordinates
            , ( bp::arg("newcoords") ) )    
        .def( 
            "toString"
            , &::SireMol::CutGroup::toString )    
        .def( 
            "translate"
            , &::SireMol::CutGroup::translate
            , ( bp::arg("delta") ) )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMol::CutGroup >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMol::CutGroup >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireMol::CutGroup > );

}
