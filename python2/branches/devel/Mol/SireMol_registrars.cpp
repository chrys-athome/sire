//WARNING - AUTOGENERATED FILE - CONTENTS WILL BE OVERWRITTEN!
#include <Python.h>

#include "SireMol_registrars.h"

#include "moleculegroups.h"
#include "beadeditor.h"
#include "beadproperty.hpp"
#include "improperid.h"
#include "atomelements.h"
#include "chain.h"
#include "residx.h"
#include "atomcharges.h"
#include "element.h"
#include "chainproperty.hpp"
#include "atomselection.h"
#include "segidentifier.h"
#include "moleditor.h"
#include "evaluator.h"
#include "chainresid.h"
#include "geometryperturbation.h"
#include "specifymol.h"
#include "atomproperty.hpp"
#include "residuecutting.h"
#include "moleculeinfodata.h"
#include "cgproperty.hpp"
#include "perturbation.h"
#include "molid.h"
#include "atomenergies.h"
#include "weightfunction.h"
#include "cgeditor.h"
#include "atomeditor.h"
#include "chainname.h"
#include "chargeperturbation.h"
#include "partialmolecule.h"
#include "atompolarisabilities.h"
#include "mgidx.h"
#include "viewsofmol.h"
#include "residentifier.h"
#include "bondid.h"
#include "molidx.h"
#include "atomnum.h"
#include "chainidentifier.h"
#include "mgidentifier.h"
#include "moleculegroup.h"
#include "resnum.h"
#include "mgid.h"
#include "atomvelocities.h"
#include "molidentifier.h"
#include "segidx.h"
#include "chainid.h"
#include "chaineditor.h"
#include "cgidx.h"
#include "resname.h"
#include "cgname.h"
#include "mgname.h"
#include "chainidx.h"
#include "cgatomidx.h"
#include "beads.h"
#include "bead.h"
#include "atomidx.h"
#include "resid.h"
#include "connectivity.h"
#include "groupgroupids.h"
#include "cgid.h"
#include "atommasses.h"
#include "segment.h"
#include "angleid.h"
#include "segeditor.h"
#include "molnum.h"
#include "atomforces.h"
#include "beadnum.h"
#include "reseditor.h"
#include "within.h"
#include "molname.h"
#include "withatoms.h"
#include "mgidsandmaps.h"
#include "mgnum.h"
#include "segid.h"
#include "atomidentifier.h"
#include "beading.h"
#include "atomcoords.h"
#include "cutgroup.h"
#include "beadidx.h"
#include "atombeads.h"
#include "segname.h"
#include "resproperty.hpp"
#include "molecule.h"
#include "bondhunter.h"
#include "moleculedata.h"
#include "segproperty.hpp"
#include "groupatomids.h"
#include "cgidentifier.h"
#include "atomid.h"
#include "molecules.h"
#include "molatomid.h"
#include "dihedralid.h"
#include "atom.h"
#include "residue.h"
#include "atommatcher.h"
#include "atomname.h"
#include "withres.h"

#include "Helpers/objectregistry.hpp"

void register_SireMol_objects()
{

    ObjectRegistry::registerConverterFor< SireMol::MoleculeGroups >();
    ObjectRegistry::registerConverterFor< SireMol::BeadEditor >();
    ObjectRegistry::registerConverterFor< SireMol::BeadStringProperty >();
    ObjectRegistry::registerConverterFor< SireMol::BeadIntProperty >();
    ObjectRegistry::registerConverterFor< SireMol::BeadFloatProperty >();
    ObjectRegistry::registerConverterFor< SireMol::BeadVariantProperty >();
    ObjectRegistry::registerConverterFor< SireMol::ImproperID >();
    ObjectRegistry::registerConverterFor< SireMol::AtomElements >();
    ObjectRegistry::registerConverterFor< SireMol::Chain >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::Chain> >();
    ObjectRegistry::registerConverterFor< SireMol::Selector<SireMol::Chain> >();
    ObjectRegistry::registerConverterFor< SireMol::Mover< SireMol::Selector<SireMol::Chain> > >();
    ObjectRegistry::registerConverterFor< SireMol::ResIdx >();
    ObjectRegistry::registerConverterFor< SireMol::AtomCharges >();
    ObjectRegistry::registerConverterFor< SireMol::Element >();
    ObjectRegistry::registerConverterFor< SireMol::ChainStringProperty >();
    ObjectRegistry::registerConverterFor< SireMol::ChainIntProperty >();
    ObjectRegistry::registerConverterFor< SireMol::ChainFloatProperty >();
    ObjectRegistry::registerConverterFor< SireMol::ChainVariantProperty >();
    ObjectRegistry::registerConverterFor< SireMol::AtomSelection >();
    ObjectRegistry::registerConverterFor< SireMol::SegIdentifier >();
    ObjectRegistry::registerConverterFor< SireMol::MolEditor >();
    ObjectRegistry::registerConverterFor< SireMol::MolStructureEditor >();
    ObjectRegistry::registerConverterFor< SireMol::Evaluator >();
    ObjectRegistry::registerConverterFor< SireMol::ChainResID >();
    ObjectRegistry::registerConverterFor< SireMol::NullGeometryPerturbation >();
    ObjectRegistry::registerConverterFor< SireMol::GeometryPerturbations >();
    ObjectRegistry::registerConverterFor< SireMol::BondPerturbation >();
    ObjectRegistry::registerConverterFor< SireMol::AnglePerturbation >();
    ObjectRegistry::registerConverterFor< SireMol::DihedralPerturbation >();
    ObjectRegistry::registerConverterFor< SireMol::SpecifyMol >();
    ObjectRegistry::registerConverterFor< SireMol::AtomStringProperty >();
    ObjectRegistry::registerConverterFor< SireMol::AtomIntProperty >();
    ObjectRegistry::registerConverterFor< SireMol::AtomFloatProperty >();
    ObjectRegistry::registerConverterFor< SireMol::AtomVariantProperty >();
    ObjectRegistry::registerConverterFor< SireMol::ResidueCutting >();
    ObjectRegistry::registerConverterFor< SireMol::MoleculeInfoData >();
    ObjectRegistry::registerConverterFor< SireMol::CGStringProperty >();
    ObjectRegistry::registerConverterFor< SireMol::CGIntProperty >();
    ObjectRegistry::registerConverterFor< SireMol::CGFloatProperty >();
    ObjectRegistry::registerConverterFor< SireMol::CGVariantProperty >();
    ObjectRegistry::registerConverterFor< SireMol::NullPerturbation >();
    ObjectRegistry::registerConverterFor< SireMol::Perturbations >();
    ObjectRegistry::registerConverterFor< SireID::IDAndSet<SireMol::MolID> >();
    ObjectRegistry::registerConverterFor< SireID::IDOrSet<SireMol::MolID> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomEnergies >();
    ObjectRegistry::registerConverterFor< SireMol::RelFromMass >();
    ObjectRegistry::registerConverterFor< SireMol::RelFromNumber >();
    ObjectRegistry::registerConverterFor< SireMol::AbsFromMass >();
    ObjectRegistry::registerConverterFor< SireMol::AbsFromNumber >();
    ObjectRegistry::registerConverterFor< SireMol::CGEditor >();
    ObjectRegistry::registerConverterFor< SireMol::CGStructureEditor >();
    ObjectRegistry::registerConverterFor< SireMol::AtomEditor >();
    ObjectRegistry::registerConverterFor< SireMol::AtomStructureEditor >();
    ObjectRegistry::registerConverterFor< SireMol::ChainName >();
    ObjectRegistry::registerConverterFor< SireMol::ChargePerturbation >();
    ObjectRegistry::registerConverterFor< SireMol::PartialMolecule >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::PartialMolecule> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomPolarisabilities >();
    ObjectRegistry::registerConverterFor< SireMol::MGIdx >();
    ObjectRegistry::registerConverterFor< SireMol::ViewsOfMol >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::ViewsOfMol> >();
    ObjectRegistry::registerConverterFor< SireMol::ResIdentifier >();
    ObjectRegistry::registerConverterFor< SireMol::BondID >();
    ObjectRegistry::registerConverterFor< SireMol::MolIdx >();
    ObjectRegistry::registerConverterFor< SireMol::AtomNum >();
    ObjectRegistry::registerConverterFor< SireMol::ChainIdentifier >();
    ObjectRegistry::registerConverterFor< SireMol::MGIdentifier >();
    ObjectRegistry::registerConverterFor< SireMol::MoleculeGroup >();
    ObjectRegistry::registerConverterFor< SireMol::ResNum >();
    ObjectRegistry::registerConverterFor< SireID::IDAndSet<SireMol::MGID> >();
    ObjectRegistry::registerConverterFor< SireID::IDOrSet<SireMol::MGID> >();
    ObjectRegistry::registerConverterFor< SireID::Specify<SireMol::MGID> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomVelocities >();
    ObjectRegistry::registerConverterFor< SireMol::Velocity3D >();
    ObjectRegistry::registerConverterFor< SireMol::MolIdentifier >();
    ObjectRegistry::registerConverterFor< SireMol::SegIdx >();
    ObjectRegistry::registerConverterFor< SireID::Specify<SireMol::ChainID> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomsIn<SireMol::ChainID> >();
    ObjectRegistry::registerConverterFor< SireMol::ResIn<SireMol::ChainID> >();
    ObjectRegistry::registerConverterFor< SireID::IDAndSet<SireMol::ChainID> >();
    ObjectRegistry::registerConverterFor< SireID::IDOrSet<SireMol::ChainID> >();
    ObjectRegistry::registerConverterFor< SireMol::ChainEditor >();
    ObjectRegistry::registerConverterFor< SireMol::ChainStructureEditor >();
    ObjectRegistry::registerConverterFor< SireMol::CGIdx >();
    ObjectRegistry::registerConverterFor< SireMol::ResName >();
    ObjectRegistry::registerConverterFor< SireMol::CGName >();
    ObjectRegistry::registerConverterFor< SireMol::MGName >();
    ObjectRegistry::registerConverterFor< SireMol::ChainIdx >();
    ObjectRegistry::registerConverterFor< SireMol::CGAtomIdx >();
    ObjectRegistry::registerConverterFor< SireMol::Beads >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::Beads> >();
    ObjectRegistry::registerConverterFor< SireMol::Bead >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::Bead> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomIdx >();
    ObjectRegistry::registerConverterFor< SireID::Specify<SireMol::ResID> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomsIn<SireMol::ResID> >();
    ObjectRegistry::registerConverterFor< SireID::IDAndSet<SireMol::ResID> >();
    ObjectRegistry::registerConverterFor< SireID::IDOrSet<SireMol::ResID> >();
    ObjectRegistry::registerConverterFor< SireMol::Connectivity >();
    ObjectRegistry::registerConverterFor< SireMol::ConnectivityEditor >();
    ObjectRegistry::registerConverterFor< SireMol::SegResID >();
    ObjectRegistry::registerConverterFor< SireMol::SegChainID >();
    ObjectRegistry::registerConverterFor< SireMol::SegCGID >();
    ObjectRegistry::registerConverterFor< SireMol::CGResID >();
    ObjectRegistry::registerConverterFor< SireMol::CGChainID >();
    ObjectRegistry::registerConverterFor< SireID::Specify<SireMol::CGID> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomsIn<SireMol::CGID> >();
    ObjectRegistry::registerConverterFor< SireID::IDAndSet<SireMol::CGID> >();
    ObjectRegistry::registerConverterFor< SireID::IDOrSet<SireMol::CGID> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomMasses >();
    ObjectRegistry::registerConverterFor< SireMol::Segment >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::Segment> >();
    ObjectRegistry::registerConverterFor< SireMol::Selector<SireMol::Segment> >();
    ObjectRegistry::registerConverterFor< SireMol::Mover< SireMol::Selector<SireMol::Segment> > >();
    ObjectRegistry::registerConverterFor< SireMol::AngleID >();
    ObjectRegistry::registerConverterFor< SireMol::SegEditor >();
    ObjectRegistry::registerConverterFor< SireMol::SegStructureEditor >();
    ObjectRegistry::registerConverterFor< SireMol::MolNum >();
    ObjectRegistry::registerConverterFor< SireMol::AtomForces >();
    ObjectRegistry::registerConverterFor< SireMol::Force3D >();
    ObjectRegistry::registerConverterFor< SireMol::BeadNum >();
    ObjectRegistry::registerConverterFor< SireMol::ResEditor >();
    ObjectRegistry::registerConverterFor< SireMol::ResStructureEditor >();
    ObjectRegistry::registerConverterFor< SireMol::Within >();
    ObjectRegistry::registerConverterFor< SireMol::MolName >();
    ObjectRegistry::registerConverterFor< SireMol::ResWithAtoms >();
    ObjectRegistry::registerConverterFor< SireMol::CGsWithAtoms >();
    ObjectRegistry::registerConverterFor< SireMol::ChainsWithAtoms >();
    ObjectRegistry::registerConverterFor< SireMol::SegsWithAtoms >();
    ObjectRegistry::registerConverterFor< SireMol::MGIDsAndMaps >();
    ObjectRegistry::registerConverterFor< SireMol::MGNum >();
    ObjectRegistry::registerConverterFor< SireID::Specify<SireMol::SegID> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomsIn<SireMol::SegID> >();
    ObjectRegistry::registerConverterFor< SireID::IDAndSet<SireMol::SegID> >();
    ObjectRegistry::registerConverterFor< SireID::IDOrSet<SireMol::SegID> >();
    ObjectRegistry::registerConverterFor< SireMol::AtomIdentifier >();
    ObjectRegistry::registerConverterFor< SireMol::MoleculeBeading >();
    ObjectRegistry::registerConverterFor< SireMol::ResidueBeading >();
    ObjectRegistry::registerConverterFor< SireMol::UserBeading >();
    ObjectRegistry::registerConverterFor< SireMol::NullBeading >();
    ObjectRegistry::registerConverterFor< SireMol::AtomCoords >();
    ObjectRegistry::registerConverterFor< SireMol::CutGroup >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::CutGroup> >();
    ObjectRegistry::registerConverterFor< SireMol::Selector<SireMol::CutGroup> >();
    ObjectRegistry::registerConverterFor< SireMol::Mover< SireMol::Selector<SireMol::CutGroup> > >();
    ObjectRegistry::registerConverterFor< SireMol::BeadIdx >();
    ObjectRegistry::registerConverterFor< SireMol::AtomBeads >();
    ObjectRegistry::registerConverterFor< SireMol::SegName >();
    ObjectRegistry::registerConverterFor< SireMol::ResStringProperty >();
    ObjectRegistry::registerConverterFor< SireMol::ResIntProperty >();
    ObjectRegistry::registerConverterFor< SireMol::ResFloatProperty >();
    ObjectRegistry::registerConverterFor< SireMol::ResVariantProperty >();
    ObjectRegistry::registerConverterFor< SireMol::Molecule >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::Molecule> >();
    ObjectRegistry::registerConverterFor< SireMol::CovalentBondHunter >();
    ObjectRegistry::registerConverterFor< SireMol::ChemicalBondHunter >();
    ObjectRegistry::registerConverterFor< SireMol::MoleculeData >();
    ObjectRegistry::registerConverterFor< SireMol::SegStringProperty >();
    ObjectRegistry::registerConverterFor< SireMol::SegIntProperty >();
    ObjectRegistry::registerConverterFor< SireMol::SegFloatProperty >();
    ObjectRegistry::registerConverterFor< SireMol::SegVariantProperty >();
    ObjectRegistry::registerConverterFor< SireMol::ResAtomID >();
    ObjectRegistry::registerConverterFor< SireMol::ChainAtomID >();
    ObjectRegistry::registerConverterFor< SireMol::SegAtomID >();
    ObjectRegistry::registerConverterFor< SireMol::CGAtomID >();
    ObjectRegistry::registerConverterFor< SireMol::CGIdentifier >();
    ObjectRegistry::registerConverterFor< SireID::Specify<SireMol::AtomID> >();
    ObjectRegistry::registerConverterFor< SireID::IDAndSet<SireMol::AtomID> >();
    ObjectRegistry::registerConverterFor< SireID::IDOrSet<SireMol::AtomID> >();
    ObjectRegistry::registerConverterFor< SireMol::Molecules >();
    ObjectRegistry::registerConverterFor< SireMol::MolAtomID >();
    ObjectRegistry::registerConverterFor< SireMol::DihedralID >();
    ObjectRegistry::registerConverterFor< SireMol::Atom >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::Atom> >();
    ObjectRegistry::registerConverterFor< SireMol::Selector<SireMol::Atom> >();
    ObjectRegistry::registerConverterFor< SireMol::Mover< SireMol::Selector<SireMol::Atom> > >();
    ObjectRegistry::registerConverterFor< SireMol::Residue >();
    ObjectRegistry::registerConverterFor< SireMol::Mover<SireMol::Residue> >();
    ObjectRegistry::registerConverterFor< SireMol::Selector<SireMol::Residue> >();
    ObjectRegistry::registerConverterFor< SireMol::Mover< SireMol::Selector<SireMol::Residue> > >();
    ObjectRegistry::registerConverterFor< SireMol::AtomIdxMatcher >();
    ObjectRegistry::registerConverterFor< SireMol::AtomNameMatcher >();
    ObjectRegistry::registerConverterFor< SireMol::AtomName >();
    ObjectRegistry::registerConverterFor< SireMol::ChainsWithRes >();

}

