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

using namespace SireMM;
using namespace SireMM::detail;

/////////
///////// Implementation of MolCLJInfoData
/////////

/** Null constructor */
MolCLJInfoData::MolCLJInfoData() : QSharedData()
{}

/** Construct to hold the parameters (in 'params') for the
    molecule 'mol'

    \throw SireError::incompatible_error
*/
MolCLJInfoData::MolCLJInfoData(const Molecule &mol,
                               const QVector< QVector<CLJParameter> > &params)
               : QSharedData(), molecule(mol), parameters(params)
{
    this->assertCompatibleParameters(params);
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

    \throw SireError::version_error
*/
void MolCLJInfoData::assertSameMajorVersion(const Molecule &mol) const
{
    if (molecule.version().major() != mol.version().major())
        throw SireError::version_error( QObject::tr(
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
                          const QVector< QVector<CLJParameter> > &params) const
{
    const MoleculeInfo &molinfo = molecule.info();

    int ncg = molinfo.nCutGroups();

    if (params.count() == ncg)
    {
        const QVector<CLJParameter> *paramarray;

        for (CutGroupID i(0); i<ncg; ++i)
        {
            if (paramarray[i].count() != molinfo.nAtoms(i))
            {
                throw SireError::incompatible_error( QObject::tr(
                        "The number of atoms for the CutGroup with ID == %1 "
                        "in the molecule \"%2\" does not match the number of "
                        "CLJ parameters supplied for that CutGroup.")
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
    \throw SireError::version_error
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
                            const QVector< QVector<CLJParameter> > &changedparams)
{
  this->assertSameMolecule(changedmol);
  this->assertCompatibleParameters(changedparams);

  molecule = changedmol;
  coordinates = molecule.coordGroups();

  parameters = changedparams;
}

/////////
///////// Implementation of MolCLJInfo
/////////

static const QSharedDataPointer<MolCLJInfoData> shared_null(new MolCLJInfoData());

/** Null constructor */
MolCLJInfo::MolCLJInfo() : d( shared_null )
{}

/** Construct the info that holds the CLJ parameters (in parameters)
    for the molecule 'molecule'

    \throw SireError::incompatible_error
*/
MolCLJInfo::MolCLJInfo(const Molecule &molecule,
                       const QVector< ParameterGroup<CLJParameter> > &parameters)
           : d( new MolCLJInfoData(molecule,parameters) )
{}

/** Destructor */
MolCLJInfo::~MolCLJInfo()
{}
