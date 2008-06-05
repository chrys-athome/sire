#ifndef SIREMM_ASSIGN_DIHEDRALS_H
#define SIREMM_ASSIGN_DIHEDRALS_H

#include "assign_internals.hpp"
#include "moldihedralinfo.h"
#include "dihedralgenerator.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class assign_dihedrals;
}

QDataStream& operator<<(QDataStream&, const SireMM::assign_dihedrals&);
QDataStream& operator>>(QDataStream&, SireMM::assign_dihedrals&);

namespace SireMM
{


using SireMol::Dihedral;

/**
This is to Dihedral as assign_atoms is to Atom - namely this is the class used to control the assignment of dihedral parameters.
	
@author Christopher Woods
*/
class SIREMM_EXPORT assign_dihedrals : public assign_internals<MolDihedralInfo>
{

friend QDataStream& ::operator<<(QDataStream&, const assign_dihedrals&);
friend QDataStream& ::operator>>(QDataStream&, assign_dihedrals&);

public:
    assign_dihedrals(const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0, const using_database &db1,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0, const using_database &db1,
                 const using_database &db2,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8, const using_database &db9,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
    
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
    
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0, const using_database &db1,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
  
    assign_dihedrals(const QSet<Dihedral> &dihedrals,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8, const using_database &db9,
                 const DihedralGeneratorBase &generator = DihedralGenerator());
    
    assign_dihedrals(const assign_dihedrals &other);

    ~assign_dihedrals();

    assign_dihedrals* clone() const
    {
        return new assign_dihedrals(*this);
    }

    static const char* typeName()
    {
        return "SireDB::assign_dihedrals";
    }
    
    const char* what() const
    {
        return assign_dihedrals::typeName();
    }

    void assignParameters( ParameterTable &param_table, 
                           ParameterDB &database,
                           const MatchMRData &matchmr = MatchMRData() ) const;
};

}

Q_DECLARE_METATYPE(SireMM::assign_dihedrals)

SIRE_END_HEADER

#endif
