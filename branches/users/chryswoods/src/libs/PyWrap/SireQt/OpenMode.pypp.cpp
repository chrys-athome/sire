// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "OpenMode.pypp.hpp"
#include "boost/python.hpp"
#include "sireqt_headers.h"

namespace bp = boost::python;

void register_OpenMode_class(){

    bp::class_< QFlags<QIODevice::OpenModeFlag> >( "OpenMode", bp::init< QIODevice::OpenModeFlag >(( bp::arg("f") )) )    
        .def( bp::init< bp::optional< void * * > >(( bp::arg("arg0")=bp::object() )) )    
        .def( bp::init< QFlag >(( bp::arg("f") )) )    
        .def( "__int__", &QFlags<QIODevice::OpenModeFlag>::operator int  )    
        .def( !bp::self )    
        .def( bp::self & bp::other< int >() )    
        .def( bp::self & bp::other< uint >() )    
        .def( bp::self & bp::other< QIODevice::OpenModeFlag >() )    
        .def( bp::self &= bp::other< int >() )    
        .def( bp::self &= bp::other< uint >() )    
        .def( bp::self ^ bp::self )    
        .def( bp::self ^ bp::other< QIODevice::OpenModeFlag >() )    
        .def( bp::self ^= bp::self )    
        .def( bp::self ^= bp::other< QIODevice::OpenModeFlag >() )    
        .def( bp::self | bp::self )    
        .def( bp::self | bp::other< QIODevice::OpenModeFlag >() )    
        .def( bp::self |= bp::self )    
        .def( bp::self |= bp::other< QIODevice::OpenModeFlag >() )    
        .def( ~bp::self )    
        .def( 
            "testFlag"
            , (bool ( ::QFlags<QIODevice::OpenModeFlag>::* )( ::QIODevice::OpenModeFlag ) const)( &::QFlags<QIODevice::OpenModeFlag>::testFlag )
            , ( bp::arg("f") ) );

}
