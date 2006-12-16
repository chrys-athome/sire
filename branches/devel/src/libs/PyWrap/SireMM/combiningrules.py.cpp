
#include <boost/python.hpp>

#include "SireMM/combiningrules.h"

using namespace boost::python;

namespace SireMM
{

void  
SIREMM_EXPORT
export_CombiningRules()
{
    class_<CombiningRuleBase, boost::noncopyable>("CombiningRuleBase", no_init)
    ;

    class_<CombiningRules>("CombiningRules", init<>())
        .def( init<const CombiningRuleBase&>() )
    ;

    implicitly_convertible<CombiningRuleBase, CombiningRules>();

    class_<ArithmeticCombiningRules, bases<CombiningRuleBase> >("ArithmeticCombiningRules", init<>())
    ;

    class_<GeometricCombiningRules, bases<CombiningRuleBase> >("GeometricCombiningRules", init<>())
    ;
}

}

// #include "boost/python.hpp"
//
// #include "SireMM/combiningrules.h"
//
// namespace bp = boost::python;
//
// namespace SireMM
// {
//
// struct CombiningRuleBase_wrapper : SireMM::CombiningRuleBase, bp::wrapper< SireMM::CombiningRuleBase > {
//
//     CombiningRuleBase_wrapper( )
//     : SireMM::CombiningRuleBase( )
//       , bp::wrapper< SireMM::CombiningRuleBase >()
//     {   // Normal constructor
//
//     }
//
//     virtual ::SireMM::CombiningRuleBase * clone(  ) const {
//         bp::override func_clone = this->get_override( "clone" );
//         return func_clone(  );
//     }
//
//     virtual void combine( ::QVector<SireMM::ChargeParameter> const & chg0, ::QVector<SireMM::LJParameter> const & lj0, ::QVector<SireMM::ChargeParameter> const & chg1, ::QVector<SireMM::LJParameter> const & lj1, ::SireMM::CLJPairMatrix & cljmatrix ) const {
//         bp::override func_combine = this->get_override( "combine" );
//         func_combine( boost::ref(chg0), boost::ref(lj0), boost::ref(chg1), boost::ref(lj1), boost::ref(cljmatrix) );
//     }
//
//     virtual void combine( ::QVector<SireMM::ChargeParameter> const & chgs, ::QVector<SireMM::LJParameter> const & ljs, ::SireMM::CLJPairMatrix & cljmatrix ) const {
//         bp::override func_combine = this->get_override( "combine" );
//         func_combine( boost::ref(chgs), boost::ref(ljs), boost::ref(cljmatrix) );
//     }
//
//     virtual char const * what(  ) const {
//         bp::override func_what = this->get_override( "what" );
//         return func_what(  );
//     }
//
// };
//
// struct ArithmeticCombiningRules_wrapper : SireMM::ArithmeticCombiningRules, bp::wrapper< SireMM::ArithmeticCombiningRules > {
//
//     ArithmeticCombiningRules_wrapper(SireMM::ArithmeticCombiningRules const & arg )
//     : SireMM::ArithmeticCombiningRules( arg )
//       , bp::wrapper< SireMM::ArithmeticCombiningRules >(){
//         // copy constructor
//
//     }
//
//     ArithmeticCombiningRules_wrapper( )
//     : SireMM::ArithmeticCombiningRules( )
//       , bp::wrapper< SireMM::ArithmeticCombiningRules >()
//     {   // Normal constructor
//
//     }
//
//     virtual ::SireMM::ArithmeticCombiningRules * clone(  ) const  {
//         if( bp::override func_clone = this->get_override( "clone" ) )
//             return func_clone(  );
//         else
//             return SireMM::ArithmeticCombiningRules::clone(  );
//     }
//
//
//     ::SireMM::ArithmeticCombiningRules * default_clone(  ) const  {
//         return SireMM::ArithmeticCombiningRules::clone( );
//     }
//
//     virtual void combine( ::QVector<SireMM::ChargeParameter> const & chg0, ::QVector<SireMM::LJParameter> const & lj0, ::QVector<SireMM::ChargeParameter> const & chg1, ::QVector<SireMM::LJParameter> const & lj1, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
//         if( bp::override func_combine = this->get_override( "combine" ) )
//             func_combine( boost::ref(chg0), boost::ref(lj0), boost::ref(chg1), boost::ref(lj1), boost::ref(cljmatrix) );
//         else
//             SireMM::ArithmeticCombiningRules::combine( boost::ref(chg0), boost::ref(lj0), boost::ref(chg1), boost::ref(lj1), boost::ref(cljmatrix) );
//     }
//
//
//     void default_combine( ::QVector<SireMM::ChargeParameter> const & chg0, ::QVector<SireMM::LJParameter> const & lj0, ::QVector<SireMM::ChargeParameter> const & chg1, ::QVector<SireMM::LJParameter> const & lj1, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
//         SireMM::ArithmeticCombiningRules::combine( boost::ref(chg0), boost::ref(lj0), boost::ref(chg1), boost::ref(lj1), boost::ref(cljmatrix) );
//     }
//
//     virtual void combine( ::QVector<SireMM::ChargeParameter> const & chgs, ::QVector<SireMM::LJParameter> const & ljs, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
//         if( bp::override func_combine = this->get_override( "combine" ) )
//             func_combine( boost::ref(chgs), boost::ref(ljs), boost::ref(cljmatrix) );
//         else
//             SireMM::ArithmeticCombiningRules::combine( boost::ref(chgs), boost::ref(ljs), boost::ref(cljmatrix) );
//     }
//
//
//     void default_combine( ::QVector<SireMM::ChargeParameter> const & chgs, ::QVector<SireMM::LJParameter> const & ljs, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
//         SireMM::ArithmeticCombiningRules::combine( boost::ref(chgs), boost::ref(ljs), boost::ref(cljmatrix) );
//     }
//
//     virtual char const * what(  ) const  {
//         if( bp::override func_what = this->get_override( "what" ) )
//             return func_what(  );
//         else
//             return SireMM::ArithmeticCombiningRules::what(  );
//     }
//
//
//     char const * default_what(  ) const  {
//         return SireMM::ArithmeticCombiningRules::what( );
//     }
// };
//
// struct CLJParameter_wrapper : SireMM::CLJParameter, bp::wrapper< SireMM::CLJParameter > {
//
//     CLJParameter_wrapper(SireMM::CLJParameter const & arg )
//     : SireMM::CLJParameter( arg )
//       , bp::wrapper< SireMM::CLJParameter >(){
//         // copy constructor
//
//     }
//
//     CLJParameter_wrapper( )
//     : SireMM::CLJParameter( )
//       , bp::wrapper< SireMM::CLJParameter >()
//     {   // Normal constructor
//
//     }
//
//     CLJParameter_wrapper(::SireMM::ChargeParameter const & charge, ::SireMM::LJParameter const & lj )
//     : SireMM::CLJParameter( charge, lj )
//       , bp::wrapper< SireMM::CLJParameter >()
//     {   // Normal constructor
//
//     }
//
//     CLJParameter_wrapper(::SireMM::ChargeParameter const & charge, double sigma, double epsilon )
//     : SireMM::CLJParameter( charge, sigma, epsilon )
//       , bp::wrapper< SireMM::CLJParameter >()
//     {   // Normal constructor
//
//     }
//
// };
//
// struct GeometricCombiningRules_wrapper : SireMM::GeometricCombiningRules, bp::wrapper< SireMM::GeometricCombiningRules > {
//
//     GeometricCombiningRules_wrapper(SireMM::GeometricCombiningRules const & arg )
//     : SireMM::GeometricCombiningRules( arg )
//       , bp::wrapper< SireMM::GeometricCombiningRules >(){
//         // copy constructor
//
//     }
//
//     GeometricCombiningRules_wrapper( )
//     : SireMM::GeometricCombiningRules( )
//       , bp::wrapper< SireMM::GeometricCombiningRules >()
//     {   // Normal constructor
//
//     }
//
//     virtual ::SireMM::GeometricCombiningRules * clone(  ) const  {
//         if( bp::override func_clone = this->get_override( "clone" ) )
//             return func_clone(  );
//         else
//             return SireMM::GeometricCombiningRules::clone(  );
//     }
//
//
//     ::SireMM::GeometricCombiningRules * default_clone(  ) const  {
//         return SireMM::GeometricCombiningRules::clone( );
//     }
//
//     virtual void combine( ::QVector<SireMM::ChargeParameter> const & chg0, ::QVector<SireMM::LJParameter> const & lj0, ::QVector<SireMM::ChargeParameter> const & chg1, ::QVector<SireMM::LJParameter> const & lj1, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
//         if( bp::override func_combine = this->get_override( "combine" ) )
//             func_combine( boost::ref(chg0), boost::ref(lj0), boost::ref(chg1), boost::ref(lj1), boost::ref(cljmatrix) );
//         else
//             SireMM::GeometricCombiningRules::combine( boost::ref(chg0), boost::ref(lj0), boost::ref(chg1), boost::ref(lj1), boost::ref(cljmatrix) );
//     }
//
//
//     void default_combine( ::QVector<SireMM::ChargeParameter> const & chg0, ::QVector<SireMM::LJParameter> const & lj0, ::QVector<SireMM::ChargeParameter> const & chg1, ::QVector<SireMM::LJParameter> const & lj1, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
//         SireMM::GeometricCombiningRules::combine( boost::ref(chg0), boost::ref(lj0), boost::ref(chg1), boost::ref(lj1), boost::ref(cljmatrix) );
//     }
//
//     virtual void combine( ::QVector<SireMM::ChargeParameter> const & chgs, ::QVector<SireMM::LJParameter> const & ljs, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
//         if( bp::override func_combine = this->get_override( "combine" ) )
//             func_combine( boost::ref(chgs), boost::ref(ljs), boost::ref(cljmatrix) );
//         else
//             SireMM::GeometricCombiningRules::combine( boost::ref(chgs), boost::ref(ljs), boost::ref(cljmatrix) );
//     }
//
//
//     void default_combine( ::QVector<SireMM::ChargeParameter> const & chgs, ::QVector<SireMM::LJParameter> const & ljs, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
//         SireMM::GeometricCombiningRules::combine( boost::ref(chgs), boost::ref(ljs), boost::ref(cljmatrix) );
//     }
//
//     virtual char const * what(  ) const  {
//         if( bp::override func_what = this->get_override( "what" ) )
//             return func_what(  );
//         else
//             return SireMM::GeometricCombiningRules::what(  );
//     }
//
//
//     char const * default_what(  ) const  {
//         return SireMM::GeometricCombiningRules::what( );
//     }
// };
//
// void export_CombiningRules()
// {
//     bp::class_< CombiningRuleBase_wrapper, boost::noncopyable >( "CombiningRuleBase" )
//         .def( bp::init< >()[bp::default_call_policies()] )
//         .def(
//             "combine"
//             , bp::pure_virtual( (void ( ::SireMM::CombiningRuleBase::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::CombiningRuleBase::combine) )
//             , ( bp::arg("chg0"), bp::arg("lj0"), bp::arg("chg1"), bp::arg("lj1"), bp::arg("cljmatrix") )
//             , bp::default_call_policies() )
//         .def(
//             "combine"
//             , bp::pure_virtual( (void ( ::SireMM::CombiningRuleBase::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::CombiningRuleBase::combine) )
//             , ( bp::arg("chgs"), bp::arg("ljs"), bp::arg("cljmatrix") )
//             , bp::default_call_policies() )
//         .def(
//             "what"
//             , bp::pure_virtual( &::SireMM::CombiningRuleBase::what )
//             , bp::default_call_policies() );
//
//     bp::class_< ArithmeticCombiningRules_wrapper, bp::bases< SireMM::CombiningRuleBase > >( "ArithmeticCombiningRules" )
//         .def( bp::init< >()[bp::default_call_policies()] )
//         .def(
//             "combine"
//             , (void ( ::SireMM::ArithmeticCombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::ArithmeticCombiningRules::combine)
//             , (void ( ArithmeticCombiningRules_wrapper::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&ArithmeticCombiningRules_wrapper::default_combine)
//             , ( bp::arg("chg0"), bp::arg("lj0"), bp::arg("chg1"), bp::arg("lj1"), bp::arg("cljmatrix") )
//             , bp::default_call_policies() )
//         .def(
//             "combine"
//             , (void ( ::SireMM::ArithmeticCombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::ArithmeticCombiningRules::combine)
//             , (void ( ArithmeticCombiningRules_wrapper::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&ArithmeticCombiningRules_wrapper::default_combine)
//             , ( bp::arg("chgs"), bp::arg("ljs"), bp::arg("cljmatrix") )
//             , bp::default_call_policies() )
//         .def(
//             "typeName"
//             , &::SireMM::ArithmeticCombiningRules::typeName
//             , bp::default_call_policies() )
//         .def(
//             "what"
//             , &::SireMM::ArithmeticCombiningRules::what
//             , &ArithmeticCombiningRules_wrapper::default_what
//             , bp::default_call_policies() )
//         .staticmethod( "typeName" );
//
//     bp::class_< SireMM::CombiningRules >( "CombiningRules" )
//         .def( bp::init< >()[bp::default_call_policies()] )
//         .def( bp::init< SireMM::CombiningRuleBase const & >(( bp::arg("rules") )) )
//         .def(
//             "combine"
//             , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
//             , ( bp::arg("chg0"), bp::arg("lj0"), bp::arg("chg1"), bp::arg("lj1"), bp::arg("cljmatrix") )
//             , bp::default_call_policies() )
//         .def(
//             "combine"
//             , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
//             , ( bp::arg("chgs"), bp::arg("ljs"), bp::arg("cljmatrix") )
//             , bp::default_call_policies() )
//         .def(
//             "what"
//             , &::SireMM::CombiningRules::what
//             , bp::default_call_policies() );
//
//     bp::implicitly_convertible< SireMM::CombiningRuleBase const &, SireMM::CombiningRules >();
//
//     bp::class_< GeometricCombiningRules_wrapper, bp::bases< SireMM::CombiningRuleBase > >( "GeometricCombiningRules" )
//         .def( bp::init< >()[bp::default_call_policies()] )
//         .def(
//             "combine"
//             , (void ( ::SireMM::GeometricCombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::GeometricCombiningRules::combine)
//             , (void ( GeometricCombiningRules_wrapper::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&GeometricCombiningRules_wrapper::default_combine)
//             , ( bp::arg("chg0"), bp::arg("lj0"), bp::arg("chg1"), bp::arg("lj1"), bp::arg("cljmatrix") )
//             , bp::default_call_policies() )
//         .def(
//             "combine"
//             , (void ( ::SireMM::GeometricCombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::GeometricCombiningRules::combine)
//             , (void ( GeometricCombiningRules_wrapper::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&GeometricCombiningRules_wrapper::default_combine)
//             , ( bp::arg("chgs"), bp::arg("ljs"), bp::arg("cljmatrix") )
//             , bp::default_call_policies() )
//         .def(
//             "typeName"
//             , &::SireMM::GeometricCombiningRules::typeName
//             , bp::default_call_policies() )
//         .def(
//             "what"
//             , &::SireMM::GeometricCombiningRules::what
//             , &GeometricCombiningRules_wrapper::default_what
//             , bp::default_call_policies() )
//         .staticmethod( "typeName" );
// }
//
// }
