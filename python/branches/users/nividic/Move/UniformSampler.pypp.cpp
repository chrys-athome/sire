// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "UniformSampler.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/errors.h"

#include "SireMol/molecule.h"

#include "SireMol/partialmolecule.h"

#include "SireStream/datastream.h"

#include "SireSystem/system.h"

#include "uniformsampler.h"

#include <QMutex>

#include "uniformsampler.h"

SireMove::UniformSampler __copy__(const SireMove::UniformSampler &other){ return SireMove::UniformSampler(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_UniformSampler_class(){

    { //::SireMove::UniformSampler
        typedef bp::class_< SireMove::UniformSampler, bp::bases< SireMove::Sampler, SireBase::Property > > UniformSampler_exposer_t;
        UniformSampler_exposer_t UniformSampler_exposer = UniformSampler_exposer_t( "UniformSampler", bp::init< >() );
        bp::scope UniformSampler_scope( UniformSampler_exposer );
        UniformSampler_exposer.def( bp::init< SireMol::MoleculeGroup const & >(( bp::arg("molgroup") )) );
        UniformSampler_exposer.def( bp::init< SireMove::UniformSampler const & >(( bp::arg("other") )) );
        { //::SireMove::UniformSampler::operator=
        
            typedef ::SireMove::UniformSampler & ( ::SireMove::UniformSampler::*assign_function_type )( ::SireMove::UniformSampler const & ) ;
            assign_function_type assign_function_value( &::SireMove::UniformSampler::operator= );
            
            UniformSampler_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMove::UniformSampler::probabilityOf
        
            typedef double ( ::SireMove::UniformSampler::*probabilityOf_function_type )( ::SireMol::PartialMolecule const & ) const;
            probabilityOf_function_type probabilityOf_function_value( &::SireMove::UniformSampler::probabilityOf );
            
            UniformSampler_exposer.def( 
                "probabilityOf"
                , probabilityOf_function_value
                , ( bp::arg("molecule") ) );
        
        }
        { //::SireMove::UniformSampler::probabilityOfMolecule
        
            typedef double ( ::SireMove::UniformSampler::*probabilityOfMolecule_function_type )( ::SireMol::Molecule const & ) const;
            probabilityOfMolecule_function_type probabilityOfMolecule_function_value( &::SireMove::UniformSampler::probabilityOfMolecule );
            
            UniformSampler_exposer.def( 
                "probabilityOfMolecule"
                , probabilityOfMolecule_function_value
                , ( bp::arg("molecule") ) );
        
        }
        { //::SireMove::UniformSampler::sample
        
            typedef ::boost::tuples::tuple< SireMol::PartialMolecule, double, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMove::UniformSampler::*sample_function_type )(  ) const;
            sample_function_type sample_function_value( &::SireMove::UniformSampler::sample );
            
            UniformSampler_exposer.def( 
                "sample"
                , sample_function_value );
        
        }
        { //::SireMove::UniformSampler::sampleMolecule
        
            typedef ::boost::tuples::tuple< SireMol::Molecule, double, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMove::UniformSampler::*sampleMolecule_function_type )(  ) const;
            sampleMolecule_function_type sampleMolecule_function_value( &::SireMove::UniformSampler::sampleMolecule );
            
            UniformSampler_exposer.def( 
                "sampleMolecule"
                , sampleMolecule_function_value );
        
        }
        { //::SireMove::UniformSampler::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMove::UniformSampler::typeName );
            
            UniformSampler_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        UniformSampler_exposer.staticmethod( "typeName" );
        UniformSampler_exposer.def( "__copy__", &__copy__);
        UniformSampler_exposer.def( "__deepcopy__", &__copy__);
        UniformSampler_exposer.def( "clone", &__copy__);
        UniformSampler_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMove::UniformSampler >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        UniformSampler_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMove::UniformSampler >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        UniformSampler_exposer.def( "__str__", &__str__< ::SireMove::UniformSampler > );
        UniformSampler_exposer.def( "__repr__", &__str__< ::SireMove::UniformSampler > );
    }

}
