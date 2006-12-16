
#include <boost/python.hpp>

#include "SireMM/switchingfunction.h"

using namespace boost::python;

namespace SireMM
{

void  
SIREMM_EXPORT
export_SwitchingFunction()
{
    class_<SwitchFuncBase, boost::noncopyable>("SwitchFuncBase", no_init)
    ;

    class_<SwitchingFunction>("SwitchingFunction", init<>())
        .def( init<const SwitchFuncBase&>() )
    ;

    implicitly_convertible<SwitchFuncBase,SwitchingFunction>();

    class_<HarmonicSwitchingFunction, bases<SwitchFuncBase> >("HarmonicSwitchingFunction", init<>())
        .def( init<double>() )
        .def( init<double,double>() )
        .def( init<double,double,double,double>() )
    ;

    class_<NoCutoff, bases<SwitchFuncBase> >("NoCutoff", init<>() )
    ;
}

}


// #include "boost/python.hpp"
//
// #include "SireMM/switchingfunction.h"
//
// namespace bp = boost::python;
//
// namespace SireMM
// {
//
// struct SwitchFuncBase_wrapper : SireMM::SwitchFuncBase, bp::wrapper< SireMM::SwitchFuncBase > {
//
//     SwitchFuncBase_wrapper( )
//     : SireMM::SwitchFuncBase( )
//       , bp::wrapper< SireMM::SwitchFuncBase >()
//     {   // Normal constructor
//
//     }
//
//     SwitchFuncBase_wrapper(double cutdistance )
//     : SireMM::SwitchFuncBase( cutdistance )
//       , bp::wrapper< SireMM::SwitchFuncBase >()
//     {   // Normal constructor
//
//     }
//
//     virtual ::SireMM::SwitchFuncBase * clone(  ) const {
//         bp::override func_clone = this->get_override( "clone" );
//         return func_clone(  );
//     }
//
//     virtual double electrostaticScaleFactor( double dist ) const {
//         bp::override func_electrostaticScaleFactor = this->get_override( "electrostaticScaleFactor" );
//         return func_electrostaticScaleFactor( dist );
//     }
//
//     virtual double vdwScaleFactor( double dist ) const {
//         bp::override func_vdwScaleFactor = this->get_override( "vdwScaleFactor" );
//         return func_vdwScaleFactor( dist );
//     }
//
//     virtual char const * what(  ) const {
//         bp::override func_what = this->get_override( "what" );
//         return func_what(  );
//     }
//
// };
//
// struct HarmonicSwitchingFunction_wrapper : SireMM::HarmonicSwitchingFunction, bp::wrapper< SireMM::HarmonicSwitchingFunction > {
//
//     HarmonicSwitchingFunction_wrapper(SireMM::HarmonicSwitchingFunction const & arg )
//     : SireMM::HarmonicSwitchingFunction( arg )
//       , bp::wrapper< SireMM::HarmonicSwitchingFunction >(){
//         // copy constructor
//
//     }
//
//     HarmonicSwitchingFunction_wrapper( )
//     : SireMM::HarmonicSwitchingFunction( )
//       , bp::wrapper< SireMM::HarmonicSwitchingFunction >()
//     {   // Normal constructor
//
//     }
//
//     HarmonicSwitchingFunction_wrapper(double cutoffdist )
//     : SireMM::HarmonicSwitchingFunction( cutoffdist )
//       , bp::wrapper< SireMM::HarmonicSwitchingFunction >()
//     {   // Normal constructor
//
//     }
//
//     HarmonicSwitchingFunction_wrapper(double cutoffdist, double featherdist )
//     : SireMM::HarmonicSwitchingFunction( cutoffdist, featherdist )
//       , bp::wrapper< SireMM::HarmonicSwitchingFunction >()
//     {   // Normal constructor
//
//     }
//
//     HarmonicSwitchingFunction_wrapper(double cutoffdist, double elecfeather, double vdwfeather )
//     : SireMM::HarmonicSwitchingFunction( cutoffdist, elecfeather, vdwfeather )
//       , bp::wrapper< SireMM::HarmonicSwitchingFunction >()
//     {   // Normal constructor
//
//     }
//
//     HarmonicSwitchingFunction_wrapper(double eleccutoff, double elecfeather, double vdwcutoff, double vdwfeather )
//     : SireMM::HarmonicSwitchingFunction( eleccutoff, elecfeather, vdwcutoff, vdwfeather )
//       , bp::wrapper< SireMM::HarmonicSwitchingFunction >()
//     {   // Normal constructor
//
//     }
//
//     virtual double electrostaticScaleFactor( double dist ) const  {
//         if( bp::override func_electrostaticScaleFactor = this->get_override( "electrostaticScaleFactor" ) )
//             return func_electrostaticScaleFactor( dist );
//         else
//             return SireMM::HarmonicSwitchingFunction::electrostaticScaleFactor( dist );
//     }
//
//
//     double default_electrostaticScaleFactor( double dist ) const  {
//         return SireMM::HarmonicSwitchingFunction::electrostaticScaleFactor( dist );
//     }
//
//     void set( double cutelec, double featherelec, double cutvdw, double feathervdw ){
//         SireMM::HarmonicSwitchingFunction::set( cutelec, featherelec, cutvdw, feathervdw );
//     }
//
//     virtual double vdwScaleFactor( double dist ) const  {
//         if( bp::override func_vdwScaleFactor = this->get_override( "vdwScaleFactor" ) )
//             return func_vdwScaleFactor( dist );
//         else
//             return SireMM::HarmonicSwitchingFunction::vdwScaleFactor( dist );
//     }
//
//
//     double default_vdwScaleFactor( double dist ) const  {
//         return SireMM::HarmonicSwitchingFunction::vdwScaleFactor( dist );
//     }
//
//     virtual char const * what(  ) const  {
//         if( bp::override func_what = this->get_override( "what" ) )
//             return func_what(  );
//         else
//             return SireMM::HarmonicSwitchingFunction::what(  );
//     }
//
//
//     char const * default_what(  ) const  {
//         return SireMM::HarmonicSwitchingFunction::what( );
//     }
//
// };
//
// struct NoCutoff_wrapper : SireMM::NoCutoff, bp::wrapper< SireMM::NoCutoff > {
//
//     NoCutoff_wrapper(SireMM::NoCutoff const & arg )
//     : SireMM::NoCutoff( arg )
//       , bp::wrapper< SireMM::NoCutoff >(){
//         // copy constructor
//
//     }
//
//     NoCutoff_wrapper( )
//     : SireMM::NoCutoff( )
//       , bp::wrapper< SireMM::NoCutoff >()
//     {   // Normal constructor
//
//     }
//
//     virtual double electrostaticScaleFactor( double dist ) const  {
//         if( bp::override func_electrostaticScaleFactor = this->get_override( "electrostaticScaleFactor" ) )
//             return func_electrostaticScaleFactor( dist );
//         else
//             return SireMM::NoCutoff::electrostaticScaleFactor( dist );
//     }
//
//
//     double default_electrostaticScaleFactor( double dist ) const  {
//         return SireMM::NoCutoff::electrostaticScaleFactor( dist );
//     }
//
//     virtual double vdwScaleFactor( double dist ) const  {
//         if( bp::override func_vdwScaleFactor = this->get_override( "vdwScaleFactor" ) )
//             return func_vdwScaleFactor( dist );
//         else
//             return SireMM::NoCutoff::vdwScaleFactor( dist );
//     }
//
//
//     double default_vdwScaleFactor( double dist ) const  {
//         return SireMM::NoCutoff::vdwScaleFactor( dist );
//     }
//
//     virtual char const * what(  ) const  {
//         if( bp::override func_what = this->get_override( "what" ) )
//             return func_what(  );
//         else
//             return SireMM::NoCutoff::what(  );
//     }
//
//
//     char const * default_what(  ) const  {
//         return SireMM::NoCutoff::what( );
//     }
//
// };
//
// void export_SwitchingFunction()
// {
//
//     bp::class_< SwitchFuncBase_wrapper, boost::noncopyable >( "SwitchFuncBase" )
//         .def( bp::init< >()[bp::default_call_policies()] )
//         .def( bp::init< double >(( bp::arg("cutdistance") ))[bp::default_call_policies()] )
//         .def(
//             "cutoffDistance"
//             , &::SireMM::SwitchFuncBase::cutoffDistance
//             , bp::default_call_policies() )
//         .def(
//             "electrostaticScaleFactor"
//             , bp::pure_virtual( &::SireMM::SwitchFuncBase::electrostaticScaleFactor )
//             , ( bp::arg("dist") )
//             , bp::default_call_policies() )
//         .def(
//             "vdwScaleFactor"
//             , bp::pure_virtual( &::SireMM::SwitchFuncBase::vdwScaleFactor )
//             , ( bp::arg("dist") )
//             , bp::default_call_policies() )
//         .def(
//             "what"
//             , bp::pure_virtual( &::SireMM::SwitchFuncBase::what )
//             , bp::default_call_policies() );
//
//     bp::class_< HarmonicSwitchingFunction_wrapper, bp::bases< SireMM::SwitchFuncBase > >( "HarmonicSwitchingFunction" )
//         .def( bp::init< >()[bp::default_call_policies()] )
//         .def( bp::init< double >(( bp::arg("cutoffdist") ))[bp::default_call_policies()] )
//         .def( bp::init< double, double >(( bp::arg("cutoffdist"), bp::arg("featherdist") ))[bp::default_call_policies()] )
//         .def( bp::init< double, double, double >(( bp::arg("cutoffdist"), bp::arg("elecfeather"), bp::arg("vdwfeather") ))[bp::default_call_policies()] )
//         .def( bp::init< double, double, double, double >(( bp::arg("eleccutoff"), bp::arg("elecfeather"), bp::arg("vdwcutoff"), bp::arg("vdwfeather") ))[bp::default_call_policies()] )
//         .def(
//             "electrostaticScaleFactor"
//             , &::SireMM::HarmonicSwitchingFunction::electrostaticScaleFactor
//             , &HarmonicSwitchingFunction_wrapper::default_electrostaticScaleFactor
//             , ( bp::arg("dist") )
//             , bp::default_call_policies() )
//         .def(
//             "set"
//             , &HarmonicSwitchingFunction_wrapper::set
//             , ( bp::arg("cutelec"), bp::arg("featherelec"), bp::arg("cutvdw"), bp::arg("feathervdw") )
//             , bp::default_call_policies() )
//         .def(
//             "typeName"
//             , &::SireMM::HarmonicSwitchingFunction::typeName
//             , bp::default_call_policies() )
//         .def(
//             "vdwScaleFactor"
//             , &::SireMM::HarmonicSwitchingFunction::vdwScaleFactor
//             , &HarmonicSwitchingFunction_wrapper::default_vdwScaleFactor
//             , ( bp::arg("dist") )
//             , bp::default_call_policies() )
//         .def(
//             "what"
//             , &::SireMM::HarmonicSwitchingFunction::what
//             , &HarmonicSwitchingFunction_wrapper::default_what
//             , bp::default_call_policies() )
//         .staticmethod( "typeName" );
//
//     bp::class_< NoCutoff_wrapper, bp::bases< SireMM::SwitchFuncBase > >( "NoCutoff" )
//         .def( bp::init< >()[bp::default_call_policies()] )
//         .def(
//             "electrostaticScaleFactor"
//             , &::SireMM::NoCutoff::electrostaticScaleFactor
//             , &NoCutoff_wrapper::default_electrostaticScaleFactor
//             , ( bp::arg("dist") )
//             , bp::default_call_policies() )
//         .def(
//             "typeName"
//             , &::SireMM::NoCutoff::typeName
//             , bp::default_call_policies() )
//         .def(
//             "vdwScaleFactor"
//             , &::SireMM::NoCutoff::vdwScaleFactor
//             , &NoCutoff_wrapper::default_vdwScaleFactor
//             , ( bp::arg("dist") )
//             , bp::default_call_policies() )
//         .def(
//             "what"
//             , &::SireMM::NoCutoff::what
//             , &NoCutoff_wrapper::default_what
//             , bp::default_call_policies() )
//         .staticmethod( "typeName" );
//
//     bp::class_< SireMM::SwitchingFunction >( "SwitchingFunction" )
//         .def( bp::init< >()[bp::default_call_policies()] )
//         .def( bp::init< SireMM::SwitchFuncBase const & >(( bp::arg("switchingfunction") )) )
//         .def(
//             "cutoffDistance"
//             , &::SireMM::SwitchingFunction::cutoffDistance
//             , bp::default_call_policies() )
//         .def(
//             "electrostaticScaleFactor"
//             , &::SireMM::SwitchingFunction::electrostaticScaleFactor
//             , ( bp::arg("dist") )
//             , bp::default_call_policies() )
//         .def(
//             "vdwScaleFactor"
//             , &::SireMM::SwitchingFunction::vdwScaleFactor
//             , ( bp::arg("dist") )
//             , bp::default_call_policies() )
//         .def(
//             "what"
//             , &::SireMM::SwitchingFunction::what
//             , bp::default_call_policies() );
//
//     bp::implicitly_convertible< SireMM::SwitchFuncBase const &, SireMM::SwitchingFunction >();
//
// }
//
// }
