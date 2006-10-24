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

/** Assert that 'mol' is compatible with the molecule recorded in 
    this move record 
    
    \throw SireError::incompatible_error
*/
void MovedMol::assertCompatibleWith(const Molecule &mol) const
{
    //check that the IDs are the same

}

/** Assert that 'mol' is a compatible version to be recorded in 
    this record (i.e. the major version number is the same)
    
    \throw SireError::version_error
*/
void MovedMol::assertCompatibleVersion(const Molecule &mol) const
{
}

/** Record that the molecule 'mol' has moved. Note that this will
    throw an exception if the molecule has been changed since being
    added to this record 
    
    \throw SireError::incompatible_error
    \throw SireError::version_error
*/
ChangeRecordBase* MovedMol::moveMolecule(const Molecule &mol)
{
    this->assertCompatibleWith(mol);
    this->assertCompatibleVersion(mol);
    
    movedmol = mol;
                        
    return this;
}

/** Record that the residue 'res' has moved
    
    \throw SireError::incompatible_error
    \throw SireError::version_error
*/
ChangeRecordBase* MovedMol::moveResidue(const Residue &res)
{
    //we should record the movement of the whole molecule, as
    //it has already been moved
    return this->moveMolecule(res.molecule());
}

/** Record that the residue 'res', with parameters 'param' has changed.

    \throw SireError::incompatible_error
*/
ChangeRecordBase* MovedMol::changeResidue(const Residue &res,
                                          const ParameterTable &params)
{
    this->assertCompatibleWith(res.molecule());
    
    return new ChangedRes(movedmol, res, params);
}

/** Record that the molecule 'mol', with parameters 'params' has changed. 

    \throw SireError::incompatible_error
*/
ChangeRecordBase* MovedMol::changeMolecule(const Molecule &mol,
                                           const ParameterTable &params)
{
    this->assertCompatibleWith(mol);
    
    return new ChangedMol(mol,params);
}

/** Record that the molecule 'mol' has been removed. */
ChangeRecordBase* MovedMol::removeMolecule(const Molecule &mol)
{
    this->assertCompatibleWith(mol);
    
    return new RemovedMol(mol);
}
    
/** Record that the molecule 'mol' has been added (with parameters  
    'params'. 
    
    This will throw an exception as we cannot add a molecule that already exists! 
    
    \throw SireError::program_bug
*/
ChangeRecordBase* MovedMol::addMolecule(const Molecule &mol,
                                        const ParameterTable &params)
{
    this->assertCompatibleWith(mol);
    
    throw SireError::program_bug( QObject::tr(
               "Cannot add a molecule that already exists! %1 vs. %2")
                    .arg(movedmol.idString(), mol.idString()), CODELOC );

    return this;
}
