#ifndef SIREMM_ASSIGN_ANGLES_H
#define SIREMM_ASSIGN_ANGLES_H

#include "assign_internals.hpp"
#include "molangleinfo.h"
#include "anglegenerator.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class assign_angles;
}

QDataStream& operator<<(QDataStream&, const SireMM::assign_angles&);
QDataStream& operator>>(QDataStream&, SireMM::assign_angles&);

namespace SireMM
{


using SireMol::Angle;

/**
This is to Angle as assign_atoms is to Atom - namely this is the class used to control the assignment of angle parameters.
	
@author Christopher Woods
*/
class SIREMM_EXPORT assign_angles : public assign_internals<MolAngleInfo>
{

friend QDataStream& ::operator<<(QDataStream&, const assign_angles&);
friend QDataStream& ::operator>>(QDataStream&, assign_angles&);

public:
    assign_angles(const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0, const using_database &db1,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0, const using_database &db1,
                 const using_database &db2,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8, const using_database &db9,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const QSet<Angle> &angles,
                 const AngleGeneratorBase &generator = AngleGenerator());
    
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0,
                 const AngleGeneratorBase &generator = AngleGenerator());
    
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0, const using_database &db1,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8,
                 const AngleGeneratorBase &generator = AngleGenerator());
  
    assign_angles(const QSet<Angle> &angles,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8, const using_database &db9,
                 const AngleGeneratorBase &generator = AngleGenerator());
    
    assign_angles(const assign_angles &other);

    ~assign_angles();

    assign_angles* clone() const
    {
        return new assign_angles(*this);
    }

    static const char* typeName()
    {
        return "SireDB::assign_angles";
    }
    
    const char* what() const
    {
        return assign_angles::typeName();
    }

    void assignParameters( const Molecule &molecule,
                           ParameterTable &param_table, 
                           ParameterDB &database,
                           const MatchMRData &matchmr = MatchMRData() ) const;
};

}

Q_DECLARE_METATYPE(SireMM::assign_angles)

SIRE_END_HEADER

#endif
