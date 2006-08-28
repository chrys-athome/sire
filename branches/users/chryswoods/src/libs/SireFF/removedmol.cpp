/** 
  * @file
  *
  * C++ Implementation: RemovedMol
  *
  * Description: 
  * Implementation of the RemovedMol class
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "removedmol.h"

#include "SireMol/molecule.h"
#include "SireDB/parametertable.h"
#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireStream;

static const RegisterMetaType<RemovedMol> r_removedmol("SireFF::RemovedMol");

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const RemovedMol &removedmol)
{
    writeHeader(ds, r_removedmol, 1) 
                << removedmol.molid
                << static_cast<const ChangeRecordBase&>(removedmol);
                
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, RemovedMol &removedmol)
{
    VersionID v = readHeader(ds, r_removedmol);
    
    if (v == 1)
    {
        ds >> removedmol.molid
           >> static_cast<ChangeRecordBase&>(removedmol);
    }
    else
        throw version_error(v, "1", r_removedmol, CODELOC);
    
    return ds;
}

/** Null constructor */
RemovedMol::RemovedMol() 
           : ChangeRecordBase(), molid(0)
{}

/** Construct the record that the molecule 'mol' has been removed */
RemovedMol::RemovedMol(const Molecule &mol) 
           : ChangeRecordBase(), molid(mol.ID())
{}

/** Copy constructor */
RemovedMol::RemovedMol(const RemovedMol &other)
           : ChangeRecordBase(other), molid(other.molid)
{}

/** Destructor */
RemovedMol::~RemovedMol()
{}

/** Throw an exception because the new molecule is incompatible with the old!
 
    \throw SireMol::incompatible_molecule
*/
void RemovedMol::incompatibleError(const Molecule &mol) const
{
    throw SireMol::incompatible_molecule( QObject::tr(
            "Incompatible molecules! MoleculeID == %1 vs. %2")
                .arg(molid).arg(mol.toString()), CODELOC );
}

/** Throw an exception because we cannot manipulate a removed molecule!
  
    \throw SireError::program_bug
*/
void RemovedMol::invalidError(const Molecule &mol) const
{
    if (molid != mol.ID())
        incompatibleError(mol);
        
    throw SireError::program_bug( QObject::tr(
            "Cannot manipulate a molecule that has been removed! %1 vs. %2")
                .arg(molid).arg(mol.toString()), CODELOC );
}

/** Move the residue 'res' - this will throw an exception as we cannot move
    a deleted molecule!
    
    \throw SireError::program_bug
    \throw SireMol::incompatible_molecule
*/
ChangeRecordBase* RemovedMol::moveResidue(const Residue &res)
{
    invalidError(res.molecule());
    return this;
}

/** Move the molecule 'mol' - this will throw an exception as we cannot move
    a deleted molecule!
    
    \throw SireError::program_bug
    \throw SireMol::incompatible_molecule
*/
ChangeRecordBase* RemovedMol::moveMolecule(const Molecule &mol)
{
    invalidError(mol);
    return this;
}
    
/** Change the molecule in 'mol_and_params' - this will throw an exception as 
    we cannot move a deleted molecule!
    
    \throw SireError::program_bug
    \throw SireMol::incompatible_molecule
*/
ChangeRecordBase* RemovedMol::changeMolecule(const ParameterTable &mol_and_params)
{
    invalidError(mol_and_params.molecule());
    return this;
}
    
/** Remove the molecule 'mol' - this will throw an exception as we cannot
    remove a deleted molecule!

    \throw SireMol::incompatible_molecule
*/
ChangeRecordBase* RemovedMol::removeMolecule(const Molecule &mol)
{
    invalidError(mol);
    return this;
}

/** Add the molecule which is contained with its parameters in 'mol_and_params'.
    This will return a 'ChangedMol' if this is a compatible molecule.
    
    \throw SireMol::incompatible_molecule
*/
ChangeRecordBase* RemovedMol::addMolecule(const ParameterTable &mol_and_params)
{
    MoleculeID newid = mol_and_params.molecule().ID();
    
    if (newid != 0 and molid == newid)
        return new ChangedMol(mol_and_params);
    else
        incompatibleError(mol_and_params.molecule());
        
    return this;
}
