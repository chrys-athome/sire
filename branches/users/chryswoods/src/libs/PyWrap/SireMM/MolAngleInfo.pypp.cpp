// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "MolAngleInfo.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireMM::MolAngleInfo&){ return "SireMM::MolAngleInfo";}

void register_MolAngleInfo_class(){

    bp::class_< SireMM::MolAngleInfo, bp::bases< SireMM::MolInternalInfo<SireMol::Angle> > >( "MolAngleInfo" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMol::MoleculeInfo const & >(( bp::arg("molinfo") )) )    
        .def( bp::init< SireMol::Molecule const &, SireMM::InternalGenerator<SireMM::MolAngleInfo> const & >(( bp::arg("mol"), bp::arg("anglegenerator") )) )    
        .def( 
            "addAngle"
            , (::SireMol::GroupIndexID ( ::SireMM::MolAngleInfo::* )( ::SireMol::Angle const & ) )( &::SireMM::MolAngleInfo::addAngle )
            , ( bp::arg("angle") ) )    
        .def( 
            "angles"
            , (::SireMM::detail::InternalGroupIterator<SireMol::Angle> ( ::SireMM::MolAngleInfo::* )(  ) const)( &::SireMM::MolAngleInfo::angles ) )    
        .def( 
            "angles"
            , (::SireMM::detail::InternalGroupIterator<SireMol::Angle> ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::angles )
            , ( bp::arg("resnum") ) )    
        .def( 
            "angles"
            , (::SireMM::detail::InternalGroupIterator<SireMol::Angle> ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum,::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::angles )
            , ( bp::arg("res0"), bp::arg("res1") ) )    
        .def( 
            "angles"
            , (::SireMM::detail::InternalGroupIterator<SireMol::Angle> ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum,::SireMol::ResNum,::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::angles )
            , ( bp::arg("res0"), bp::arg("res1"), bp::arg("res2") ) )    
        .def( 
            "interAngles"
            , (::SireMM::detail::InternalGroupIterator<SireMol::Angle> ( ::SireMM::MolAngleInfo::* )(  ) const)( &::SireMM::MolAngleInfo::interAngles ) )    
        .def( 
            "interAngles"
            , (::SireMM::detail::InternalGroupIterator<SireMol::Angle> ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::interAngles )
            , ( bp::arg("resnum") ) )    
        .def( 
            "intraAngles"
            , (::SireMM::detail::InternalGroupIterator<SireMol::Angle> ( ::SireMM::MolAngleInfo::* )(  ) const)( &::SireMM::MolAngleInfo::intraAngles ) )    
        .def( 
            "intraAngles"
            , (::SireMM::detail::InternalGroupIterator<SireMol::Angle> ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::intraAngles )
            , ( bp::arg("resnum") ) )    
        .def( 
            "nAngles"
            , (int ( ::SireMM::MolAngleInfo::* )(  ) const)( &::SireMM::MolAngleInfo::nAngles ) )    
        .def( 
            "nAngles"
            , (int ( ::SireMM::MolAngleInfo::* )( ::SireMol::GroupID ) const)( &::SireMM::MolAngleInfo::nAngles )
            , ( bp::arg("group") ) )    
        .def( 
            "nAngles"
            , (int ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::nAngles )
            , ( bp::arg("resnum") ) )    
        .def( 
            "nAngles"
            , (int ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum,::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::nAngles )
            , ( bp::arg("res0"), bp::arg("res1") ) )    
        .def( 
            "nAngles"
            , (int ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum,::SireMol::ResNum,::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::nAngles )
            , ( bp::arg("res0"), bp::arg("res1"), bp::arg("res2") ) )    
        .def( 
            "nInterAngles"
            , (int ( ::SireMM::MolAngleInfo::* )(  ) const)( &::SireMM::MolAngleInfo::nInterAngles ) )    
        .def( 
            "nInterAngles"
            , (int ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::nInterAngles )
            , ( bp::arg("resnum") ) )    
        .def( 
            "nIntraAngles"
            , (int ( ::SireMM::MolAngleInfo::* )(  ) const)( &::SireMM::MolAngleInfo::nIntraAngles ) )    
        .def( 
            "nIntraAngles"
            , (int ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::nIntraAngles )
            , ( bp::arg("resnum") ) )    
        .def( 
            "removeAngle"
            , (void ( ::SireMM::MolAngleInfo::* )( ::SireMol::Angle const & ) )( &::SireMM::MolAngleInfo::removeAngle )
            , ( bp::arg("angle") ) )    
        .def( 
            "removeAngle"
            , (void ( ::SireMM::MolAngleInfo::* )( ::SireMol::GroupIndexID const & ) )( &::SireMM::MolAngleInfo::removeAngle )
            , ( bp::arg("id") ) )    
        .def( 
            "residue"
            , (::SireMM::ResAngleInfo ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::residue )
            , ( bp::arg("resnum") ) )    
        .def( 
            "residuesAngled"
            , (bool ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum,::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::residuesAngled )
            , ( bp::arg("res0"), bp::arg("res1") ) )    
        .def( 
            "residuesAngled"
            , (bool ( ::SireMM::MolAngleInfo::* )( ::SireMol::ResNum,::SireMol::ResNum,::SireMol::ResNum ) const)( &::SireMM::MolAngleInfo::residuesAngled )
            , ( bp::arg("res0"), bp::arg("res1"), bp::arg("res2") ) )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMM::MolAngleInfo >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMM::MolAngleInfo >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
