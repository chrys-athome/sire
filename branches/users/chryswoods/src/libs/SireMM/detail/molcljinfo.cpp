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

#include "molcljinfo.h"

#include "SireMol/moleculeid.h"
#include "SireMol/cutgroupid.h"

#include "SireMol/moleculeversion.h"
#include "SireMol/moleculeinfo.h"

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
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const MolCLJInfoData &moldata)
{
    writeHeader(ds, r_moldata, 1)
        << moldata.molecule << moldata.chgparams << moldata.ljparams;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, MolCLJInfoData &moldata)
{
    VersionID v = readHeader(ds, r_moldata);

    if (v == 1)
    {
        ds >> moldata.molecule >> moldata.chgparams >> moldata.ljparams;

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
                               const QVector< QVector<ChargeParameter> > &chgs,
                               const QVector< QVector<LJParameter> > &ljs)
               : QSharedData(), molecule(mol), coordinates(molecule.coordGroups()),
                 chgparams(chgs), ljparams(ljs)
{
    this->assertCompatibleParameters(chgs);
    this->assertCompatibleParameters(ljs);
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

/** Assert that the parameters in 'params' are arranged in a manner that is
    compatible with the molecule stored in this object

    \throw SireError::incompatible_error
*/
void MolCLJInfoData::assertCompatibleParameters(
                          const QVector< QVector<ChargeParameter> > &params) const
{
    const MoleculeInfo &molinfo = molecule.info();

    int ncg = molinfo.nCutGroups();

    if (params.count() == ncg)
    {
        const QVector<ChargeParameter> *paramarray = params.constData();

        for (CutGroupID i(0); i<ncg; ++i)
        {
            if (paramarray[i].count() != molinfo.nAtoms(i))
            {
                throw SireError::incompatible_error( QObject::tr(
                        "The number of atoms for the CutGroup with ID == %1 "
                        "in the molecule \"%2\" does not match the number of "
                        "charge parameters supplied for that CutGroup.")
                            .arg(i).arg(molecule.idString()), CODELOC );
            }
        }
    }
    else
        throw SireError::incompatible_error( QObject::tr(
            "The number of CutGroups (%1) in the molecule \"%2\" is "
            "not the same as the number of groups that the supplied parameters "
            "are divided into (%3)")
                .arg(ncg).arg(molecule.idString()).arg(params.count()), CODELOC );
}

/** Assert that the parameters in 'params' are arranged in a manner that is
    compatible with the molecule stored in this object

    \throw SireError::incompatible_error
*/
void MolCLJInfoData::assertCompatibleParameters(
                          const QVector< QVector<LJParameter> > &params) const
{
    const MoleculeInfo &molinfo = molecule.info();

    int ncg = molinfo.nCutGroups();

    if (params.count() == ncg)
    {
        const QVector<LJParameter> *paramarray = params.constData();

        for (CutGroupID i(0); i<ncg; ++i)
        {
            if (paramarray[i].count() != molinfo.nAtoms(i))
            {
                throw SireError::incompatible_error( QObject::tr(
                        "The number of atoms for the CutGroup with ID == %1 "
                        "in the molecule \"%2\" does not match the number of "
                        "LJ parameters supplied for that CutGroup.")
                            .arg(i).arg(molecule.idString()), CODELOC );
            }
        }
    }
    else
        throw SireError::incompatible_error( QObject::tr(
            "The number of CutGroups (%1) in the molecule \"%2\" is "
            "not the same as the number of groups that the supplied parameters "
            "are divided into (%3)")
                .arg(ncg).arg(molecule.idString()).arg(params.count()), CODELOC );
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
    and now has the parameters in 'changedparams'

    \throw SireError::incompatible_error
*/
void MolCLJInfoData::change(const Molecule &changedmol,
                            const QVector< QVector<ChargeParameter> > &chgs,
                            const QVector< QVector<LJParameter> > &ljs)
{
  this->assertSameMolecule(changedmol);
  this->assertCompatibleParameters(chgs);
  this->assertCompatibleParameters(ljs);

  molecule = changedmol;
  coordinates = molecule.coordGroups();

  chgparams = chgs;
  ljparams = ljs;
}

/////////
///////// Implementation of MolCLJInfo
/////////

static const QSharedDataPointer<MolCLJInfoData> shared_null(new MolCLJInfoData());

static const RegisterMetaType<MolCLJInfo> r_molinfo("SireMM::detail::MolCLJInfo");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const MolCLJInfo &molinfo)
{
    writeHeader(ds, r_molinfo, 1);

    SharedDataStream(ds) << molinfo.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, MolCLJInfo &molinfo)
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
                       const QVector< QVector<ChargeParameter> > &chgparams,
                       const QVector< QVector<LJParameter> > &ljparams)
           : d( new MolCLJInfoData(molecule,chgparams,ljparams) )
{}

/** Destructor */
MolCLJInfo::~MolCLJInfo()
{}
