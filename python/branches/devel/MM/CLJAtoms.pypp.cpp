// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "CLJAtoms.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireID/index.h"

#include "SireMol/atom.h"

#include "SireMol/atomcharges.h"

#include "SireMol/atomcoords.h"

#include "SireMol/molecule.h"

#include "SireMol/moleculegroup.h"

#include "SireMol/molecules.h"

#include "SireMol/molidx.h"

#include "SireMol/partialmolecule.h"

#include "SireMol/selector.hpp"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"

#include "atomljs.h"

#include "cljatoms.h"

#include <QElapsedTimer>

#include "cljatoms.h"

SireMM::CLJAtoms __copy__(const SireMM::CLJAtoms &other){ return SireMM::CLJAtoms(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_CLJAtoms_class(){

    { //::SireMM::CLJAtoms
        typedef bp::class_< SireMM::CLJAtoms > CLJAtoms_exposer_t;
        CLJAtoms_exposer_t CLJAtoms_exposer = CLJAtoms_exposer_t( "CLJAtoms", bp::init< >() );
        bp::scope CLJAtoms_scope( CLJAtoms_exposer );
        bp::enum_< SireMM::CLJAtoms::ID_SOURCE>("ID_SOURCE")
            .value("USE_MOLNUM", SireMM::CLJAtoms::USE_MOLNUM)
            .value("USE_ATOMIDX", SireMM::CLJAtoms::USE_ATOMIDX)
            .export_values()
            ;
        CLJAtoms_exposer.def( bp::init< QVector< SireMM::CLJAtom > const & >(( bp::arg("atoms") )) );
        CLJAtoms_exposer.def( bp::init< QList< SireMM::CLJAtom > const & >(( bp::arg("atoms") )) );
        CLJAtoms_exposer.def( bp::init< QVector< SireMaths::Vector > const &, QVector< SireUnits::Dimension::PhysUnit< 0, 0, 0, 1, 0, 0, 0 > > const &, QVector< SireMM::LJParameter > const &, bp::optional< qint32 > >(( bp::arg("coordinates"), bp::arg("charges"), bp::arg("ljparams"), bp::arg("atomid")=(int)(1) )) );
        CLJAtoms_exposer.def( bp::init< QVector< SireMaths::Vector > const &, QVector< SireUnits::Dimension::PhysUnit< 0, 0, 0, 1, 0, 0, 0 > > const &, QVector< SireMM::LJParameter > const &, QVector< int > const & >(( bp::arg("coordinates"), bp::arg("charges"), bp::arg("ljparams"), bp::arg("ids") )) );
        CLJAtoms_exposer.def( bp::init< SireMol::MoleculeView const &, bp::optional< SireBase::PropertyMap const & > >(( bp::arg("molecule"), bp::arg("map")=SireBase::PropertyMap() )) );
        CLJAtoms_exposer.def( bp::init< SireMol::MoleculeView const &, SireMM::CLJAtoms::ID_SOURCE, bp::optional< SireBase::PropertyMap const & > >(( bp::arg("molecule"), bp::arg("id_source"), bp::arg("map")=SireBase::PropertyMap() )) );
        CLJAtoms_exposer.def( bp::init< SireMol::Molecules const &, bp::optional< SireBase::PropertyMap const & > >(( bp::arg("molecules"), bp::arg("map")=SireBase::PropertyMap() )) );
        CLJAtoms_exposer.def( bp::init< SireMol::MoleculeGroup const &, bp::optional< SireBase::PropertyMap const & > >(( bp::arg("molecules"), bp::arg("map")=SireBase::PropertyMap() )) );
        CLJAtoms_exposer.def( bp::init< SireMol::Molecules const &, SireMM::CLJAtoms::ID_SOURCE, bp::optional< SireBase::PropertyMap const & > >(( bp::arg("molecules"), bp::arg("id_source"), bp::arg("map")=SireBase::PropertyMap() )) );
        CLJAtoms_exposer.def( bp::init< SireMol::MoleculeGroup const &, SireMM::CLJAtoms::ID_SOURCE, bp::optional< SireBase::PropertyMap const & > >(( bp::arg("molecules"), bp::arg("id_source"), bp::arg("map")=SireBase::PropertyMap() )) );
        CLJAtoms_exposer.def( bp::init< SireMM::CLJAtoms const & >(( bp::arg("other") )) );
        { //::SireMM::CLJAtoms::ID
        
            typedef ::QVector< SireMaths::MultiInt > const & ( ::SireMM::CLJAtoms::*ID_function_type )(  ) const;
            ID_function_type ID_function_value( &::SireMM::CLJAtoms::ID );
            
            CLJAtoms_exposer.def( 
                "ID"
                , ID_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJAtoms::IDs
        
            typedef ::QVector< int > ( ::SireMM::CLJAtoms::*IDs_function_type )(  ) const;
            IDs_function_type IDs_function_value( &::SireMM::CLJAtoms::IDs );
            
            CLJAtoms_exposer.def( 
                "IDs"
                , IDs_function_value );
        
        }
        { //::SireMM::CLJAtoms::at
        
            typedef ::SireMM::CLJAtom ( ::SireMM::CLJAtoms::*at_function_type )( int ) const;
            at_function_type at_function_value( &::SireMM::CLJAtoms::at );
            
            CLJAtoms_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::CLJAtoms::atoms
        
            typedef ::QVector< SireMM::CLJAtom > ( ::SireMM::CLJAtoms::*atoms_function_type )(  ) const;
            atoms_function_type atoms_function_value( &::SireMM::CLJAtoms::atoms );
            
            CLJAtoms_exposer.def( 
                "atoms"
                , atoms_function_value );
        
        }
        { //::SireMM::CLJAtoms::charges
        
            typedef ::QVector< SireUnits::Dimension::PhysUnit< 0, 0, 0, 1, 0, 0, 0 > > ( ::SireMM::CLJAtoms::*charges_function_type )(  ) const;
            charges_function_type charges_function_value( &::SireMM::CLJAtoms::charges );
            
            CLJAtoms_exposer.def( 
                "charges"
                , charges_function_value );
        
        }
        { //::SireMM::CLJAtoms::coordinates
        
            typedef ::QVector< SireMaths::Vector > ( ::SireMM::CLJAtoms::*coordinates_function_type )(  ) const;
            coordinates_function_type coordinates_function_value( &::SireMM::CLJAtoms::coordinates );
            
            CLJAtoms_exposer.def( 
                "coordinates"
                , coordinates_function_value );
        
        }
        { //::SireMM::CLJAtoms::count
        
            typedef int ( ::SireMM::CLJAtoms::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMM::CLJAtoms::count );
            
            CLJAtoms_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMM::CLJAtoms::epsilon
        
            typedef ::QVector< SireMaths::MultiFloat > const & ( ::SireMM::CLJAtoms::*epsilon_function_type )(  ) const;
            epsilon_function_type epsilon_function_value( &::SireMM::CLJAtoms::epsilon );
            
            CLJAtoms_exposer.def( 
                "epsilon"
                , epsilon_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJAtoms::getitem
        
            typedef ::SireMM::CLJAtom ( ::SireMM::CLJAtoms::*getitem_function_type )( int ) const;
            getitem_function_type getitem_function_value( &::SireMM::CLJAtoms::getitem );
            
            CLJAtoms_exposer.def( 
                "getitem"
                , getitem_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::CLJAtoms::idOfDummy
        
            typedef ::SireMaths::MultiInt ( *idOfDummy_function_type )(  );
            idOfDummy_function_type idOfDummy_function_value( &::SireMM::CLJAtoms::idOfDummy );
            
            CLJAtoms_exposer.def( 
                "idOfDummy"
                , idOfDummy_function_value );
        
        }
        { //::SireMM::CLJAtoms::isDummy
        
            typedef bool ( ::SireMM::CLJAtoms::*isDummy_function_type )( int ) ;
            isDummy_function_type isDummy_function_value( &::SireMM::CLJAtoms::isDummy );
            
            CLJAtoms_exposer.def( 
                "isDummy"
                , isDummy_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::CLJAtoms::isEmpty
        
            typedef bool ( ::SireMM::CLJAtoms::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMM::CLJAtoms::isEmpty );
            
            CLJAtoms_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMM::CLJAtoms::ljParameters
        
            typedef ::QVector< SireMM::LJParameter > ( ::SireMM::CLJAtoms::*ljParameters_function_type )(  ) const;
            ljParameters_function_type ljParameters_function_value( &::SireMM::CLJAtoms::ljParameters );
            
            CLJAtoms_exposer.def( 
                "ljParameters"
                , ljParameters_function_value );
        
        }
        { //::SireMM::CLJAtoms::makeDummy
        
            typedef void ( ::SireMM::CLJAtoms::*makeDummy_function_type )( int ) ;
            makeDummy_function_type makeDummy_function_value( &::SireMM::CLJAtoms::makeDummy );
            
            CLJAtoms_exposer.def( 
                "makeDummy"
                , makeDummy_function_value
                , ( bp::arg("i") ) );
        
        }
        CLJAtoms_exposer.def( bp::self != bp::self );
        CLJAtoms_exposer.def( bp::self + bp::self );
        CLJAtoms_exposer.def( bp::self + bp::other< SireMM::CLJAtom >() );
        CLJAtoms_exposer.def( bp::self + bp::other< QVector< SireMM::CLJAtom > >() );
        { //::SireMM::CLJAtoms::operator=
        
            typedef ::SireMM::CLJAtoms & ( ::SireMM::CLJAtoms::*assign_function_type )( ::SireMM::CLJAtoms const & ) ;
            assign_function_type assign_function_value( &::SireMM::CLJAtoms::operator= );
            
            CLJAtoms_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        CLJAtoms_exposer.def( bp::self == bp::self );
        { //::SireMM::CLJAtoms::operator[]
        
            typedef ::SireMM::CLJAtom ( ::SireMM::CLJAtoms::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireMM::CLJAtoms::operator[] );
            
            CLJAtoms_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::CLJAtoms::q
        
            typedef ::QVector< SireMaths::MultiFloat > const & ( ::SireMM::CLJAtoms::*q_function_type )(  ) const;
            q_function_type q_function_value( &::SireMM::CLJAtoms::q );
            
            CLJAtoms_exposer.def( 
                "q"
                , q_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJAtoms::set
        
            typedef void ( ::SireMM::CLJAtoms::*set_function_type )( int,::SireMM::CLJAtom const & ) ;
            set_function_type set_function_value( &::SireMM::CLJAtoms::set );
            
            CLJAtoms_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("i"), bp::arg("atom") ) );
        
        }
        { //::SireMM::CLJAtoms::setAllID
        
            typedef void ( ::SireMM::CLJAtoms::*setAllID_function_type )( ::qint32 ) ;
            setAllID_function_type setAllID_function_value( &::SireMM::CLJAtoms::setAllID );
            
            CLJAtoms_exposer.def( 
                "setAllID"
                , setAllID_function_value
                , ( bp::arg("idnum") ) );
        
        }
        { //::SireMM::CLJAtoms::setCharge
        
            typedef void ( ::SireMM::CLJAtoms::*setCharge_function_type )( int,::SireUnits::Dimension::Charge ) ;
            setCharge_function_type setCharge_function_value( &::SireMM::CLJAtoms::setCharge );
            
            CLJAtoms_exposer.def( 
                "setCharge"
                , setCharge_function_value
                , ( bp::arg("i"), bp::arg("charge") ) );
        
        }
        { //::SireMM::CLJAtoms::setCoordinates
        
            typedef void ( ::SireMM::CLJAtoms::*setCoordinates_function_type )( int,::SireMaths::Vector ) ;
            setCoordinates_function_type setCoordinates_function_value( &::SireMM::CLJAtoms::setCoordinates );
            
            CLJAtoms_exposer.def( 
                "setCoordinates"
                , setCoordinates_function_value
                , ( bp::arg("i"), bp::arg("coords") ) );
        
        }
        { //::SireMM::CLJAtoms::setID
        
            typedef void ( ::SireMM::CLJAtoms::*setID_function_type )( int,::qint32 ) ;
            setID_function_type setID_function_value( &::SireMM::CLJAtoms::setID );
            
            CLJAtoms_exposer.def( 
                "setID"
                , setID_function_value
                , ( bp::arg("i"), bp::arg("idnum") ) );
        
        }
        { //::SireMM::CLJAtoms::setLJParameter
        
            typedef void ( ::SireMM::CLJAtoms::*setLJParameter_function_type )( int,::SireMM::LJParameter ) ;
            setLJParameter_function_type setLJParameter_function_value( &::SireMM::CLJAtoms::setLJParameter );
            
            CLJAtoms_exposer.def( 
                "setLJParameter"
                , setLJParameter_function_value
                , ( bp::arg("i"), bp::arg("ljparam") ) );
        
        }
        { //::SireMM::CLJAtoms::sigma
        
            typedef ::QVector< SireMaths::MultiFloat > const & ( ::SireMM::CLJAtoms::*sigma_function_type )(  ) const;
            sigma_function_type sigma_function_value( &::SireMM::CLJAtoms::sigma );
            
            CLJAtoms_exposer.def( 
                "sigma"
                , sigma_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJAtoms::size
        
            typedef int ( ::SireMM::CLJAtoms::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMM::CLJAtoms::size );
            
            CLJAtoms_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMM::CLJAtoms::squeeze
        
            typedef ::SireMM::CLJAtoms ( ::SireMM::CLJAtoms::*squeeze_function_type )(  ) const;
            squeeze_function_type squeeze_function_value( &::SireMM::CLJAtoms::squeeze );
            
            CLJAtoms_exposer.def( 
                "squeeze"
                , squeeze_function_value );
        
        }
        { //::SireMM::CLJAtoms::toString
        
            typedef ::QString ( ::SireMM::CLJAtoms::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMM::CLJAtoms::toString );
            
            CLJAtoms_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMM::CLJAtoms::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::CLJAtoms::typeName );
            
            CLJAtoms_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::CLJAtoms::what
        
            typedef char const * ( ::SireMM::CLJAtoms::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::CLJAtoms::what );
            
            CLJAtoms_exposer.def( 
                "what"
                , what_function_value );
        
        }
        { //::SireMM::CLJAtoms::x
        
            typedef ::QVector< SireMaths::MultiFloat > const & ( ::SireMM::CLJAtoms::*x_function_type )(  ) const;
            x_function_type x_function_value( &::SireMM::CLJAtoms::x );
            
            CLJAtoms_exposer.def( 
                "x"
                , x_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJAtoms::y
        
            typedef ::QVector< SireMaths::MultiFloat > const & ( ::SireMM::CLJAtoms::*y_function_type )(  ) const;
            y_function_type y_function_value( &::SireMM::CLJAtoms::y );
            
            CLJAtoms_exposer.def( 
                "y"
                , y_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJAtoms::z
        
            typedef ::QVector< SireMaths::MultiFloat > const & ( ::SireMM::CLJAtoms::*z_function_type )(  ) const;
            z_function_type z_function_value( &::SireMM::CLJAtoms::z );
            
            CLJAtoms_exposer.def( 
                "z"
                , z_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        CLJAtoms_exposer.staticmethod( "idOfDummy" );
        CLJAtoms_exposer.staticmethod( "typeName" );
        CLJAtoms_exposer.def( "__copy__", &__copy__);
        CLJAtoms_exposer.def( "__deepcopy__", &__copy__);
        CLJAtoms_exposer.def( "clone", &__copy__);
        CLJAtoms_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::CLJAtoms >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJAtoms_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::CLJAtoms >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJAtoms_exposer.def( "__str__", &__str__< ::SireMM::CLJAtoms > );
        CLJAtoms_exposer.def( "__repr__", &__str__< ::SireMM::CLJAtoms > );
        CLJAtoms_exposer.def( "__len__", &__len_size< ::SireMM::CLJAtoms > );
        CLJAtoms_exposer.def( "__getitem__", &::SireMM::CLJAtoms::getitem );
    }

}
