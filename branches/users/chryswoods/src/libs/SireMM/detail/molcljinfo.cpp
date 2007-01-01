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
        << moldata.molecule << moldata.chgparams
        << moldata.ljparams;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, MolCLJInfoData &moldata)
{
    VersionID v = readHeader(ds, r_moldata);

    if (v == 1)
    {
        ds >> moldata.molecule >> moldata.chgparams
           >> moldata.ljparams;

        moldata.coordinates = moldata.molecule.coordGroups();
    }
    else
        throw version_error(v, "1", r_moldata, CODELOC);

    return ds;
}

/** Null constructor */
MolCLJInfoData::MolCLJInfoData() : QSharedData()
{}

/** Construct to hold the parameters (in 'params') for the
    molecule 'mol'

    \throw SireError::incompatible_error
*/
MolCLJInfoData::MolCLJInfoData(const Molecule &mol,
                               const CLJFF::Parameters &parameters,
                               const ParameterMap &map)
               : QSharedData(), molecule(mol), coordinates(molecule.coordGroups()),
                 chgparams( mol.getProperty(map.source(parameters.coulomb())) ),
                 ljparams( mol.getProperty(map.source(parameters.lj())) )
{}

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

/** Assert that 'mol' has the same major version as the molecule contained
    in this object

    \throw SireStream::version_error
*/
void MolCLJInfoData::assertSameMajorVersion(const Molecule &mol) const
{
    if (molecule.version().major() != mol.version().major())
        throw SireStream::version_error( QObject::tr(
            "The molecule \"%1\" has a different major version number (%2) "
            "than the molecule stored in this MolCLJInfo (%3, version number %4)")
                .arg( mol.idString() ).arg(mol.version().major())
                .arg( molecule.idString() ).arg(molecule.version().major()), CODELOC );
}

/** Record that the molecule 'movedmol' has been moved

    \throw SireError::incompatible_error
    \throw SireStream::version_error
*/
void MolCLJInfoData::move(const Molecule &movedmol)
{
  this->assertSameMolecule(movedmol);
  this->assertSameMajorVersion(movedmol);

  molecule = movedmol;
  coordinates = molecule.coordGroups();
}

/** Record that the molecule 'changedmol' has been changed,
    using the parameters and map to get the parameters
    from the molecular properties.

    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireMol::missing_property
*/
void MolCLJInfoData::change(const Molecule &changedmol,
                            const CLJFF::Parameters &parameters,
                            const ParameterMap &map)
{
  this->assertSameMolecule(changedmol);

  molecule = changedmol;
  coordinates = molecule.coordGroups();

  chgparams = changedmol.getProperty(map.source(parameters.coulomb()));
  ljparams = changedmol.getProperty(map.source(parameters.lj()));
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
                       const ParameterMap &map)
           : d( new MolCLJInfoData(molecule,parameters,map) )
{}

/** Destructor */
MolCLJInfo::~MolCLJInfo()
{}

/////////
///////// Implementation of ChangedMolCLJInfo
/////////

static const RegisterMetaType<ChangedMolCLJInfo>
                          r_changedmol("SireMM::detail::ChangedMolCLJInfo");

/** Serialise to a binary datastream */
QDataStream &operator<<(QDataStream &ds, const ChangedMolCLJInfo &changedmol)
{
    writeHeader(ds, r_changedmol, 1) << changedmol.newmol << changedmol.newparts
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
        ds >> changedmol.newmol >> changedmol.newparts
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

/** Assert that 'molinfo' is compatible with the molecule in this
    object

    \throw SireError::incompatible_error
*/
void ChangedMolCLJInfo::assertCompatibleWith(const MolCLJInfo &molinfo)
{
}

/** Build the old and new parts from the old and new molecules and list
    of changed cutgroups */
void ChangedMolCLJInfo::buildParts()
{
    #warning TODO!!!

    //need to work out how this will work, and how this will
    //combine together...
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
{}

/** Record that the molecule is changing from 'old_molecule' to 'new_molecule'
    and that it is only the CutGroups whose IDs that are in 'movedparts'
    that are changing */
ChangedMolCLJInfo::ChangedMolCLJInfo( const MolCLJInfo &old_molecule,
                                      const MolCLJInfo &new_molecule,
                                      const QSet<CutGroupID> &movedparts )
                  : oldmol(old_molecule), newmol(new_molecule), cgids(movedparts)
{
    this->buildParts();
}

/** Destructor */
ChangedMolCLJInfo::~ChangedMolCLJInfo()
{}

/** Add additional changed parts of the molecule to this record */
void ChangedMolCLJInfo::change(const MolCLJInfo &newinfo,
                               const QSet<CutGroupID> &changedparts)
{
    this->assertCompatibleWith(newinfo);

    newmol = newinfo;

    //the changed parts are the union of the current changed parts
    //with the existing changed parts
    cgids += changedparts;

    this->buildParts();
}

/** Change the molecule again */
void ChangedMolCLJInfo::change(const MolCLJInfo &newinfo)
{
    this->assertCompatibleWith(newinfo);

    //the whole molecule has now changed
    newmol = newinfo;
    cgids.clear();

    this->buildParts();
}

/** Register the movement of the molecule to 'newmol' */
void ChangedMolCLJInfo::move(const Molecule &molecule)
{
    oldmol.molecule().assertSameMajorVersion(molecule);

    //the whole molecule has now changed
    newmol = oldmol;
    newmol.move(molecule);

    //the whole molecule has now changed
    cgids.clear();

    this->buildParts();
}

/** Register the movement of the residue to 'newres' */
void ChangedMolCLJInfo::move(const Residue &residue)
{
    Molecule molecule = residue.molecule();

    newmol = oldmol;
    newmol.move(molecule);

    //combine the residue's ids with the existing id numbers
    cgids += residue.info().cutGroupIDs();

    this->buildParts();
}
