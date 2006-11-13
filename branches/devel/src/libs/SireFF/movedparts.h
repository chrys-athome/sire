#ifndef SIREFF_MOVEDPARTS_H
#define SIREFF_MOVEDPARTS_H
/**
  * @file
  *
  * C++ Interface: MovedParts
  *
  * Description: 
  * Interface for the MovedParts class
  * 
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "changerecordbase.h"

#include "SireMol/cutgroup.h"
#include "SireMol/moleculeversion.h"

#include <QHash>

SIRE_BEGIN_HEADER

namespace SireFF
{
class MovedParts;
}

QDataStream& operator<<(QDataStream&, const SireFF::MovedParts&);
QDataStream& operator>>(QDataStream&, SireFF::MovedParts&);

namespace SireMol
{
class Residue;
}

namespace SireFF 
{

using SireMol::CutGroup;
using SireMol::CutGroupID;
using SireMol::MoleculeID;
using SireMol::MoleculeVersion;
using SireMol::Residue;

/**
This class holds the record of the movement of part of a molecule.

@author Christopher Woods
*/
class MovedParts : public ChangeRecordBase
{

friend QDataStream& ::operator<<(QDataStream&, const MovedParts&);
friend QDataStream& ::operator>>(QDataStream&, MovedParts&);

public:
    MovedParts();
    MovedParts(const Residue &residue);
    
    MovedParts(const MovedParts &other);

    ~MovedParts();

    const char* what() const
    {
        return "SireFF::MovedParts";
    }

    ChangeRecordBase* clone() const
    {
        return new MovedParts(*this);
    }

    ChangeRecordBase* moveResidue(const Residue &res);
    ChangeRecordBase* moveMolecule(const Molecule &mol);
    
    ChangeRecordBase* changeMolecule(const Molecule &mol,
                                     const ParameterTable &params);
    
    ChangeRecordBase* removeMolecule(const Molecule &mol);
    
    ChangeRecordBase* addMolecule(const Molecule &mol,
                                  const ParameterTable &params);

private:
    /** The ID of the moved molecule */
    MoleculeID molid;
    /** The version of the moved molecule */
    MoleculeVersion molversion;
    
    /** Copies of the CutGroups that contain the moved parts of
        the molecule, indexed by CutGroupID */
    QHash<CutGroupID, CutGroup> movedgroups;
};

}

Q_DECLARE_METATYPE(SireFF::MovedParts)

SIRE_END_HEADER

#endif
