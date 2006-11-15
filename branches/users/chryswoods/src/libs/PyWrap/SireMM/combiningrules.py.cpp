
#include "boost/python.hpp"

#include "SireMM/combiningrules.h"

namespace bp = boost::python;

namespace SireMM
{

struct CombiningRuleBase_wrapper : SireMM::CombiningRuleBase, bp::wrapper< SireMM::CombiningRuleBase > {

    CombiningRuleBase_wrapper( )
    : SireMM::CombiningRuleBase( )
      , bp::wrapper< SireMM::CombiningRuleBase >()
    {   // Normal constructor

    }

    virtual ::SireMM::CombiningRuleBase * clone(  ) const {
        bp::override func_clone = this->get_override( "clone" );
        return func_clone(  );
    }

    virtual void combine( ::QVector<SireMM::CLJParameter> const & clj0, ::QVector<SireMM::CLJParameter> const & clj1, ::SireMM::CLJPairMatrix & cljmatrix ) const {
        bp::override func_combine = this->get_override( "combine" );
        func_combine( boost::ref(clj0), boost::ref(clj1), boost::ref(cljmatrix) );
    }

    virtual void combine( ::QVector<SireMM::CLJParameter> const & clj, ::SireMM::CLJPairMatrix & cljmatrix ) const {
        bp::override func_combine = this->get_override( "combine" );
        func_combine( boost::ref(clj), boost::ref(cljmatrix) );
    }

    virtual char const * what(  ) const {
        bp::override func_what = this->get_override( "what" );
        return func_what(  );
    }

};

struct ArithmeticCombiningRules_wrapper : SireMM::ArithmeticCombiningRules, bp::wrapper< SireMM::ArithmeticCombiningRules > {

    ArithmeticCombiningRules_wrapper(SireMM::ArithmeticCombiningRules const & arg )
    : SireMM::ArithmeticCombiningRules( arg )
      , bp::wrapper< SireMM::ArithmeticCombiningRules >(){
        // copy constructor

    }

    ArithmeticCombiningRules_wrapper( )
    : SireMM::ArithmeticCombiningRules( )
      , bp::wrapper< SireMM::ArithmeticCombiningRules >()
    {   // Normal constructor

    }

    virtual ::SireMM::ArithmeticCombiningRules * clone(  ) const  {
        if( bp::override func_clone = this->get_override( "clone" ) )
            return func_clone(  );
        else
            return SireMM::ArithmeticCombiningRules::clone(  );
    }


    ::SireMM::ArithmeticCombiningRules * default_clone(  ) const  {
        return SireMM::ArithmeticCombiningRules::clone( );
    }

    virtual void combine( ::QVector<SireMM::CLJParameter> const & clj0, ::QVector<SireMM::CLJParameter> const & clj1, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
        if( bp::override func_combine = this->get_override( "combine" ) )
            func_combine( boost::ref(clj0), boost::ref(clj1), boost::ref(cljmatrix) );
        else
            SireMM::ArithmeticCombiningRules::combine( boost::ref(clj0), boost::ref(clj1), boost::ref(cljmatrix) );
    }


    void default_combine( ::QVector<SireMM::CLJParameter> const & clj0, ::QVector<SireMM::CLJParameter> const & clj1, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
        SireMM::ArithmeticCombiningRules::combine( boost::ref(clj0), boost::ref(clj1), boost::ref(cljmatrix) );
    }

    virtual void combine( ::QVector<SireMM::CLJParameter> const & clj, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
        if( bp::override func_combine = this->get_override( "combine" ) )
            func_combine( boost::ref(clj), boost::ref(cljmatrix) );
        else
            SireMM::ArithmeticCombiningRules::combine( boost::ref(clj), boost::ref(cljmatrix) );
    }


    void default_combine( ::QVector<SireMM::CLJParameter> const & clj, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
        SireMM::ArithmeticCombiningRules::combine( boost::ref(clj), boost::ref(cljmatrix) );
    }

    virtual char const * what(  ) const  {
        if( bp::override func_what = this->get_override( "what" ) )
            return func_what(  );
        else
            return SireMM::ArithmeticCombiningRules::what(  );
    }


    char const * default_what(  ) const  {
        return SireMM::ArithmeticCombiningRules::what( );
    }

};

struct GeometricCombiningRules_wrapper : SireMM::GeometricCombiningRules, bp::wrapper< SireMM::GeometricCombiningRules > {

    GeometricCombiningRules_wrapper(SireMM::GeometricCombiningRules const & arg )
    : SireMM::GeometricCombiningRules( arg )
      , bp::wrapper< SireMM::GeometricCombiningRules >(){
        // copy constructor

    }

    GeometricCombiningRules_wrapper( )
    : SireMM::GeometricCombiningRules( )
      , bp::wrapper< SireMM::GeometricCombiningRules >()
    {   // Normal constructor

    }


    ::SireMM::GeometricCombiningRules * default_clone(  ) const  {
        return SireMM::GeometricCombiningRules::clone( );
    }

    virtual void combine( ::QVector<SireMM::CLJParameter> const & clj0, ::QVector<SireMM::CLJParameter> const & clj1, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
        if( bp::override func_combine = this->get_override( "combine" ) )
            func_combine( boost::ref(clj0), boost::ref(clj1), boost::ref(cljmatrix) );
        else
            SireMM::GeometricCombiningRules::combine( boost::ref(clj0), boost::ref(clj1), boost::ref(cljmatrix) );
    }


    void default_combine( ::QVector<SireMM::CLJParameter> const & clj0, ::QVector<SireMM::CLJParameter> const & clj1, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
        SireMM::GeometricCombiningRules::combine( boost::ref(clj0), boost::ref(clj1), boost::ref(cljmatrix) );
    }

    virtual void combine( ::QVector<SireMM::CLJParameter> const & clj, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
        if( bp::override func_combine = this->get_override( "combine" ) )
            func_combine( boost::ref(clj), boost::ref(cljmatrix) );
        else
            SireMM::GeometricCombiningRules::combine( boost::ref(clj), boost::ref(cljmatrix) );
    }


    void default_combine( ::QVector<SireMM::CLJParameter> const & clj, ::SireMM::CLJPairMatrix & cljmatrix ) const  {
        SireMM::GeometricCombiningRules::combine( boost::ref(clj), boost::ref(cljmatrix) );
    }

    virtual char const * what(  ) const  {
        if( bp::override func_what = this->get_override( "what" ) )
            return func_what(  );
        else
            return SireMM::GeometricCombiningRules::what(  );
    }


    char const * default_what(  ) const  {
        return SireMM::GeometricCombiningRules::what( );
    }

};

void export_CombiningRules()
{
    bp::class_< CombiningRuleBase_wrapper, boost::noncopyable >( "CombiningRuleBase" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def(
            "combine"
            , bp::pure_virtual( (void ( ::SireMM::CombiningRuleBase::* )( ::QVector<SireMM::CLJParameter> const &,::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::CombiningRuleBase::combine) )
            , ( bp::arg("clj0"), bp::arg("clj1"), bp::arg("cljmatrix") )
            , bp::default_call_policies() )
        .def(
            "combine"
            , bp::pure_virtual( (void ( ::SireMM::CombiningRuleBase::* )( ::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::CombiningRuleBase::combine) )
            , ( bp::arg("clj"), bp::arg("cljmatrix") )
            , bp::default_call_policies() )
        .def(
            "what"
            , bp::pure_virtual( &::SireMM::CombiningRuleBase::what )
            , bp::default_call_policies() );

    bp::class_< ArithmeticCombiningRules_wrapper, bp::bases< SireMM::CombiningRuleBase > >( "ArithmeticCombiningRules" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def(
            "combine"
            , (void ( ::SireMM::ArithmeticCombiningRules::* )( ::QVector<SireMM::CLJParameter> const &,::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::ArithmeticCombiningRules::combine)
            , (void ( ArithmeticCombiningRules_wrapper::* )( ::QVector<SireMM::CLJParameter> const &,::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&ArithmeticCombiningRules_wrapper::default_combine)
            , ( bp::arg("clj0"), bp::arg("clj1"), bp::arg("cljmatrix") )
            , bp::default_call_policies() )
        .def(
            "combine"
            , (void ( ::SireMM::ArithmeticCombiningRules::* )( ::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::ArithmeticCombiningRules::combine)
            , (void ( ArithmeticCombiningRules_wrapper::* )( ::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&ArithmeticCombiningRules_wrapper::default_combine)
            , ( bp::arg("clj"), bp::arg("cljmatrix") )
            , bp::default_call_policies() )
        .def(
            "typeName"
            , &::SireMM::ArithmeticCombiningRules::typeName
            , bp::default_call_policies() )
        .def(
            "what"
            , &::SireMM::ArithmeticCombiningRules::what
            , &ArithmeticCombiningRules_wrapper::default_what
            , bp::default_call_policies() )
        .staticmethod( "typeName" );

    bp::class_< SireMM::CombiningRules >( "CombiningRules" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireMM::CombiningRuleBase const & >(( bp::arg("rules") )) )
        .def(
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::CLJParameter> const &,::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("clj0"), bp::arg("clj1"), bp::arg("cljmatrix") )
            , bp::default_call_policies() )
        .def(
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("clj"), bp::arg("cljmatrix") )
            , bp::default_call_policies() )
        .def(
            "what"
            , &::SireMM::CombiningRules::what
            , bp::default_call_policies() );

    bp::implicitly_convertible< SireMM::CombiningRuleBase const &, SireMM::CombiningRules >();

    bp::class_< GeometricCombiningRules_wrapper, bp::bases< SireMM::CombiningRuleBase > >( "GeometricCombiningRules" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def(
            "combine"
            , (void ( ::SireMM::GeometricCombiningRules::* )( ::QVector<SireMM::CLJParameter> const &,::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::GeometricCombiningRules::combine)
            , (void ( GeometricCombiningRules_wrapper::* )( ::QVector<SireMM::CLJParameter> const &,::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&GeometricCombiningRules_wrapper::default_combine)
            , ( bp::arg("clj0"), bp::arg("clj1"), bp::arg("cljmatrix") )
            , bp::default_call_policies() )
        .def(
            "combine"
            , (void ( ::SireMM::GeometricCombiningRules::* )( ::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&::SireMM::GeometricCombiningRules::combine)
            , (void ( GeometricCombiningRules_wrapper::* )( ::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)(&GeometricCombiningRules_wrapper::default_combine)
            , ( bp::arg("clj"), bp::arg("cljmatrix") )
            , bp::default_call_policies() )
        .def(
            "typeName"
            , &::SireMM::GeometricCombiningRules::typeName
            , bp::default_call_policies() )
        .def(
            "what"
            , &::SireMM::GeometricCombiningRules::what
            , &GeometricCombiningRules_wrapper::default_what
            , bp::default_call_policies() )
        .staticmethod( "typeName" );
}

}
