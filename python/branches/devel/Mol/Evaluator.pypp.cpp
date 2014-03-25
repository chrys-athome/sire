// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Evaluator.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/errors.h"

#include "SireMaths/axisset.h"

#include "SireMaths/line.h"

#include "SireMaths/sphere.h"

#include "SireMaths/torsion.h"

#include "SireMaths/triangle.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "SireUnits/dimensions.h"

#include "SireUnits/units.h"

#include "SireVol/coordgroup.h"

#include "angleid.h"

#include "atomcharges.h"

#include "atomcoords.h"

#include "atomelements.h"

#include "atommasses.h"

#include "atommatcher.h"

#include "bondid.h"

#include "connectivity.h"

#include "dihedralid.h"

#include "editor.hpp"

#include "evaluator.h"

#include "molecule.h"

#include "mover.hpp"

#include <QDebug>

#include <QElapsedTimer>

#include "evaluator.h"

SireMol::Evaluator __copy__(const SireMol::Evaluator &other){ return SireMol::Evaluator(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Evaluator_class(){

    { //::SireMol::Evaluator
        typedef bp::class_< SireMol::Evaluator, bp::bases< SireMol::MoleculeView, SireBase::Property > > Evaluator_exposer_t;
        Evaluator_exposer_t Evaluator_exposer = Evaluator_exposer_t( "Evaluator", bp::init< >() );
        bp::scope Evaluator_scope( Evaluator_exposer );
        Evaluator_exposer.def( bp::init< SireMol::MoleculeView const & >(( bp::arg("molecule") )) );
        Evaluator_exposer.def( bp::init< SireMol::MoleculeData const & >(( bp::arg("moldata") )) );
        Evaluator_exposer.def( bp::init< SireMol::MoleculeView const &, SireMol::AtomSelection const & >(( bp::arg("molecule"), bp::arg("selected_atoms") )) );
        Evaluator_exposer.def( bp::init< SireMol::MoleculeData const &, SireMol::AtomSelection const & >(( bp::arg("moldata"), bp::arg("selected_atoms") )) );
        Evaluator_exposer.def( bp::init< SireMol::Evaluator const & >(( bp::arg("other") )) );
        { //::SireMol::Evaluator::aaBox
        
            typedef ::SireVol::AABox ( ::SireMol::Evaluator::*aaBox_function_type )( ::SireBase::PropertyMap const & ) const;
            aaBox_function_type aaBox_function_value( &::SireMol::Evaluator::aaBox );
            
            Evaluator_exposer.def( 
                "aaBox"
                , aaBox_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::alignmentAxes
        
            typedef ::SireMaths::AxisSet ( ::SireMol::Evaluator::*alignmentAxes_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) const;
            alignmentAxes_function_type alignmentAxes_function_value( &::SireMol::Evaluator::alignmentAxes );
            
            Evaluator_exposer.def( 
                "alignmentAxes"
                , alignmentAxes_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::alignmentAxes
        
            typedef ::SireMaths::AxisSet ( ::SireMol::Evaluator::*alignmentAxes_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) const;
            alignmentAxes_function_type alignmentAxes_function_value( &::SireMol::Evaluator::alignmentAxes );
            
            Evaluator_exposer.def( 
                "alignmentAxes"
                , alignmentAxes_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") ) );
        
        }
        { //::SireMol::Evaluator::boundingSphere
        
            typedef ::SireMaths::Sphere ( ::SireMol::Evaluator::*boundingSphere_function_type )( ::SireBase::PropertyMap const & ) const;
            boundingSphere_function_type boundingSphere_function_value( &::SireMol::Evaluator::boundingSphere );
            
            Evaluator_exposer.def( 
                "boundingSphere"
                , boundingSphere_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::center
        
            typedef ::SireMaths::Vector ( ::SireMol::Evaluator::*center_function_type )( ::SireBase::PropertyMap const & ) const;
            center_function_type center_function_value( &::SireMol::Evaluator::center );
            
            Evaluator_exposer.def( 
                "center"
                , center_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::centerOfGeometry
        
            typedef ::SireMaths::Vector ( ::SireMol::Evaluator::*centerOfGeometry_function_type )( ::SireBase::PropertyMap const & ) const;
            centerOfGeometry_function_type centerOfGeometry_function_value( &::SireMol::Evaluator::centerOfGeometry );
            
            Evaluator_exposer.def( 
                "centerOfGeometry"
                , centerOfGeometry_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::centerOfMass
        
            typedef ::SireMaths::Vector ( ::SireMol::Evaluator::*centerOfMass_function_type )( ::SireBase::PropertyMap const & ) const;
            centerOfMass_function_type centerOfMass_function_value( &::SireMol::Evaluator::centerOfMass );
            
            Evaluator_exposer.def( 
                "centerOfMass"
                , centerOfMass_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::centroid
        
            typedef ::SireMaths::Vector ( ::SireMol::Evaluator::*centroid_function_type )( ::SireBase::PropertyMap const & ) const;
            centroid_function_type centroid_function_value( &::SireMol::Evaluator::centroid );
            
            Evaluator_exposer.def( 
                "centroid"
                , centroid_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::charge
        
            typedef ::SireUnits::Dimension::Charge ( ::SireMol::Evaluator::*charge_function_type )( ::SireBase::PropertyMap const & ) const;
            charge_function_type charge_function_value( &::SireMol::Evaluator::charge );
            
            Evaluator_exposer.def( 
                "charge"
                , charge_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::findMCS
        
            typedef ::QHash< SireMol::AtomIdx, SireMol::AtomIdx > ( ::SireMol::Evaluator::*findMCS_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) const;
            findMCS_function_type findMCS_function_value( &::SireMol::Evaluator::findMCS );
            
            Evaluator_exposer.def( 
                "findMCS"
                , findMCS_function_value
                , ( bp::arg("other"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::findMCS
        
            typedef ::QHash< SireMol::AtomIdx, SireMol::AtomIdx > ( ::SireMol::Evaluator::*findMCS_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) const;
            findMCS_function_type findMCS_function_value( &::SireMol::Evaluator::findMCS );
            
            Evaluator_exposer.def( 
                "findMCS"
                , findMCS_function_value
                , ( bp::arg("other"), bp::arg("atommatcher"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::findMCS
        
            typedef ::QHash< SireMol::AtomIdx, SireMol::AtomIdx > ( ::SireMol::Evaluator::*findMCS_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) const;
            findMCS_function_type findMCS_function_value( &::SireMol::Evaluator::findMCS );
            
            Evaluator_exposer.def( 
                "findMCS"
                , findMCS_function_value
                , ( bp::arg("other"), bp::arg("map0"), bp::arg("map1") ) );
        
        }
        { //::SireMol::Evaluator::findMCS
        
            typedef ::QHash< SireMol::AtomIdx, SireMol::AtomIdx > ( ::SireMol::Evaluator::*findMCS_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) const;
            findMCS_function_type findMCS_function_value( &::SireMol::Evaluator::findMCS );
            
            Evaluator_exposer.def( 
                "findMCS"
                , findMCS_function_value
                , ( bp::arg("other"), bp::arg("atommatcher"), bp::arg("map0"), bp::arg("map1") ) );
        
        }
        { //::SireMol::Evaluator::findMCS
        
            typedef ::QHash< SireMol::AtomIdx, SireMol::AtomIdx > ( ::SireMol::Evaluator::*findMCS_function_type )( ::SireMol::MoleculeView const &,::SireUnits::Dimension::Time const &,::SireBase::PropertyMap const & ) const;
            findMCS_function_type findMCS_function_value( &::SireMol::Evaluator::findMCS );
            
            Evaluator_exposer.def( 
                "findMCS"
                , findMCS_function_value
                , ( bp::arg("other"), bp::arg("timeout"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::findMCS
        
            typedef ::QHash< SireMol::AtomIdx, SireMol::AtomIdx > ( ::SireMol::Evaluator::*findMCS_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireUnits::Dimension::Time const &,::SireBase::PropertyMap const & ) const;
            findMCS_function_type findMCS_function_value( &::SireMol::Evaluator::findMCS );
            
            Evaluator_exposer.def( 
                "findMCS"
                , findMCS_function_value
                , ( bp::arg("other"), bp::arg("atommatcher"), bp::arg("timeout"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::findMCS
        
            typedef ::QHash< SireMol::AtomIdx, SireMol::AtomIdx > ( ::SireMol::Evaluator::*findMCS_function_type )( ::SireMol::MoleculeView const &,::SireUnits::Dimension::Time const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) const;
            findMCS_function_type findMCS_function_value( &::SireMol::Evaluator::findMCS );
            
            Evaluator_exposer.def( 
                "findMCS"
                , findMCS_function_value
                , ( bp::arg("other"), bp::arg("timeout"), bp::arg("map0"), bp::arg("map1") ) );
        
        }
        { //::SireMol::Evaluator::findMCS
        
            typedef ::QHash< SireMol::AtomIdx, SireMol::AtomIdx > ( ::SireMol::Evaluator::*findMCS_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireUnits::Dimension::Time const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) const;
            findMCS_function_type findMCS_function_value( &::SireMol::Evaluator::findMCS );
            
            Evaluator_exposer.def( 
                "findMCS"
                , findMCS_function_value
                , ( bp::arg("other"), bp::arg("atommatcher"), bp::arg("timeout"), bp::arg("map0"), bp::arg("map1") ) );
        
        }
        { //::SireMol::Evaluator::hasMetadata
        
            typedef bool ( ::SireMol::Evaluator::*hasMetadata_function_type )( ::SireBase::PropertyName const & ) const;
            hasMetadata_function_type hasMetadata_function_value( &::SireMol::Evaluator::hasMetadata );
            
            Evaluator_exposer.def( 
                "hasMetadata"
                , hasMetadata_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMol::Evaluator::hasMetadata
        
            typedef bool ( ::SireMol::Evaluator::*hasMetadata_function_type )( ::SireBase::PropertyName const &,::SireBase::PropertyName const & ) const;
            hasMetadata_function_type hasMetadata_function_value( &::SireMol::Evaluator::hasMetadata );
            
            Evaluator_exposer.def( 
                "hasMetadata"
                , hasMetadata_function_value
                , ( bp::arg("arg0"), bp::arg("arg1") ) );
        
        }
        { //::SireMol::Evaluator::hasProperty
        
            typedef bool ( ::SireMol::Evaluator::*hasProperty_function_type )( ::SireBase::PropertyName const & ) const;
            hasProperty_function_type hasProperty_function_value( &::SireMol::Evaluator::hasProperty );
            
            Evaluator_exposer.def( 
                "hasProperty"
                , hasProperty_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMol::Evaluator::isEmpty
        
            typedef bool ( ::SireMol::Evaluator::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::Evaluator::isEmpty );
            
            Evaluator_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::Evaluator::mass
        
            typedef ::SireUnits::Dimension::MolarMass ( ::SireMol::Evaluator::*mass_function_type )( ::SireBase::PropertyMap const & ) const;
            mass_function_type mass_function_value( &::SireMol::Evaluator::mass );
            
            Evaluator_exposer.def( 
                "mass"
                , mass_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::measure
        
            typedef ::SireUnits::Dimension::Length ( ::SireMol::Evaluator::*measure_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireBase::PropertyMap const & ) const;
            measure_function_type measure_function_value( &::SireMol::Evaluator::measure );
            
            Evaluator_exposer.def( 
                "measure"
                , measure_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::measure
        
            typedef ::SireUnits::Dimension::Length ( ::SireMol::Evaluator::*measure_function_type )( ::SireMol::BondID const &,::SireBase::PropertyMap const & ) const;
            measure_function_type measure_function_value( &::SireMol::Evaluator::measure );
            
            Evaluator_exposer.def( 
                "measure"
                , measure_function_value
                , ( bp::arg("bond"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::measure
        
            typedef ::SireUnits::Dimension::Angle ( ::SireMol::Evaluator::*measure_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const &,::SireBase::PropertyMap const & ) const;
            measure_function_type measure_function_value( &::SireMol::Evaluator::measure );
            
            Evaluator_exposer.def( 
                "measure"
                , measure_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::measure
        
            typedef ::SireUnits::Dimension::Angle ( ::SireMol::Evaluator::*measure_function_type )( ::SireMol::AngleID const &,::SireBase::PropertyMap const & ) const;
            measure_function_type measure_function_value( &::SireMol::Evaluator::measure );
            
            Evaluator_exposer.def( 
                "measure"
                , measure_function_value
                , ( bp::arg("angle"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::measure
        
            typedef ::SireUnits::Dimension::Angle ( ::SireMol::Evaluator::*measure_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const &,::SireBase::PropertyMap const & ) const;
            measure_function_type measure_function_value( &::SireMol::Evaluator::measure );
            
            Evaluator_exposer.def( 
                "measure"
                , measure_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("atom3"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::measure
        
            typedef ::SireUnits::Dimension::Angle ( ::SireMol::Evaluator::*measure_function_type )( ::SireMol::DihedralID const &,::SireBase::PropertyMap const & ) const;
            measure_function_type measure_function_value( &::SireMol::Evaluator::measure );
            
            Evaluator_exposer.def( 
                "measure"
                , measure_function_value
                , ( bp::arg("dihedral"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::metadataKeys
        
            typedef ::QStringList ( ::SireMol::Evaluator::*metadataKeys_function_type )(  ) const;
            metadataKeys_function_type metadataKeys_function_value( &::SireMol::Evaluator::metadataKeys );
            
            Evaluator_exposer.def( 
                "metadataKeys"
                , metadataKeys_function_value );
        
        }
        { //::SireMol::Evaluator::metadataKeys
        
            typedef ::QStringList ( ::SireMol::Evaluator::*metadataKeys_function_type )( ::SireBase::PropertyName const & ) const;
            metadataKeys_function_type metadataKeys_function_value( &::SireMol::Evaluator::metadataKeys );
            
            Evaluator_exposer.def( 
                "metadataKeys"
                , metadataKeys_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMol::Evaluator::operator=
        
            typedef ::SireMol::Evaluator & ( ::SireMol::Evaluator::*assign_function_type )( ::SireMol::Evaluator const & ) ;
            assign_function_type assign_function_value( &::SireMol::Evaluator::operator= );
            
            Evaluator_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Evaluator::operator=
        
            typedef ::SireMol::Evaluator & ( ::SireMol::Evaluator::*assign_function_type )( ::SireMol::MoleculeView const & ) ;
            assign_function_type assign_function_value( &::SireMol::Evaluator::operator= );
            
            Evaluator_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Evaluator::principalAxes
        
            typedef ::SireMaths::AxisSet ( ::SireMol::Evaluator::*principalAxes_function_type )( ::SireBase::PropertyMap const & ) const;
            principalAxes_function_type principalAxes_function_value( &::SireMol::Evaluator::principalAxes );
            
            Evaluator_exposer.def( 
                "principalAxes"
                , principalAxes_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::principalAxes
        
            typedef ::SireMaths::AxisSet ( ::SireMol::Evaluator::*principalAxes_function_type )( ::SireMaths::Vector &,::SireBase::PropertyMap const & ) const;
            principalAxes_function_type principalAxes_function_value( &::SireMol::Evaluator::principalAxes );
            
            Evaluator_exposer.def( 
                "principalAxes"
                , principalAxes_function_value
                , ( bp::arg("principal_moments"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::propertyKeys
        
            typedef ::QStringList ( ::SireMol::Evaluator::*propertyKeys_function_type )(  ) const;
            propertyKeys_function_type propertyKeys_function_value( &::SireMol::Evaluator::propertyKeys );
            
            Evaluator_exposer.def( 
                "propertyKeys"
                , propertyKeys_function_value );
        
        }
        { //::SireMol::Evaluator::rmsd
        
            typedef ::SireUnits::Dimension::Length ( ::SireMol::Evaluator::*rmsd_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) const;
            rmsd_function_type rmsd_function_value( &::SireMol::Evaluator::rmsd );
            
            Evaluator_exposer.def( 
                "rmsd"
                , rmsd_function_value
                , ( bp::arg("other"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::rmsd
        
            typedef ::SireUnits::Dimension::Length ( ::SireMol::Evaluator::*rmsd_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) const;
            rmsd_function_type rmsd_function_value( &::SireMol::Evaluator::rmsd );
            
            Evaluator_exposer.def( 
                "rmsd"
                , rmsd_function_value
                , ( bp::arg("other"), bp::arg("map0"), bp::arg("map1") ) );
        
        }
        { //::SireMol::Evaluator::rmsd
        
            typedef ::SireUnits::Dimension::Length ( ::SireMol::Evaluator::*rmsd_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) const;
            rmsd_function_type rmsd_function_value( &::SireMol::Evaluator::rmsd );
            
            Evaluator_exposer.def( 
                "rmsd"
                , rmsd_function_value
                , ( bp::arg("other"), bp::arg("atommatcher"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::Evaluator::rmsd
        
            typedef ::SireUnits::Dimension::Length ( ::SireMol::Evaluator::*rmsd_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) const;
            rmsd_function_type rmsd_function_value( &::SireMol::Evaluator::rmsd );
            
            Evaluator_exposer.def( 
                "rmsd"
                , rmsd_function_value
                , ( bp::arg("other"), bp::arg("atommatcher"), bp::arg("map0"), bp::arg("map1") ) );
        
        }
        { //::SireMol::Evaluator::selectedAll
        
            typedef bool ( ::SireMol::Evaluator::*selectedAll_function_type )(  ) const;
            selectedAll_function_type selectedAll_function_value( &::SireMol::Evaluator::selectedAll );
            
            Evaluator_exposer.def( 
                "selectedAll"
                , selectedAll_function_value );
        
        }
        { //::SireMol::Evaluator::selection
        
            typedef ::SireMol::AtomSelection ( ::SireMol::Evaluator::*selection_function_type )(  ) const;
            selection_function_type selection_function_value( &::SireMol::Evaluator::selection );
            
            Evaluator_exposer.def( 
                "selection"
                , selection_function_value );
        
        }
        { //::SireMol::Evaluator::toString
        
            typedef ::QString ( ::SireMol::Evaluator::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::Evaluator::toString );
            
            Evaluator_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::Evaluator::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::Evaluator::typeName );
            
            Evaluator_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        Evaluator_exposer.staticmethod( "typeName" );
        Evaluator_exposer.def( "__copy__", &__copy__);
        Evaluator_exposer.def( "__deepcopy__", &__copy__);
        Evaluator_exposer.def( "clone", &__copy__);
        Evaluator_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::Evaluator >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Evaluator_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::Evaluator >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Evaluator_exposer.def( "__str__", &__str__< ::SireMol::Evaluator > );
        Evaluator_exposer.def( "__repr__", &__str__< ::SireMol::Evaluator > );
    }

}
