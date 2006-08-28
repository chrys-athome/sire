#ifndef SIREFF_REMOVEDMOL_H
#define SIREFF_REMOVEDMOL_H
/**
  * @file
  *
  * C++ Interface: RemoveMol
  *
  * Description: 
  * Interface to RemovedMol
  * 
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "changerecordbase.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class RemovedMol;
}

QDataStream& operator<<(QDataStream&, const SireFF::RemovedMol&);
QDataStream& operator>>(QDataStream&, SireFF::RemovedMol&);

namespace SireFF 
{

/**
This class is used to record that a molecule has been removed.

@author Christopher Woods
*/
class SIREFF_EXPORT RemovedMol : public ChangeRecordBase
{

friend QDataStream& ::operator<<(QDataStream&, const RemovedMol&);
friend QDataStream& ::operator>>(QDataStream&, RemovedMol&);

public:
    RemovedMol();
    RemovedMol(const Molecule &mol);
    
    RemovedMol(const RemovedMol &other);

    ~RemovedMol();

    const char* what() const
    {
        return "SireFF::RemovedMol";
    }

    ChangeRecordBase* clone() const
    {
        return new RemovedMol(*this);
    }

    ChangeRecordBase* moveResidue(const Residue &res);
    ChangeRecordBase* moveMolecule(const Molecule &mol);
    
    ChangeRecordBase* changeMolecule(const ParameterTable &mol_and_params);
    
    ChangeRecordBase* removeMolecule(const Molecule &mol);
    
    ChangeRecordBase* addMolecule(const ParameterTable &mol_and_params);

private:
    void invalidError(const Molecule &mol) const;
    void incompatibleError(const Molecule &mol) const;
    
    /** The ID number of the removed molecule */
    MoleculeID molid;
};

}

Q_DECLARE_METATYPE(SireFF::RemovedMol)

SIRE_END_HEADER

#endif
