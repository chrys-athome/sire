/**
  * @file
  *
  * C++ Interface: MovedMol
  *
  * Description: 
  * Interface for MovedMol
  * 
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREFF_MOVEDMOL_H
#define SIREFF_MOVEDMOL_H

#include "changerecordbase.h"

#include "SireMol/molecule.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class MovedMol;
}

QDataStream& operator<<(QDataStream&, const SireFF::MovedMol&);
QDataStream& operator>>(QDataStream&, SireFF::MovedMol&);

namespace SireFF 
{

/**
This class holds the record of a moved molecule.

@author Christopher Woods
*/
class SIREFF_EXPORT MovedMol : public ChangeRecordBase
{

friend QDataStream& ::operator<<(QDataStream&, const MovedMol&);
friend QDataStream& ::operator>>(QDataStream&, MovedMol&);

public:
    MovedMol();
    MovedMol(const Molecule &mol);

    MovedMol(const MovedMol &other);

    ~MovedMol();

    MovedMol& operator=(const MovedMol &other);
    
    bool operator==(const MovedMol &other) const;
    bool operator!=(const MovedMol &other) const;

    const char* what() const
    {
        return "SireFF::MovedMol";
    }
    
    ChangeRecordBase* clone() const
    {
        return new MovedMol(*this);
    }

    ChangeRecordBase* moveResidue(const Residue &res);
    ChangeRecordBase* moveMolecule(const Molecule &mol);
    
    ChangeRecordBase* changeResidue(const Residue &res,
                                    const ParameterTable &params);
    
    ChangeRecordBase* changeMolecule(const Molecule &mol,
                                     const ParameterTable &params);
    
    ChangeRecordBase* removeMolecule(const Molecule &mol);
    
    ChangeRecordBase* addMolecule(const Molecule &mol,
                                  const ParameterTable &params);

private:
    void assertCompatibleWith(const Molecule &mol) const;
    void assertCompatibleVersion(const Molecule &mol) const;
    
    /** Copy of the molecule that has been moved */
    Molecule movedmol;
};

}

Q_DECLARE_METATYPE(SireFF::MovedMol)

SIRE_END_HEADER

#endif
