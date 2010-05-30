// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "Molecule.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "atom.h"

#include "chain.h"

#include "cutgroup.h"

#include "evaluator.h"

#include "molecule.h"

#include "moleditor.h"

#include "molviewproperty.h"

#include "mover.hpp"

#include "residue.h"

#include "segment.h"

#include "selector.hpp"

#include <QDebug>

#include "molecule.h"

SireMol::Molecule __copy__(const SireMol::Molecule &other){ return SireMol::Molecule(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Molecule_class(){

    { //::SireMol::Molecule
        typedef bp::class_< SireMol::Molecule, bp::bases< SireMol::MoleculeView, SireBase::Property > > Molecule_exposer_t;
        Molecule_exposer_t Molecule_exposer = Molecule_exposer_t( "Molecule", bp::init< >() );
        bp::scope Molecule_scope( Molecule_exposer );
        Molecule_exposer.def( bp::init< QString const & >(( bp::arg("molname") )) );
        Molecule_exposer.def( bp::init< SireMol::MoleculeData const & >(( bp::arg("moldata") )) );
        Molecule_exposer.def( bp::init< SireMol::Molecule const & >(( bp::arg("other") )) );
        { //::SireMol::Molecule::assertContainsMetadata
        
            typedef void ( ::SireMol::Molecule::*assertContainsMetadata_function_type )( ::SireBase::PropertyName const & ) const;
            assertContainsMetadata_function_type assertContainsMetadata_function_value( &::SireMol::Molecule::assertContainsMetadata );
            
            Molecule_exposer.def( 
                "assertContainsMetadata"
                , assertContainsMetadata_function_value
                , ( bp::arg("metakey") ) );
        
        }
        { //::SireMol::Molecule::assertContainsMetadata
        
            typedef void ( ::SireMol::Molecule::*assertContainsMetadata_function_type )( ::SireBase::PropertyName const &,::SireBase::PropertyName const & ) const;
            assertContainsMetadata_function_type assertContainsMetadata_function_value( &::SireMol::Molecule::assertContainsMetadata );
            
            Molecule_exposer.def( 
                "assertContainsMetadata"
                , assertContainsMetadata_function_value
                , ( bp::arg("key"), bp::arg("metakey") ) );
        
        }
        { //::SireMol::Molecule::assertContainsProperty
        
            typedef void ( ::SireMol::Molecule::*assertContainsProperty_function_type )( ::SireBase::PropertyName const & ) const;
            assertContainsProperty_function_type assertContainsProperty_function_value( &::SireMol::Molecule::assertContainsProperty );
            
            Molecule_exposer.def( 
                "assertContainsProperty"
                , assertContainsProperty_function_value
                , ( bp::arg("key") ) );
        
        }
        { //::SireMol::Molecule::atom
        
            typedef ::SireMol::Atom ( ::SireMol::Molecule::*atom_function_type )( ::SireMol::AtomID const & ) const;
            atom_function_type atom_function_value( &::SireMol::Molecule::atom );
            
            Molecule_exposer.def( 
                "atom"
                , atom_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::Molecule::atoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::Molecule::*atoms_function_type )( ::SireMol::AtomID const & ) const;
            atoms_function_type atoms_function_value( &::SireMol::Molecule::atoms );
            
            Molecule_exposer.def( 
                "atoms"
                , atoms_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::Molecule::atoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::Molecule::*atoms_function_type )( ::SireMol::ResID const & ) const;
            atoms_function_type atoms_function_value( &::SireMol::Molecule::atoms );
            
            Molecule_exposer.def( 
                "atoms"
                , atoms_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::Molecule::atoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::Molecule::*atoms_function_type )( ::SireMol::ChainID const & ) const;
            atoms_function_type atoms_function_value( &::SireMol::Molecule::atoms );
            
            Molecule_exposer.def( 
                "atoms"
                , atoms_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::Molecule::atoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::Molecule::*atoms_function_type )( ::SireMol::CGID const & ) const;
            atoms_function_type atoms_function_value( &::SireMol::Molecule::atoms );
            
            Molecule_exposer.def( 
                "atoms"
                , atoms_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::Molecule::atoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::Molecule::*atoms_function_type )( ::SireMol::SegID const & ) const;
            atoms_function_type atoms_function_value( &::SireMol::Molecule::atoms );
            
            Molecule_exposer.def( 
                "atoms"
                , atoms_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::Molecule::atoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::Molecule::*atoms_function_type )(  ) const;
            atoms_function_type atoms_function_value( &::SireMol::Molecule::atoms );
            
            Molecule_exposer.def( 
                "atoms"
                , atoms_function_value );
        
        }
        { //::SireMol::Molecule::chain
        
            typedef ::SireMol::Chain ( ::SireMol::Molecule::*chain_function_type )( ::SireMol::ChainID const & ) const;
            chain_function_type chain_function_value( &::SireMol::Molecule::chain );
            
            Molecule_exposer.def( 
                "chain"
                , chain_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::Molecule::chains
        
            typedef ::SireMol::Selector< SireMol::Chain > ( ::SireMol::Molecule::*chains_function_type )( ::SireMol::ChainID const & ) const;
            chains_function_type chains_function_value( &::SireMol::Molecule::chains );
            
            Molecule_exposer.def( 
                "chains"
                , chains_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::Molecule::chains
        
            typedef ::SireMol::Selector< SireMol::Chain > ( ::SireMol::Molecule::*chains_function_type )(  ) ;
            chains_function_type chains_function_value( &::SireMol::Molecule::chains );
            
            Molecule_exposer.def( 
                "chains"
                , chains_function_value );
        
        }
        { //::SireMol::Molecule::cutGroup
        
            typedef ::SireMol::CutGroup ( ::SireMol::Molecule::*cutGroup_function_type )( ::SireMol::CGID const & ) const;
            cutGroup_function_type cutGroup_function_value( &::SireMol::Molecule::cutGroup );
            
            Molecule_exposer.def( 
                "cutGroup"
                , cutGroup_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::Molecule::cutGroups
        
            typedef ::SireMol::Selector< SireMol::CutGroup > ( ::SireMol::Molecule::*cutGroups_function_type )( ::SireMol::CGID const & ) const;
            cutGroups_function_type cutGroups_function_value( &::SireMol::Molecule::cutGroups );
            
            Molecule_exposer.def( 
                "cutGroups"
                , cutGroups_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::Molecule::cutGroups
        
            typedef ::SireMol::Selector< SireMol::CutGroup > ( ::SireMol::Molecule::*cutGroups_function_type )(  ) ;
            cutGroups_function_type cutGroups_function_value( &::SireMol::Molecule::cutGroups );
            
            Molecule_exposer.def( 
                "cutGroups"
                , cutGroups_function_value );
        
        }
        { //::SireMol::Molecule::edit
        
            typedef ::SireMol::MolEditor ( ::SireMol::Molecule::*edit_function_type )(  ) const;
            edit_function_type edit_function_value( &::SireMol::Molecule::edit );
            
            Molecule_exposer.def( 
                "edit"
                , edit_function_value );
        
        }
        { //::SireMol::Molecule::evaluate
        
            typedef ::SireMol::Evaluator ( ::SireMol::Molecule::*evaluate_function_type )(  ) const;
            evaluate_function_type evaluate_function_value( &::SireMol::Molecule::evaluate );
            
            Molecule_exposer.def( 
                "evaluate"
                , evaluate_function_value );
        
        }
        { //::SireMol::Molecule::hasMetadata
        
            typedef bool ( ::SireMol::Molecule::*hasMetadata_function_type )( ::SireBase::PropertyName const & ) const;
            hasMetadata_function_type hasMetadata_function_value( &::SireMol::Molecule::hasMetadata );
            
            Molecule_exposer.def( 
                "hasMetadata"
                , hasMetadata_function_value
                , ( bp::arg("metakey") ) );
        
        }
        { //::SireMol::Molecule::hasMetadata
        
            typedef bool ( ::SireMol::Molecule::*hasMetadata_function_type )( ::SireBase::PropertyName const &,::SireBase::PropertyName const & ) const;
            hasMetadata_function_type hasMetadata_function_value( &::SireMol::Molecule::hasMetadata );
            
            Molecule_exposer.def( 
                "hasMetadata"
                , hasMetadata_function_value
                , ( bp::arg("key"), bp::arg("metakey") ) );
        
        }
        { //::SireMol::Molecule::hasProperty
        
            typedef bool ( ::SireMol::Molecule::*hasProperty_function_type )( ::SireBase::PropertyName const & ) const;
            hasProperty_function_type hasProperty_function_value( &::SireMol::Molecule::hasProperty );
            
            Molecule_exposer.def( 
                "hasProperty"
                , hasProperty_function_value
                , ( bp::arg("key") ) );
        
        }
        { //::SireMol::Molecule::isEmpty
        
            typedef bool ( ::SireMol::Molecule::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::Molecule::isEmpty );
            
            Molecule_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::Molecule::metadata
        
            typedef ::SireBase::Property const & ( ::SireMol::Molecule::*metadata_function_type )( ::SireBase::PropertyName const & ) const;
            metadata_function_type metadata_function_value( &::SireMol::Molecule::metadata );
            
            Molecule_exposer.def( 
                "metadata"
                , metadata_function_value
                , ( bp::arg("metakey") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecule::metadata
        
            typedef ::SireBase::Property const & ( ::SireMol::Molecule::*metadata_function_type )( ::SireBase::PropertyName const &,::SireBase::PropertyName const & ) const;
            metadata_function_type metadata_function_value( &::SireMol::Molecule::metadata );
            
            Molecule_exposer.def( 
                "metadata"
                , metadata_function_value
                , ( bp::arg("key"), bp::arg("metakey") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecule::metadataKeys
        
            typedef ::QStringList ( ::SireMol::Molecule::*metadataKeys_function_type )(  ) const;
            metadataKeys_function_type metadataKeys_function_value( &::SireMol::Molecule::metadataKeys );
            
            Molecule_exposer.def( 
                "metadataKeys"
                , metadataKeys_function_value );
        
        }
        { //::SireMol::Molecule::metadataKeys
        
            typedef ::QStringList ( ::SireMol::Molecule::*metadataKeys_function_type )( ::SireBase::PropertyName const & ) const;
            metadataKeys_function_type metadataKeys_function_value( &::SireMol::Molecule::metadataKeys );
            
            Molecule_exposer.def( 
                "metadataKeys"
                , metadataKeys_function_value
                , ( bp::arg("key") ) );
        
        }
        { //::SireMol::Molecule::move
        
            typedef ::SireMol::Mover< SireMol::Molecule > ( ::SireMol::Molecule::*move_function_type )(  ) const;
            move_function_type move_function_value( &::SireMol::Molecule::move );
            
            Molecule_exposer.def( 
                "move"
                , move_function_value );
        
        }
        { //::SireMol::Molecule::nAtoms
        
            typedef int ( ::SireMol::Molecule::*nAtoms_function_type )(  ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::Molecule::nAtoms );
            
            Molecule_exposer.def( 
                "nAtoms"
                , nAtoms_function_value );
        
        }
        { //::SireMol::Molecule::nChains
        
            typedef int ( ::SireMol::Molecule::*nChains_function_type )(  ) const;
            nChains_function_type nChains_function_value( &::SireMol::Molecule::nChains );
            
            Molecule_exposer.def( 
                "nChains"
                , nChains_function_value );
        
        }
        { //::SireMol::Molecule::nCutGroups
        
            typedef int ( ::SireMol::Molecule::*nCutGroups_function_type )(  ) const;
            nCutGroups_function_type nCutGroups_function_value( &::SireMol::Molecule::nCutGroups );
            
            Molecule_exposer.def( 
                "nCutGroups"
                , nCutGroups_function_value );
        
        }
        { //::SireMol::Molecule::nResidues
        
            typedef int ( ::SireMol::Molecule::*nResidues_function_type )(  ) const;
            nResidues_function_type nResidues_function_value( &::SireMol::Molecule::nResidues );
            
            Molecule_exposer.def( 
                "nResidues"
                , nResidues_function_value );
        
        }
        { //::SireMol::Molecule::nSegments
        
            typedef int ( ::SireMol::Molecule::*nSegments_function_type )(  ) const;
            nSegments_function_type nSegments_function_value( &::SireMol::Molecule::nSegments );
            
            Molecule_exposer.def( 
                "nSegments"
                , nSegments_function_value );
        
        }
        { //::SireMol::Molecule::name
        
            typedef ::SireMol::MolName const & ( ::SireMol::Molecule::*name_function_type )(  ) const;
            name_function_type name_function_value( &::SireMol::Molecule::name );
            
            Molecule_exposer.def( 
                "name"
                , name_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecule::number
        
            typedef ::SireMol::MolNum ( ::SireMol::Molecule::*number_function_type )(  ) const;
            number_function_type number_function_value( &::SireMol::Molecule::number );
            
            Molecule_exposer.def( 
                "number"
                , number_function_value );
        
        }
        { //::SireMol::Molecule::operator=
        
            typedef ::SireMol::Molecule & ( ::SireMol::Molecule::*assign_function_type )( ::SireMol::Molecule const & ) ;
            assign_function_type assign_function_value( &::SireMol::Molecule::operator= );
            
            Molecule_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Molecule::properties
        
            typedef ::SireBase::Properties const & ( ::SireMol::Molecule::*properties_function_type )(  ) const;
            properties_function_type properties_function_value( &::SireMol::Molecule::properties );
            
            Molecule_exposer.def( 
                "properties"
                , properties_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::Molecule::property
        
            typedef ::SireBase::Property const & ( ::SireMol::Molecule::*property_function_type )( ::SireBase::PropertyName const & ) const;
            property_function_type property_function_value( &::SireMol::Molecule::property );
            
            Molecule_exposer.def( 
                "property"
                , property_function_value
                , ( bp::arg("key") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::Molecule::propertyKeys
        
            typedef ::QStringList ( ::SireMol::Molecule::*propertyKeys_function_type )(  ) const;
            propertyKeys_function_type propertyKeys_function_value( &::SireMol::Molecule::propertyKeys );
            
            Molecule_exposer.def( 
                "propertyKeys"
                , propertyKeys_function_value );
        
        }
        { //::SireMol::Molecule::residue
        
            typedef ::SireMol::Residue ( ::SireMol::Molecule::*residue_function_type )( ::SireMol::ResID const & ) const;
            residue_function_type residue_function_value( &::SireMol::Molecule::residue );
            
            Molecule_exposer.def( 
                "residue"
                , residue_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::Molecule::residues
        
            typedef ::SireMol::Selector< SireMol::Residue > ( ::SireMol::Molecule::*residues_function_type )( ::SireMol::ResID const & ) const;
            residues_function_type residues_function_value( &::SireMol::Molecule::residues );
            
            Molecule_exposer.def( 
                "residues"
                , residues_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::Molecule::residues
        
            typedef ::SireMol::Selector< SireMol::Residue > ( ::SireMol::Molecule::*residues_function_type )( ::SireMol::ChainID const & ) const;
            residues_function_type residues_function_value( &::SireMol::Molecule::residues );
            
            Molecule_exposer.def( 
                "residues"
                , residues_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::Molecule::residues
        
            typedef ::SireMol::Selector< SireMol::Residue > ( ::SireMol::Molecule::*residues_function_type )(  ) const;
            residues_function_type residues_function_value( &::SireMol::Molecule::residues );
            
            Molecule_exposer.def( 
                "residues"
                , residues_function_value );
        
        }
        { //::SireMol::Molecule::segment
        
            typedef ::SireMol::Segment ( ::SireMol::Molecule::*segment_function_type )( ::SireMol::SegID const & ) const;
            segment_function_type segment_function_value( &::SireMol::Molecule::segment );
            
            Molecule_exposer.def( 
                "segment"
                , segment_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::Molecule::segments
        
            typedef ::SireMol::Selector< SireMol::Segment > ( ::SireMol::Molecule::*segments_function_type )( ::SireMol::SegID const & ) const;
            segments_function_type segments_function_value( &::SireMol::Molecule::segments );
            
            Molecule_exposer.def( 
                "segments"
                , segments_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::Molecule::segments
        
            typedef ::SireMol::Selector< SireMol::Segment > ( ::SireMol::Molecule::*segments_function_type )(  ) ;
            segments_function_type segments_function_value( &::SireMol::Molecule::segments );
            
            Molecule_exposer.def( 
                "segments"
                , segments_function_value );
        
        }
        { //::SireMol::Molecule::select
        
            typedef ::SireMol::Atom ( ::SireMol::Molecule::*select_function_type )( ::SireMol::AtomID const & ) const;
            select_function_type select_function_value( &::SireMol::Molecule::select );
            
            Molecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::Molecule::select
        
            typedef ::SireMol::Residue ( ::SireMol::Molecule::*select_function_type )( ::SireMol::ResID const & ) const;
            select_function_type select_function_value( &::SireMol::Molecule::select );
            
            Molecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::Molecule::select
        
            typedef ::SireMol::Chain ( ::SireMol::Molecule::*select_function_type )( ::SireMol::ChainID const & ) const;
            select_function_type select_function_value( &::SireMol::Molecule::select );
            
            Molecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::Molecule::select
        
            typedef ::SireMol::CutGroup ( ::SireMol::Molecule::*select_function_type )( ::SireMol::CGID const & ) const;
            select_function_type select_function_value( &::SireMol::Molecule::select );
            
            Molecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::Molecule::select
        
            typedef ::SireMol::Segment ( ::SireMol::Molecule::*select_function_type )( ::SireMol::SegID const & ) const;
            select_function_type select_function_value( &::SireMol::Molecule::select );
            
            Molecule_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::Molecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::Molecule::*selectAll_function_type )( ::SireMol::AtomID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::Molecule::selectAll );
            
            Molecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::Molecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::Residue > ( ::SireMol::Molecule::*selectAll_function_type )( ::SireMol::ResID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::Molecule::selectAll );
            
            Molecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::Molecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::Chain > ( ::SireMol::Molecule::*selectAll_function_type )( ::SireMol::ChainID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::Molecule::selectAll );
            
            Molecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::Molecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::CutGroup > ( ::SireMol::Molecule::*selectAll_function_type )( ::SireMol::CGID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::Molecule::selectAll );
            
            Molecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::Molecule::selectAll
        
            typedef ::SireMol::Selector< SireMol::Segment > ( ::SireMol::Molecule::*selectAll_function_type )( ::SireMol::SegID const & ) const;
            selectAll_function_type selectAll_function_value( &::SireMol::Molecule::selectAll );
            
            Molecule_exposer.def( 
                "selectAll"
                , selectAll_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::Molecule::selectAllAtoms
        
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::Molecule::*selectAllAtoms_function_type )(  ) const;
            selectAllAtoms_function_type selectAllAtoms_function_value( &::SireMol::Molecule::selectAllAtoms );
            
            Molecule_exposer.def( 
                "selectAllAtoms"
                , selectAllAtoms_function_value );
        
        }
        { //::SireMol::Molecule::selectAllChains
        
            typedef ::SireMol::Selector< SireMol::Chain > ( ::SireMol::Molecule::*selectAllChains_function_type )(  ) const;
            selectAllChains_function_type selectAllChains_function_value( &::SireMol::Molecule::selectAllChains );
            
            Molecule_exposer.def( 
                "selectAllChains"
                , selectAllChains_function_value );
        
        }
        { //::SireMol::Molecule::selectAllCutGroups
        
            typedef ::SireMol::Selector< SireMol::CutGroup > ( ::SireMol::Molecule::*selectAllCutGroups_function_type )(  ) const;
            selectAllCutGroups_function_type selectAllCutGroups_function_value( &::SireMol::Molecule::selectAllCutGroups );
            
            Molecule_exposer.def( 
                "selectAllCutGroups"
                , selectAllCutGroups_function_value );
        
        }
        { //::SireMol::Molecule::selectAllResidues
        
            typedef ::SireMol::Selector< SireMol::Residue > ( ::SireMol::Molecule::*selectAllResidues_function_type )(  ) const;
            selectAllResidues_function_type selectAllResidues_function_value( &::SireMol::Molecule::selectAllResidues );
            
            Molecule_exposer.def( 
                "selectAllResidues"
                , selectAllResidues_function_value );
        
        }
        { //::SireMol::Molecule::selectAllSegments
        
            typedef ::SireMol::Selector< SireMol::Segment > ( ::SireMol::Molecule::*selectAllSegments_function_type )(  ) const;
            selectAllSegments_function_type selectAllSegments_function_value( &::SireMol::Molecule::selectAllSegments );
            
            Molecule_exposer.def( 
                "selectAllSegments"
                , selectAllSegments_function_value );
        
        }
        { //::SireMol::Molecule::selectedAll
        
            typedef bool ( ::SireMol::Molecule::*selectedAll_function_type )(  ) const;
            selectedAll_function_type selectedAll_function_value( &::SireMol::Molecule::selectedAll );
            
            Molecule_exposer.def( 
                "selectedAll"
                , selectedAll_function_value );
        
        }
        { //::SireMol::Molecule::selection
        
            typedef ::SireMol::AtomSelection ( ::SireMol::Molecule::*selection_function_type )(  ) const;
            selection_function_type selection_function_value( &::SireMol::Molecule::selection );
            
            Molecule_exposer.def( 
                "selection"
                , selection_function_value );
        
        }
        { //::SireMol::Molecule::toString
        
            typedef ::QString ( ::SireMol::Molecule::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::Molecule::toString );
            
            Molecule_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::Molecule::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::Molecule::typeName );
            
            Molecule_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::Molecule::update
        
            typedef void ( ::SireMol::Molecule::*update_function_type )( ::SireMol::MoleculeData const & ) ;
            update_function_type update_function_value( &::SireMol::Molecule::update );
            
            Molecule_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("moldata") ) );
        
        }
        { //::SireMol::Molecule::version
        
            typedef ::quint64 ( ::SireMol::Molecule::*version_function_type )(  ) const;
            version_function_type version_function_value( &::SireMol::Molecule::version );
            
            Molecule_exposer.def( 
                "version"
                , version_function_value );
        
        }
        { //::SireMol::Molecule::version
        
            typedef ::quint64 ( ::SireMol::Molecule::*version_function_type )( ::SireBase::PropertyName const & ) const;
            version_function_type version_function_value( &::SireMol::Molecule::version );
            
            Molecule_exposer.def( 
                "version"
                , version_function_value
                , ( bp::arg("key") ) );
        
        }
        Molecule_exposer.staticmethod( "typeName" );
        Molecule_exposer.def( "__copy__", &__copy__);
        Molecule_exposer.def( "__deepcopy__", &__copy__);
        Molecule_exposer.def( "clone", &__copy__);
        Molecule_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::Molecule >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Molecule_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::Molecule >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Molecule_exposer.def( "__str__", &__str__< ::SireMol::Molecule > );
        Molecule_exposer.def( "__repr__", &__str__< ::SireMol::Molecule > );
    }

}
