// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "OpenMMAMDIntegrator.pypp.hpp"

namespace bp = boost::python;

#include "SireFF/forcetable.h"

#include "SireIO/amber.h"

#include "SireMM/atomljs.h"

#include "SireMM/internalff.h"

#include "SireMaths/constants.h"

#include "SireMaths/rangenerator.h"

#include "SireMaths/vector.h"

#include "SireMol/amberparameters.h"

#include "SireMol/atomcharges.h"

#include "SireMol/atomcoords.h"

#include "SireMol/atommasses.h"

#include "SireMol/bondid.h"

#include "SireMol/connectivity.h"

#include "SireMol/mgname.h"

#include "SireMol/molecule.h"

#include "SireMol/moleculegroup.h"

#include "SireMol/moleditor.h"

#include "SireMol/partialmolecule.h"

#include "SireMove/flexibility.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireSystem/system.h"

#include "SireUnits/convert.h"

#include "SireUnits/temperature.h"

#include "SireUnits/units.h"

#include "SireVol/periodicbox.h"

#include "ensemble.h"

#include "openmmacceleratedmdintegrator.h"

#include <QDebug>

#include <QTime>

#include <iostream>

#include "openmmacceleratedmdintegrator.h"

SireMove::OpenMMAMDIntegrator __copy__(const SireMove::OpenMMAMDIntegrator &other){ return SireMove::OpenMMAMDIntegrator(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_OpenMMAMDIntegrator_class(){

    { //::SireMove::OpenMMAMDIntegrator
        typedef bp::class_< SireMove::OpenMMAMDIntegrator, bp::bases< SireMove::Integrator, SireBase::Property > > OpenMMAMDIntegrator_exposer_t;
        OpenMMAMDIntegrator_exposer_t OpenMMAMDIntegrator_exposer = OpenMMAMDIntegrator_exposer_t( "OpenMMAMDIntegrator", bp::init< bp::optional< bool > >(( bp::arg("frequent_save_velocities")=(bool)(false) )) );
        bp::scope OpenMMAMDIntegrator_scope( OpenMMAMDIntegrator_exposer );
        OpenMMAMDIntegrator_exposer.def( bp::init< SireMol::MoleculeGroup const &, bp::optional< bool > >(( bp::arg("molecule_group"), bp::arg("frequent_save_velocities")=(bool)(false) )) );
        OpenMMAMDIntegrator_exposer.def( bp::init< SireMove::OpenMMAMDIntegrator const & >(( bp::arg("other") )) );
        { //::SireMove::OpenMMAMDIntegrator::createWorkspace
        
            typedef ::SireMove::IntegratorWorkspacePtr ( ::SireMove::OpenMMAMDIntegrator::*createWorkspace_function_type )( ::SireBase::PropertyMap const & ) const;
            createWorkspace_function_type createWorkspace_function_value( &::SireMove::OpenMMAMDIntegrator::createWorkspace );
            
            OpenMMAMDIntegrator_exposer.def( 
                "createWorkspace"
                , createWorkspace_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::createWorkspace
        
            typedef ::SireMove::IntegratorWorkspacePtr ( ::SireMove::OpenMMAMDIntegrator::*createWorkspace_function_type )( ::SireMol::MoleculeGroup const &,::SireBase::PropertyMap const & ) const;
            createWorkspace_function_type createWorkspace_function_value( &::SireMove::OpenMMAMDIntegrator::createWorkspace );
            
            OpenMMAMDIntegrator_exposer.def( 
                "createWorkspace"
                , createWorkspace_function_value
                , ( bp::arg("molgroup"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::ensemble
        
            typedef ::SireMove::Ensemble ( ::SireMove::OpenMMAMDIntegrator::*ensemble_function_type )(  ) const;
            ensemble_function_type ensemble_function_value( &::SireMove::OpenMMAMDIntegrator::ensemble );
            
            OpenMMAMDIntegrator_exposer.def( 
                "ensemble"
                , ensemble_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getAlphatorsion
        
            typedef double ( ::SireMove::OpenMMAMDIntegrator::*getAlphatorsion_function_type )(  ) ;
            getAlphatorsion_function_type getAlphatorsion_function_value( &::SireMove::OpenMMAMDIntegrator::getAlphatorsion );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getAlphatorsion"
                , getAlphatorsion_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getAndersen
        
            typedef bool ( ::SireMove::OpenMMAMDIntegrator::*getAndersen_function_type )(  ) ;
            getAndersen_function_type getAndersen_function_value( &::SireMove::OpenMMAMDIntegrator::getAndersen );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getAndersen"
                , getAndersen_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getAndersen_frequency
        
            typedef double ( ::SireMove::OpenMMAMDIntegrator::*getAndersen_frequency_function_type )(  ) ;
            getAndersen_frequency_function_type getAndersen_frequency_function_value( &::SireMove::OpenMMAMDIntegrator::getAndersen_frequency );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getAndersen_frequency"
                , getAndersen_frequency_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getBoostedTorsions
        
            typedef ::QList< SireMol::DihedralID > ( ::SireMove::OpenMMAMDIntegrator::*getBoostedTorsions_function_type )(  ) ;
            getBoostedTorsions_function_type getBoostedTorsions_function_value( &::SireMove::OpenMMAMDIntegrator::getBoostedTorsions );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getBoostedTorsions"
                , getBoostedTorsions_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getBufferFrequency
        
            typedef int ( ::SireMove::OpenMMAMDIntegrator::*getBufferFrequency_function_type )(  ) ;
            getBufferFrequency_function_type getBufferFrequency_function_value( &::SireMove::OpenMMAMDIntegrator::getBufferFrequency );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getBufferFrequency"
                , getBufferFrequency_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getCMMremoval_frequency
        
            typedef int ( ::SireMove::OpenMMAMDIntegrator::*getCMMremoval_frequency_function_type )(  ) ;
            getCMMremoval_frequency_function_type getCMMremoval_frequency_function_value( &::SireMove::OpenMMAMDIntegrator::getCMMremoval_frequency );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getCMMremoval_frequency"
                , getCMMremoval_frequency_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getConstraintType
        
            typedef ::QString ( ::SireMove::OpenMMAMDIntegrator::*getConstraintType_function_type )(  ) ;
            getConstraintType_function_type getConstraintType_function_value( &::SireMove::OpenMMAMDIntegrator::getConstraintType );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getConstraintType"
                , getConstraintType_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getCutoffType
        
            typedef ::QString ( ::SireMove::OpenMMAMDIntegrator::*getCutoffType_function_type )(  ) ;
            getCutoffType_function_type getCutoffType_function_value( &::SireMove::OpenMMAMDIntegrator::getCutoffType );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getCutoffType"
                , getCutoffType_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getCutoff_distance
        
            typedef ::SireUnits::Dimension::Length ( ::SireMove::OpenMMAMDIntegrator::*getCutoff_distance_function_type )(  ) ;
            getCutoff_distance_function_type getCutoff_distance_function_value( &::SireMove::OpenMMAMDIntegrator::getCutoff_distance );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getCutoff_distance"
                , getCutoff_distance_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getDeviceIndex
        
            typedef ::QString ( ::SireMove::OpenMMAMDIntegrator::*getDeviceIndex_function_type )(  ) ;
            getDeviceIndex_function_type getDeviceIndex_function_value( &::SireMove::OpenMMAMDIntegrator::getDeviceIndex );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getDeviceIndex"
                , getDeviceIndex_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getEtorsion
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireMove::OpenMMAMDIntegrator::*getEtorsion_function_type )(  ) ;
            getEtorsion_function_type getEtorsion_function_value( &::SireMove::OpenMMAMDIntegrator::getEtorsion );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getEtorsion"
                , getEtorsion_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getField_dielectric
        
            typedef double ( ::SireMove::OpenMMAMDIntegrator::*getField_dielectric_function_type )(  ) ;
            getField_dielectric_function_type getField_dielectric_function_value( &::SireMove::OpenMMAMDIntegrator::getField_dielectric );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getField_dielectric"
                , getField_dielectric_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getFriction
        
            typedef ::SireUnits::Dimension::Time ( ::SireMove::OpenMMAMDIntegrator::*getFriction_function_type )(  ) ;
            getFriction_function_type getFriction_function_value( &::SireMove::OpenMMAMDIntegrator::getFriction );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getFriction"
                , getFriction_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getIntegrator
        
            typedef ::QString ( ::SireMove::OpenMMAMDIntegrator::*getIntegrator_function_type )(  ) ;
            getIntegrator_function_type getIntegrator_function_value( &::SireMove::OpenMMAMDIntegrator::getIntegrator );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getIntegrator"
                , getIntegrator_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getMCBarostat
        
            typedef bool ( ::SireMove::OpenMMAMDIntegrator::*getMCBarostat_function_type )(  ) ;
            getMCBarostat_function_type getMCBarostat_function_value( &::SireMove::OpenMMAMDIntegrator::getMCBarostat );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getMCBarostat"
                , getMCBarostat_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getMCBarostat_frequency
        
            typedef int ( ::SireMove::OpenMMAMDIntegrator::*getMCBarostat_frequency_function_type )(  ) ;
            getMCBarostat_frequency_function_type getMCBarostat_frequency_function_value( &::SireMove::OpenMMAMDIntegrator::getMCBarostat_frequency );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getMCBarostat_frequency"
                , getMCBarostat_frequency_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getPlatform
        
            typedef ::QString ( ::SireMove::OpenMMAMDIntegrator::*getPlatform_function_type )(  ) ;
            getPlatform_function_type getPlatform_function_value( &::SireMove::OpenMMAMDIntegrator::getPlatform );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getPlatform"
                , getPlatform_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getPressure
        
            typedef ::SireUnits::Dimension::Pressure ( ::SireMove::OpenMMAMDIntegrator::*getPressure_function_type )(  ) ;
            getPressure_function_type getPressure_function_value( &::SireMove::OpenMMAMDIntegrator::getPressure );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getPressure"
                , getPressure_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getRestraint
        
            typedef bool ( ::SireMove::OpenMMAMDIntegrator::*getRestraint_function_type )(  ) ;
            getRestraint_function_type getRestraint_function_value( &::SireMove::OpenMMAMDIntegrator::getRestraint );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getRestraint"
                , getRestraint_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::getTemperature
        
            typedef ::SireUnits::Dimension::Temperature ( ::SireMove::OpenMMAMDIntegrator::*getTemperature_function_type )(  ) ;
            getTemperature_function_type getTemperature_function_value( &::SireMove::OpenMMAMDIntegrator::getTemperature );
            
            OpenMMAMDIntegrator_exposer.def( 
                "getTemperature"
                , getTemperature_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::initialise
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*initialise_function_type )(  ) ;
            initialise_function_type initialise_function_value( &::SireMove::OpenMMAMDIntegrator::initialise );
            
            OpenMMAMDIntegrator_exposer.def( 
                "initialise"
                , initialise_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::integrate
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*integrate_function_type )( ::SireMove::IntegratorWorkspace &,::SireCAS::Symbol const &,::SireUnits::Dimension::Time,int,bool ) const;
            integrate_function_type integrate_function_value( &::SireMove::OpenMMAMDIntegrator::integrate );
            
            OpenMMAMDIntegrator_exposer.def( 
                "integrate"
                , integrate_function_value
                , ( bp::arg("workspace"), bp::arg("nrg_component"), bp::arg("timestep"), bp::arg("nmoves"), bp::arg("record_stats") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::isTimeReversible
        
            typedef bool ( ::SireMove::OpenMMAMDIntegrator::*isTimeReversible_function_type )(  ) const;
            isTimeReversible_function_type isTimeReversible_function_value( &::SireMove::OpenMMAMDIntegrator::isTimeReversible );
            
            OpenMMAMDIntegrator_exposer.def( 
                "isTimeReversible"
                , isTimeReversible_function_value );
        
        }
        OpenMMAMDIntegrator_exposer.def( bp::self != bp::self );
        { //::SireMove::OpenMMAMDIntegrator::operator=
        
            typedef ::SireMove::OpenMMAMDIntegrator & ( ::SireMove::OpenMMAMDIntegrator::*assign_function_type )( ::SireMove::OpenMMAMDIntegrator const & ) ;
            assign_function_type assign_function_value( &::SireMove::OpenMMAMDIntegrator::operator= );
            
            OpenMMAMDIntegrator_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        OpenMMAMDIntegrator_exposer.def( bp::self == bp::self );
        { //::SireMove::OpenMMAMDIntegrator::setAlphatorsion
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setAlphatorsion_function_type )( double ) ;
            setAlphatorsion_function_type setAlphatorsion_function_value( &::SireMove::OpenMMAMDIntegrator::setAlphatorsion );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setAlphatorsion"
                , setAlphatorsion_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setAndersen
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setAndersen_function_type )( bool ) ;
            setAndersen_function_type setAndersen_function_value( &::SireMove::OpenMMAMDIntegrator::setAndersen );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setAndersen"
                , setAndersen_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setAndersen_frequency
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setAndersen_frequency_function_type )( double ) ;
            setAndersen_frequency_function_type setAndersen_frequency_function_value( &::SireMove::OpenMMAMDIntegrator::setAndersen_frequency );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setAndersen_frequency"
                , setAndersen_frequency_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setBoostedTorsions
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setBoostedTorsions_function_type )( ::QList< SireMol::DihedralID > ) ;
            setBoostedTorsions_function_type setBoostedTorsions_function_value( &::SireMove::OpenMMAMDIntegrator::setBoostedTorsions );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setBoostedTorsions"
                , setBoostedTorsions_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setBufferFrequency
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setBufferFrequency_function_type )( int ) ;
            setBufferFrequency_function_type setBufferFrequency_function_value( &::SireMove::OpenMMAMDIntegrator::setBufferFrequency );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setBufferFrequency"
                , setBufferFrequency_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setCMMremoval_frequency
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setCMMremoval_frequency_function_type )( int ) ;
            setCMMremoval_frequency_function_type setCMMremoval_frequency_function_value( &::SireMove::OpenMMAMDIntegrator::setCMMremoval_frequency );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setCMMremoval_frequency"
                , setCMMremoval_frequency_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setConstraintType
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setConstraintType_function_type )( ::QString ) ;
            setConstraintType_function_type setConstraintType_function_value( &::SireMove::OpenMMAMDIntegrator::setConstraintType );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setConstraintType"
                , setConstraintType_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setCutoffType
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setCutoffType_function_type )( ::QString ) ;
            setCutoffType_function_type setCutoffType_function_value( &::SireMove::OpenMMAMDIntegrator::setCutoffType );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setCutoffType"
                , setCutoffType_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setCutoff_distance
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setCutoff_distance_function_type )( ::SireUnits::Dimension::Length ) ;
            setCutoff_distance_function_type setCutoff_distance_function_value( &::SireMove::OpenMMAMDIntegrator::setCutoff_distance );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setCutoff_distance"
                , setCutoff_distance_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setDeviceIndex
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setDeviceIndex_function_type )( ::QString ) ;
            setDeviceIndex_function_type setDeviceIndex_function_value( &::SireMove::OpenMMAMDIntegrator::setDeviceIndex );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setDeviceIndex"
                , setDeviceIndex_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setEtorsion
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setEtorsion_function_type )( ::SireUnits::Dimension::MolarEnergy ) ;
            setEtorsion_function_type setEtorsion_function_value( &::SireMove::OpenMMAMDIntegrator::setEtorsion );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setEtorsion"
                , setEtorsion_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setField_dielectric
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setField_dielectric_function_type )( double ) ;
            setField_dielectric_function_type setField_dielectric_function_value( &::SireMove::OpenMMAMDIntegrator::setField_dielectric );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setField_dielectric"
                , setField_dielectric_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setFriction
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setFriction_function_type )( ::SireUnits::Dimension::Time ) ;
            setFriction_function_type setFriction_function_value( &::SireMove::OpenMMAMDIntegrator::setFriction );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setFriction"
                , setFriction_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setIntegrator
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setIntegrator_function_type )( ::QString ) ;
            setIntegrator_function_type setIntegrator_function_value( &::SireMove::OpenMMAMDIntegrator::setIntegrator );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setIntegrator"
                , setIntegrator_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setMCBarostat
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setMCBarostat_function_type )( bool ) ;
            setMCBarostat_function_type setMCBarostat_function_value( &::SireMove::OpenMMAMDIntegrator::setMCBarostat );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setMCBarostat"
                , setMCBarostat_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setMCBarostat_frequency
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setMCBarostat_frequency_function_type )( int ) ;
            setMCBarostat_frequency_function_type setMCBarostat_frequency_function_value( &::SireMove::OpenMMAMDIntegrator::setMCBarostat_frequency );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setMCBarostat_frequency"
                , setMCBarostat_frequency_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setPlatform
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setPlatform_function_type )( ::QString ) ;
            setPlatform_function_type setPlatform_function_value( &::SireMove::OpenMMAMDIntegrator::setPlatform );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setPlatform"
                , setPlatform_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setPressure
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setPressure_function_type )( ::SireUnits::Dimension::Pressure ) ;
            setPressure_function_type setPressure_function_value( &::SireMove::OpenMMAMDIntegrator::setPressure );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setPressure"
                , setPressure_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setRestraint
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setRestraint_function_type )( bool ) ;
            setRestraint_function_type setRestraint_function_value( &::SireMove::OpenMMAMDIntegrator::setRestraint );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setRestraint"
                , setRestraint_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::setTemperature
        
            typedef void ( ::SireMove::OpenMMAMDIntegrator::*setTemperature_function_type )( ::SireUnits::Dimension::Temperature ) ;
            setTemperature_function_type setTemperature_function_value( &::SireMove::OpenMMAMDIntegrator::setTemperature );
            
            OpenMMAMDIntegrator_exposer.def( 
                "setTemperature"
                , setTemperature_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::toString
        
            typedef ::QString ( ::SireMove::OpenMMAMDIntegrator::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMove::OpenMMAMDIntegrator::toString );
            
            OpenMMAMDIntegrator_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMove::OpenMMAMDIntegrator::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMove::OpenMMAMDIntegrator::typeName );
            
            OpenMMAMDIntegrator_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        OpenMMAMDIntegrator_exposer.staticmethod( "typeName" );
        OpenMMAMDIntegrator_exposer.def( "__copy__", &__copy__);
        OpenMMAMDIntegrator_exposer.def( "__deepcopy__", &__copy__);
        OpenMMAMDIntegrator_exposer.def( "clone", &__copy__);
        OpenMMAMDIntegrator_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMove::OpenMMAMDIntegrator >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        OpenMMAMDIntegrator_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMove::OpenMMAMDIntegrator >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        OpenMMAMDIntegrator_exposer.def( "__str__", &__str__< ::SireMove::OpenMMAMDIntegrator > );
        OpenMMAMDIntegrator_exposer.def( "__repr__", &__str__< ::SireMove::OpenMMAMDIntegrator > );
    }

}