/**
  * @file
  *
  * C++ Implementation: ChangeRecordBase
  *
  * Description: 
  * Implementation of ChangeRecordBase
  * 
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "changerecordbase.h"

#include "movedparts.h"
#include "movedmol.h"
#include "changedmol.h"
#include "removedmol.h"
#include "addedmol.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireDB/parameterdb.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireDB;

/** Constructor */
ChangeRecordBase::ChangeRecordBase()
{}

/** Copy constructor */
ChangeRecordBase::ChangeRecordBase(const ChangeRecordBase&)
{}

/** Destructor */
ChangeRecordBase::~ChangeRecordBase()
{}

/** Record that the residue 'res' has moved. Return a pointer to 
    the ChangeRecord that contains the new information. This will
    append this information onto this record if that is possible,
    or else it will create a new ChangeRecord and will append
    the information onto that. You are responsible for
    managing the returned pointer. */
ChangeRecordBase* ChangeRecordBase::moveResidue(const Residue &res)
{
    //as we are here, this means that this is the null ChangeRecord,
    //so just return a new MovedParts with the correct information
    return new MovedParts(res);
}

/** Record that the whole molecule 'mol' has moved. Return a pointer
    to the ChangeRecord that contains the new information. This will
    append this information onto this record if that is possible,
    or will create a new ChangeRecord and will append the information
    onto that. You are responsible for managing the returned pointer. */
ChangeRecordBase* ChangeRecordBase::moveMolecule(const Molecule &mol)
{
    //as we are here, this means that this is a null ChangeRecord, so
    //just return a new MovedMol with the correct information
    return new MovedMol(mol);
}
    
/** Record that the molecule contained with its parameters in the parameter table
    'mol_and_params' has changed. This will append this information onto this
    record, if possible, else will create a new ChangeRecord and will append
    onto that. You are responsible for managing the returned pointer. */
ChangeRecordBase* ChangeRecordBase::changeMolecule(const ParameterTable &mol_and_params)
{
    //as we are here, this means that this is a null ChangeRecord, so
    //just return a new ChangedMol with the correct information
    return new ChangedMol(mol_and_params);
}

/** Record that the molecule 'mol' has been removed. This will append this information onto this
    record, if possible, else will create a new ChangeRecord and will append
    onto that. You are responsible for managing the returned pointer. */
ChangeRecordBase* ChangeRecordBase::removeMolecule(const Molecule &mol)
{
    //as we are here, this means that this is a null ChangeRecord, so
    //just return a new RemovedMol with the correct information
    return new RemovedMol(mol);
}

/** Record that the molecule contained with its parameters in the parameter table
    'mol_and_params' has been added. This will append this information onto this
    record, if possible, else will create a new ChangeRecord and will append
    onto that. You are responsible for managing the returned pointer. */
ChangeRecordBase* ChangeRecordBase::addMolecule(const ParameterTable &mol_and_params)
{
    //as we are here, this means that this is a null ChangeRecord, so
    //just return a new AddedMol with the correct information
    return new AddedMol(mol);
}
