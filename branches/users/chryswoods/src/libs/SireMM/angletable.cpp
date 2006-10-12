
#include "SireMol/qhash_siremol.h"
#include "qhash_siremm.h"

#include "angletable.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireMol;
using namespace SireCAS;

static const RegisterMetaType<AngleTable> r_angletable("SireMM::AngleTable");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const AngleTable &table)
{
    writeHeader(ds, r_angletable, 1) 
        << static_cast<const AngleTableT<Expression>&>(table);
        
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, AngleTable &table)
{
    VersionID v = readHeader(ds, r_angletable);
    
    if (v == 1)
    {
        ds >> static_cast<AngleTableT<Expression>&>(table);
    }
    else
        throw version_error(v, "1", r_angletable, CODELOC);
    
    return ds;
}

/** Empty constructor */
AngleTable::AngleTable() : AngleTableT<Expression>()
{}

/** Construct a AngleTable to hold the expressions for the 
    angles in 'angleinfo' */
AngleTable::AngleTable(const MolAngleInfo &angleinfo) : AngleTableT<Expression>(angleinfo)
{}

/** Construct a AngleTable that holds the angles for the molecule 'mol'. Note that 
    no angles will be contained in this object initially. */
AngleTable::AngleTable(const MoleculeInfo &molinfo) : AngleTableT<Expression>(molinfo)
{}

/** Construct a AngleTable to hold the angles in the molecule 'mol' that 
    were generated using the angle generator 'generator' */
AngleTable::AngleTable(const Molecule &molecule, const AngleGeneratorBase &generator)
          : AngleTableT<Expression>( generator.generate(molecule) )
{}

/** Copy constructor */
AngleTable::AngleTable(const AngleTable &other) : AngleTableT<Expression>(other)
{}

/** Destructor */
AngleTable::~AngleTable()
{}

/** Add the contents of the other table 'other' to this table. This will throw
    an exception if this table is of the wrong type, or if it is incompatible
    with this table. 
    
    The parameters of 'other' will overwrite the parameters in this table.
    
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void AngleTable::add(const TableBase &other)
{
    //is 'other' a AngleTable?
    if (not other.isA<AngleTable>())
        throw SireError::invalid_cast( QObject::tr(
                    "Cannot add a %1 to a AngleTable!").arg(other.what()), CODELOC );
                    
    const AngleTable &other_angle = other.asA<AngleTable>();
    
    //are these tables compatible?
    if (info().info() != other_angle.info().info())
        throw SireError::incompatible_error( QObject::tr(
                    "Cannot add the table as the molecules are incompatible."),
                                  CODELOC );

    //add each angle parameter from 'other' to this table
    for (MolAngleInfo::const_iterator it = other_angle.info().begin();
         it.isValid();
         ++it)
    {
        const Angle &angle = it.key();
        
        if (other_angle.assignedParameter(angle))
            //set the parameter for this angle
            this->setParameter( angle, other_angle[angle] );
        else
            //just add the angle
            this->addAngle(angle);
    }
}
