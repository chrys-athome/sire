
#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "assign_dihedrals.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireDB;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<assign_dihedrals> r_assign_dihedrals;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const assign_dihedrals &dihedrals)
{
    writeHeader(ds, r_assign_dihedrals, 0)
        << static_cast<const assign_internals<MolDihedralInfo>&>(dihedrals);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, assign_dihedrals &dihedrals)
{
    VersionID v = readHeader(ds, r_assign_dihedrals);

    if (v == 0)
    {
        ds >> static_cast<assign_internals<MolDihedralInfo>&>(dihedrals);
    }
    else
        throw version_error(v, "1", r_assign_dihedrals, CODELOC);

    return ds;
}

/** Constructor */
assign_dihedrals::assign_dihedrals(const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
{}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
{
    addDataBase(db0);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0, const using_database &db1,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0, const using_database &db1,
                           const using_database &db2,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
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

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
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

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8, const using_database &db9,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(generator)
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

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
{}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
{
    addDataBase(db0);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0, const using_database &db1,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
{
    addDataBase(db0);
    addDataBase(db1);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
}

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
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

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
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

/** Assign the dihedrals using the requirements specified */
assign_dihedrals::assign_dihedrals(const QSet<Dihedral> &dihedrals,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8, const using_database &db9,
                           const DihedralGeneratorBase &generator)
             : assign_internals<MolDihedralInfo>(dihedrals, generator)
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
assign_dihedrals::assign_dihedrals(const assign_dihedrals &other)
             : assign_internals<MolDihedralInfo>(other)
{}

/** Destructor */
assign_dihedrals::~assign_dihedrals()
{}

/** Assign the parameters in the table 'param_table' using
    the database 'database' according to the requirements contained in this object.
    If 'overWrite()' is true then the parameters will be overwritten,
    otherwise only parameters for dihedrals that are currently missing
    parameters will be found. */
void assign_dihedrals::assignParameters( const Molecule &molecule,
                                         ParameterTable &param_table,
                                         ParameterDB &database,
                                         const MatchMRData &matchmr ) const
{
    this->assignInternals(molecule, param_table, database, matchmr);
}
