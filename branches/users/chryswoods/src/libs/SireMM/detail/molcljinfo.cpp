/**
  * @file
  *
  * C++ Implementation: MolCLJInfo and MolCLJInfoData
  *
  * Description:
  * Implementation of MolCLJInfo and MolCLJInfoData
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "SireCAS/qhash_sirecas.h"

#include "molcljinfo.h"

#include "SireMol/moleculeid.h"
#include "SireMol/cutgroupid.h"

#include "SireMol/moleculeversion.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/residueinfo.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireMol;
using namespace SireStream;

/////////
///////// Implementation of MolCLJInfoData
/////////

static const RegisterMetaType<MolCLJInfoData> r_moldata("SireMM::detail::MolCLJInfoData");

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const MolCLJInfoData &moldata)
{
    writeHeader(ds, r_moldata, 1)
        << moldata.molecule << moldata.map << moldata.cgids
        << moldata.chgparams << moldata.ljparams;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, MolCLJInfoData &moldata)
{
    VersionID v = readHeader(ds, r_moldata);

    if (v == 1)
    {
        ds >> moldata.molecule >> moldata.map >> moldata.cgids
           >> moldata.chgparams >> moldata.ljparams;

        moldata.coordinates = moldata.molecule.coordGroups();
        
        //set the coulomb and lj properties to null to trigger
        //their re-aquisition during the next change
        moldata.coulombproperty = ParameterName();
        moldata.ljproperty = ParameterName();
    }
    else
        throw version_error(v, "1", r_moldata, CODELOC);

    return ds;
}

/** Null constructor */
MolCLJInfoData::MolCLJInfoData() : QSharedData()
{}

/** Build the arrays for the whole molecule */
void MolCLJInfoData::buildWhole()
{
    if (not cgids.isEmpty())
    {
        cgids = QSet<CutGroupID>();
    
        coordinates = molecule.coordGroups();
        chgparams = molecule.getProperty( map.source(coulombproperty) );
        ljparams = molecule.getProperty( map.source(ljproperty) );
    }
}

/** Build the parts of the molecule from the list of CutGroupIDs */
void MolCLJInfoData::buildParts(const QSet<CutGroupID> &cutgroupids)
{
    if (cgids == cutgroupids)
        return;
    
    this->buildWhole();
    
    cgids = cutgroupids;

    if (cgids.isEmpty())
        return;

    const CoordGroup *coodinates_array = coordinates.constData();
    const QVector<ChargeParameter> *chgparams_array = chgparams.constData();
    const QVector<LJParameter> *ljparams_array = ljparams.constData();
        
    int n = cgids.count();
        
    QVector<CoordGroup> part_coords(n);
    QVector< QVector<ChargeParameter> > part_chgs(n);
    QVector< QVector<LJParameter> > part_ljs(n);
        
    CoordGroup *part_coords_array = part_coords.data();
    QVector<ChargeParameter> *part_chgs_array = part_chgs.data();
    QVector<LJParameter> *part_ljs_array = part_ljs.data();
        
    int i = 0;
        
    //copy the CoordGroups, Charge and LJ parameters for all of 
    //the CutGroups whose IDs are in cgids (in the iterator 
    //order of cgids)
    for (QSet<CutGroupID>::const_iterator it = cgids.constBegin();
         it != cgids.constEnd();
         ++it)
    {
        int idx = *it;
        
        part_coords_array[i] = coordinates_array[idx];
        part_chgs_array[i] = chgparams_array[idx];
        part_ljs_array[i] = ljparams_array[idx];
            
        ++i;
    }
        
    coordinates = part_coords;
    chgparams = part_chgs;
    ljparams = part_ljs;
}

/** Construct to hold the parameters (in 'params') for the
    molecule 'mol'

    \throw SireError::incompatible_error
*/
MolCLJInfoData::MolCLJInfoData(const Molecule &mol,
                               const CLJFF::Parameters &parameters,
                               const ParameterMap &parammap,
                               const QSet<CutGroupID> &cutgroupids)
               : QSharedData(),
                 coulombproperty(parameters.coulomb()),
                 ljproperty(parameters.lj()),
                 molecule(mol), 
                 map(parammap),
                 cgids(cutgroupids)
{
    if (not cgids.isEmpty())
        this->buildParts(cgids);
}

/** Destructor */
MolCLJInfoData::~MolCLJInfoData()
{}

/** Assert that 'mol' is the same molecule as is contained in this object

    \throw SireError::incompatible_error
*/
void MolCLJInfoData::assertSameMolecule(const Molecule &mol) const
{
    if (molecule.ID() != mol.ID())
        throw SireError::incompatible_error( QObject::tr(
            "The molecule that is changing (%1) is not the same as the molecule "
            "that is stored in this MolCLJInfo (%2)")
                .arg(mol.idString(), molecule.idString()), CODELOC );
}

/////////
///////// Implementation of MolCLJInfo
/////////

static const QSharedDataPointer<MolCLJInfoData> shared_null(new MolCLJInfoData());

static const RegisterMetaType<MolCLJInfo> r_molinfo("SireMM::detail::MolCLJInfo");

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const MolCLJInfo &molinfo)
{
    writeHeader(ds, r_molinfo, 1);

    SharedDataStream(ds) << molinfo.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, MolCLJInfo &molinfo)
{
    VersionID v = readHeader(ds, r_molinfo);

    if (v == 1)
    {
        SharedDataStream(ds) >> molinfo.d;
    }
    else
        throw version_error(v, "1", r_molinfo, CODELOC);

    return ds;
}

/** Null constructor */
MolCLJInfo::MolCLJInfo() : d( shared_null )
{}

/** Construct the info that holds the CLJ parameters (in parameters)
    for the molecule 'molecule'

    \throw SireError::incompatible_error
*/
MolCLJInfo::MolCLJInfo(const Molecule &molecule,
                       const CLJFF::Parameters &parameters,
                       const ParameterMap &map,
                       const QSet<CutGroupID> &cgids)
           : d( new MolCLJInfoData(molecule,parameters,map,cgids) )
{}

/** Destructor */
MolCLJInfo::~MolCLJInfo()
{}

/** Change this info to represent the whole of the molecule */
void MolCLJInfo::setWholeMolecule()
{
    d->buildWhole();
}

/** Return whether or not this represents the whole molecule */
bool MolCLJInfo::isWholeMolecule() const
{
    return d->cgids.isEmpty();
}

/** Change this info to represent the parts of the molecule whose
    CutGroupIDs are in 'cgids'
    
    \throw SireError::invalid_index
*/
void MolCLJInfo::setPartialMolecule(const QSet<CutGroupID> &cgids)
{
    //check validity first here...
    uint ngroups = molecule().nCutGroups();
    
    for (QSet<CutGroupID>::const_iterator it = cgids.begin();
         it != cgids.end();
         ++it)
    {
        if (*it >= ngroups)
            throw SireError::invalid_index( QObject::tr(
                    "There is no CutGroup with ID == %1 in the molecule "
                    "\"%2\" (%3) as the number of CutGroups is equal to %4")
                        .arg(*it)
                        .arg(molecule().name()).arg(molecule().ID())
                        .arg(ngroups), CODELOC );
    }
    
    //all of the CutGroup IDs are valid - if the number of 
    //them equals ngroups then they represent the entire molecule
    if (cgids.count() == ngroups)
        this->setWholeMolecule();
    else
        d->setPartialMolecule(cgids);
}

/** Return whether the properties have changed, and set the new
    properties if they have */
bool MolCLJInfo::propertiesChanged(const CLJFF::Parameters &parameters)
{
    if ( d->coulombproperty != parameters.coulomb() or 
         d->ljproperty != parameters.lj() )
    {
        d->coulombproperty = parameters.coulomb();
        d->ljproperty = parameters.lj();
        return true;
    }
    else
        return false;    
}

/** Return a ChangedMolCLJInfo that records that the 
    contained molecule has changed to 'newmol'

    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireMol::missing_property
*/
ChangedMolCLJInfo MolCLJInfo::change(const Molecule &newmol,
                                     const CLJFF::Parameters &parameters,
                                     const ParameterMap &map) const
{
    this->assertSameMolecule(newmol);
    
    //save the old state of the whole molecule
    MolCLJInfo oldinfo(*this);
    
    if (not oldinfo.isWholeMolecule())
        oldinfo.setWholeMolecule();
    
    //get the state of the new molecule
    MolCLJInfo newinfo(oldinfo);
        
    newinfo.d->molecule = newmol;
    newinfo.d->coordinates = newmol.coordGroups();
    
    if ( newinfo.propertiesChanged(parameters) or 
         newmol.version().major() != molecule().version().major() )
    {
        //the major version number has changed - this implies that
        //the molecular properties may have changed as well!
        newinfo.d->chgparams = newmol.getProperty(map.source(coulombproperty));
        newinfo.d->ljparams = newmol.getProperty(map.source(ljproperty));
    }
        
    return ChangedMolCLJInfo(oldinfo,newinfo);
}

/** Return a ChangedMolCLJInfo that records that the contained
    molecule has a changed residue 'newres'
    
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireMol::missing_property
*/
ChangedMolCLJInfo MolCLJInfo::change(const Residue &newres,
                                     const CLJFF::Parameters &parameters,
                                     const ParameterMap &map) const
{
    Molecule newmol = newres.molecule();
    
    this->assertSameMolecule(newmol);
    
    //create a copy of this info that represents the 
    //whole molecule
    MolCLJInfo oldinfo(*this);
    
    if (not oldinfo.isWholeMolecule())
        oldinfo.setWholeMolecule();
    
    //we need to create a list of CutGroupIDs that is the union of 
    //what already exists and those that are used by 'newres'
    QSet<CutGroupID> cgids = d->cgids + newres.cutGroupIDs();
    
    MolCLJInfo newinfo(oldinfo);
        
    newinfo.d->molecule = newmol;
    newinfo.d->coordinates = newmol.coordGroups();
    
    if ( newinfo.propertiesChanged(parameters) or 
         newmol.version().major() != molecule().version().major() )
    {
        //the major version number has changed - this implies that
        //the molecular properties may have changed as well!
        newinfo.d->chgparams = newmol.getProperty(map.source(coulombproperty));
        newinfo.d->ljparams = newmol.getProperty(map.source(ljproperty));
    }
        
    return ChangedMolCLJInfo(oldinfo, newmol, cgids);
}

/////////
///////// Implementation of ChangedMolCLJInfo
/////////

static const RegisterMetaType<ChangedMolCLJInfo>
                          r_changedmol("SireMM::detail::ChangedMolCLJInfo");

/** Serialise to a binary datastream */
QDataStream &operator<<(QDataStream &ds, const ChangedMolCLJInfo &changedmol)
{
    writeHeader(ds, r_changedmol, 1);
     
    SharedDataStream(ds) << changedmol.newmol << changedmol.newparts
                         << changedmol.oldmol << changedmol.oldparts
                         << changedmol.cgids;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream &operator>>(QDataStream &ds, ChangedMolCLJInfo &changedmol)
{
    VersionID v = readHeader(ds, r_changedmol);

    if (v == 1)
    {
        SharedDataStream(ds) >> changedmol.newmol >> changedmol.newparts
                             >> changedmol.oldmol >> changedmol.oldparts
                             >> changedmol.cgids;
    }
    else
        throw version_error(v, "1", r_changedmol, CODELOC);

    return ds;
}

/** Null constructor */
ChangedMolCLJInfo::ChangedMolCLJInfo()
{}

/** Assert that this is sanely constructed (refers to the same molecule!)

    \throw SireError::incompatible_error
*/
void ChangedMolCLJInfo::assertSane() const
{
    if (oldmol.molecule().ID() != newmol.molecule().ID())
        throw SireError::incompatible_error( QObject::tr(
            "You cannot create a ChangedMolCLJInfo that refers to two "
            "different molecules! - \"%1\" (%2) vs. \"%3\" (%4)")
                .arg(oldmol.molecule().name())
                .arg(oldmol.molecule().ID())
                .arg(newmol.molecule().name())
                .arg(newmol.molecule().ID()), CODELOC );
}

/** Record that the molecule is changing from 'old_molecule' to 'new_molecule'
     - the entire molecule is recorded as changing

    The old and new molecules must be compatible, i.e. they
    must have the same MoleculeID numbers.

    \throw SireError::incompatible_error
*/
ChangedMolCLJInfo::ChangedMolCLJInfo( const MolCLJInfo &old_molecule,
                                      const MolCLJInfo &new_molecule )
                  : oldmol(old_molecule), newmol(new_molecule),
                    oldparts(old_molecule), newparts(new_molecule)
{
    this->assertSane();
}

/** Record that the molecule is changing from 'old_molecule' to 'new_molecule'
    and that it is only the CutGroups whose IDs that are in 'movedparts'
    that are changing */
ChangedMolCLJInfo::ChangedMolCLJInfo( const MolCLJInfo &old_molecule,
                                      const MolCLJInfo &new_molecule,
                                      const QSet<CutGroupID> &movedparts )
                  : oldmol(old_molecule), newmol(new_molecule), 
                    oldparts(old_molecule), newparts(new_molecule),
                    cgids(movedparts)
{
    this->assertSane();

    if (not cgids.isEmpty())
    {
        oldparts.setPartialMolecule(cgids);
        newparts.setPartialMolecule(cgids);
    }
}

/** Destructor */
ChangedMolCLJInfo::~ChangedMolCLJInfo()
{}

/** Return a new Change record that adds the change of 'new_molecule' */
ChangedMolCLJInfo ChangedMolCLJInfo::change(const Molecule &new_molecule,
                                            const CLJFF::Parameters &parameters) const
{
    return oldmol.change(new_molecule, parameters);
}

/** Return a new Change record that adds the change of 'new_residue' */
ChangedMolCLJInfo ChangedMolCLJInfo::change(const Residue &new_residue,
                                            const CLJFF::Parameters &parameters) const
{
    //is this a whole-molecule change? - if so then it will remain so
    if (this->movedAll())
    {
        return this->change(new_residue.molecule(), parameters);
    }
    else
    {
        //this is already a partial change - create the change from 'oldparts'
        //as this holds the old state, and the cgids of the parts that have changed
        //so far
        return oldparts.change(new_residue, parameters);
    }
}
