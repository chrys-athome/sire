/**
  * @file
  *
  * C++ Interface: ResidueCutting
  *
  * Description: Interface for ResidueCutting
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREMOL_RESIDUECUTTING_H
#define SIREMOL_RESIDUECUTTING_H

#include "cuttingfunction.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResidueCutting;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResidueCutting&);
QDataStream& operator>>(QDataStream&, SireMol::ResidueCutting&);

namespace SireMol
{

/**
This is the ConvertFunction that is used to create a molecule that uses
residue-based cutting - i.e. each residue is in its own CutGroup.

This is a simple function object. Its only purpose is to return
the number of the CutGroup into which a particular atom should be
placed within a molecule whose data is contained in an EditMolData object.

@author Christopher Woods
*/
class SIREMOL_EXPORT ResidueCutting : public CuttingFunctionBase
{

friend QDataStream& ::operator<<(QDataStream&, const ResidueCutting&);
friend QDataStream& ::operator>>(QDataStream&, ResidueCutting&);

public:
    ResidueCutting();
    ResidueCutting(const ResidueCutting &other);

    ~ResidueCutting();

    CutGroupNum operator()(const AtomIndex &atom, const EditMolData &moldata) const;

    static const char* typeName()
    {
        return "SireMol::ResidueCutting";
    }

    const char* what() const
    {
        return ResidueCutting::typeName();
    }

    ResidueCutting* clone() const
    {
        return new ResidueCutting(*this);
    }
};

}

Q_DECLARE_METATYPE(SireMol::ResidueCutting)

SIRE_END_HEADER

#endif
