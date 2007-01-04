
#include <QMetaType>

#include "SireMol/molecule.h"

#include "assign_parameters.h"
#include "parametertable.h"

#include "using_database.h"
#include "overwriteparams.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireMol;

static const RegisterMetaType<assign_parameters> r_assign_params;

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds,
                                      const SireDB::assign_parameters &assign_params)
{
    writeHeader(ds, r_assign_params, 1);

    //save the number of instructions to the stream
    quint32 n = assign_params.instrctns.count();
    ds << n;

    //use QMetaType to stream each type...
    for (uint i=0; i<n; ++i)
    {
        const DynamicSharedPtr<AssignBase> &ptr = assign_params.instrctns.at(i);

        //get the name of the class
        const char *type_name = ptr->what();

        //is this a registered QMetaType?
        int type_id = QMetaType::type(type_name);

        if (not QMetaType::isRegistered(type_id))
        {
            throw SireError::program_bug(QObject::tr(
                "The instruction with type name \"%1\" (typeid == %2) has not been "
                "registered with QMetaType. Please ensure that this type has been "
                "loaded and has been registered.")
                      .arg(type_name).arg(type_id), CODELOC);
        }

        //serialise the type name and the type
        ds << QLatin1String(type_name);

        if ( not QMetaType::save(ds, type_id, ptr.constData()) )
            throw SireError::program_bug(QObject::tr(
                "Serialisation of the instruction with type name \"%1\" (typeid == "
                "%2) has failed, despite the type having been registered with "
                "QMetaType!").arg(type_name).arg(type_id), CODELOC);
    }

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds,
                                      SireDB::assign_parameters &assign_params)
{
    VersionID v = readHeader(ds, r_assign_params);

    if (v == 1)
    {
        //clear any existing instructions
        assign_params.instrctns.clear();

        //how many instructions need to be loaded?
        quint32 n;
        ds >> n;

        //load each instruction in turn
        for (uint i=0; i<n; ++i)
        {
            //load the type name first
            QString type_name;
            ds >> type_name;

            //has this type been registered with QMetaType?
            int type_id = QMetaType::type( qPrintable(type_name) );

            if (not QMetaType::isRegistered(type_id))
            {
                throw version_error(QObject::tr(
                    "The instruction with type name \"%1\" (typeid == %2) has not been "
                    "registered with QMetaType. Please ensure that this type has been "
                    "loaded and has been registered.")
                          .arg(type_name).arg(type_id), CODELOC);
            }

            //create an object with this type
            DynamicSharedPtr<AssignBase> ptr = static_cast<AssignBase*>
                                          ( QMetaType::construct(type_id, 0) );

            if (ptr.constData() == 0)
                throw SireError::program_bug(QObject::tr(
                    "Failed to construct an instruction of type name \"%1\" "
                    "(typeid == %2), despite the type having been registered with "
                    "QMetaType").arg(type_name).arg(type_id), CODELOC);

            //now deserialise the data into this new object
            if ( not QMetaType::load(ds, type_id, ptr) )
                throw SireError::program_bug(QObject::tr(
                    "Failed to deserialise an instruction of type name \"%1\" "
                    "(typeid == %2), despite the type having been registered with "
                    "QMetaType").arg(type_name).arg(type_id), CODELOC);

            //add the instruction to the list
            assign_params.instrctns.append(ptr);
        }
    }
    else
        throw version_error(v, "1", r_assign_params, CODELOC);

    return ds;
}

/** Null constructor */
assign_parameters::assign_parameters()
{}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(const AssignInstruction &i0)
{
    addInstruction(i0);
}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(
                        const AssignInstruction &i0, const AssignInstruction &i1)
{
    addInstruction(i0);
    addInstruction(i1);
}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(
                        const AssignInstruction &i0, const AssignInstruction &i1,
                        const AssignInstruction &i2)
{
    addInstruction(i0);
    addInstruction(i1);
    addInstruction(i2);
}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(
                        const AssignInstruction &i0, const AssignInstruction &i1,
                        const AssignInstruction &i2, const AssignInstruction &i3)
{
    addInstruction(i0);
    addInstruction(i1);
    addInstruction(i2);
    addInstruction(i3);
}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(
                        const AssignInstruction &i0, const AssignInstruction &i1,
                        const AssignInstruction &i2, const AssignInstruction &i3,
                        const AssignInstruction &i4)
{
    addInstruction(i0);
    addInstruction(i1);
    addInstruction(i2);
    addInstruction(i3);
    addInstruction(i4);
}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(
                        const AssignInstruction &i0, const AssignInstruction &i1,
                        const AssignInstruction &i2, const AssignInstruction &i3,
                        const AssignInstruction &i4, const AssignInstruction &i5)
{
    addInstruction(i0);
    addInstruction(i1);
    addInstruction(i2);
    addInstruction(i3);
    addInstruction(i4);
    addInstruction(i5);
}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(
                        const AssignInstruction &i0, const AssignInstruction &i1,
                        const AssignInstruction &i2, const AssignInstruction &i3,
                        const AssignInstruction &i4, const AssignInstruction &i5,
                        const AssignInstruction &i6)
{
    addInstruction(i0);
    addInstruction(i1);
    addInstruction(i2);
    addInstruction(i3);
    addInstruction(i4);
    addInstruction(i5);
    addInstruction(i6);
}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(
                        const AssignInstruction &i0, const AssignInstruction &i1,
                        const AssignInstruction &i2, const AssignInstruction &i3,
                        const AssignInstruction &i4, const AssignInstruction &i5,
                        const AssignInstruction &i6, const AssignInstruction &i7)
{
    addInstruction(i0);
    addInstruction(i1);
    addInstruction(i2);
    addInstruction(i3);
    addInstruction(i4);
    addInstruction(i5);
    addInstruction(i6);
    addInstruction(i7);
}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(
                        const AssignInstruction &i0, const AssignInstruction &i1,
                        const AssignInstruction &i2, const AssignInstruction &i3,
                        const AssignInstruction &i4, const AssignInstruction &i5,
                        const AssignInstruction &i6, const AssignInstruction &i7,
                        const AssignInstruction &i8)
{
    addInstruction(i0);
    addInstruction(i1);
    addInstruction(i2);
    addInstruction(i3);
    addInstruction(i4);
    addInstruction(i5);
    addInstruction(i6);
    addInstruction(i7);
    addInstruction(i8);
}

/** Construct an object with the following instructions. Assignment instructions
    (e.g. assign_atoms) are added sequentially to the list, and will be performed
    in sequence. Using instructions (e.g. using_parameters or using_relationships)
    will be added to all of the instructions that have preceeded them. */
assign_parameters::assign_parameters(
                        const AssignInstruction &i0, const AssignInstruction &i1,
                        const AssignInstruction &i2, const AssignInstruction &i3,
                        const AssignInstruction &i4, const AssignInstruction &i5,
                        const AssignInstruction &i6, const AssignInstruction &i7,
                        const AssignInstruction &i8, const AssignInstruction &i9)
{
    addInstruction(i0);
    addInstruction(i1);
    addInstruction(i2);
    addInstruction(i3);
    addInstruction(i4);
    addInstruction(i5);
    addInstruction(i6);
    addInstruction(i7);
    addInstruction(i8);
    addInstruction(i9);
}

/** Copy constructor - fast as the class is implicitly shared */
assign_parameters::assign_parameters(const assign_parameters &other)
                  : instrctns(other.instrctns)
{}

/** Destructor */
assign_parameters::~assign_parameters()
{}

/** Add an instruction onto the list of instructions to use for the
    parametisation. If this instruction is an assignment (e.g.
    assign_atoms) then add the instruction to the end of the list.
    If this is a 'using' instruction, e.g. using_parameters or
    using_relationships, then this using instruction is added to
    each of the assignments that have already been loaded into
    the list. */
void assign_parameters::addInstruction(const AssignInstruction &instruc)
{
    const AssignBase *assign_base = dynamic_cast<const AssignBase*>(&instruc);

    if (assign_base)
    {
        //this is an assignment
        instrctns.append( DynamicSharedPtr<AssignBase>(*assign_base) );
        return;
    }

    const using_database *using_base = dynamic_cast<const using_database*>(&instruc);

    if (using_base)
    {
        //this is a using instruction - add this to all of the
        //current assignment instructions
        for (QList< DynamicSharedPtr<AssignBase> >::iterator it = instrctns.begin();
             it != instrctns.end();
             ++it)
        {
            (*it)->addDataBase( *using_base );
        }

        return;
    }

    const OverWriteParams *overwrite = dynamic_cast<const OverWriteParams*>(&instruc);

    if (overwrite)
    {
        //this is an overwrite instruction - set the overwrite
        //value for all of the current assignment instructions
        for (QList< DynamicSharedPtr<AssignBase> >::iterator it = instrctns.begin();
             it != instrctns.end();
             ++it)
        {
            (*it)->setOverwriteParameters( *overwrite );
        }

        return;
    }

    //getting here is a program bug!
    throw SireError::program_bug(QObject::tr(
              "Cannot work out the type of the AssignInstruction! "
              "(typeid == \"%1\")")
                  .arg( typeid(instruc).name() ), CODELOC);
}

/** Continue the assignment of the parameters in 'orig_table' using the
    information stored in the database 'database' and using the instructions
    contained in this object. If 'overwrite' is true then parameters will
    be overwritten - otherwise we will search only for missing parameters.

    The updated parameter table is returned (the original is not changed)
*/
ParameterTable assign_parameters::assign(const Molecule &molecule,
                                         const ParameterTable &orig_table,
                                         ParameterDB &database,
                                         const MatchMRData &matchmr) const
{
    //the table must be compatible with 'molecule', or else
    //it cannot be used to store parameters for this molecule
    orig_table.assertCompatibleWith(molecule);

    //create a copy of the original table - this prevents corrupting
    //the original if there are problems during the assignment!
    ParameterTable new_table(orig_table);

    //apply each instruction in turn
    for (QList< DynamicSharedPtr<AssignBase> >::const_iterator it = instrctns.begin();
         it != instrctns.end();
         ++it)
    {
        (*it)->assignParameters(molecule, new_table, database, matchmr);
    }

    //return the newly assigned parameter table
    return new_table;
}

/** Assign the parameters for the molecule using the information stored
    in the database 'database' and using the instructions stored in this
    object. The resulting parameters are returned in a ParameterTable. */
ParameterTable assign_parameters::assign(const Molecule &molecule,
                                         ParameterDB &database,
                                         const MatchMRData &matchmr) const
{
    return assign(molecule, database, matchmr);
}
