/**
  * @file
  *
  * C++ Implementation: ChangeRecord
  *
  * Description: 
  * Implementation of ChangeRecord
  * 
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "changerecord.h"

#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireDB;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<ChangeRecord> r_changerecord("SireFF::ChangeRecord");

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ChangeRecord &changerecord)
{
    writeHeader(ds, r_changerecord, 1) << changerecord.d;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ChangeRecord &changerecord)
{
    VersionID v = readHeader(ds, r_changerecord);
    
    if (v == 1)
    {
        ds >> changerecord.d;
    }
    else
        throw version_error(v, "1", r_changerecord, CODELOC);
    
    return ds;
}

/** Shared null ChangeRecord */
static const DynamicSharedPtr<ChangeRecordBase> shared_null(new ChangeRecordBase());

/** Null constructor */
ChangeRecord::ChangeRecord() : d(shared_null)
{}

/** Destructor */
ChangeRecord::~ChangeRecord()
{}

/** Return the type name of the ChangeRecord */
const char* ChangeRecord::what() const
{
    return d->what();
}

/** Private function used to update the shared pointer when the
    type of record changes */
inline void ChangeRecord::updatePointer(ChangeRecordBase *newrecord)
{
    if (newrecord != d.constData())
    {
        //the update has caused a change in record type
        // - update our pointer
        d = newrecord;
    }
}

/** Move the residue 'res' */
void ChangeRecord::moveResidue(const Residue &res)
{
    updatePointer( d->moveResidue(res) );
}

/** Move the whole molecule 'mol' */
void ChangeRecord::moveMolecule(const Molecule &mol)
{
    updatePointer( d->moveMolecule(mol) );
}

/** Change the molecule that is recorded with its parameters in 'mol_and_params' */
void ChangeRecord::changeMolecule(const ParameterTable &mol_and_params)
{
    updatePointer( d->changeMolecule(mol_and_params) );
}

/** Remove the molecule 'mol' */
void ChangeRecord::removeMolecule(const Molecule &mol)
{
    updatePointer( d->removeMolecule(mol) );
}

/** Add the molecule 'mol' */
void ChangeRecord::addMolecule(const ParameterTable &mol_and_params)
{
    updatePointer( d->addMolecule(mol_and_params) );
}
