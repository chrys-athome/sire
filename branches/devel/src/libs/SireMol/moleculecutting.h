/**
  * @file
  *
  * C++ Interface: MoleculeCutting
  *
  * Description: Interface for MoleculeCutting
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREMOL_MOLECULECUTTING_H
#define SIREMOL_MOLECULEUTTING_H

#include "cuttingfunction.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeCutting;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeCutting&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeCutting&);

namespace SireMol
{

/**
This is the ConvertFunction that is used to create a molecule that uses
molecule-based cutting - i.e. every atom in the molecule is in the same
CutGroup (CutGroup number 1).

This is a simple function object. Its only purpose is to return
the number of the CutGroup into which a particular atom should be
placed within a molecule whose data is contained in an EditMolData object.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeCutting : public CuttingFunctionBase
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeCutting&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeCutting&);

public:
    MoleculeCutting();
    MoleculeCutting(const MoleculeCutting &other);

    ~MoleculeCutting();

    CutGroupNum operator()(const AtomIndex &atom, const EditMolData &moldata) const;

    static const char* typeName()
    {
        return "SireMol::MoleculeCutting";
    }

    const char* what() const
    {
        return MoleculeCutting::typeName();
    }

    MoleculeCutting* clone() const
    {
        return new MoleculeCutting(*this);
    }
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeCutting)

SIRE_END_HEADER

#endif
