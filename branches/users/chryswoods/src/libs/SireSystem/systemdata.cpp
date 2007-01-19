
#include "SireSystem/qhash_siresystem.h"

#include "systemdata.h"

#include "SireBase/incremint.h"

#include "SireMol/property.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"

#include "SireFF/ffcomponent.h"

#include "SireSystem/moleculegroup.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"
#include "SireFF/errors.h"

using namespace SireSystem;
using namespace SireStream;
using namespace SireBase;
using namespace SireCAS;
using namespace SireMol;
using namespace SireFF;

//////////
////////// Implementation of SystemData::ExpressionInfo
//////////

//////////
////////// Implementation of SystemData
//////////

static const RegisterMetaType<SystemData> r_sysdata(MAGIC_ONLY,
                                                    "SireSystem::SystemData");

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const SystemData &sysdata)
{
    writeHeader(ds, r_sysdata, 1);

    SharedDataStream sds(ds);

    //sds << .... stuff;

    //stream the info about all of the expressions
    sds << quint32( sysdata.ff_equations.count() );

    for (QHash<SymbolID,SystemData::ExpressionInfo>::const_iterator it
                                = sysdata.ff_equations.begin();
         it != sysdata.ff_equations.end();
         ++it)
    {
        sds << it->ex << it->deps << it->ffids;
    }

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, SystemData &sysdata)
{
    VersionID v = readHeader(ds, r_sysdata);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        //sds >> .... stuff

        //stream in the info about all of the expressions
        quint32 neqns;

        sds >> neqns;

        QHash<SymbolID,SystemData::ExpressionInfo> ff_equations;

        if (neqns > 0)
        {
            ff_equations.reserve(neqns);

            for (uint i=0; i<neqns; ++i)
            {
                SystemData::ExpressionInfo exinfo;

                sds >> exinfo.ex >> exinfo.deps >> exinfo.ffids;

                ff_equations.insert(exinfo.ex.function().ID(), exinfo);
            }
        }

        sysdata.ff_equations = ff_equations;
    }
    else
        throw version_error(v, "1", r_sysdata, CODELOC);

    return ds;
}

/** Static Incremint used to supply ID numbers to Systems */
static Incremint system_incremint;

/** Construct with an unspecified name */
SystemData::SystemData()
           : nme( QObject::tr("Unnamed") ), id_and_version(&system_incremint)
{}

/** Construct a SystemData with a specified name */
SystemData::SystemData(const QString &name)
           : nme(name), id_and_version(&system_incremint)
{}

/** Copy constructor */
SystemData::SystemData(const SystemData &other)
           : ff_equations(other.ff_equations),
             ff_params(other.ff_params),
             cached_energies(other.cached_energies),
             molgroups(other.molgroups),
             props(other.props),
             sys_space(other.sys_space),
             nme(other.nme),
             id_and_version(other.id_and_version)
{}

/** Destructor */
SystemData::~SystemData()
{}

/** Comparison operator - two SystemData objects are equal if they
    have the same ID and version */
bool SystemData::operator==(const SystemData &other) const
{
    return ID() == other.ID() and version() == other.version();
}

/** Comparison operator - two SystemData objects are equal if they
    have the same ID and version */
bool SystemData::operator!=(const SystemData &other) const
{
    return ID() != other.ID() or version() != other.version();
}

/** Increment the major version number of the System. The major number
    is incremented if forcefields or groups are added or removed from
    the system. The minor version number is incremented if anything
    else in the system is changed. */
void SystemData::incrementMajorVersion()
{
    id_and_version.incrementMajor();
    cached_energies.clear();
}

/** Increment the minor version number. This is incremented if any of the
    molecules or properties in the system are changed. */
void SystemData::incrementMinorVersion()
{
    id_and_version.incrementMinor();
    cached_energies.clear();
}

/** Return all of the equations that are part of this system */
QHash<Function,FFExpression> SystemData::equations() const
{
    if (ff_equations.isEmpty())
        return QHash<Function,FFExpression>();

    QHash<Function,FFExpression> eqns;
    eqns.reserve(ff_equations.count());

    for (QHash<SymbolID,ExpressionInfo>::const_iterator it = ff_equations.begin();
         it != ff_equations.end();
         ++it)
    {
        eqns.insert( it->expression().function(), it->expression() );
    }

    return eqns;
}

/** Return the MoleculeGroup with ID == id

    \throw SireSystem::missing_group
*/
const MoleculeGroup& SystemData::group(MoleculeGroupID id) const
{
    QHash<MoleculeGroupID,MoleculeGroup>::const_iterator it = molgroups.find(id);

    //if (it == molgroups.end())
    //    throw SireSystem::missing_group( QObject::tr(
    //        "There is no MoleculeGroup with ID == %1 in the System %2 (%3)")
    //            .arg(id).arg(ID()).arg(version().toString()), CODELOC );

    return *it;
}

/** Add the forcefield equation represented by the expression
    'equation' to this System. Any dependencies of this function
    must already be in this system or else an exception will be
    thrown.

    \throw SireError::dependency_error
*/
void SystemData::add(const FFExpression &equation)
{
    //do we already contain this function? If so, then replace it
    if ( ff_equations.contains(equation.function().ID()) )
    {
        this->replace(equation);
        return;
    }

    //create an ExpressionInfo for this expression
    ExpressionInfo exprinfo(equation, ff_equations);

    //add this to the list of expressions
    ff_equations.insert(equation.function().ID(), exprinfo);

    //changing the equations changes the major version
    //of the system
    this->incrementMajorVersion();
}

/** Add the set of expressions in 'equations' to the list
    of equations in this system. Any dependencies of these
    functions must either be in this forcefield, or must
    be in 'equations' else an exception will be thrown.

    \throw SireError::dependency_error
*/
void SystemData::add(const QHash<Function, FFExpression> &equations)
{
    //lots to do here... - need to resolve the right
    //order to add things....
    #warning TODO SystemData::add

    //changing the equations changes the major version
    //of the system
    this->incrementMajorVersion();
}

/** Remove all of the equations in 'equations'. These equations
    may only be removed if they are not depended on by
    any other equations (not including themselves)

    \throw SireError::dependency_error
*/
void SystemData::remove(const QList<FFExpression> &equations)
{
    //create a copy of the current list of expressions
    //so that it may be restored in something goes wrong...
    QHash<SymbolID,ExpressionInfo> copy_of_ff_equations = ff_equations;

    try
    {
        //remove all of the supplied equations from the current set
        for (QList<FFExpression>::const_iterator it = equations.constBegin();
             it != equations.constEnd();
             ++it)
        {
            ff_equations.remove(it->function().ID());
        }

        //get the list of existing functions
        QHash<Function, FFExpression> remaining_expressions = this->equations();

        //clear all of the expressions
        ff_equations.clear();

        //now add them all in again - this recalculates the dependencies
        this->add(remaining_expressions);
    }
    catch(...)
    {
        //something went wrong...
        //restore the invariant
        ff_equations = copy_of_ff_equations;
        throw;
    }
}

/** Remove the equation 'equation'. An equation may only
    be removed if it is is not depended on by any other equation.

    \throw SireError::dependency_error
*/
void SystemData::remove(const FFExpression &equation)
{
    QList<FFExpression> equations;
    equations.append(equation);

    this->remove(equations);
}

/** Remove the equation represented by the function 'function'.

    \throw SireError::dependency_error
*/
void SystemData::remove(const Function &function)
{
    if ( ff_equations.contains(function.ID()) )
        this->remove( ff_equations.value(function.ID()).expression() );
}

/** Remove the equations represented by the functions 'functions'

    \throw SireError::dependency_error
*/
void SystemData::remove(const QSet<Function> &functions)
{
    //convert the list of functions into a list of expressions
    //that are in this System
    QSet<FFExpression> expressions;

    for (QSet<Function>::const_iterator it = functions.begin();
         it != functions.end();
         ++it)
    {
        QHash<SymbolID,ExpressionInfo>::const_iterator expr =
                        ff_equations.constFind(it->ID());

        if (expr != ff_equations.constEnd())
            expressions.insert(expr->expression());
    }

    this->remove(expressions);
}

/** Replace the equation 'oldexpr' with 'newexpr'. This will add 'newexpr' if
    'oldexpr' doesn't already exist in this system.

    \throw SireError::dependency_error
*/
void SystemData::replace(const FFExpression &oldexpr, const FFExpression &newexpr)
{
    if ( not ff_equations.contains(oldexpr.function().ID()) )
    {
        this->add(newexpr);
        return;
    }

    //replace the existing function and rebuild the entire
    //dependency tree...
    QHash<SymbolID,ExpressionInfo> orig_ff_equations = ff_equations;

    try
    {
        //remove the existing expression
        ff_equations.remove(oldexpr.function().ID());

        //get the list of all expressions
        QSet<FFExpression> all_expressions = this->equations();

        //insert the new expression
        all_expressions.insert(newexpr);

        //now add all of the expressions - this will
        //rebuild the complete dependency tree and will
        //throw an exception if something went wrong
        this->add(all_expressions);
    }
    catch(...)
    {
        //Something went wrong...
        //restore the invariant
        ff_equations = orig_ff_equations;
        throw;
    }
}

/** Replace the expression represented by the function 'oldfunc' with
    the expression 'newexpr'. This adds the expression if 'oldfunc'
    doesn't exist.

    \throw SireError::dependency_error
*/
void SystemData::replace(const Function &oldfunc, const FFExpression &newexpr)
{
    if ( ff_equations.contains(oldfunc.ID()) )
        this->replace( ff_equations.value(oldfunc.ID()).expression(), newexpr );
    else
        this->add(newexpr);
}

/** This adds the new expression 'newexpr' as a replacement for the
    existing expression with the same function */
void SystemData::replace(const FFExpression &newexpr)
{
    this->replace( newexpr.function(), newexpr );
}

//void recordAverage(const FFExpression &ff_equation,
//                   const Averager &averager);

/** Add the MoleculeGroup 'group' to this System. */
void SystemData::add(const MoleculeGroup &group)
{
    molgroups.add(group);
    this->incrementMajorVersion();
}

/** Remove the MoleculeGroup 'group' from this System */
void SystemData::remove(const MoleculeGroup &group)
{
    if (molgroups.remove(group))
        this->incrementMajorVersion();
}

/** Remove the molecule 'molecule' from the system */
bool SystemData::remove(const Molecule &molecule)
{
    if (molgroups.remove(molecule))
    {
        this->incrementMinor();
        return true;
    }
    else
        return false;
}

/** Change the molecule 'molecule' */
bool SystemData::change(const Molecule &molecule)
{
    if ( molgroups.change(molecule) )
    {
        this->incrementMinorVersion();
        return true;
    }
    else
        return false;
}

/** Change the residue 'residue' */
bool SystemData::change(const Residue &residue)
{
    return this->change( residue.molecule() );
}

/** Change the atom 'atom' */
bool SystemData::change(const NewAtom &atom)
{
    return this->change( atom.molecule() );
}

/** Update all of the statistics of the System. This calculates
    the values of all of the properties that should be averaged
    and adds them to their respective averages. */
void SystemData::updateStatistics()
{

    this->incrementMinorVersion();
}

/** Set the parameter 'param' to the value 'value'. */
void SystemData::setParameter(const Symbol &param, double value)
{
    double oldvalue = ff_params.value(param);

    try
    {
        //set the parameter
        ff_params.set(param, value);

        //update anything that depends on this parameter
        // .... how?

        this->updateMinorVersion();
    }
    catch(...)
    {
        //maintain the invariant
        ff_params.set(param, oldvalue);
        throw;
    }
}

/** Evaluate and return the energy of the forcefield component 'component' */
double SystemData::energy(const FFComponent &component)
{
    return this->getEnergyComponent(component);
}

/** Internal function used to return the energy of an expression */
double SystemData::energy(const ExpressionInfo &expr)
{
    //get all of the dependencies of this equation
    const QVector<FFExpression> &deps = expr.dependencies();

    //get all of the IDs of forcefields that contain components
    //of this forcefield
    const QSet<ForceFieldID> &ffids = expr.forcefieldIDs();

    //get all of the energy components for all of the forcefields
    Values nrg_components = this->getEnergyComponents(ffids);

    //add on all of the forcefield parameters
    nrg_components += this->parameters();

    if (deps.isEmpty())
    {
        //there are no dependencies - just calculate this expression
        double nrg = expr.expression().evaluate(nrg_components);

        cached_energies.insert( expr.expression().function().ID(), nrg );

        return nrg;
    }
    else
    {
        //we need to evaluate the energy of all of the functions
        //that this depends on (the dependencies are ordered so that
        //if evaluate from first to last, then all dependencies are
        //fulfilled).
        const FFExpression *deps_array = deps.constData();
        int ndeps = deps.count();

        for (int i=0; i<ndeps; ++i)
        {
            const FFExpression &dep = deps_array[i];
            const Function &depfunc = dep.function();

            if ( cached_energies.contains(depfunc.ID()) )
            {
                nrg_components.set(depfunc, cached_energies.value(depfunc.ID()));
            }
            else
            {
                double nrg = dep.evaluate(nrg_components);
                cached_energies.insert(depfunc.ID(), nrg);
                nrg_components.set(depfunc, nrg);
            }
        }

        //we can now evaluate the energy of the function
        double nrg = expr.expression().evaluate(nrg_components);

        //cache this value
        cached_energies.insert( expr.expression().function().ID(), nrg );

        //return the energy :-)
        return nrg;
    }
}

/** Evaluate and return the energy of the energy component 'component' */
double SystemData::energy(const Function &component)
{
    //look in the cache of calculated energies - this is
    //completely cleared whenever *anything* in the system is changed
    if (cached_energies.contains(component.ID()))
        return cached_energies.value(component.ID());

    QHash<SymbolID,ExpressionInfo>::const_iterator it =
                                            ff_equations.constFind(component.ID());

    if ( it != ff_equations.constEnd() )
    {
        //this is one of the forcefield expressions in this system
        BOOST_ASSERT( it->expression().function().ID() == component.ID() );

        return this->energy(*it);
    }
    else
    {
        //this is just a function - it could be a forcefield function,
        //but that's it
        FFComponent ffcomp;

        try
        {
            ffcomp = component;
        }
        catch(const SireError::incompatible_error&)
        {
            //this is a function that doesn't exist in this forcefield!
            throw SireFF::missing_component( QObject::tr(
                  "There is no forcefield component in this system that "
                  "represents the function \"%1\"")
                      .arg(component.toString()), CODELOC );
        }

        //get the energy of this component
        return this->energy(ffcomp);
    }
}

/** Evaluate and return the energy expression 'expression' */
double SystemData::energy(const FFExpression &expression)
{
    if ( ff_equations.contains(expression.function().ID()) )
    {
        //this is one of the expressions that are already loaded
        //into this system
        return this->energy(expression.function());
    }
    else
    {
        //this expression is not in this system - we need to create
        //an ExpressionInfo for the expression...
        ExpressionInfo expr(expression, ff_equations);

        return this->energy(expr);
    }
}
