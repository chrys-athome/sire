// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "OpenMMFrEnergyST.pypp.hpp"

namespace bp = boost::python;

#include "SireFF/forcetable.h"

#include "SireIO/amber.h"

#include "SireMM/atomljs.h"

#include "SireMM/internalff.h"

#include "SireMM/internalperturbation.h"

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

#include "SireMol/perturbation.h"

#include "SireMove/flexibility.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireSystem/system.h"

#include "SireUnits/convert.h"

#include "SireUnits/temperature.h"

#include "SireUnits/units.h"

#include "SireVol/periodicbox.h"

#include "dcd_writer.h"

#include "ensemble.h"

#include "openmmfrenergyst.h"

#include <QDebug>

#include <QTime>

#include <iostream>

#include "openmmfrenergyst.h"

SireMove::OpenMMFrEnergyST __copy__(const SireMove::OpenMMFrEnergyST &other){ return SireMove::OpenMMFrEnergyST(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_OpenMMFrEnergyST_class(){

    { //::SireMove::OpenMMFrEnergyST
        typedef bp::class_< SireMove::OpenMMFrEnergyST, bp::bases< SireMove::Integrator, SireBase::Property > > OpenMMFrEnergyST_exposer_t;
        OpenMMFrEnergyST_exposer_t OpenMMFrEnergyST_exposer = OpenMMFrEnergyST_exposer_t( "OpenMMFrEnergyST", bp::init< bp::optional< bool > >(( bp::arg("frequent_save_velocities")=(bool)(false) )) );
        bp::scope OpenMMFrEnergyST_scope( OpenMMFrEnergyST_exposer );
        OpenMMFrEnergyST_exposer.def( bp::init< SireMol::MoleculeGroup const &, SireMol::MoleculeGroup const &, SireMol::MoleculeGroup const &, SireMol::MoleculeGroup const &, SireMol::MoleculeGroup const &, bp::optional< bool > >(( bp::arg("molecule_group"), bp::arg("solutes"), bp::arg("solute_hard"), bp::arg("solute_todummy"), bp::arg("solute_fromdummy"), bp::arg("frequent_save_velocities")=(bool)(false) )) );
        OpenMMFrEnergyST_exposer.def( bp::init< SireMove::OpenMMFrEnergyST const & >(( bp::arg("other") )) );
        { //::SireMove::OpenMMFrEnergyST::createWorkspace
        
            typedef ::SireMove::IntegratorWorkspacePtr ( ::SireMove::OpenMMFrEnergyST::*createWorkspace_function_type )( ::SireBase::PropertyMap const & ) const;
            createWorkspace_function_type createWorkspace_function_value( &::SireMove::OpenMMFrEnergyST::createWorkspace );
            
            OpenMMFrEnergyST_exposer.def( 
                "createWorkspace"
                , createWorkspace_function_value
                , ( bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::createWorkspace
        
            typedef ::SireMove::IntegratorWorkspacePtr ( ::SireMove::OpenMMFrEnergyST::*createWorkspace_function_type )( ::SireMol::MoleculeGroup const &,::SireBase::PropertyMap const & ) const;
            createWorkspace_function_type createWorkspace_function_value( &::SireMove::OpenMMFrEnergyST::createWorkspace );
            
            OpenMMFrEnergyST_exposer.def( 
                "createWorkspace"
                , createWorkspace_function_value
                , ( bp::arg("molgroup"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::ensemble
        
            typedef ::SireMove::Ensemble ( ::SireMove::OpenMMFrEnergyST::*ensemble_function_type )(  ) const;
            ensemble_function_type ensemble_function_value( &::SireMove::OpenMMFrEnergyST::ensemble );
            
            OpenMMFrEnergyST_exposer.def( 
                "ensemble"
                , ensemble_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getAlchemical_value
        
            typedef double ( ::SireMove::OpenMMFrEnergyST::*getAlchemical_value_function_type )(  ) ;
            getAlchemical_value_function_type getAlchemical_value_function_value( &::SireMove::OpenMMFrEnergyST::getAlchemical_value );
            
            OpenMMFrEnergyST_exposer.def( 
                "getAlchemical_value"
                , getAlchemical_value_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getAndersen
        
            typedef bool ( ::SireMove::OpenMMFrEnergyST::*getAndersen_function_type )(  ) ;
            getAndersen_function_type getAndersen_function_value( &::SireMove::OpenMMFrEnergyST::getAndersen );
            
            OpenMMFrEnergyST_exposer.def( 
                "getAndersen"
                , getAndersen_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getAndersen_frequency
        
            typedef double ( ::SireMove::OpenMMFrEnergyST::*getAndersen_frequency_function_type )(  ) ;
            getAndersen_frequency_function_type getAndersen_frequency_function_value( &::SireMove::OpenMMFrEnergyST::getAndersen_frequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "getAndersen_frequency"
                , getAndersen_frequency_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getBufferFrequency
        
            typedef int ( ::SireMove::OpenMMFrEnergyST::*getBufferFrequency_function_type )(  ) ;
            getBufferFrequency_function_type getBufferFrequency_function_value( &::SireMove::OpenMMFrEnergyST::getBufferFrequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "getBufferFrequency"
                , getBufferFrequency_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getCMMremoval_frequency
        
            typedef int ( ::SireMove::OpenMMFrEnergyST::*getCMMremoval_frequency_function_type )(  ) ;
            getCMMremoval_frequency_function_type getCMMremoval_frequency_function_value( &::SireMove::OpenMMFrEnergyST::getCMMremoval_frequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "getCMMremoval_frequency"
                , getCMMremoval_frequency_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getConstraintType
        
            typedef ::QString ( ::SireMove::OpenMMFrEnergyST::*getConstraintType_function_type )(  ) ;
            getConstraintType_function_type getConstraintType_function_value( &::SireMove::OpenMMFrEnergyST::getConstraintType );
            
            OpenMMFrEnergyST_exposer.def( 
                "getConstraintType"
                , getConstraintType_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getCoulomb_power
        
            typedef float ( ::SireMove::OpenMMFrEnergyST::*getCoulomb_power_function_type )(  ) ;
            getCoulomb_power_function_type getCoulomb_power_function_value( &::SireMove::OpenMMFrEnergyST::getCoulomb_power );
            
            OpenMMFrEnergyST_exposer.def( 
                "getCoulomb_power"
                , getCoulomb_power_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getCutoffType
        
            typedef ::QString ( ::SireMove::OpenMMFrEnergyST::*getCutoffType_function_type )(  ) ;
            getCutoffType_function_type getCutoffType_function_value( &::SireMove::OpenMMFrEnergyST::getCutoffType );
            
            OpenMMFrEnergyST_exposer.def( 
                "getCutoffType"
                , getCutoffType_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getCutoff_distance
        
            typedef ::SireUnits::Dimension::Length ( ::SireMove::OpenMMFrEnergyST::*getCutoff_distance_function_type )(  ) ;
            getCutoff_distance_function_type getCutoff_distance_function_value( &::SireMove::OpenMMFrEnergyST::getCutoff_distance );
            
            OpenMMFrEnergyST_exposer.def( 
                "getCutoff_distance"
                , getCutoff_distance_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getDeltaAlchemical
        
            typedef double ( ::SireMove::OpenMMFrEnergyST::*getDeltaAlchemical_function_type )(  ) ;
            getDeltaAlchemical_function_type getDeltaAlchemical_function_value( &::SireMove::OpenMMFrEnergyST::getDeltaAlchemical );
            
            OpenMMFrEnergyST_exposer.def( 
                "getDeltaAlchemical"
                , getDeltaAlchemical_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getDeviceIndex
        
            typedef ::QString ( ::SireMove::OpenMMFrEnergyST::*getDeviceIndex_function_type )(  ) ;
            getDeviceIndex_function_type getDeviceIndex_function_value( &::SireMove::OpenMMFrEnergyST::getDeviceIndex );
            
            OpenMMFrEnergyST_exposer.def( 
                "getDeviceIndex"
                , getDeviceIndex_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getEnergies
        
            typedef ::QVector< double > ( ::SireMove::OpenMMFrEnergyST::*getEnergies_function_type )(  ) ;
            getEnergies_function_type getEnergies_function_value( &::SireMove::OpenMMFrEnergyST::getEnergies );
            
            OpenMMFrEnergyST_exposer.def( 
                "getEnergies"
                , getEnergies_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getEnergyFrequency
        
            typedef int ( ::SireMove::OpenMMFrEnergyST::*getEnergyFrequency_function_type )(  ) ;
            getEnergyFrequency_function_type getEnergyFrequency_function_value( &::SireMove::OpenMMFrEnergyST::getEnergyFrequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "getEnergyFrequency"
                , getEnergyFrequency_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getEquilib_iterations
        
            typedef int ( ::SireMove::OpenMMFrEnergyST::*getEquilib_iterations_function_type )(  ) ;
            getEquilib_iterations_function_type getEquilib_iterations_function_value( &::SireMove::OpenMMFrEnergyST::getEquilib_iterations );
            
            OpenMMFrEnergyST_exposer.def( 
                "getEquilib_iterations"
                , getEquilib_iterations_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getEquilib_time_step
        
            typedef ::SireUnits::Dimension::Time ( ::SireMove::OpenMMFrEnergyST::*getEquilib_time_step_function_type )(  ) ;
            getEquilib_time_step_function_type getEquilib_time_step_function_value( &::SireMove::OpenMMFrEnergyST::getEquilib_time_step );
            
            OpenMMFrEnergyST_exposer.def( 
                "getEquilib_time_step"
                , getEquilib_time_step_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getField_dielectric
        
            typedef double ( ::SireMove::OpenMMFrEnergyST::*getField_dielectric_function_type )(  ) ;
            getField_dielectric_function_type getField_dielectric_function_value( &::SireMove::OpenMMFrEnergyST::getField_dielectric );
            
            OpenMMFrEnergyST_exposer.def( 
                "getField_dielectric"
                , getField_dielectric_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getFriction
        
            typedef ::SireUnits::Dimension::Time ( ::SireMove::OpenMMFrEnergyST::*getFriction_function_type )(  ) ;
            getFriction_function_type getFriction_function_value( &::SireMove::OpenMMFrEnergyST::getFriction );
            
            OpenMMFrEnergyST_exposer.def( 
                "getFriction"
                , getFriction_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getGradient
        
            typedef double ( ::SireMove::OpenMMFrEnergyST::*getGradient_function_type )(  ) ;
            getGradient_function_type getGradient_function_value( &::SireMove::OpenMMFrEnergyST::getGradient );
            
            OpenMMFrEnergyST_exposer.def( 
                "getGradient"
                , getGradient_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getGradients
        
            typedef ::QVector< double > ( ::SireMove::OpenMMFrEnergyST::*getGradients_function_type )(  ) ;
            getGradients_function_type getGradients_function_value( &::SireMove::OpenMMFrEnergyST::getGradients );
            
            OpenMMFrEnergyST_exposer.def( 
                "getGradients"
                , getGradients_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getIntegration_tollerance
        
            typedef double ( ::SireMove::OpenMMFrEnergyST::*getIntegration_tollerance_function_type )(  ) ;
            getIntegration_tollerance_function_type getIntegration_tollerance_function_value( &::SireMove::OpenMMFrEnergyST::getIntegration_tollerance );
            
            OpenMMFrEnergyST_exposer.def( 
                "getIntegration_tollerance"
                , getIntegration_tollerance_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getIntegrator
        
            typedef ::QString ( ::SireMove::OpenMMFrEnergyST::*getIntegrator_function_type )(  ) ;
            getIntegrator_function_type getIntegrator_function_value( &::SireMove::OpenMMFrEnergyST::getIntegrator );
            
            OpenMMFrEnergyST_exposer.def( 
                "getIntegrator"
                , getIntegrator_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getMCBarostat
        
            typedef bool ( ::SireMove::OpenMMFrEnergyST::*getMCBarostat_function_type )(  ) ;
            getMCBarostat_function_type getMCBarostat_function_value( &::SireMove::OpenMMFrEnergyST::getMCBarostat );
            
            OpenMMFrEnergyST_exposer.def( 
                "getMCBarostat"
                , getMCBarostat_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getMCBarostat_frequency
        
            typedef int ( ::SireMove::OpenMMFrEnergyST::*getMCBarostat_frequency_function_type )(  ) ;
            getMCBarostat_frequency_function_type getMCBarostat_frequency_function_value( &::SireMove::OpenMMFrEnergyST::getMCBarostat_frequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "getMCBarostat_frequency"
                , getMCBarostat_frequency_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getMinimizeIterations
        
            typedef int ( ::SireMove::OpenMMFrEnergyST::*getMinimizeIterations_function_type )(  ) ;
            getMinimizeIterations_function_type getMinimizeIterations_function_value( &::SireMove::OpenMMFrEnergyST::getMinimizeIterations );
            
            OpenMMFrEnergyST_exposer.def( 
                "getMinimizeIterations"
                , getMinimizeIterations_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getMinimizeTol
        
            typedef double ( ::SireMove::OpenMMFrEnergyST::*getMinimizeTol_function_type )(  ) ;
            getMinimizeTol_function_type getMinimizeTol_function_value( &::SireMove::OpenMMFrEnergyST::getMinimizeTol );
            
            OpenMMFrEnergyST_exposer.def( 
                "getMinimizeTol"
                , getMinimizeTol_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getPlatform
        
            typedef ::QString ( ::SireMove::OpenMMFrEnergyST::*getPlatform_function_type )(  ) ;
            getPlatform_function_type getPlatform_function_value( &::SireMove::OpenMMFrEnergyST::getPlatform );
            
            OpenMMFrEnergyST_exposer.def( 
                "getPlatform"
                , getPlatform_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getPrecision
        
            typedef ::QString ( ::SireMove::OpenMMFrEnergyST::*getPrecision_function_type )(  ) ;
            getPrecision_function_type getPrecision_function_value( &::SireMove::OpenMMFrEnergyST::getPrecision );
            
            OpenMMFrEnergyST_exposer.def( 
                "getPrecision"
                , getPrecision_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getPressure
        
            typedef ::SireUnits::Dimension::Pressure ( ::SireMove::OpenMMFrEnergyST::*getPressure_function_type )(  ) ;
            getPressure_function_type getPressure_function_value( &::SireMove::OpenMMFrEnergyST::getPressure );
            
            OpenMMFrEnergyST_exposer.def( 
                "getPressure"
                , getPressure_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getRestraint
        
            typedef bool ( ::SireMove::OpenMMFrEnergyST::*getRestraint_function_type )(  ) ;
            getRestraint_function_type getRestraint_function_value( &::SireMove::OpenMMFrEnergyST::getRestraint );
            
            OpenMMFrEnergyST_exposer.def( 
                "getRestraint"
                , getRestraint_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getShift_delta
        
            typedef double ( ::SireMove::OpenMMFrEnergyST::*getShift_delta_function_type )(  ) ;
            getShift_delta_function_type getShift_delta_function_value( &::SireMove::OpenMMFrEnergyST::getShift_delta );
            
            OpenMMFrEnergyST_exposer.def( 
                "getShift_delta"
                , getShift_delta_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getTemperature
        
            typedef ::SireUnits::Dimension::Temperature ( ::SireMove::OpenMMFrEnergyST::*getTemperature_function_type )(  ) ;
            getTemperature_function_type getTemperature_function_value( &::SireMove::OpenMMFrEnergyST::getTemperature );
            
            OpenMMFrEnergyST_exposer.def( 
                "getTemperature"
                , getTemperature_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::getTimetoSkip
        
            typedef ::SireUnits::Dimension::Time ( ::SireMove::OpenMMFrEnergyST::*getTimetoSkip_function_type )(  ) ;
            getTimetoSkip_function_type getTimetoSkip_function_value( &::SireMove::OpenMMFrEnergyST::getTimetoSkip );
            
            OpenMMFrEnergyST_exposer.def( 
                "getTimetoSkip"
                , getTimetoSkip_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::initialise
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*initialise_function_type )(  ) ;
            initialise_function_type initialise_function_value( &::SireMove::OpenMMFrEnergyST::initialise );
            
            OpenMMFrEnergyST_exposer.def( 
                "initialise"
                , initialise_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::integrate
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*integrate_function_type )( ::SireMove::IntegratorWorkspace &,::SireCAS::Symbol const &,::SireUnits::Dimension::Time,int,bool ) ;
            integrate_function_type integrate_function_value( &::SireMove::OpenMMFrEnergyST::integrate );
            
            OpenMMFrEnergyST_exposer.def( 
                "integrate"
                , integrate_function_value
                , ( bp::arg("workspace"), bp::arg("nrg_component"), bp::arg("timestep"), bp::arg("nmoves"), bp::arg("record_stats") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::isTimeReversible
        
            typedef bool ( ::SireMove::OpenMMFrEnergyST::*isTimeReversible_function_type )(  ) const;
            isTimeReversible_function_type isTimeReversible_function_value( &::SireMove::OpenMMFrEnergyST::isTimeReversible );
            
            OpenMMFrEnergyST_exposer.def( 
                "isTimeReversible"
                , isTimeReversible_function_value );
        
        }
        OpenMMFrEnergyST_exposer.def( bp::self != bp::self );
        { //::SireMove::OpenMMFrEnergyST::operator=
        
            typedef ::SireMove::OpenMMFrEnergyST & ( ::SireMove::OpenMMFrEnergyST::*assign_function_type )( ::SireMove::OpenMMFrEnergyST const & ) ;
            assign_function_type assign_function_value( &::SireMove::OpenMMFrEnergyST::operator= );
            
            OpenMMFrEnergyST_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        OpenMMFrEnergyST_exposer.def( bp::self == bp::self );
        { //::SireMove::OpenMMFrEnergyST::setAlchemical_value
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setAlchemical_value_function_type )( double ) ;
            setAlchemical_value_function_type setAlchemical_value_function_value( &::SireMove::OpenMMFrEnergyST::setAlchemical_value );
            
            OpenMMFrEnergyST_exposer.def( 
                "setAlchemical_value"
                , setAlchemical_value_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setAndersen
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setAndersen_function_type )( bool ) ;
            setAndersen_function_type setAndersen_function_value( &::SireMove::OpenMMFrEnergyST::setAndersen );
            
            OpenMMFrEnergyST_exposer.def( 
                "setAndersen"
                , setAndersen_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setAndersen_frequency
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setAndersen_frequency_function_type )( double ) ;
            setAndersen_frequency_function_type setAndersen_frequency_function_value( &::SireMove::OpenMMFrEnergyST::setAndersen_frequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "setAndersen_frequency"
                , setAndersen_frequency_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setBufferFrequency
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setBufferFrequency_function_type )( int ) ;
            setBufferFrequency_function_type setBufferFrequency_function_value( &::SireMove::OpenMMFrEnergyST::setBufferFrequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "setBufferFrequency"
                , setBufferFrequency_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setCMMremoval_frequency
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setCMMremoval_frequency_function_type )( int ) ;
            setCMMremoval_frequency_function_type setCMMremoval_frequency_function_value( &::SireMove::OpenMMFrEnergyST::setCMMremoval_frequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "setCMMremoval_frequency"
                , setCMMremoval_frequency_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setConstraintType
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setConstraintType_function_type )( ::QString ) ;
            setConstraintType_function_type setConstraintType_function_value( &::SireMove::OpenMMFrEnergyST::setConstraintType );
            
            OpenMMFrEnergyST_exposer.def( 
                "setConstraintType"
                , setConstraintType_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setCoulomb_power
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setCoulomb_power_function_type )( float ) ;
            setCoulomb_power_function_type setCoulomb_power_function_value( &::SireMove::OpenMMFrEnergyST::setCoulomb_power );
            
            OpenMMFrEnergyST_exposer.def( 
                "setCoulomb_power"
                , setCoulomb_power_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setCutoffType
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setCutoffType_function_type )( ::QString ) ;
            setCutoffType_function_type setCutoffType_function_value( &::SireMove::OpenMMFrEnergyST::setCutoffType );
            
            OpenMMFrEnergyST_exposer.def( 
                "setCutoffType"
                , setCutoffType_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setCutoff_distance
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setCutoff_distance_function_type )( ::SireUnits::Dimension::Length ) ;
            setCutoff_distance_function_type setCutoff_distance_function_value( &::SireMove::OpenMMFrEnergyST::setCutoff_distance );
            
            OpenMMFrEnergyST_exposer.def( 
                "setCutoff_distance"
                , setCutoff_distance_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setDeltatAlchemical
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setDeltatAlchemical_function_type )( double ) ;
            setDeltatAlchemical_function_type setDeltatAlchemical_function_value( &::SireMove::OpenMMFrEnergyST::setDeltatAlchemical );
            
            OpenMMFrEnergyST_exposer.def( 
                "setDeltatAlchemical"
                , setDeltatAlchemical_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setDeviceIndex
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setDeviceIndex_function_type )( ::QString ) ;
            setDeviceIndex_function_type setDeviceIndex_function_value( &::SireMove::OpenMMFrEnergyST::setDeviceIndex );
            
            OpenMMFrEnergyST_exposer.def( 
                "setDeviceIndex"
                , setDeviceIndex_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setEnergyFrequency
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setEnergyFrequency_function_type )( int ) ;
            setEnergyFrequency_function_type setEnergyFrequency_function_value( &::SireMove::OpenMMFrEnergyST::setEnergyFrequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "setEnergyFrequency"
                , setEnergyFrequency_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setEquilib_iterations
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setEquilib_iterations_function_type )( int ) ;
            setEquilib_iterations_function_type setEquilib_iterations_function_value( &::SireMove::OpenMMFrEnergyST::setEquilib_iterations );
            
            OpenMMFrEnergyST_exposer.def( 
                "setEquilib_iterations"
                , setEquilib_iterations_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setEquilib_time_step
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setEquilib_time_step_function_type )( ::SireUnits::Dimension::Time ) ;
            setEquilib_time_step_function_type setEquilib_time_step_function_value( &::SireMove::OpenMMFrEnergyST::setEquilib_time_step );
            
            OpenMMFrEnergyST_exposer.def( 
                "setEquilib_time_step"
                , setEquilib_time_step_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setField_dielectric
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setField_dielectric_function_type )( double ) ;
            setField_dielectric_function_type setField_dielectric_function_value( &::SireMove::OpenMMFrEnergyST::setField_dielectric );
            
            OpenMMFrEnergyST_exposer.def( 
                "setField_dielectric"
                , setField_dielectric_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setFriction
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setFriction_function_type )( ::SireUnits::Dimension::Time ) ;
            setFriction_function_type setFriction_function_value( &::SireMove::OpenMMFrEnergyST::setFriction );
            
            OpenMMFrEnergyST_exposer.def( 
                "setFriction"
                , setFriction_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setIntegration_tollerance
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setIntegration_tollerance_function_type )( double ) ;
            setIntegration_tollerance_function_type setIntegration_tollerance_function_value( &::SireMove::OpenMMFrEnergyST::setIntegration_tollerance );
            
            OpenMMFrEnergyST_exposer.def( 
                "setIntegration_tollerance"
                , setIntegration_tollerance_function_value
                , ( bp::arg("tollerance") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setIntegrator
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setIntegrator_function_type )( ::QString ) ;
            setIntegrator_function_type setIntegrator_function_value( &::SireMove::OpenMMFrEnergyST::setIntegrator );
            
            OpenMMFrEnergyST_exposer.def( 
                "setIntegrator"
                , setIntegrator_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setMCBarostat
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setMCBarostat_function_type )( bool ) ;
            setMCBarostat_function_type setMCBarostat_function_value( &::SireMove::OpenMMFrEnergyST::setMCBarostat );
            
            OpenMMFrEnergyST_exposer.def( 
                "setMCBarostat"
                , setMCBarostat_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setMCBarostat_frequency
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setMCBarostat_frequency_function_type )( int ) ;
            setMCBarostat_frequency_function_type setMCBarostat_frequency_function_value( &::SireMove::OpenMMFrEnergyST::setMCBarostat_frequency );
            
            OpenMMFrEnergyST_exposer.def( 
                "setMCBarostat_frequency"
                , setMCBarostat_frequency_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setMinimization
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setMinimization_function_type )( bool ) ;
            setMinimization_function_type setMinimization_function_value( &::SireMove::OpenMMFrEnergyST::setMinimization );
            
            OpenMMFrEnergyST_exposer.def( 
                "setMinimization"
                , setMinimization_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setMinimizeIterations
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setMinimizeIterations_function_type )( int ) ;
            setMinimizeIterations_function_type setMinimizeIterations_function_value( &::SireMove::OpenMMFrEnergyST::setMinimizeIterations );
            
            OpenMMFrEnergyST_exposer.def( 
                "setMinimizeIterations"
                , setMinimizeIterations_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setMinimizeTol
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setMinimizeTol_function_type )( double ) ;
            setMinimizeTol_function_type setMinimizeTol_function_value( &::SireMove::OpenMMFrEnergyST::setMinimizeTol );
            
            OpenMMFrEnergyST_exposer.def( 
                "setMinimizeTol"
                , setMinimizeTol_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setPlatform
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setPlatform_function_type )( ::QString ) ;
            setPlatform_function_type setPlatform_function_value( &::SireMove::OpenMMFrEnergyST::setPlatform );
            
            OpenMMFrEnergyST_exposer.def( 
                "setPlatform"
                , setPlatform_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setPrecision
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setPrecision_function_type )( ::QString ) ;
            setPrecision_function_type setPrecision_function_value( &::SireMove::OpenMMFrEnergyST::setPrecision );
            
            OpenMMFrEnergyST_exposer.def( 
                "setPrecision"
                , setPrecision_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setPressure
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setPressure_function_type )( ::SireUnits::Dimension::Pressure ) ;
            setPressure_function_type setPressure_function_value( &::SireMove::OpenMMFrEnergyST::setPressure );
            
            OpenMMFrEnergyST_exposer.def( 
                "setPressure"
                , setPressure_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setReinitializeContext
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setReinitializeContext_function_type )( bool ) ;
            setReinitializeContext_function_type setReinitializeContext_function_value( &::SireMove::OpenMMFrEnergyST::setReinitializeContext );
            
            OpenMMFrEnergyST_exposer.def( 
                "setReinitializeContext"
                , setReinitializeContext_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setRestraint
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setRestraint_function_type )( bool ) ;
            setRestraint_function_type setRestraint_function_value( &::SireMove::OpenMMFrEnergyST::setRestraint );
            
            OpenMMFrEnergyST_exposer.def( 
                "setRestraint"
                , setRestraint_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setShift_delta
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setShift_delta_function_type )( double ) ;
            setShift_delta_function_type setShift_delta_function_value( &::SireMove::OpenMMFrEnergyST::setShift_delta );
            
            OpenMMFrEnergyST_exposer.def( 
                "setShift_delta"
                , setShift_delta_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setTemperature
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setTemperature_function_type )( ::SireUnits::Dimension::Temperature ) ;
            setTemperature_function_type setTemperature_function_value( &::SireMove::OpenMMFrEnergyST::setTemperature );
            
            OpenMMFrEnergyST_exposer.def( 
                "setTemperature"
                , setTemperature_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::setTimetoSkip
        
            typedef void ( ::SireMove::OpenMMFrEnergyST::*setTimetoSkip_function_type )( ::SireUnits::Dimension::Time ) ;
            setTimetoSkip_function_type setTimetoSkip_function_value( &::SireMove::OpenMMFrEnergyST::setTimetoSkip );
            
            OpenMMFrEnergyST_exposer.def( 
                "setTimetoSkip"
                , setTimetoSkip_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::SireMove::OpenMMFrEnergyST::toString
        
            typedef ::QString ( ::SireMove::OpenMMFrEnergyST::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMove::OpenMMFrEnergyST::toString );
            
            OpenMMFrEnergyST_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMove::OpenMMFrEnergyST::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMove::OpenMMFrEnergyST::typeName );
            
            OpenMMFrEnergyST_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        OpenMMFrEnergyST_exposer.staticmethod( "typeName" );
        OpenMMFrEnergyST_exposer.def( "__copy__", &__copy__);
        OpenMMFrEnergyST_exposer.def( "__deepcopy__", &__copy__);
        OpenMMFrEnergyST_exposer.def( "clone", &__copy__);
        OpenMMFrEnergyST_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMove::OpenMMFrEnergyST >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        OpenMMFrEnergyST_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMove::OpenMMFrEnergyST >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        OpenMMFrEnergyST_exposer.def( "__str__", &__str__< ::SireMove::OpenMMFrEnergyST > );
        OpenMMFrEnergyST_exposer.def( "__repr__", &__str__< ::SireMove::OpenMMFrEnergyST > );
    }

}
