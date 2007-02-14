/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#ifndef SIREMM_ANGLEGENERATOR_H
#define SIREMM_ANGLEGENERATOR_H

#include "internalgenerator.hpp"
#include "molangleinfo.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class AngleGenerator;
class UsePassedAngles;
}

QDataStream& operator<<(QDataStream&, const SireMM::AngleGenerator&);
QDataStream& operator>>(QDataStream&, SireMM::AngleGenerator&);

QDataStream& operator<<(QDataStream&, const SireMM::UsePassedAngles&);
QDataStream& operator>>(QDataStream&, SireMM::UsePassedAngles&);

namespace SireMM
{

typedef InternalGenerator<MolAngleInfo> AngleGeneratorBase;
typedef UsePassedInternals<MolAngleInfo> UsePassedAnglesBase;

/**
This is the default implementation of a angle generator. This function object is used to generate all of the angles in a molecule and to place them into a MolAngleInfo. It uses the connectivity of the molecule to find all of the angles, though ignores any angles that involve dummy atoms.

@author Christopher Woods
*/
class SIREMM_EXPORT AngleGenerator : public AngleGeneratorBase
{
public:
    AngleGenerator();

    AngleGenerator(const AngleGenerator&);

    ~AngleGenerator();

    void generate(const Molecule &mol, MolAngleInfo &angleinfo) const;
    
    AngleGenerator* clone() const
    {
        return new AngleGenerator(*this);
    }
    
    static const char* typeName()
    {
        return "SireMM::AngleGenerator";
    }
    
    const char* what() const
    {
        return AngleGenerator::typeName();
    }
};

/**
This is a angle generator that generates only the passed angles.

@author Christopher Woods
*/
class SIREMM_EXPORT UsePassedAngles : public UsePassedAnglesBase
{
public:
    UsePassedAngles();
    UsePassedAngles(const QSet<Angle> &angles);

    UsePassedAngles(const UsePassedAngles &other);

    ~UsePassedAngles();

    void generate(const Molecule &mol, MolAngleInfo &angleinfo) const;
    
    UsePassedAngles* clone() const
    {
        return new UsePassedAngles(*this);
    }
    
    static const char* typeName()
    {
        return "SireMM::UsePassedAngles";
    }
    
    const char* what() const
    {
        return UsePassedAngles::typeName();
    }
};

}

Q_DECLARE_METATYPE(SireMM::AngleGenerator)
Q_DECLARE_METATYPE(SireMM::UsePassedAngles)

SIRE_END_HEADER

#endif
