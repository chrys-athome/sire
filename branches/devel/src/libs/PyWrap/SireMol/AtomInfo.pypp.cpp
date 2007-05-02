// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "AtomInfo.pypp.hpp"
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

SireMol::AtomInfo __copy__(const SireMol::AtomInfo &other){ return SireMol::AtomInfo(other); }

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_AtomInfo_class(){

    bp::class_< SireMol::AtomInfo, bp::bases< SireMol::AtomIndex, SireMol::Element > >( "AtomInfo" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMol::AtomIndex const &, SireMol::Element const & >(( bp::arg("atomindex"), bp::arg("element") )) )    
        .def( bp::init< SireMol::AtomInfo const &, SireMol::ResNum >(( bp::arg("other"), bp::arg("resnum") )) )    
        .def( bp::init< SireMol::AtomNum, SireMol::AtomIndex const &, SireMol::Element const & >(( bp::arg("atmnum"), bp::arg("atomindex"), bp::arg("element") )) )    
        .def( bp::init< SireMol::AtomNum, SireMol::AtomInfo const & >(( bp::arg("atmnum"), bp::arg("other") )) )    
        .def( bp::init< SireMol::AtomNum, SireMol::AtomInfo const &, SireMol::ResNum >(( bp::arg("atmnum"), bp::arg("other"), bp::arg("resnum") )) )    
        .def( bp::init< QString const & >(( bp::arg("name") )) )    
        .def( bp::init< QString const &, QString const & >(( bp::arg("name"), bp::arg("element") )) )    
        .def( bp::init< QString const &, SireMol::ResNum >(( bp::arg("name"), bp::arg("resnum") )) )    
        .def( bp::init< QString const &, SireMol::ResNum, QString const & >(( bp::arg("name"), bp::arg("resnum"), bp::arg("element") )) )    
        .def( bp::init< SireMol::AtomIndex const & >(( bp::arg("atm") )) )    
        .def( bp::init< QString const &, SireMol::AtomInfo const & >(( bp::arg("name"), bp::arg("other") )) )    
        .def( bp::init< SireMol::AtomNum, QString const & >(( bp::arg("num"), bp::arg("name") )) )    
        .def( bp::init< SireMol::AtomNum, QString const &, QString const & >(( bp::arg("num"), bp::arg("name"), bp::arg("element") )) )    
        .def( 
            "atomNum"
            , &::SireMol::AtomInfo::atomNum )    
        .def( 
            "name"
            , &::SireMol::AtomInfo::name )    
        .def( 
            "number"
            , &::SireMol::AtomInfo::number )    
        .def( bp::self != bp::other< SireMol::AtomIndex >() )    
        .def( bp::self != bp::self )    
        .def( bp::self == bp::other< SireMol::AtomIndex >() )    
        .def( bp::self == bp::self )    
        .def( 
            "toString"
            , &::SireMol::AtomInfo::toString )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMol::AtomInfo >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMol::AtomInfo >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireMol::AtomInfo > );

}
