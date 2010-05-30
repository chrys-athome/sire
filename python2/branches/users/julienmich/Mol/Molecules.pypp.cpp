// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "Molecules.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "atom.h"

#include "atomselection.h"

#include "chain.h"

#include "cutgroup.h"

#include "editor.hpp"

#include "molecule.h"

#include "molecules.h"

#include "mover.hpp"

#include "partialmolecule.h"

#include "residue.h"

#include "segment.h"

#include "selector.hpp"

#include "tostring.h"

#include "viewsofmol.h"

#include <QDebug>

#include <boost/tuple/tuple.hpp>

#include "molecules.h"

SireMol::Molecules __copy__(const SireMol::Molecules &other){ return SireMol::Molecules(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Molecules_class(){

    { //::SireMol::Molecules
        typedef bp::class_< SireMol::Molecules, bp::bases< SireBase::Property > > Molecules_exposer_t;
        Molecules_exposer_t Molecules_exposer = Molecules_exposer_t( "Molecules", bp::init< >() );
        bp::scope Molecules_scope( Molecules_exposer );
        Molecules_exposer.def( bp::init< SireMol::MoleculeView const & >(( bp::arg("molecule") )) );
        Molecules_exposer.def( bp::init< SireMol::ViewsOfMol const & >(( bp::arg("molviews") )) );
        Molecules_exposer.def( bp::init< SireMol::Molecules const & >(( bp::arg("other") )) );
        { //::SireMol::Molecules::add
        
            typedef void ( ::SireMol::Molecules::*add_function_type )( ::SireMol::MoleculeView const & ) ;
            add_function_type add_function_value( &::SireMol::Molecules::add );
            
            Molecules_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molview") ) );
        
        }
        { //::SireMol::Molecules::add
        
            typedef void ( ::SireMol::Molecules::*add_function_type )( ::SireMol::ViewsOfMol const & ) ;
            add_function_type add_function_value( &::SireMol::Molecules::add );
            
            Molecules_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molviews") ) );
        
        }
        { //::SireMol::Molecules::add
        
            typedef void ( ::SireMol::Molecules::*add_function_type )( ::SireMol::Molecules const & ) ;
            add_function_type add_function_value( &::SireMol::Molecules::add );
            
            Molecules_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMol::Molecules::addIfUnique
        
            typedef bool ( ::SireMol::Molecules::*addIfUnique_function_type )( ::SireMol::MoleculeView const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireMol::Molecules::addIfUnique );
            
            Molecules_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molview") ) );
        
        }
        { //::SireMol::Molecules::addIfUnique
        
            typedef ::SireMol::ViewsOfMol ( ::SireMol::Molecules::*addIfUnique_function_type )( ::SireMol::ViewsOfMol const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireMol::Molecules::addIfUnique );
            
            Molecules_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molviews") ) );
        
        }
        { //::SireMol::Molecules::addIfUnique
        
            typedef ::QList< SireMol::ViewsOfMol > ( ::SireMol::Molecules::*addIfUnique_function_type )( ::SireMol::Molecules const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireMol::Molecules::addIfUnique );
            
            Molecules_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMol::Molecules::assertContains
        
            typedef void ( ::SireMol::Molecules::*assertContains_function_type )( ::SireMol::MolNum ) const;
            assertContains_function_type assertContains_function_value( &::SireMol::Molecules::assertContains );
            
            Molecules_exposer.def( 
                "assertContains"
                , assertContains_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireMol::Molecules::at
        
            typedef ::SireMol::ViewsOfMol const & ( ::SireMol::Molecules::*at_function_type )( ::SireMol::MolNum ) const;
            at_function_type at_function_value( &::SireMol::Molecules::at );
            
            Molecules_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("molnum") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecules::at
        
            typedef ::SireMol::PartialMolecule ( ::SireMol::Molecules::*at_function_type )( ::boost::tuples::tuple< SireMol::MolNum, SireID::Index, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > const & ) const;
            at_function_type at_function_value( &::SireMol::Molecules::at );
            
            Molecules_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("viewidx") ) );
        
        }
        { //::SireMol::Molecules::at
        
            typedef ::SireMol::PartialMolecule ( ::SireMol::Molecules::*at_function_type )( ::SireMol::MolNum,int ) const;
            at_function_type at_function_value( &::SireMol::Molecules::at );
            
            Molecules_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("molnum"), bp::arg("idx") ) );
        
        }
        { //::SireMol::Molecules::back
        
            typedef ::SireMol::ViewsOfMol const & ( ::SireMol::Molecules::*back_function_type )(  ) const;
            back_function_type back_function_value( &::SireMol::Molecules::back );
            
            Molecules_exposer.def( 
                "back"
                , back_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecules::begin
        
            typedef ::SireBase::ChunkedHash< SireMol::MolNum, SireMol::ViewsOfMol, 100 >::const_iterator ( ::SireMol::Molecules::*begin_function_type )(  ) const;
            begin_function_type begin_function_value( &::SireMol::Molecules::begin );
            
            Molecules_exposer.def( 
                "begin"
                , begin_function_value );
        
        }
        { //::SireMol::Molecules::clear
        
            typedef void ( ::SireMol::Molecules::*clear_function_type )(  ) ;
            clear_function_type clear_function_value( &::SireMol::Molecules::clear );
            
            Molecules_exposer.def( 
                "clear"
                , clear_function_value );
        
        }
        { //::SireMol::Molecules::constBegin
        
            typedef ::SireBase::ChunkedHash< SireMol::MolNum, SireMol::ViewsOfMol, 100 >::const_iterator ( ::SireMol::Molecules::*constBegin_function_type )(  ) const;
            constBegin_function_type constBegin_function_value( &::SireMol::Molecules::constBegin );
            
            Molecules_exposer.def( 
                "constBegin"
                , constBegin_function_value );
        
        }
        { //::SireMol::Molecules::constEnd
        
            typedef ::SireBase::ChunkedHash< SireMol::MolNum, SireMol::ViewsOfMol, 100 >::const_iterator ( ::SireMol::Molecules::*constEnd_function_type )(  ) const;
            constEnd_function_type constEnd_function_value( &::SireMol::Molecules::constEnd );
            
            Molecules_exposer.def( 
                "constEnd"
                , constEnd_function_value );
        
        }
        { //::SireMol::Molecules::constFind
        
            typedef ::SireBase::ChunkedHash< SireMol::MolNum, SireMol::ViewsOfMol, 100 >::const_iterator ( ::SireMol::Molecules::*constFind_function_type )( ::SireMol::MolNum ) const;
            constFind_function_type constFind_function_value( &::SireMol::Molecules::constFind );
            
            Molecules_exposer.def( 
                "constFind"
                , constFind_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireMol::Molecules::contains
        
            typedef bool ( ::SireMol::Molecules::*contains_function_type )( ::SireMol::MolNum ) const;
            contains_function_type contains_function_value( &::SireMol::Molecules::contains );
            
            Molecules_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireMol::Molecules::contains
        
            typedef bool ( ::SireMol::Molecules::*contains_function_type )( ::SireMol::MoleculeView const & ) const;
            contains_function_type contains_function_value( &::SireMol::Molecules::contains );
            
            Molecules_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("molview") ) );
        
        }
        { //::SireMol::Molecules::contains
        
            typedef bool ( ::SireMol::Molecules::*contains_function_type )( ::SireMol::ViewsOfMol const & ) const;
            contains_function_type contains_function_value( &::SireMol::Molecules::contains );
            
            Molecules_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("molviews") ) );
        
        }
        { //::SireMol::Molecules::contains
        
            typedef bool ( ::SireMol::Molecules::*contains_function_type )( ::SireMol::Molecules const & ) const;
            contains_function_type contains_function_value( &::SireMol::Molecules::contains );
            
            Molecules_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMol::Molecules::count
        
            typedef int ( ::SireMol::Molecules::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMol::Molecules::count );
            
            Molecules_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMol::Molecules::end
        
            typedef ::SireBase::ChunkedHash< SireMol::MolNum, SireMol::ViewsOfMol, 100 >::const_iterator ( ::SireMol::Molecules::*end_function_type )(  ) const;
            end_function_type end_function_value( &::SireMol::Molecules::end );
            
            Molecules_exposer.def( 
                "end"
                , end_function_value );
        
        }
        { //::SireMol::Molecules::find
        
            typedef ::SireBase::ChunkedHash< SireMol::MolNum, SireMol::ViewsOfMol, 100 >::const_iterator ( ::SireMol::Molecules::*find_function_type )( ::SireMol::MolNum ) const;
            find_function_type find_function_value( &::SireMol::Molecules::find );
            
            Molecules_exposer.def( 
                "find"
                , find_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireMol::Molecules::first
        
            typedef ::SireMol::ViewsOfMol const & ( ::SireMol::Molecules::*first_function_type )(  ) const;
            first_function_type first_function_value( &::SireMol::Molecules::first );
            
            Molecules_exposer.def( 
                "first"
                , first_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecules::front
        
            typedef ::SireMol::ViewsOfMol const & ( ::SireMol::Molecules::*front_function_type )(  ) const;
            front_function_type front_function_value( &::SireMol::Molecules::front );
            
            Molecules_exposer.def( 
                "front"
                , front_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecules::intersects
        
            typedef bool ( ::SireMol::Molecules::*intersects_function_type )( ::SireMol::MoleculeView const & ) const;
            intersects_function_type intersects_function_value( &::SireMol::Molecules::intersects );
            
            Molecules_exposer.def( 
                "intersects"
                , intersects_function_value
                , ( bp::arg("molview") ) );
        
        }
        { //::SireMol::Molecules::intersects
        
            typedef bool ( ::SireMol::Molecules::*intersects_function_type )( ::SireMol::Molecules const & ) const;
            intersects_function_type intersects_function_value( &::SireMol::Molecules::intersects );
            
            Molecules_exposer.def( 
                "intersects"
                , intersects_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMol::Molecules::isEmpty
        
            typedef bool ( ::SireMol::Molecules::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::Molecules::isEmpty );
            
            Molecules_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::Molecules::last
        
            typedef ::SireMol::ViewsOfMol const & ( ::SireMol::Molecules::*last_function_type )(  ) const;
            last_function_type last_function_value( &::SireMol::Molecules::last );
            
            Molecules_exposer.def( 
                "last"
                , last_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecules::molNums
        
            typedef ::QSet< SireMol::MolNum > ( ::SireMol::Molecules::*molNums_function_type )(  ) const;
            molNums_function_type molNums_function_value( &::SireMol::Molecules::molNums );
            
            Molecules_exposer.def( 
                "molNums"
                , molNums_function_value );
        
        }
        { //::SireMol::Molecules::molecule
        
            typedef ::SireMol::ViewsOfMol const & ( ::SireMol::Molecules::*molecule_function_type )( ::SireMol::MolNum ) const;
            molecule_function_type molecule_function_value( &::SireMol::Molecules::molecule );
            
            Molecules_exposer.def( 
                "molecule"
                , molecule_function_value
                , ( bp::arg("molnum") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecules::nMolecules
        
            typedef int ( ::SireMol::Molecules::*nMolecules_function_type )(  ) const;
            nMolecules_function_type nMolecules_function_value( &::SireMol::Molecules::nMolecules );
            
            Molecules_exposer.def( 
                "nMolecules"
                , nMolecules_function_value );
        
        }
        { //::SireMol::Molecules::nViews
        
            typedef int ( ::SireMol::Molecules::*nViews_function_type )(  ) const;
            nViews_function_type nViews_function_value( &::SireMol::Molecules::nViews );
            
            Molecules_exposer.def( 
                "nViews"
                , nViews_function_value );
        
        }
        { //::SireMol::Molecules::nViews
        
            typedef int ( ::SireMol::Molecules::*nViews_function_type )( ::SireMol::MolNum ) const;
            nViews_function_type nViews_function_value( &::SireMol::Molecules::nViews );
            
            Molecules_exposer.def( 
                "nViews"
                , nViews_function_value
                , ( bp::arg("molnum") ) );
        
        }
        Molecules_exposer.def( bp::self != bp::self );
        Molecules_exposer.def( bp::self + bp::self );
        Molecules_exposer.def( bp::self - bp::self );
        { //::SireMol::Molecules::operator=
        
            typedef ::SireMol::Molecules & ( ::SireMol::Molecules::*assign_function_type )( ::SireMol::Molecules const & ) ;
            assign_function_type assign_function_value( &::SireMol::Molecules::operator= );
            
            Molecules_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Molecules_exposer.def( bp::self == bp::self );
        { //::SireMol::Molecules::operator[]
        
            typedef ::SireMol::ViewsOfMol const & ( ::SireMol::Molecules::*__getitem___function_type )( ::SireMol::MolNum ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::Molecules::operator[] );
            
            Molecules_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("molnum") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecules::operator[]
        
            typedef ::SireMol::PartialMolecule ( ::SireMol::Molecules::*__getitem___function_type )( ::boost::tuples::tuple< SireMol::MolNum, SireID::Index, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::Molecules::operator[] );
            
            Molecules_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("viewidx") ) );
        
        }
        { //::SireMol::Molecules::remove
        
            typedef bool ( ::SireMol::Molecules::*remove_function_type )( ::SireMol::MoleculeView const & ) ;
            remove_function_type remove_function_value( &::SireMol::Molecules::remove );
            
            Molecules_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molview") ) );
        
        }
        { //::SireMol::Molecules::remove
        
            typedef ::SireMol::ViewsOfMol ( ::SireMol::Molecules::*remove_function_type )( ::SireMol::ViewsOfMol const & ) ;
            remove_function_type remove_function_value( &::SireMol::Molecules::remove );
            
            Molecules_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molviews") ) );
        
        }
        { //::SireMol::Molecules::remove
        
            typedef ::QList< SireMol::ViewsOfMol > ( ::SireMol::Molecules::*remove_function_type )( ::SireMol::Molecules const & ) ;
            remove_function_type remove_function_value( &::SireMol::Molecules::remove );
            
            Molecules_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMol::Molecules::remove
        
            typedef ::SireMol::ViewsOfMol ( ::SireMol::Molecules::*remove_function_type )( ::SireMol::MolNum ) ;
            remove_function_type remove_function_value( &::SireMol::Molecules::remove );
            
            Molecules_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireMol::Molecules::removeAll
        
            typedef bool ( ::SireMol::Molecules::*removeAll_function_type )( ::SireMol::MoleculeView const & ) ;
            removeAll_function_type removeAll_function_value( &::SireMol::Molecules::removeAll );
            
            Molecules_exposer.def( 
                "removeAll"
                , removeAll_function_value
                , ( bp::arg("molview") ) );
        
        }
        { //::SireMol::Molecules::removeAll
        
            typedef ::SireMol::ViewsOfMol ( ::SireMol::Molecules::*removeAll_function_type )( ::SireMol::ViewsOfMol const & ) ;
            removeAll_function_type removeAll_function_value( &::SireMol::Molecules::removeAll );
            
            Molecules_exposer.def( 
                "removeAll"
                , removeAll_function_value
                , ( bp::arg("molviews") ) );
        
        }
        { //::SireMol::Molecules::removeAll
        
            typedef ::QList< SireMol::ViewsOfMol > ( ::SireMol::Molecules::*removeAll_function_type )( ::SireMol::Molecules const & ) ;
            removeAll_function_type removeAll_function_value( &::SireMol::Molecules::removeAll );
            
            Molecules_exposer.def( 
                "removeAll"
                , removeAll_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMol::Molecules::removeAll
        
            typedef bool ( ::SireMol::Molecules::*removeAll_function_type )(  ) ;
            removeAll_function_type removeAll_function_value( &::SireMol::Molecules::removeAll );
            
            Molecules_exposer.def( 
                "removeAll"
                , removeAll_function_value );
        
        }
        { //::SireMol::Molecules::removeDuplicates
        
            typedef bool ( ::SireMol::Molecules::*removeDuplicates_function_type )(  ) ;
            removeDuplicates_function_type removeDuplicates_function_value( &::SireMol::Molecules::removeDuplicates );
            
            Molecules_exposer.def( 
                "removeDuplicates"
                , removeDuplicates_function_value );
        
        }
        { //::SireMol::Molecules::toString
        
            typedef ::QString ( ::SireMol::Molecules::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::Molecules::toString );
            
            Molecules_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::Molecules::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::Molecules::typeName );
            
            Molecules_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::Molecules::unite
        
            typedef bool ( ::SireMol::Molecules::*unite_function_type )( ::SireMol::MoleculeView const & ) ;
            unite_function_type unite_function_value( &::SireMol::Molecules::unite );
            
            Molecules_exposer.def( 
                "unite"
                , unite_function_value
                , ( bp::arg("molview") ) );
        
        }
        { //::SireMol::Molecules::unite
        
            typedef ::SireMol::ViewsOfMol ( ::SireMol::Molecules::*unite_function_type )( ::SireMol::ViewsOfMol const & ) ;
            unite_function_type unite_function_value( &::SireMol::Molecules::unite );
            
            Molecules_exposer.def( 
                "unite"
                , unite_function_value
                , ( bp::arg("molviews") ) );
        
        }
        { //::SireMol::Molecules::unite
        
            typedef ::QList< SireMol::ViewsOfMol > ( ::SireMol::Molecules::*unite_function_type )( ::SireMol::Molecules const & ) ;
            unite_function_type unite_function_value( &::SireMol::Molecules::unite );
            
            Molecules_exposer.def( 
                "unite"
                , unite_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMol::Molecules::uniteViews
        
            typedef bool ( ::SireMol::Molecules::*uniteViews_function_type )(  ) ;
            uniteViews_function_type uniteViews_function_value( &::SireMol::Molecules::uniteViews );
            
            Molecules_exposer.def( 
                "uniteViews"
                , uniteViews_function_value );
        
        }
        { //::SireMol::Molecules::update
        
            typedef bool ( ::SireMol::Molecules::*update_function_type )( ::SireMol::MoleculeData const & ) ;
            update_function_type update_function_value( &::SireMol::Molecules::update );
            
            Molecules_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("moldata") ) );
        
        }
        { //::SireMol::Molecules::update
        
            typedef bool ( ::SireMol::Molecules::*update_function_type )( ::SireMol::MoleculeView const & ) ;
            update_function_type update_function_value( &::SireMol::Molecules::update );
            
            Molecules_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("molview") ) );
        
        }
        { //::SireMol::Molecules::update
        
            typedef ::QList< SireMol::Molecule > ( ::SireMol::Molecules::*update_function_type )( ::SireMol::Molecules const & ) ;
            update_function_type update_function_value( &::SireMol::Molecules::update );
            
            Molecules_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMol::Molecules::what
        
            typedef char const * ( ::SireMol::Molecules::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMol::Molecules::what );
            
            Molecules_exposer.def( 
                "what"
                , what_function_value );
        
        }
        Molecules_exposer.staticmethod( "typeName" );
        Molecules_exposer.def( "__copy__", &__copy__);
        Molecules_exposer.def( "__deepcopy__", &__copy__);
        Molecules_exposer.def( "clone", &__copy__);
        Molecules_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::Molecules >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Molecules_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::Molecules >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Molecules_exposer.def( "__str__", &__str__< ::SireMol::Molecules > );
        Molecules_exposer.def( "__repr__", &__str__< ::SireMol::Molecules > );
    }

}
