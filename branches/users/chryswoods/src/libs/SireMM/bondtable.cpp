
#include "SireMol/qhash_siremol.h"
#include "qhash_siremm.h"

#include "bondtable.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireMol;
using namespace SireCAS;

static const RegisterMetaType<BondTable> r_bondtable("SireMM::BondTable");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const BondTable &table)
{
    writeHeader(ds, r_bondtable, 1) 
        << static_cast<const BondTableT<Expression>&>(table);
        
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, BondTable &table)
{
    VersionID v = readHeader(ds, r_bondtable);
    
    if (v == 1)
    {
        ds >> static_cast<BondTableT<Expression>&>(table);
    }
    else
        throw version_error(v, "1", r_bondtable, CODELOC);
    
    return ds;
}

/** Empty constructor */
BondTable::BondTable() : BondTableT<Expression>()
{}

/** Construct a BondTable to hold the expressions for the 
    bonds in 'bondinfo' */
BondTable::BondTable(const MolBondInfo &bondinfo) : BondTableT<Expression>(bondinfo)
{}

/** Construct a BondTable that holds the bonds for the molecule 'mol'. Note that 
    no bonds will be contained in this object initially. */
BondTable::BondTable(const MoleculeInfo &molinfo) : BondTableT<Expression>(molinfo)
{}

/** Construct a BondTable to hold the bonds in the molecule 'mol' that 
    were generated using the bond generator 'generator' */
BondTable::BondTable(const Molecule &mol, const BondGeneratorBase &generator)
          : BondTableT<Expression>( generator.generate(mol) )
{}

/** Copy constructor */
BondTable::BondTable(const BondTable &other) : BondTableT<Expression>(other)
{}

/** Destructor */
BondTable::~BondTable()
{}

/** Add the contents of the other table 'other' to this table. This will throw
    an exception if this table is of the wrong type, or if it is incompatible
    with this table. 
    
    The parameters of 'other' will overwrite the parameters in this table.
    
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void BondTable::add(const TableBase &other)
{
    //is 'other' a BondTable?
    if (not other.isA<BondTable>())
        throw SireError::invalid_cast( QObject::tr(
                    "Cannot add a %1 to a BondTable!").arg(other.what()), CODELOC );
                    
    const BondTable &other_bond = other.asA<BondTable>();
    
    //are these tables compatible?
    if (info().info() != other_bond.info().info())
        throw SireError::incompatible_error( QObject::tr(
                    "Cannot add the table as the molecules are incompatible."),
                                  CODELOC );

    //add each bond parameter from 'other' to this table
    for (MolBondInfo::const_iterator it = other_bond.info().begin();
         it.isValid();
         ++it)
    {
        const Bond &bond = it.key();
        
        if (other_bond.assignedParameter(bond))
            //set the parameter for this bond
            this->setParameter( bond, other_bond[bond] );
        else
            //just add the bond
            this->addBond(bond);
    }
}
