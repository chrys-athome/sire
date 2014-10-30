// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "FF.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireFF/errors.h"

#include "SireMol/mgidx.h"

#include "SireMol/mgname.h"

#include "SireMol/mgnum.h"

#include "SireMol/molecule.h"

#include "SireMol/moleculegroup.h"

#include "SireMol/molecules.h"

#include "SireMol/mover.hpp"

#include "SireMol/partialmolecule.h"

#include "SireMol/viewsofmol.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "ff.h"

#include "ffcomponent.h"

#include "forcefield.h"

#include "tostring.h"

#include <QDebug>

#include <QElapsedTimer>

#include "ff.h"

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_FF_class(){

    { //::SireFF::FF
        typedef bp::class_< SireFF::FF, bp::bases< SireMol::MolGroupsBase, SireBase::Property >, boost::noncopyable > FF_exposer_t;
        FF_exposer_t FF_exposer = FF_exposer_t( "FF", bp::no_init );
        bp::scope FF_scope( FF_exposer );
        { //::SireFF::FF::UID
        
            typedef ::QUuid const & ( ::SireFF::FF::*UID_function_type )(  ) const;
            UID_function_type UID_function_value( &::SireFF::FF::UID );
            
            FF_exposer.def( 
                "UID"
                , UID_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireFF::FF::add
        
            typedef void ( ::SireFF::FF::*add_function_type )( ::SireMol::MoleculeView const &,::SireMol::MGID const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireFF::FF::add );
            
            FF_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molview"), bp::arg("mgid"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::add
        
            typedef void ( ::SireFF::FF::*add_function_type )( ::SireMol::ViewsOfMol const &,::SireMol::MGID const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireFF::FF::add );
            
            FF_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molviews"), bp::arg("mgid"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::add
        
            typedef void ( ::SireFF::FF::*add_function_type )( ::SireMol::Molecules const &,::SireMol::MGID const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireFF::FF::add );
            
            FF_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecules"), bp::arg("mgid"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::add
        
            typedef void ( ::SireFF::FF::*add_function_type )( ::SireMol::MoleculeGroup const &,::SireMol::MGID const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireFF::FF::add );
            
            FF_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molgroup"), bp::arg("mgid"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::add
        
            typedef void ( ::SireFF::FF::*add_function_type )( ::SireMol::MoleculeView const &,::SireMol::MGID const & ) ;
            add_function_type add_function_value( &::SireFF::FF::add );
            
            FF_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molview"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::add
        
            typedef void ( ::SireFF::FF::*add_function_type )( ::SireMol::ViewsOfMol const &,::SireMol::MGID const & ) ;
            add_function_type add_function_value( &::SireFF::FF::add );
            
            FF_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molviews"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::add
        
            typedef void ( ::SireFF::FF::*add_function_type )( ::SireMol::Molecules const &,::SireMol::MGID const & ) ;
            add_function_type add_function_value( &::SireFF::FF::add );
            
            FF_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecules"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::add
        
            typedef void ( ::SireFF::FF::*add_function_type )( ::SireMol::MoleculeGroup const &,::SireMol::MGID const & ) ;
            add_function_type add_function_value( &::SireFF::FF::add );
            
            FF_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molgroup"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::addIfUnique
        
            typedef void ( ::SireFF::FF::*addIfUnique_function_type )( ::SireMol::MoleculeView const &,::SireMol::MGID const &,::SireBase::PropertyMap const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireFF::FF::addIfUnique );
            
            FF_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molview"), bp::arg("mgid"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::addIfUnique
        
            typedef void ( ::SireFF::FF::*addIfUnique_function_type )( ::SireMol::ViewsOfMol const &,::SireMol::MGID const &,::SireBase::PropertyMap const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireFF::FF::addIfUnique );
            
            FF_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molviews"), bp::arg("mgid"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::addIfUnique
        
            typedef void ( ::SireFF::FF::*addIfUnique_function_type )( ::SireMol::Molecules const &,::SireMol::MGID const &,::SireBase::PropertyMap const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireFF::FF::addIfUnique );
            
            FF_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molecules"), bp::arg("mgid"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::addIfUnique
        
            typedef void ( ::SireFF::FF::*addIfUnique_function_type )( ::SireMol::MoleculeGroup const &,::SireMol::MGID const &,::SireBase::PropertyMap const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireFF::FF::addIfUnique );
            
            FF_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molgroup"), bp::arg("mgid"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::addIfUnique
        
            typedef void ( ::SireFF::FF::*addIfUnique_function_type )( ::SireMol::MoleculeView const &,::SireMol::MGID const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireFF::FF::addIfUnique );
            
            FF_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molview"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::addIfUnique
        
            typedef void ( ::SireFF::FF::*addIfUnique_function_type )( ::SireMol::ViewsOfMol const &,::SireMol::MGID const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireFF::FF::addIfUnique );
            
            FF_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molviews"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::addIfUnique
        
            typedef void ( ::SireFF::FF::*addIfUnique_function_type )( ::SireMol::Molecules const &,::SireMol::MGID const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireFF::FF::addIfUnique );
            
            FF_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molecules"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::addIfUnique
        
            typedef void ( ::SireFF::FF::*addIfUnique_function_type )( ::SireMol::MoleculeGroup const &,::SireMol::MGID const & ) ;
            addIfUnique_function_type addIfUnique_function_value( &::SireFF::FF::addIfUnique );
            
            FF_exposer.def( 
                "addIfUnique"
                , addIfUnique_function_value
                , ( bp::arg("molgroup"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::components
        
            typedef ::SireFF::FFComponent const & ( ::SireFF::FF::*components_function_type )(  ) const;
            components_function_type components_function_value( &::SireFF::FF::components );
            
            FF_exposer.def( 
                "components"
                , components_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireFF::FF::containsProperty
        
            typedef bool ( ::SireFF::FF::*containsProperty_function_type )( ::QString const & ) const;
            containsProperty_function_type containsProperty_function_value( &::SireFF::FF::containsProperty );
            
            FF_exposer.def( 
                "containsProperty"
                , containsProperty_function_value
                , ( bp::arg("name") ) );
        
        }
        { //::SireFF::FF::energies
        
            typedef ::SireCAS::Values ( ::SireFF::FF::*energies_function_type )( ::QSet< SireCAS::Symbol > const & ) ;
            energies_function_type energies_function_value( &::SireFF::FF::energies );
            
            FF_exposer.def( 
                "energies"
                , energies_function_value
                , ( bp::arg("components") ) );
        
        }
        { //::SireFF::FF::energies
        
            typedef ::SireCAS::Values ( ::SireFF::FF::*energies_function_type )(  ) ;
            energies_function_type energies_function_value( &::SireFF::FF::energies );
            
            FF_exposer.def( 
                "energies"
                , energies_function_value );
        
        }
        { //::SireFF::FF::energy
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireFF::FF::*energy_function_type )(  ) ;
            energy_function_type energy_function_value( &::SireFF::FF::energy );
            
            FF_exposer.def( 
                "energy"
                , energy_function_value );
        
        }
        { //::SireFF::FF::energy
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireFF::FF::*energy_function_type )( ::SireCAS::Symbol const & ) ;
            energy_function_type energy_function_value( &::SireFF::FF::energy );
            
            FF_exposer.def( 
                "energy"
                , energy_function_value
                , ( bp::arg("component") ) );
        
        }
        { //::SireFF::FF::isClean
        
            typedef bool ( ::SireFF::FF::*isClean_function_type )(  ) const;
            isClean_function_type isClean_function_value( &::SireFF::FF::isClean );
            
            FF_exposer.def( 
                "isClean"
                , isClean_function_value );
        
        }
        { //::SireFF::FF::isDirty
        
            typedef bool ( ::SireFF::FF::*isDirty_function_type )(  ) const;
            isDirty_function_type isDirty_function_value( &::SireFF::FF::isDirty );
            
            FF_exposer.def( 
                "isDirty"
                , isDirty_function_value );
        
        }
        { //::SireFF::FF::mustNowRecalculateFromScratch
        
            typedef void ( ::SireFF::FF::*mustNowRecalculateFromScratch_function_type )(  ) ;
            mustNowRecalculateFromScratch_function_type mustNowRecalculateFromScratch_function_value( &::SireFF::FF::mustNowRecalculateFromScratch );
            
            FF_exposer.def( 
                "mustNowRecalculateFromScratch"
                , mustNowRecalculateFromScratch_function_value );
        
        }
        { //::SireFF::FF::name
        
            typedef ::SireFF::FFName const & ( ::SireFF::FF::*name_function_type )(  ) const;
            name_function_type name_function_value( &::SireFF::FF::name );
            
            FF_exposer.def( 
                "name"
                , name_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireFF::FF::null
        
            typedef ::SireFF::NullFF const & ( *null_function_type )(  );
            null_function_type null_function_value( &::SireFF::FF::null );
            
            FF_exposer.def( 
                "null"
                , null_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        FF_exposer.def( bp::self != bp::self );
        FF_exposer.def( bp::self == bp::self );
        { //::SireFF::FF::properties
        
            typedef ::SireBase::Properties const & ( ::SireFF::FF::*properties_function_type )(  ) const;
            properties_function_type properties_function_value( &::SireFF::FF::properties );
            
            FF_exposer.def( 
                "properties"
                , properties_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireFF::FF::property
        
            typedef ::SireBase::Property const & ( ::SireFF::FF::*property_function_type )( ::QString const & ) const;
            property_function_type property_function_value( &::SireFF::FF::property );
            
            FF_exposer.def( 
                "property"
                , property_function_value
                , ( bp::arg("name") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireFF::FF::propertyKeys
        
            typedef ::QStringList ( ::SireFF::FF::*propertyKeys_function_type )(  ) const;
            propertyKeys_function_type propertyKeys_function_value( &::SireFF::FF::propertyKeys );
            
            FF_exposer.def( 
                "propertyKeys"
                , propertyKeys_function_value );
        
        }
        { //::SireFF::FF::remove
        
            typedef bool ( ::SireFF::FF::*remove_function_type )( ::SireMol::MoleculeView const &,::SireMol::MGID const & ) ;
            remove_function_type remove_function_value( &::SireFF::FF::remove );
            
            FF_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molview"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::remove
        
            typedef bool ( ::SireFF::FF::*remove_function_type )( ::SireMol::ViewsOfMol const &,::SireMol::MGID const & ) ;
            remove_function_type remove_function_value( &::SireFF::FF::remove );
            
            FF_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molviews"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::remove
        
            typedef bool ( ::SireFF::FF::*remove_function_type )( ::SireMol::Molecules const &,::SireMol::MGID const & ) ;
            remove_function_type remove_function_value( &::SireFF::FF::remove );
            
            FF_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molecules"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::remove
        
            typedef bool ( ::SireFF::FF::*remove_function_type )( ::SireMol::MoleculeGroup const &,::SireMol::MGID const & ) ;
            remove_function_type remove_function_value( &::SireFF::FF::remove );
            
            FF_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molgroup"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::remove
        
            typedef bool ( ::SireFF::FF::*remove_function_type )( ::SireMol::MolNum,::SireMol::MGID const & ) ;
            remove_function_type remove_function_value( &::SireFF::FF::remove );
            
            FF_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molnum"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::remove
        
            typedef bool ( ::SireFF::FF::*remove_function_type )( ::QSet< SireMol::MolNum > const &,::SireMol::MGID const & ) ;
            remove_function_type remove_function_value( &::SireFF::FF::remove );
            
            FF_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("molnums"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::removeAll
        
            typedef bool ( ::SireFF::FF::*removeAll_function_type )( ::SireMol::MGID const & ) ;
            removeAll_function_type removeAll_function_value( &::SireFF::FF::removeAll );
            
            FF_exposer.def( 
                "removeAll"
                , removeAll_function_value
                , ( bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::removeAll
        
            typedef bool ( ::SireFF::FF::*removeAll_function_type )( ::SireMol::MoleculeView const &,::SireMol::MGID const & ) ;
            removeAll_function_type removeAll_function_value( &::SireFF::FF::removeAll );
            
            FF_exposer.def( 
                "removeAll"
                , removeAll_function_value
                , ( bp::arg("molview"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::removeAll
        
            typedef bool ( ::SireFF::FF::*removeAll_function_type )( ::SireMol::ViewsOfMol const &,::SireMol::MGID const & ) ;
            removeAll_function_type removeAll_function_value( &::SireFF::FF::removeAll );
            
            FF_exposer.def( 
                "removeAll"
                , removeAll_function_value
                , ( bp::arg("molviews"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::removeAll
        
            typedef bool ( ::SireFF::FF::*removeAll_function_type )( ::SireMol::Molecules const &,::SireMol::MGID const & ) ;
            removeAll_function_type removeAll_function_value( &::SireFF::FF::removeAll );
            
            FF_exposer.def( 
                "removeAll"
                , removeAll_function_value
                , ( bp::arg("molecules"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::removeAll
        
            typedef bool ( ::SireFF::FF::*removeAll_function_type )( ::SireMol::MoleculeGroup const &,::SireMol::MGID const & ) ;
            removeAll_function_type removeAll_function_value( &::SireFF::FF::removeAll );
            
            FF_exposer.def( 
                "removeAll"
                , removeAll_function_value
                , ( bp::arg("molgroup"), bp::arg("mgid") ) );
        
        }
        { //::SireFF::FF::setContents
        
            typedef void ( ::SireFF::FF::*setContents_function_type )( ::SireMol::MGID const &,::SireMol::MoleculeView const & ) ;
            setContents_function_type setContents_function_value( &::SireFF::FF::setContents );
            
            FF_exposer.def( 
                "setContents"
                , setContents_function_value
                , ( bp::arg("mgid"), bp::arg("molview") ) );
        
        }
        { //::SireFF::FF::setContents
        
            typedef void ( ::SireFF::FF::*setContents_function_type )( ::SireMol::MGID const &,::SireMol::ViewsOfMol const & ) ;
            setContents_function_type setContents_function_value( &::SireFF::FF::setContents );
            
            FF_exposer.def( 
                "setContents"
                , setContents_function_value
                , ( bp::arg("mgid"), bp::arg("molview") ) );
        
        }
        { //::SireFF::FF::setContents
        
            typedef void ( ::SireFF::FF::*setContents_function_type )( ::SireMol::MGID const &,::SireMol::Molecules const & ) ;
            setContents_function_type setContents_function_value( &::SireFF::FF::setContents );
            
            FF_exposer.def( 
                "setContents"
                , setContents_function_value
                , ( bp::arg("mgid"), bp::arg("molecules") ) );
        
        }
        { //::SireFF::FF::setContents
        
            typedef void ( ::SireFF::FF::*setContents_function_type )( ::SireMol::MGID const &,::SireMol::MoleculeGroup const & ) ;
            setContents_function_type setContents_function_value( &::SireFF::FF::setContents );
            
            FF_exposer.def( 
                "setContents"
                , setContents_function_value
                , ( bp::arg("mgid"), bp::arg("molgroup") ) );
        
        }
        { //::SireFF::FF::setContents
        
            typedef void ( ::SireFF::FF::*setContents_function_type )( ::SireMol::MGID const &,::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            setContents_function_type setContents_function_value( &::SireFF::FF::setContents );
            
            FF_exposer.def( 
                "setContents"
                , setContents_function_value
                , ( bp::arg("mgid"), bp::arg("molview"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::setContents
        
            typedef void ( ::SireFF::FF::*setContents_function_type )( ::SireMol::MGID const &,::SireMol::ViewsOfMol const &,::SireBase::PropertyMap const & ) ;
            setContents_function_type setContents_function_value( &::SireFF::FF::setContents );
            
            FF_exposer.def( 
                "setContents"
                , setContents_function_value
                , ( bp::arg("mgid"), bp::arg("molviews"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::setContents
        
            typedef void ( ::SireFF::FF::*setContents_function_type )( ::SireMol::MGID const &,::SireMol::Molecules const &,::SireBase::PropertyMap const & ) ;
            setContents_function_type setContents_function_value( &::SireFF::FF::setContents );
            
            FF_exposer.def( 
                "setContents"
                , setContents_function_value
                , ( bp::arg("mgid"), bp::arg("molecules"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::setContents
        
            typedef void ( ::SireFF::FF::*setContents_function_type )( ::SireMol::MGID const &,::SireMol::MoleculeGroup const &,::SireBase::PropertyMap const & ) ;
            setContents_function_type setContents_function_value( &::SireFF::FF::setContents );
            
            FF_exposer.def( 
                "setContents"
                , setContents_function_value
                , ( bp::arg("mgid"), bp::arg("molgroup"), bp::arg("map") ) );
        
        }
        { //::SireFF::FF::setName
        
            typedef void ( ::SireFF::FF::*setName_function_type )( ::QString const & ) ;
            setName_function_type setName_function_value( &::SireFF::FF::setName );
            
            FF_exposer.def( 
                "setName"
                , setName_function_value
                , ( bp::arg("name") ) );
        
        }
        { //::SireFF::FF::setName
        
            typedef void ( ::SireFF::FF::*setName_function_type )( ::SireMol::MGID const &,::QString const & ) ;
            setName_function_type setName_function_value( &::SireFF::FF::setName );
            
            FF_exposer.def( 
                "setName"
                , setName_function_value
                , ( bp::arg("mgid"), bp::arg("name") ) );
        
        }
        { //::SireFF::FF::setProperty
        
            typedef bool ( ::SireFF::FF::*setProperty_function_type )( ::QString const &,::SireBase::Property const & ) ;
            setProperty_function_type setProperty_function_value( &::SireFF::FF::setProperty );
            
            FF_exposer.def( 
                "setProperty"
                , setProperty_function_value
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::SireFF::FF::toString
        
            typedef ::QString ( ::SireFF::FF::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireFF::FF::toString );
            
            FF_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireFF::FF::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireFF::FF::typeName );
            
            FF_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireFF::FF::update
        
            typedef void ( ::SireFF::FF::*update_function_type )( ::SireMol::MoleculeData const &,bool ) ;
            update_function_type update_function_value( &::SireFF::FF::update );
            
            FF_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("moldata"), bp::arg("auto_commit")=(bool)(true) ) );
        
        }
        { //::SireFF::FF::update
        
            typedef void ( ::SireFF::FF::*update_function_type )( ::SireMol::MoleculeView const &,bool ) ;
            update_function_type update_function_value( &::SireFF::FF::update );
            
            FF_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("molview"), bp::arg("auto_commit")=(bool)(true) ) );
        
        }
        { //::SireFF::FF::update
        
            typedef void ( ::SireFF::FF::*update_function_type )( ::SireMol::Molecules const &,bool ) ;
            update_function_type update_function_value( &::SireFF::FF::update );
            
            FF_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("molecules"), bp::arg("auto_commit")=(bool)(true) ) );
        
        }
        { //::SireFF::FF::update
        
            typedef void ( ::SireFF::FF::*update_function_type )( ::SireMol::MoleculeGroup const &,bool ) ;
            update_function_type update_function_value( &::SireFF::FF::update );
            
            FF_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("molgroup"), bp::arg("auto_commit")=(bool)(true) ) );
        
        }
        { //::SireFF::FF::version
        
            typedef ::quint64 ( ::SireFF::FF::*version_function_type )(  ) const;
            version_function_type version_function_value( &::SireFF::FF::version );
            
            FF_exposer.def( 
                "version"
                , version_function_value );
        
        }
        { //::SireFF::FF::what
        
            typedef char const * ( ::SireFF::FF::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireFF::FF::what );
            
            FF_exposer.def( 
                "what"
                , what_function_value );
        
        }
        FF_exposer.staticmethod( "null" );
        FF_exposer.staticmethod( "typeName" );
        FF_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireFF::FF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        FF_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireFF::FF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        FF_exposer.def( "__str__", &__str__< ::SireFF::FF > );
        FF_exposer.def( "__repr__", &__str__< ::SireFF::FF > );
        FF_exposer.def( "__len__", &__len_count< ::SireFF::FF > );
    }

}
