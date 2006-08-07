#ifndef SIREMM_BONDGENERATOR_H
#define SIREMM_BONDGENERATOR_H

#include "internalgenerator.hpp"
#include "molbondinfo.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class BondGenerator;
class UsePassedBonds;
}

QDataStream& operator<<(QDataStream&, const SireMM::BondGenerator&);
QDataStream& operator>>(QDataStream&, SireMM::BondGenerator&);

QDataStream& operator<<(QDataStream&, const SireMM::UsePassedBonds&);
QDataStream& operator>>(QDataStream&, SireMM::UsePassedBonds&);

namespace SireMM
{

typedef InternalGenerator<MolBondInfo> BondGeneratorBase;
typedef UsePassedInternals<MolBondInfo> UsePassedBondsBase;

/**
This is the default implementation of a bond generator. This function object is used to generate all of the bonds in a molecule and to place them into a MolBondInfo. It uses the connectivity of the molecule to find all of the bonds, though ignores any bonds to dummy atoms.

@author Christopher Woods
*/
class SIREMM_EXPORT BondGenerator : public BondGeneratorBase
{
public:
    BondGenerator();

    BondGenerator(const BondGenerator&);

    ~BondGenerator();

    void generate(const Molecule &mol, MolBondInfo &bondinfo) const;
    
    BondGenerator* clone() const
    {
        return new BondGenerator(*this);
    }
    
    static const char* typeName()
    {
        return "SireMM::BondGenerator";
    }
    
    const char* what() const
    {
        return BondGenerator::typeName();
    }
};

/**
This is a bond generator that generates only the passed bonds.

@author Christopher Woods
*/
class SIREMM_EXPORT UsePassedBonds : public UsePassedBondsBase
{
public:
    UsePassedBonds();
    UsePassedBonds(const QSet<Bond> &bonds);

    UsePassedBonds(const UsePassedBonds &other);

    ~UsePassedBonds();

    void generate(const Molecule &mol, MolBondInfo &bondinfo) const;
    
    UsePassedBonds* clone() const
    {
        return new UsePassedBonds(*this);
    }
    
    static const char* typeName()
    {
        return "SireMM::UsePassedBonds";
    }
    
    const char* what() const
    {
        return UsePassedBonds::typeName();
    }
};

}

Q_DECLARE_METATYPE(SireMM::BondGenerator)
Q_DECLARE_METATYPE(SireMM::UsePassedBonds)

SIRE_END_HEADER

#endif
