// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "PartialMolecule.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "atom.h"

#include "chain.h"

#include "cutgroup.h"

#include "editor.hpp"

#include "evaluator.h"

#include "molecule.h"

#include "mover.hpp"

#include "partialmolecule.h"

#include "residue.h"

#include "segment.h"

#include "selector.hpp"

#include "tostring.h"

#include <QDebug>

#include "partialmolecule.h"

SireMol::PartialMolecule __copy__(const SireMol::PartialMolecule &other){ return SireMol::PartialMolecule(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_PartialMolecule_class(){

    { //::SireMol::PartialMolecule
        typedef bp::class_< SireMol::PartialMolecule, bp::bases< SireMol::MoleculeView, SireBase::Property > > PartialMolecule_exposer_t;
        PartialMolecule_exposer_t PartialMolecule_exposer = PartialMolecule_exposer_t( "PartialMolecule", bp::init< >() );
        bp::scope PartialMolecule_scope( PartialMolecule_exposer );
        PartialMolecule_exposer.def( bp::init< SireMol::MoleculeView const & >(( bp::arg("molecule") )) );
        PartialMolecule_exposer.def( bp::init< SireMol::MoleculeData const &, SireMol::AtomSelection const & >(( bp::arg("moldata"), bp::arg("atoms") )) );
        PartialMolecule_exposer.def( bp::init< SireMol::PartialMolecule const & >(( bp::arg("other") )) );
        { //::SireMol::PartialMolecule::atom
        
            typedef ::SireMol::Atom ( ::SireMol::PartialMolecule::*atom_function_type )( ::SireMol::AtomIdx ) const;
            atom_function_type atom_function_value( &::SireMol::PartialMolecule::atom );
            
            PartialMolecule_exposer.def( 
                "atom"
                , atom_function_value
                , ( bp::arg("atomidx") ) );
        
        }
        { //::SireMol::PartialMolecule::atom
        
            typedef ::SireMol::Atom ( ::SireMol::PartialMolecule::*atom_function_type )( ::SireMol::AtomID const & ) const;
            atom_function_type atom_function_value( &::SireMol::PartialMolecule::atom );
            
            PartialMolecule_exposer.def( 
                "atom"
                , atom_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::PartialMolecule::atoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::PartialMolecule::*atoms_function_type )( ::SireMol::AtomID const & ) const;
            atoms_function_type atoms_function_value( &::SireMol::PartialMolecule::atoms );
            
            PartialMolecule_exposer.def( 
                "atoms"
                , atoms_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::PartialMolecule::atoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::PartialMolecule::*atoms_function_type )(  ) const;
            atoms_function_type atoms_function_value( &::SireMol::PartialMolecule::atoms );
            
            PartialMolecule_exposer.def( 
                "atoms"
                , atoms_function_value );
        
        }
        { //::SireMol::PartialMolecule::chain
        
            typedef ::SireMol::Chain ( ::SireMol::PartialMolecule::*chain_function_type )( ::SireMol::ChainID const & ) const;
            chain_function_type chain_function_value( &::SireMol::PartialMolecule::chain );
            
            PartialMolecule_exposer.def( 
                "chain"
                , chain_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::PartialMolecule::chains
        
            typedef ::SireMol::Selector< SireMol::Chain > ( ::SireMol::PartialMolecule::*chains_function_type )( ::SireMol::ChainID const & ) const;
            chains_function_type chains_function_value( &::SireMol::PartialMolecule::chains );
            
            PartialMolecule_exposer.def( 
                "chains"
                , chains_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::PartialMolecule::chains
        
            typedef ::SireMol::Selector< SireMol::Chain > ( ::SireMol::PartialMolecule::*chains_function_type )(  ) const;
            chains_function_type chains_function_value( &::SireMol::PartialMolecule::chains );
            
            PartialMolecule_exposer.def( 
                "chains"
                , chains_function_value );
        
        }
        { //::SireMol::PartialMolecule::cutGroup
        
            typedef ::SireMol::CutGroup ( ::SireMol::PartialMolecule::*cutGroup_function_type )( ::SireMol::CGID const & ) const;
            cutGroup_function_type cutGroup_function_value( &::SireMol::PartialMolecule::cutGroup );
            
            PartialMolecule_exposer.def( 
                "cutGroup"
                , cutGroup_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::PartialMolecule::cutGroups
        
            typedef ::SireMol::Selector< SireMol::CutGroup > ( ::SireMol::PartialMolecule::*cutGroups_function_type )( ::SireMol::CGID const & ) const;
            cutGroups_function_type cutGroups_function_value( &::SireMol::PartialMolecule::cutGroups );
            
            PartialMolecule_exposer.def( 
                "cutGroups"
                , cutGroups_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::PartialMolecule::cutGroups
        
            typedef ::SireMol::Selector< SireMol::CutGroup > ( ::SireMol::PartialMolecule::*cutGroups_function_type )(  ) const;
            cutGroups_function_type cutGroups_function_value( &::SireMol::PartialMolecule::cutGroups );
            
            PartialMolecule_exposer.def( 
                "cutGroups"
                , cutGroups_function_value );
        
        }
        { //::SireMol::PartialMolecule::evaluate
        
            typedef ::SireMol::Evaluator ( ::SireMol::PartialMolecule::*evaluate_function_type )(  ) const;
            evaluate_function_type evaluate_function_value( &::SireMol::PartialMolecule::evaluate );
            
            PartialMolecule_exposer.def( 
                "evaluate"
                , evaluate_function_value );
        
        }
        { //::SireMol::PartialMolecule::hasMetadata
        
            typedef bool ( ::SireMol::PartialMolecule::*hasMetadata_function_type )( ::SireBase::PropertyName const & ) const;
            hasMetadata_function_type hasMetadata_function_value( &::SireMol::PartialMolecule::hasMetadata );
            
            PartialMolecule_exposer.def( 
                "hasMetadata"
                , hasMetadata_function_value
                , ( bp::arg("metakey") ) );
        
        }
        { //::SireMol::PartialMolecule::hasMetadata
        
            typedef bool ( ::SireMol::PartialMolecule::*hasMetadata_function_type )( ::SireBase::PropertyName const &,::SireBase::PropertyName const & ) const;
            hasMetadata_function_type hasMetadata_function_value( &::SireMol::PartialMolecule::hasMetadata );
            
            PartialMolecule_exposer.def( 
                "hasMetadata"
                , hasMetadata_function_value
                , ( bp::arg("key"), bp::arg("metakey") ) );
        
        }
        { //::SireMol::PartialMolecule::hasProperty
        
            typedef bool ( ::SireMol::PartialMolecule::*hasProperty_function_type )( ::SireBase::PropertyName const & ) const;
            hasProperty_function_type hasProperty_function_value( &::SireMol::PartialMolecule::hasProperty );
            
            PartialMolecule_exposer.def( 
                "hasProperty"
                , hasProperty_function_value
                , ( bp::arg("key") ) );
        
        }
        { //::SireMol::PartialMolecule::isEmpty
        
            typedef bool ( ::SireMol::PartialMolecule::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::PartialMolecule::isEmpty );
            
            PartialMolecule_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::PartialMolecule::metadata
        
            typedef ::SireBase::Property const & ( ::SireMol::PartialMolecule::*metadata_function_type )( ::SireBase::PropertyName const & ) const;
            metadata_function_type metadata_function_value( &::SireMol::PartialMolecule::metadata );
            
            PartialMolecule_exposer.def( 
                "metadata"
                , metadata_function_value
                , ( bp::arg("metakey") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::PartialMolecule::metadata
        
            typedef ::SireBase::Property const & ( ::SireMol::PartialMolecule::*metadata_function_type )( ::SireBase::PropertyName const &,::SireBase::PropertyName const & ) const;
            metadata_function_type metadata_function_value( &::SireMol::PartialMolecule::metadata );
            
            PartialMolecule_exposer.def( 
                "metadata"
                , metadata_function_value
                , ( bp::arg("key"), bp::arg("metakey") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::PartialMolecule::metadataKeys
        
            typedef ::QStringList ( ::SireMol::PartialMolecule::*metadataKeys_function_type )(  ) const;
            metadataKeys_function_type metadataKeys_function_value( &::SireMol::PartialMolecule::metadataKeys );
            
            PartialMolecule_exposer.def( 
                "metadataKeys"
                , metadataKeys_function_value );
        
        }
        { //::SireMol::PartialMolecule::metadataKeys
        
            typedef ::QStringList ( ::SireMol::PartialMolecule::*metadataKeys_function_type )( ::SireBase::PropertyName const & ) const;
            metadataKeys_function_type metadataKeys_function_value( &::SireMol::PartialMolecule::metadataKeys );
            
            PartialMolecule_exposer.def( 
                "metadataKeys"
                , metadataKeys_function_value
                , ( bp::arg("key") ) );
        
        }
        { //::SireMol::PartialMolecule::molecule
        
            typedef ::SireMol::Molecule ( ::SireMol::PartialMolecule::*molecule_function_type )(  ) const;
            molecule_function_type molecule_function_value( &::SireMol::PartialMolecule::molecule );
            
            PartialMolecule_exposer.def( 
                "molecule"
                , molecule_function_value );
        
        }
        { //::SireMol::PartialMolecule::move
        
            typedef ::SireMol::Mover< SireMol::PartialMolecule > ( ::SireMol::PartialMolecule::*move_function_type )(  ) const;
            move_function_type move_function_value( &::SireMol::PartialMolecule::move );
            
            PartialMolecule_exposer.def( 
                "move"
                , move_function_value );
        
        }
        { //::SireMol::PartialMolecule::nAtoms
        
            typedef int ( ::SireMol::PartialMolecule::*nAtoms_function_type )(  ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::PartialMolecule::nAtoms );
            
            PartialMolecule_exposer.def( 
                "nAtoms"
                , nAtoms_function_value );
        
        }
        { //::SireMol::PartialMolecule::nChains
        
            typedef int ( ::SireMol::PartialMolecule::*nChains_function_type )(  ) const;
            nChains_function_type nChains_function_value( &::SireMol::PartialMolecule::nChains );
            
            PartialMolecule_exposer.def( 
                "nChains"
                , nChains_function_value );
        
        }
        { //::SireMol::PartialMolecule::nCutGroups
        
            typedef int ( ::SireMol::PartialMolecule::*nCutGroups_function_type )(  ) const;
            nCutGroups_function_type nCutGroups_function_value( &::SireMol::PartialMolecule::nCutGroups );
            
            PartialMolecule_exposer.def( 
                "nCutGroups"
                , nCutGroups_function_value );
        
        }
        { //::SireMol::PartialMolecule::nResidues
        
            typedef int ( ::SireMol::PartialMolecule::*nResidues_function_type )(  ) const;
            nResidues_function_type nResidues_function_value( &::SireMol::PartialMolecule::nResidues );
            
            PartialMolecule_exposer.def( 
                "nResidues"
                , nResidues_function_value );
        
        }
        { //::SireMol::PartialMolecule::nSegments
        
            typedef int ( ::SireMol::PartialMolecule::*nSegments_function_type )(  ) const;
            nSegments_function_type nSegments_function_value( &::SireMol::PartialMolecule::nSegments );
            
            PartialMolecule_exposer.def( 
                "nSegments"
                , nSegments_function_value );
        
        }
        { //::SireMol::PartialMolecule::name
        
            typedef ::SireMol::MolName const & ( ::SireMol::PartialMolecule::*name_function_type )(  ) const;
            name_function_type name_function_value( &::SireMol::PartialMolecule::name );
            
            PartialMolecule_exposer.def( 
                "name"
                , name_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::PartialMolecule::number
        
            typedef ::SireMol::MolNum ( ::SireMol::PartialMolecule::*number_function_type )(  ) const;
            number_function_type number_function_value( &::SireMol::PartialMolecule::number );
            
            PartialMolecule_exposer.def( 
                "number"
                , number_function_value );
        
        }
        PartialMolecule_exposer.def( bp::self != bp::self );
        { //::SireMol::PartialMolecule::operator=
        
            typedef ::SireMol::PartialMolecule & ( ::SireMol::PartialMolecule::*assign_function_type )( ::SireMol::MoleculeView const & ) ;
            assign_function_type assign_function_value( &::SireMol::PartialMolecule::operator= );
            
            PartialMolecule_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::PartialMolecule::operator=
        
            typedef ::SireMol::PartialMolecule & ( ::SireMol::PartialMolecule::*assign_function_type )( ::SireMol::PartialMolecule const & ) ;
            assign_function_type assign_function_value( &::SireMol::PartialMolecule::operator= );
            
            PartialMolecule_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        PartialMolecule_exposer.def( bp::self == bp::self );
        { //::SireMol::PartialMolecule::property
        
            typedef ::SireBase::Property const & ( ::SireMol::PartialMolecule::*property_function_type )( ::SireBase::PropertyName const & ) const;
            property_function_type property_function_value( &::SireMol::PartialMolecule::property );
            
            PartialMolecule_exposer.def( 
                "property"
                , property_function_value
                , ( bp::arg("key") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::PartialMolecule::propertyKeys
        
            typedef ::QStringList ( ::SireMol::PartialMolecule::*propertyKeys_function_type )(  ) const;
            propertyKeys_function_type propertyKeys_function_value( &::SireMol::PartialMolecule::propertyKeys );
            
            PartialMolecule_exposer.def( 
                "propertyKeys"
                , propertyKeys_function_value );
        
        }
        { //::SireMol::PartialMolecule::residue
        
            typedef ::SireMol::Residue ( ::SireMol::PartialMolecule::*residue_function_type )( ::SireMol::ResID const & ) const;
            residue_function_type residue_function_value( &::SireMol::PartialMolecule::residue );
            
            PartialMolecule_exposer.def( 
                "residue"
                , residue_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::PartialMolecule::residues
        
            typedef ::SireMol::Selector< SireMol::Residue > ( ::SireMol::PartialMolecule::*residues_function_type )( ::SireMol::ResID const & ) const;
            residues_function_type residues_function_value( &::SireMol::PartialMolecule::residues );
            
            PartialMolecule_exposer.def( 
                "residues"
                , residues_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::PartialMolecule::residues
        
            typedef ::SireMol::Selector< SireMol::Residue > ( ::SireMol::PartialMolecule::*residues_function_type )(  ) const;
            residues_function_type residues_function_value( &::SireMol::PartialMolecule::residues );
            
            PartialMolecule_exposer.def( 
                "residues"
                , residues_function_value );
        
        }
        { //::SireMol::PartialMolecule::segment
        
            typedef ::SireMol::Segment ( ::SireMol::PartialMolecule::*segment_function_type )( ::SireMol::SegID const & ) const;
            segment_function_type segment_function_value( &::SireMol::PartialMolecule::segment );
            
            PartialMolecule_exposer.def( 
                "segment"
                , segment_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::PartialMolecule::segments
        
            typedef ::SireMol::Selector< SireMol::Segment > ( ::SireMol::PartialMolecule::*segments_function_type )( ::SireMol::SegID const & ) const;
            segments_function_type segments_function_value( &::SireMol::PartialMolecule::segments );
            
            PartialMolecule_exposer.def( 
                "segments"
                , segments_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::PartialMolecule::segments
        
            typedef ::SireMol::Selector< SireMol::Segment > ( ::SireMol::PartialMolecule::*segments_function_type )(  ) const;
            segments_function_type segments_function_value( &::SireMol::PartialMolecule::segments );
            
            PartialMolecule_exposer.def( 
                "segments"
                , segments_function_value );
        
        }
        { //::SireMol::PartialMolecule::select
        
            typedef ::SireMol::Atom ( ::SireMol::PartialMolecule::*select_function_type )( ::SireMol::AtomIdx ) const;
            select_function_type select_function_value( &::SireMol::PartialMolecule::select );
            
            PartialMolecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("atomidx") ) );
        
        }
        { //::SireMol::PartialMolecule::select
        
            typedef ::SireMol::Atom ( ::SireMol::PartialMolecule::*select_function_type )( ::SireMol::AtomID const & ) const;
            select_function_type select_function_value( &::SireMol::PartialMolecule::select );
            
            PartialMolecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::PartialMolecule::select
        
            typedef ::SireMol::CutGroup ( ::SireMol::PartialMolecule::*select_function_type )( ::SireMol::CGID const & ) const;
            select_function_type select_function_value( &::SireMol::PartialMolecule::select );
            
            PartialMolecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::PartialMolecule::select
        
            typedef ::SireMol::Residue ( ::SireMol::PartialMolecule::*select_function_type )( ::SireMol::ResID const & ) const;
            select_function_type select_function_value( &::SireMol::PartialMolecule::select );
            
            PartialMolecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::PartialMolecule::select
        
            typedef ::SireMol::Chain ( ::SireMol::PartialMolecule::*select_function_type )( ::SireMol::ChainID const & ) const;
            select_function_type select_function_value( &::SireMol::PartialMolecule::select );
            
            PartialMolecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::PartialMolecule::select
        
            typedef ::SireMol::Segment ( ::SireMol::PartialMolecule::*select_function_type )( ::SireMol::SegID const & ) const;
            select_function_type select_function_value( &::SireMol::PartialMolecule::select );
            
            PartialMolecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::PartialMolecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::PartialMolecule::*selectAll_function_type )( ::SireMol::AtomID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::PartialMolecule::selectAll );
            
            PartialMolecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::PartialMolecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::CutGroup > ( ::SireMol::PartialMolecule::*selectAll_function_type )( ::SireMol::CGID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::PartialMolecule::selectAll );
            
            PartialMolecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::PartialMolecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::Residue > ( ::SireMol::PartialMolecule::*selectAll_function_type )( ::SireMol::ResID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::PartialMolecule::selectAll );
            
            PartialMolecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::PartialMolecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::Chain > ( ::SireMol::PartialMolecule::*selectAll_function_type )( ::SireMol::ChainID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::PartialMolecule::selectAll );
            
            PartialMolecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::PartialMolecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::Segment > ( ::SireMol::PartialMolecule::*selectAll_function_type )( ::SireMol::SegID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::PartialMolecule::selectAll );
            
            PartialMolecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::PartialMolecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::PartialMolecule::*selectAll_function_type )(  ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::PartialMolecule::selectAll );
            
            PartialMolecule_exposer.def( 
                "selectAll"
                , selectAll_function_value );
        
        }
        { //::SireMol::PartialMolecule::selectAllAtoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::PartialMolecule::*selectAllAtoms_function_type )(  ) const;
            selectAllAtoms_function_type selectAllAtoms_function_value( &::SireMol::PartialMolecule::selectAllAtoms );
            
            PartialMolecule_exposer.def( 
                "selectAllAtoms"
                , selectAllAtoms_function_value );
        
        }
        { //::SireMol::PartialMolecule::selectAllChains
        
            typedef ::SireMol::Selector< SireMol::Chain > ( ::SireMol::PartialMolecule::*selectAllChains_function_type )(  ) const;
            selectAllChains_function_type selectAllChains_function_value( &::SireMol::PartialMolecule::selectAllChains );
            
            PartialMolecule_exposer.def( 
                "selectAllChains"
                , selectAllChains_function_value );
        
        }
        { //::SireMol::PartialMolecule::selectAllCutGroups
        
            typedef ::SireMol::Selector< SireMol::CutGroup > ( ::SireMol::PartialMolecule::*selectAllCutGroups_function_type )(  ) const;
            selectAllCutGroups_function_type selectAllCutGroups_function_value( &::SireMol::PartialMolecule::selectAllCutGroups );
            
            PartialMolecule_exposer.def( 
                "selectAllCutGroups"
                , selectAllCutGroups_function_value );
        
        }
        { //::SireMol::PartialMolecule::selectAllResidues
        
            typedef ::SireMol::Selector< SireMol::Residue > ( ::SireMol::PartialMolecule::*selectAllResidues_function_type )(  ) const;
            selectAllResidues_function_type selectAllResidues_function_value( &::SireMol::PartialMolecule::selectAllResidues );
            
            PartialMolecule_exposer.def( 
                "selectAllResidues"
                , selectAllResidues_function_value );
        
        }
        { //::SireMol::PartialMolecule::selectAllSegments
        
            typedef ::SireMol::Selector< SireMol::Segment > ( ::SireMol::PartialMolecule::*selectAllSegments_function_type )(  ) const;
            selectAllSegments_function_type selectAllSegments_function_value( &::SireMol::PartialMolecule::selectAllSegments );
            
            PartialMolecule_exposer.def( 
                "selectAllSegments"
                , selectAllSegments_function_value );
        
        }
        { //::SireMol::PartialMolecule::selectedAll
        
            typedef bool ( ::SireMol::PartialMolecule::*selectedAll_function_type )(  ) const;
            selectedAll_function_type selectedAll_function_value( &::SireMol::PartialMolecule::selectedAll );
            
            PartialMolecule_exposer.def( 
                "selectedAll"
                , selectedAll_function_value );
        
        }
        { //::SireMol::PartialMolecule::selection
        
            typedef ::SireMol::AtomSelection ( ::SireMol::PartialMolecule::*selection_function_type )(  ) const;
            selection_function_type selection_function_value( &::SireMol::PartialMolecule::selection );
            
            PartialMolecule_exposer.def( 
                "selection"
                , selection_function_value );
        
        }
        { //::SireMol::PartialMolecule::toString
        
            typedef ::QString ( ::SireMol::PartialMolecule::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::PartialMolecule::toString );
            
            PartialMolecule_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::PartialMolecule::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::PartialMolecule::typeName );
            
            PartialMolecule_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::PartialMolecule::version
        
            typedef ::quint64 ( ::SireMol::PartialMolecule::*version_function_type )(  ) const;
            version_function_type version_function_value( &::SireMol::PartialMolecule::version );
            
            PartialMolecule_exposer.def( 
                "version"
                , version_function_value );
        
        }
        { //::SireMol::PartialMolecule::version
        
            typedef ::quint64 ( ::SireMol::PartialMolecule::*version_function_type )( ::SireBase::PropertyName const & ) const;
            version_function_type version_function_value( &::SireMol::PartialMolecule::version );
            
            PartialMolecule_exposer.def( 
                "version"
                , version_function_value
                , ( bp::arg("key") ) );
        
        }
        PartialMolecule_exposer.staticmethod( "typeName" );
        PartialMolecule_exposer.def( "__copy__", &__copy__);
        PartialMolecule_exposer.def( "__deepcopy__", &__copy__);
        PartialMolecule_exposer.def( "clone", &__copy__);
        PartialMolecule_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::PartialMolecule >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        PartialMolecule_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::PartialMolecule >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        PartialMolecule_exposer.def( "__str__", &__str__< ::SireMol::PartialMolecule > );
        PartialMolecule_exposer.def( "__repr__", &__str__< ::SireMol::PartialMolecule > );
    }

}
