
#include "systemdata.h"

#include "SireBase/incremint.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireStream;
using namespace SireBase;

//////////
////////// Implementation of SystemData::ExpressionInfo
//////////

/** Null constructor */
SystemData::ExpressionInfo::ExpressionInfo()
{}

QString getString(const QSet<Function> &functions)
{
    if (functions.isEmpty())
        return "[ ]";
    else
    {
        QSet<Function>::const_iterator it = functions.constBegin();

        QString retval("[ %1").arg(it->toString());

        for( ++it; it != functions.constEnd(); ++it )
        {
            retval += QString(", %1").arg(it->toString());
        }

        return QString("%1 ]").arg(retval);
    }
}

/** Validates that 'ff_equations' contains the dependencies of 'expression'

    \throw SireError::dependency_error
*/
void SystemData::ExpressionInfo::validateDependencies(const FFExpression &expression,
                      const QHash<SymbolID,ExpressionInfo> &ff_equations)
{
    const QSet<Function> &ex_deps = expression.dependencies();

    if (ex_deps.isEmpty())
        return;

    QSet<Function> missing_funcs;

    for (QSet<Function>::const_iterator it = ex_deps.constBegin();
         it != ex_deps.constEnd();
         ++it)
    {
        if ( not ff_equations.contains(it->ID()) )
            missing_funcs.insert(*it);
    }

    if (not missing_funcs.isEmpty())
    {
        throw SireError::dependency_error( QObject::tr(
                "Cannot resolve all of the dependencies of the function \"%1\" "
                "from the functions already in the System. "
                "Missing == %2")
                    .arg(expression.toString(), getString(missing_funcs)), CODELOC );
    }
}

/** Construct the info for the expression 'expression' using the info of
    the other expressions in 'ff_equations' to resolve any
    dependencies. This will throw an exception if there are
    any unresolved functions in the expression.

    \throw SireError::dependency_error
*/
SystemData::ExpressionInfo::ExpressionInfo(const FFExpression &expression,
                       const QHash<SymbolID,ExpressionInfo> &ff_equations)
           : ex(expression)
{
    //get all of the direct dependencies on forcefields from the expression
    ffids = expression.forcefieldIDs();

    //validate that ff_equations satisfies all of the expressions
    //dependencies
    this->validateDependencies(expression, ff_equations);

    //get all of the dependencies of this expression
    const QSet<Function> &ex_deps = expression.dependencies();

    if (not ex_deps.isEmpty())
    {
        //resolve all of the dependencies from ff_equations...

        //set used to keep track of functions we have already processed...
        QSet<SymbolID> done_funcs;

        //set used to keep track of the order of the dependencies
        QList<SymbolID> ordered_funcs;

        for (QSet<Function>::const_iterator it = ex_deps.constBegin();
             it != ex_deps.constEnd();
             ++it)
        {
            const ExpressionInfo &dep_exprinfo = ff_equations[it->ID()];

            const FFExpression &dep_expr = dep_exprinfo.expression();
            const FFExpression &dep_func = dep_expr.function();

            if ( done_funcs.contains(depfunc.ID()) )
                continue;

            //find the dependencies of this function, and them to the
            //list
            const QVector<FFExpression> &dep_expr_deps = dep_exprinfo.dependencies();

            int ndeps = dep_expr_deps.count();
            const FFExpression *dep_expr_deps_array = dep_expr_deps.constData();

            for (int i=0; i<ndeps; ++i)
            {
                const FFExpression &dep_dep_func = dep_expr_deps_array[i].function();

                if ( not done_funcs.contains(dep_dep_func.ID()) )
                {
                    ordered_funcs.append(dep_dep_func.ID());
                    done_funcs.insert(dep_dep_func.ID());
                }
            }

            //now add the dependent function
            ordered_funcs.append(depfunc.ID());
            done_funcs.insert(depfunc.ID());

            //add all of the forcefield dependencies from this function
            for (QSet<ForceFieldID>::const_iterator it = dep_exprinfo.forcefieldIDs().begin();
                 it != dep_exprinfo.forcefieldIDs().end();
                 ++it)
            {
                ffids.insert(*it);
            }
        }

        //finally(!) convert the list of dependencies into an array of dependent
        //expressions
        deps.reserve(ordered_funcs.count());

        for (QList<FunctionID>::const_iterator it = ordered_funcs.constBegin();
             it != ordered_funcs.constEnd();
             ++it)
        {
            deps.append( ff_equations.value(*it).expression() );
        }

        deps.squeeze();
        ffids.squeeze();
    }
    else
    {
        //there are no dependencies
        ffids = expression.forcefieldIDs();
    }
}

/** Copy constructor */
SystemData::ExpressionInfo::ExpressionInfo(const ExpressionInfo &other)
           : ex(other.ex), deps(other.deps), ffids(other.ffids)
{}

/** Destructor */
SystemData::ExpressionInfo::~ExpressionInfo()
{}

/** Copy assignment */
SystemData::ExpressionInfo& SystemData::ExpressionInfo::operator=(
                                    const SystemData::ExpressionInfo &other)
{
    ex = other.ex;
    deps = other.deps;
    ffids = other.ffids;

    return *this;
}

//////////
////////// Implementation of SystemData
//////////

static const RegisterMetaType<SystemData> r_sysdata;

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

/** Construct a SystemData with a specified name */
SystemData::SystemData(const QString &name)
           : nme(name), id_and_version(&system_incremint)
{}

/** Copy constructor */
SystemData::SystemData(const SystemData &other)
           : ff_equations(other.ff_equations),
             ff_params(other.ff_params),
             energy_cache(other.energy_cache),
             molgroups(other.molgroups),
             props(other.props),
             sys_space(other.sys_space),
             nme(other.nme),
             id_and_version(other.id_and_version)
{}

/** Return all of the equations that are part of this system */
QSet<FFExpression> SystemData::equations() const
{
    if (ff_equations.isEmpty())
        return QSet<FFExpression>();

    QSet<FFExpression> eqns;
    eqns.reserve(ff_equations.count());

    for (QHash<SymbolID,ExpressionInfo>::const_iterator it = ff_equations.begin();
         it != ff_equations.end();
         ++it)
    {
        eqns.insert( it->expression() );
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
}

/** Add the set of expressions in 'equations' to the list
    of equations in this system. Any dependencies of these
    functions must either be in this forcefield, or must
    be in 'equations' else an exception will be thrown.

    \throw SireError::dependency_error
*/
void SystemData::add(const QSet<FFExpression> &equations)
{
    //lots to do here... - need to resolve the right
    //order to add things....
}

/** Remove all of the equations in 'equations'. These equations
    may only be removed if they are not depended on by
    any other equations (not including themselves)

    \throw SireError::dependency_error
*/
void SystemData::remove(const QSet<FFExpression> &equations)
{
    //create a copy of the current list of expressions
    //so that it may be restored in something goes wrong...
    QHash<SymbolID,ExpressionInfo> copy_of_ff_equations = ff_equations;

    try
    {
        //remove all of the supplied equations from the current set
        for (QSet<FFExpression>::const_iterator it = equations.constBegin();
             it != equations.constEnd();
             ++it)
        {
            ff_equations.remove(it->function().ID());
        }

        //get the list of existing functions
        QSet<FFExpression> remaining_expressions = this->equations();

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
    QSet<FFExpression> equations;
    equations.insert(equation);

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
    QHash<SymbolID,FFExpression> orig_ff_equations = ff_equations;

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

void add(const MoleculeGroup &group);
void remove(const MoleculeGroup &group);

/** Change the molecule 'molecule' */
void SystemData::change(const Molecule &molecule)
{
}

/** Change the residue 'residue' */
void SystemData::change(const Residue &residue)
{
}

/** Change the atom 'atom' */
void SystemData::change(const NewAtom &atom)
{
}

/** Update all of the statistics of the System. This calculates
    the values of all of the properties that should be averaged
    and adds them to their respective averages. */
void SystemData::updateStatistics()
{}

/** Set the parameter 'param' to the value 'value'. */
void SystemData::setParameter(const Symbol &param, double value)
{
    double oldvalue = params.value(param);

    try
    {
        //set the parameter
        params.set(param, value);

        //update anything that depends on this parameter
        // .... how?
    }
    catch(...)
    {
        //maintain the invariant
        params.set(param, oldvalue);
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

        cached_energies.insert( expr.expression().function(), nrg );

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
            const FFExpression &dep = deps[i];
            const Function &depfunc = dep.function();

            if ( cached_energies.contains(depfunc.ID()) )
            {
                nrg_components.insert(depfunc, cached_energies.value(depfunc));
            }
            else
            {
                double nrg = dep.evaluate(nrg_components);
                cached_energies.insert(depfunc.ID(), nrg);
                nrg_components.insert(depfunc, nrg);
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

    if ( it != exprsns.constEnd() )
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
