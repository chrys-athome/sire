/** 
  * @file
  *
  * C++ Implementation: MovedMol
  *
  * Description: 
  * Implementation of the MovedMol class.
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "movedmol.h"
#include "movedparts.h"
#include "changedmol.h"
#include "removedmol.h"
#include "addedmol.h"

#include "SireMol/molecule.h"
#include "SireMol/moleculeversion.h"
#include "SireDB/parametertable.h"

#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MovedMol> r_movedmol("SireFF::MovedMol");

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const MovedMol &movedmol)
{
    writeHeader(ds, r_movedmol, 1) 
                  << movedmol.movedmol
                  << static_cast<const ChangeRecordBase&>(movedmol);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, MovedMol &movedmol)
{
    VersionID v = readHeader(ds, r_movedmol);
    
    if (v == 1)
    {
        ds >> movedmol.movedmol
           >> static_cast<ChangeRecordBase&>(movedmol);
    }
    else
        throw version_error(v, "1", r_movedmol, CODELOC);
    
    return ds;
}

/** Null constructor */
MovedMol::MovedMol() : ChangeRecordBase()
{}

/** Construct a MovedMol to hold the move information 
    for the molecule 'mol' */
MovedMol::MovedMol(const Molecule &mol)
         : ChangeRecordBase(), movedmol(mol)
{}

/** Copy constructor */
MovedMol::MovedMol(const MovedMol &other)
         : ChangeRecordBase(other), movedmol(other.movedmol)
{}

/** Destructor */
MovedMol::~MovedMol()
{}

/** Internal function used to generate an error string for 
    the incompatible_molecule error */
QString MovedMol::errorString(const Molecule &mol) const
{
    return QObject::tr("Cannot combine the move record of "
                       "incompatible molecules! %1 vs. %2")
                    .arg(movedmol.toString(), mol.toString());
}

/** Record that the molecule 'mol' has moved. Note that this will
    throw an exception if the molecule has been changed since being
    added to this record (i.e. the major version number of 'mol' is
    not the same as the major version number of 'movedmol'), or
    if 'mol' is a different molecule to the one recorded in this
    record.
     
    \throw SireMol::incompatible_molecule
*/
ChangeRecordBase* MovedMol::moveMolecule(const Molecule &mol)
{
    if ( movedmol.isNull() or 
         (movedmol.ID() == mol.ID() and 
          movedmol.version().major() == mol.version().major())
       )
    {
        movedmol = mol;
    }
    else if (not mol.isNull())
        throw SireMol::incompatible_molecule(errorString(mol), CODELOC);
                        
    return this;
}

/** Record that the residue 'res' has moved */
ChangeRecordBase* MovedMol::moveResidue(const Residue &res)
{
    if (movedmol.isNull())
        //only the residue has moved
        return new MovedParts(res);
    else
        //we should record the movement of the whole molecule, as
        //it has already been moved
        return this->moveMolecule(res.molecule());
}

/** Record that the molecule (contained with its parameters in 'mol_and_params')
    has changed. */
ChangeRecordBase* MovedMol::changeMolecule(const ParameterTable &mol_and_params)
{
    if ( movedmol.isNull() or 
         movedmol.ID() == mol_and_params.molecule().ID() )
    {
        return new ChangedMol(mol_and_params);
    }
    else if (not mol_and_params.molecule().isNull())
        throw SireMol::incompatible_molecule(errorString(mol_and_params.molecule()), 
                                             CODELOC);

    return this;
}

/** Record that the molecule 'mol' has been removed. */
ChangeRecordBase* MovedMol::removeMolecule(const Molecule &mol)
{
    if (movedmol.isNull() or movedmol.ID() == mol.ID())
        return new RemovedMol(mol);
    else if (not mol.isNull())
        throw SireMol::incompatible_molecule(errorString(mol), CODELOC);
    
    return this;
}
    
/** Record that the molecule 'mol' has been added. This will throw an 
    exception as we cannot add a molecule that already exists! 
    
    \throw SireError::program_bug
*/
ChangeRecordBase* MovedMol::addMolecule(const ParameterTable &mol_and_params)
{
    const Molecule &mol = mol_and_params.molecule();
    
    if (mol.isNull())
        //nothing to do
        return this;
    else if (movedmol.isNull())
        //this is null, so lets just add the molecule
        return new AddedMolecule(mol_and_params);
    else if (movedmol.ID() != mol.ID())
        throw SireMol::incompatible_molecule(errorString(mol), CODELOC);
    else
        throw SireError::program_bug( QObject::tr(
                  "Cannot add a molecule that already exists! %1 vs. %2")
                      .arg(movedmol.toString(), mol.toString()), CODELOC );

    return this;
}
