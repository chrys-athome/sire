// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "CoordGroupEditor.pypp.hpp"
#include "boost/python.hpp"
#include "sirevol_headers.h"
#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"

namespace bp = boost::python;

SireVol::CoordGroupEditor __copy__(const SireVol::CoordGroupEditor &other){ return SireVol::CoordGroupEditor(other); }

const char* pvt_get_name(const SireVol::CoordGroupEditor&){ return "SireVol::CoordGroupEditor";}

void register_CoordGroupEditor_class(){

    bp::class_< SireVol::CoordGroupEditor, bp::bases< SireVol::CoordGroupBase > >( "CoordGroupEditor" )    
        .def( bp::init< >() )    
        .def( bp::init< SireVol::CoordGroupBase const & >(( bp::arg("other") )) )    
        .def( 
            "commit"
            , (::SireVol::CoordGroup ( ::SireVol::CoordGroupEditor::* )(  ) )( &::SireVol::CoordGroupEditor::commit ) )    
        .def( 
            "__getitem__"
            , (::SireMaths::Vector & ( ::SireVol::CoordGroupEditor::* )( ::quint32 ) )( &::SireVol::CoordGroupEditor::operator[] )
            , ( bp::arg("i") )
            , bp::return_internal_reference< 1 >() )    
        .def( 
            "rotate"
            , (void ( ::SireVol::CoordGroupEditor::* )( ::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireVol::CoordGroupEditor::rotate )
            , ( bp::arg("quat"), bp::arg("point") ) )    
        .def( 
            "rotate"
            , (void ( ::SireVol::CoordGroupEditor::* )( ::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireVol::CoordGroupEditor::rotate )
            , ( bp::arg("rotmat"), bp::arg("point") ) )    
        .def( 
            "rotate"
            , (void ( ::SireVol::CoordGroupEditor::* )( ::quint32,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) )( &::SireVol::CoordGroupEditor::rotate )
            , ( bp::arg("i"), bp::arg("quat"), bp::arg("point") ) )    
        .def( 
            "rotate"
            , (void ( ::SireVol::CoordGroupEditor::* )( ::quint32,::SireMaths::Matrix const &,::SireMaths::Vector const & ) )( &::SireVol::CoordGroupEditor::rotate )
            , ( bp::arg("i"), bp::arg("rotmat"), bp::arg("point") ) )    
        .def( 
            "setCoordinates"
            , (void ( ::SireVol::CoordGroupEditor::* )( ::QVector<SireMaths::Vector> const & ) )( &::SireVol::CoordGroupEditor::setCoordinates )
            , ( bp::arg("newcoords") ) )    
        .def( 
            "setCoordinates"
            , (void ( ::SireVol::CoordGroupEditor::* )( ::SireVol::CoordGroupBase const & ) )( &::SireVol::CoordGroupEditor::setCoordinates )
            , ( bp::arg("newcoords") ) )    
        .def( 
            "translate"
            , (void ( ::SireVol::CoordGroupEditor::* )( ::SireMaths::Vector const & ) )( &::SireVol::CoordGroupEditor::translate )
            , ( bp::arg("delta") ) )    
        .def( 
            "translate"
            , (void ( ::SireVol::CoordGroupEditor::* )( ::quint32,::SireMaths::Vector const & ) )( &::SireVol::CoordGroupEditor::translate )
            , ( bp::arg("i"), bp::arg("delta") ) )    
        .def( "__copy__", &__copy__)    
        .def( "__str__", &pvt_get_name);

}
