
#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "assign_angles.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireDB;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<assign_angles> r_assign_angles("SireMM::assign_angles");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const assign_angles &angles)
{
    writeHeader(ds, r_assign_angles, 0) 
        << static_cast<const assign_internals<MolAngleInfo>&>(angles);
        
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, assign_angles &angles)
{
    VersionID v = readHeader(ds, r_assign_angles);
    
    if (v == 0)
    {
        ds >> static_cast<assign_internals<MolAngleInfo>&>(angles);
    }
    else
        throw version_error(v, "1", r_assign_angles, CODELOC);
    
    return ds;
}

/** Constructor */
assign_angles::assign_angles(const AngleGeneratorBase &generator) 
             : assign_internals<MolAngleInfo>(generator)
{}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0, const using_database &db1,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0, const using_database &db1,
                           const using_database &db2,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8, const using_database &db9,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
    addDataBase(db9);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0, const using_database &db1,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
    addDataBase(db1);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
}

/** Assign the angles using the requirements specified */
assign_angles::assign_angles(const QSet<Angle> &angles,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8, const using_database &db9,
                           const AngleGeneratorBase &generator)
             : assign_internals<MolAngleInfo>(angles, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
    addDataBase(db9);
}

/** Copy constructor */
assign_angles::assign_angles(const assign_angles &other)
             : assign_internals<MolAngleInfo>(other)
{}

/** Destructor */
assign_angles::~assign_angles()
{}

/** Assign the parameters in the table 'param_table' using 
    the database 'database' according to the requirements contained in this object.
    If 'overWrite()' is true then the parameters will be overwritten, 
    otherwise only parameters for angles that are currently missing 
    parameters will be found. */
void assign_angles::assignParameters(const Molecule &molecule, 
                                     ParameterTable &param_table, 
                                     ParameterDB &database,
                                     const MatchMRData &matchmr ) const
{
    this->assignInternals(molecule, param_table, database, matchmr);
}
