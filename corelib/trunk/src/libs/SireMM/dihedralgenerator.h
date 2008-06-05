#ifndef SIREMM_DIHEDRALGENERATOR_H
#define SIREMM_DIHEDRALGENERATOR_H

#include "internalgenerator.hpp"
#include "moldihedralinfo.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class DihedralGenerator;
class UsePassedDihedrals;
}

QDataStream& operator<<(QDataStream&, const SireMM::DihedralGenerator&);
QDataStream& operator>>(QDataStream&, SireMM::DihedralGenerator&);

QDataStream& operator<<(QDataStream&, const SireMM::UsePassedDihedrals&);
QDataStream& operator>>(QDataStream&, SireMM::UsePassedDihedrals&);

namespace SireMM
{

typedef InternalGenerator<MolDihedralInfo> DihedralGeneratorBase;
typedef UsePassedInternals<MolDihedralInfo> UsePassedDihedralsBase;

/**
This is the default implementation of a dihedral generator. This function object is used to generate all of the dihedrals in a molecule and to place them into a MolDihedralInfo. It uses the connectivity of the molecule to find all of the dihedrals, though ignores any dihedrals that involve dummy atoms.

@author Christopher Woods
*/
class SIREMM_EXPORT DihedralGenerator : public DihedralGeneratorBase
{
public:
    DihedralGenerator();

    DihedralGenerator(const DihedralGenerator&);

    ~DihedralGenerator();

    void generate(const Molecule &mol, MolDihedralInfo &dihedralinfo) const;
    
    DihedralGenerator* clone() const
    {
        return new DihedralGenerator(*this);
    }
    
    static const char* typeName()
    {
        return "SireMM::DihedralGenerator";
    }
    
    const char* what() const
    {
        return DihedralGenerator::typeName();
    }
};

/**
This is a dihedral generator that generates only the passed dihedrals.

@author Christopher Woods
*/
class SIREMM_EXPORT UsePassedDihedrals : public UsePassedDihedralsBase
{
public:
    UsePassedDihedrals();
    UsePassedDihedrals(const QSet<Dihedral> &dihedrals);

    UsePassedDihedrals(const UsePassedDihedrals &other);

    ~UsePassedDihedrals();

    void generate(const Molecule &mol, MolDihedralInfo &dihedralinfo) const;
    
    UsePassedDihedrals* clone() const
    {
        return new UsePassedDihedrals(*this);
    }
    
    static const char* typeName()
    {
        return "SireMM::UsePassedDihedrals";
    }
    
    const char* what() const
    {
        return UsePassedDihedrals::typeName();
    }
};

}

Q_DECLARE_METATYPE(SireMM::DihedralGenerator)
Q_DECLARE_METATYPE(SireMM::UsePassedDihedrals)

SIRE_END_HEADER

#endif
