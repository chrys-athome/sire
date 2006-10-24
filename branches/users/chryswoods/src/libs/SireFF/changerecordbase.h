#ifndef SIREFF_CHANGERECORDBASE_H
#define SIREFF_CHANGERECORDBASE_H
/**
  * @file
  *
  * C++ Interface: ChangeRecordBase
  *
  * Description: 
  * File contains the interface to ChangeRecordBase.
  * 
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
class Residue;
}

namespace SireDB
{
class ParameterTable;
}

namespace SireFF
{

using SireMol::Molecule;
using SireMol::Residue;

using SireDB::ParameterTable;

/**
This is the virtual base class of all change records. A ChangeRecord is a record of a change to a molecule. Derivations of this class are used to record changes to a molecule, e.g. MovedParts records moves made on parts of a molecule, MovedMol records moves of the whole molecule, ChangedMol records changes of a molecule, AddedMol records additions of a molecule and RemovedMol records removed molecules.

This class is designed to be used either by classes that inherit from this class, or by ChangeRecord. You should not directly use this class in your own code.

@author Christopher Woods
*/
class SIREFF_EXPORT ChangeRecordBase
{
public:
    ChangeRecordBase();
    ChangeRecordBase(const ChangeRecordBase &other);

    virtual ~ChangeRecordBase();

    /** Return the type of this ChangeRecord */
    virtual const char* what() const
    {
        return "SireFF::ChangeRecordBase";
    }
    
    /** Return a clone of this ChangeRecord */
    virtual ChangeRecordBase* clone() const
    {
        return new ChangeRecordBase(*this);
    }

    /** Return whether or not this is a change record of type 'T' */
    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this);
    }

    /** Return this record cast as type 'T' - will have undefined
        results unless isA<T>() returns true */
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }

    /** Return this record cast as type 'T' - will have undefined
        results unless isA<T>() returns true */
    template<class T>
    T& asA()
    {
        return dynamic_cast<T&>(*this);
    }

    virtual ChangeRecordBase* moveResidue(const Residue &res);
    virtual ChangeRecordBase* moveMolecule(const Molecule &mol);
    
    virtual ChangeRecordBase* changeResidue(const Residue &res, 
                                            const ParameterTable &newparams);
    
    virtual ChangeRecordBase* changeMolecule(const Molecule &mol,
                                             const ParameterTable &newparams);
    
    virtual ChangeRecordBase* removeMolecule(const Molecule &mol);
    
    virtual ChangeRecordBase* addMolecule(const Molecule &mol,
                                          const ParameterTable &params);
};

}

SIRE_END_HEADER

#endif
