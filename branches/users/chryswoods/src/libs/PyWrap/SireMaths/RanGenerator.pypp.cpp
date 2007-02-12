// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "RanGenerator.pypp.hpp"
#include "boost/python.hpp"
#include "siremaths_headers.h"

namespace bp = boost::python;

void register_RanGenerator_class(){

    bp::class_< SireMaths::RanGenerator >( "RanGenerator" )    
        .def( bp::init< >() )    
        .def( bp::init< quint32 >(( bp::arg("seed") )) )    
        .def( bp::init< QVector<unsigned> const & >(( bp::arg("seed") )) )    
        .def( 
            "getState"
            , (::QVector<unsigned> ( ::SireMaths::RanGenerator::* )(  ) )( &::SireMaths::RanGenerator::getState ) )    
        .def( 
            "rand"
            , (double ( ::SireMaths::RanGenerator::* )(  ) )( &::SireMaths::RanGenerator::rand ) )    
        .def( 
            "rand"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<double> &,::uint ) )( &::SireMaths::RanGenerator::rand )
            , ( bp::arg("array"), bp::arg("n")=(unsigned int)(0) ) )    
        .def( 
            "rand53"
            , (double ( ::SireMaths::RanGenerator::* )(  ) )( &::SireMaths::RanGenerator::rand53 ) )    
        .def( 
            "rand53"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<double> &,::uint ) )( &::SireMaths::RanGenerator::rand53 )
            , ( bp::arg("array"), bp::arg("n")=(unsigned int)(0) ) )    
        .def( 
            "randInt"
            , (::quint32 ( ::SireMaths::RanGenerator::* )(  ) )( &::SireMaths::RanGenerator::randInt ) )    
        .def( 
            "randInt"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<unsigned> &,::uint ) )( &::SireMaths::RanGenerator::randInt )
            , ( bp::arg("array"), bp::arg("n")=(unsigned int)(0) ) )    
        .def( 
            "randInt"
            , (::quint32 ( ::SireMaths::RanGenerator::* )( ::quint32 ) )( &::SireMaths::RanGenerator::randInt )
            , ( bp::arg("maxval") ) )    
        .def( 
            "randInt"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<unsigned> &,::quint32,::uint ) )( &::SireMaths::RanGenerator::randInt )
            , ( bp::arg("array"), bp::arg("maxval"), bp::arg("n")=(unsigned int)(0) ) )    
        .def( 
            "randInt64"
            , (::quint64 ( ::SireMaths::RanGenerator::* )(  ) )( &::SireMaths::RanGenerator::randInt64 ) )    
        .def( 
            "randInt64"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<long long unsigned int> &,::uint ) )( &::SireMaths::RanGenerator::randInt64 )
            , ( bp::arg("array"), bp::arg("n")=(unsigned int)(0) ) )    
        .def( 
            "randInt64"
            , (::quint64 ( ::SireMaths::RanGenerator::* )( ::quint64 ) )( &::SireMaths::RanGenerator::randInt64 )
            , ( bp::arg("maxval") ) )    
        .def( 
            "randInt64"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<long long unsigned int> &,::quint64,::uint ) )( &::SireMaths::RanGenerator::randInt64 )
            , ( bp::arg("array"), bp::arg("maxval"), bp::arg("n")=(unsigned int)(0) ) )    
        .def( 
            "randNorm"
            , (double ( ::SireMaths::RanGenerator::* )( double,double ) )( &::SireMaths::RanGenerator::randNorm )
            , ( bp::arg("mean"), bp::arg("variance") ) )    
        .def( 
            "randNorm"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<double> &,double,double,::uint ) )( &::SireMaths::RanGenerator::randNorm )
            , ( bp::arg("array"), bp::arg("mean"), bp::arg("variance"), bp::arg("n")=(unsigned int)(0) ) )    
        .def( 
            "seed"
            , (void ( ::SireMaths::RanGenerator::* )(  ) )( &::SireMaths::RanGenerator::seed ) )    
        .def( 
            "seed"
            , (void ( ::SireMaths::RanGenerator::* )( ::quint32 ) )( &::SireMaths::RanGenerator::seed )
            , ( bp::arg("seed") ) )    
        .def( 
            "seed"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<unsigned> const & ) )( &::SireMaths::RanGenerator::seed )
            , ( bp::arg("seed") ) )    
        .def( 
            "seed"
            , (void ( ::SireMaths::RanGenerator::* )( ::SireMaths::RanGenerator const & ) )( &::SireMaths::RanGenerator::seed )
            , ( bp::arg("other") ) )    
        .def( 
            "setState"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<unsigned> const & ) )( &::SireMaths::RanGenerator::setState )
            , ( bp::arg("state") ) )    
        .def( 
            "vectorOnSphere"
            , (::SireMaths::Vector ( ::SireMaths::RanGenerator::* )(  ) )( &::SireMaths::RanGenerator::vectorOnSphere ) )    
        .def( 
            "vectorOnSphere"
            , (void ( ::SireMaths::RanGenerator::* )( ::QVector<SireMaths::Vector> &,::uint ) )( &::SireMaths::RanGenerator::vectorOnSphere )
            , ( bp::arg("array"), bp::arg("n")=(unsigned int)(0) ) );

}
