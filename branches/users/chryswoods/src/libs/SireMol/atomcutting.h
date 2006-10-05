/**
  * @file
  *
  * C++ Interface: AtomCutting
  *
  * Description: Interface for AtomCutting
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREMOL_ATOMCUTTING_H
#define SIREMOL_ATOMCUTTING_H

#include "cuttingfunction.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomCutting;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomCutting&);
QDataStream& operator>>(QDataStream&, SireMol::AtomCutting&);

namespace SireMol
{

/**
This is the ConvertFunction that is used to create a molecule that uses
atom-based cutting - i.e. each atom is in its own CutGroup.

This is a simple function object. Its only purpose is to return
the number of the CutGroup into which a particular atom should be
placed within a molecule whose data is contained in an EditMolData object.

@author Christopher Woods
*/
class SIREMOL_EXPORT AtomCutting : public CuttingFunctionBase
{

friend QDataStream& ::operator<<(QDataStream&, const AtomCutting&);
friend QDataStream& ::operator>>(QDataStream&, AtomCutting&);

public:
    AtomCutting();
    AtomCutting(const AtomCutting &other);

    ~AtomCutting();

    CutGroupNum operator()(const AtomIndex &atom, const EditMolData &moldata) const;

    static const char* typeName()
    {
        return "SireMol::AtomCutting";
    }

    const char* what() const
    {
        return AtomCutting::typeName();
    }

    AtomCutting* clone() const
    {
        return new AtomCutting(*this);
    }
};

}

Q_DECLARE_METATYPE(SireMol::AtomCutting)

SIRE_END_HEADER

#endif
