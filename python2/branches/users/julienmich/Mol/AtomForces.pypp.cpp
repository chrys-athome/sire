// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "AtomForces.pypp.hpp"

namespace bp = boost::python;

#include "atomforces.h"

#include "atomforces.h"

SireMol::AtomProperty<SireMaths::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > > __copy__(const SireMol::AtomProperty<SireMaths::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > > &other){ return SireMol::AtomProperty<SireMaths::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > >(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_AtomForces_class(){

    { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >
        typedef bp::class_< SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >, bp::bases< SireMol::AtomProp, SireMol::MolViewProperty, SireBase::Property > > AtomForces_exposer_t;
        AtomForces_exposer_t AtomForces_exposer = AtomForces_exposer_t( "AtomForces" );
        bp::scope AtomForces_scope( AtomForces_exposer );
        AtomForces_exposer.def( bp::init< >() );
        AtomForces_exposer.def( bp::init< SireMol::MoleculeInfoData const & >(( bp::arg("molinfo") )) );
        AtomForces_exposer.def( bp::init< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > const & >(( bp::arg("value") )) );
        AtomForces_exposer.def( bp::init< SireBase::PackedArray2D< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const & >(( bp::arg("values") )) );
        AtomForces_exposer.def( bp::init< SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const & >(( bp::arg("other") )) );
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::array
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireBase::PackedArray2D< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*array_function_type )(  ) const;
            array_function_type array_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::array );
            
            AtomForces_exposer.def( 
                "array"
                , array_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::assertCanConvert
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*assertCanConvert_function_type )( ::QVariant const & ) const;
            assertCanConvert_function_type assertCanConvert_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::assertCanConvert );
            
            AtomForces_exposer.def( 
                "assertCanConvert"
                , assertCanConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::assignFrom
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*assignFrom_function_type )( ::SireMol::AtomProperty< QVariant > const & ) ;
            assignFrom_function_type assignFrom_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::assignFrom );
            
            AtomForces_exposer.def( 
                "assignFrom"
                , assignFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::at
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*at_function_type )( ::SireMol::CGIdx ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::at );
            
            AtomForces_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::at
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > const & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*at_function_type )( ::SireMol::CGAtomIdx const & ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::at );
            
            AtomForces_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::canConvert
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*canConvert_function_type )( ::QVariant const & ) const;
            canConvert_function_type canConvert_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::canConvert );
            
            AtomForces_exposer.def( 
                "canConvert"
                , canConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::copyFrom
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*copyFrom_function_type )( ::QVector< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::copyFrom );
            
            AtomForces_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::copyFrom
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*copyFrom_function_type )( ::QVector< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const &,::SireMol::AtomSelection const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::copyFrom );
            
            AtomForces_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values"), bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::count
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::count );
            
            AtomForces_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::fromVariant
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > ( *fromVariant_function_type )( ::SireMol::AtomProperty< QVariant > const & );
            fromVariant_function_type fromVariant_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::fromVariant );
            
            AtomForces_exposer.def( 
                "fromVariant"
                , fromVariant_function_value
                , ( bp::arg("variant") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::get
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*get_function_type )( ::SireMol::CGIdx ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::get );
            
            AtomForces_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::get
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > const & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*get_function_type )( ::SireMol::CGAtomIdx const & ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::get );
            
            AtomForces_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::isCompatibleWith
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*isCompatibleWith_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            isCompatibleWith_function_type isCompatibleWith_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::isCompatibleWith );
            
            AtomForces_exposer.def( 
                "isCompatibleWith"
                , isCompatibleWith_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::isEmpty
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::isEmpty );
            
            AtomForces_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::matchToSelection
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*matchToSelection_function_type )( ::SireMol::AtomSelection const & ) const;
            matchToSelection_function_type matchToSelection_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::matchToSelection );
            
            AtomForces_exposer.def( 
                "matchToSelection"
                , matchToSelection_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::nAtoms
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*nAtoms_function_type )(  ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::nAtoms );
            
            AtomForces_exposer.def( 
                "nAtoms"
                , nAtoms_function_value );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::nAtoms
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*nAtoms_function_type )( ::SireMol::CGIdx ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::nAtoms );
            
            AtomForces_exposer.def( 
                "nAtoms"
                , nAtoms_function_value
                , ( bp::arg("cgidx") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::nCutGroups
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*nCutGroups_function_type )(  ) const;
            nCutGroups_function_type nCutGroups_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::nCutGroups );
            
            AtomForces_exposer.def( 
                "nCutGroups"
                , nCutGroups_function_value );
        
        }
        AtomForces_exposer.def( bp::self != bp::self );
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::operator=
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*assign_function_type )( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const & ) ;
            assign_function_type assign_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::operator= );
            
            AtomForces_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        AtomForces_exposer.def( bp::self == bp::self );
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::operator[]
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*__getitem___function_type )( ::SireMol::CGIdx ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::operator[] );
            
            AtomForces_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::operator[]
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > const & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*__getitem___function_type )( ::SireMol::CGAtomIdx const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::operator[] );
            
            AtomForces_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::set
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*set_function_type )( ::SireMol::CGAtomIdx const &,::SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::set );
            
            AtomForces_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgatomidx"), bp::arg("value") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::set
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > & ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*set_function_type )( ::SireMol::CGIdx,::QVector< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::set );
            
            AtomForces_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgidx"), bp::arg("values") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::size
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::size );
            
            AtomForces_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::toString
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::QString ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::toString );
            
            AtomForces_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::toVariant
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*toVariant_function_type )(  ) const;
            toVariant_function_type toVariant_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::toVariant );
            
            AtomForces_exposer.def( 
                "toVariant"
                , toVariant_function_value );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::toVector
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::QVector< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*toVector_function_type )(  ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::toVector );
            
            AtomForces_exposer.def( 
                "toVector"
                , toVector_function_value );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::toVector
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef ::QVector< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > ( ::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::*toVector_function_type )( ::SireMol::AtomSelection const & ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::toVector );
            
            AtomForces_exposer.def( 
                "toVector"
                , toVector_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::typeName
        
            typedef SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::AtomProperty< SireMaths::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -0x000000002, 0, 0, 0, 0 > > >::typeName );
            
            AtomForces_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        AtomForces_exposer.staticmethod( "fromVariant" );
        AtomForces_exposer.staticmethod( "typeName" );
        AtomForces_exposer.def( "__copy__", &__copy__);
        AtomForces_exposer.def( "__deepcopy__", &__copy__);
        AtomForces_exposer.def( "clone", &__copy__);
        AtomForces_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::AtomProperty<SireMaths::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > > >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomForces_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::AtomProperty<SireMaths::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > > >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomForces_exposer.def( "__str__", &__str__< ::SireMol::AtomProperty<SireMaths::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > > > );
        AtomForces_exposer.def( "__repr__", &__str__< ::SireMol::AtomProperty<SireMaths::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > > > );
    }

}
