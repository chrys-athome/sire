
#include "SireCAS/qhash_sirecas.h"

#include <QMap>

#include "forcefields.h"

#include "SireFF/errors.h"
#include "SireError/errors.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireFF;
using namespace SireCAS;
using namespace SireMol;

using namespace SireStream;

////////
//////// Implementation of ForceFieldsBase::ExpressionInfo
////////

/** Null constructor */
ForceFieldsBase::ExpressionInfo::ExpressionInfo()
{}

inline QString getString(const QSet<Function> &functions)
{
    if (functions.isEmpty())
        return "[ ]";
    else
    {
        QStringList strings;

        for (QSet<Function>::const_iterator it = functions.begin();
             it != functions.end();
             ++it)
        {
            strings.append(it->toString());
        }

        return QString("[ %1 ]").arg(strings.join(", "));
    }
}

/** Validates that 'ff_equations' contains the dependencies of 'expression'

    \throw SireError::dependency_error
*/
void ForceFieldsBase::
ExpressionInfo::validateDependencies(const FFExpression &expression,
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
                    .arg(expression.expression().toString(),
                         getString(missing_funcs)), CODELOC );
    }
}

/** Construct the info for the expression 'expression' using the info of
    the other expressions in 'ff_equations' to resolve any
    dependencies. This will throw an exception if there are
    any unresolved functions in the expression.

    \throw SireError::dependency_error
*/
ForceFieldsBase::
ExpressionInfo::ExpressionInfo(const FFExpression &expression,
                               const QHash<SymbolID,ExpressionInfo> &ff_equations)
           : ex(expression)
{
    //get all of the direct dependencies on forcefields from the expression
    ffids = expression.forceFieldIDs();

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
            const FFComponent &dep_func = dep_expr.function();

            if ( done_funcs.contains(dep_func.ID()) )
                continue;

            //find the dependencies of this function, and them to the
            //list
            const QVector<FFExpression> &dep_expr_deps = dep_exprinfo.dependencies();

            int ndeps = dep_expr_deps.count();
            const FFExpression *dep_expr_deps_array = dep_expr_deps.constData();

            for (int i=0; i<ndeps; ++i)
            {
                const FFComponent &dep_dep_func = dep_expr_deps_array[i].function();

                if ( not done_funcs.contains(dep_dep_func.ID()) )
                {
                    ordered_funcs.append(dep_dep_func.ID());
                    done_funcs.insert(dep_dep_func.ID());
                }
            }

            //now add the dependent function
            ordered_funcs.append(dep_func.ID());
            done_funcs.insert(dep_func.ID());

            //add all of the forcefield dependencies from this function
            for (QSet<ForceFieldID>::const_iterator it = dep_exprinfo.forceFieldIDs().begin();
                 it != dep_exprinfo.forceFieldIDs().end();
                 ++it)
            {
                ffids.insert(*it);
            }
        }

        //finally(!) convert the list of dependencies into an array of dependent
        //expressions
        deps.reserve(ordered_funcs.count());

        for (QList<SymbolID>::const_iterator it = ordered_funcs.constBegin();
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
        ffids = expression.forceFieldIDs();
    }
}

/** Copy constructor */
ForceFieldsBase::
ExpressionInfo::ExpressionInfo(const ExpressionInfo &other)
               : ex(other.ex), deps(other.deps), ffids(other.ffids)
{}

/** Destructor */
ForceFieldsBase::ExpressionInfo::~ExpressionInfo()
{}

/** Copy assignment */
ForceFieldsBase::ExpressionInfo&
ForceFieldsBase::ExpressionInfo::operator=(const ForceFieldsBase::ExpressionInfo &other)
{
    ex = other.ex;
    deps = other.deps;
    ffids = other.ffids;

    return *this;
}

////////
//////// Implementation of ForceFieldsBase
////////

static const RegisterMetaType<ForceFieldsBase> r_ffbase(MAGIC_ONLY,
                                                        "SireFF::ForceFieldsBase");

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ForceFieldsBase &ffbase)
{
    writeHeader(ds, r_ffbase, 1);

    SharedDataStream sds(ds);

    //stream info about all of the forcefield equations....
    sds << quint32( ffbase.ff_equations.count() );

    for (QHash<SymbolID,ForceFieldsBase::ExpressionInfo>::const_iterator it
                                = ffbase.ff_equations.begin();
         it != ffbase.ff_equations.end();
         ++it)
    {
        sds << it->ex << it->deps << it->ffids;
    }

    //now stream the remaining contents of the object
    sds << ffbase.index << ffbase.ff_dependents
        << ffbase.ff_params << ffbase.cached_energies
        << ffbase.total_id;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ForceFieldsBase &ffbase)
{
    VersionID v = readHeader(ds, r_ffbase);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        //destream the forcefield equations
        quint32 neqns;

        sds >> neqns;

        QHash<SymbolID,ForceFieldsBase::ExpressionInfo> ff_equations;

        if (neqns > 0)
        {
            ff_equations.reserve(neqns);

            for (uint i=0; i<neqns; ++i)
            {
                ForceFieldsBase::ExpressionInfo exinfo;

                sds >> exinfo.ex >> exinfo.deps >> exinfo.ffids;

                ff_equations.insert(exinfo.ex.function().ID(), exinfo);
            }
        }

        ffbase.ff_equations = ff_equations;

        //now stream in the remaining data
        sds >> ffbase.index >> ffbase.ff_dependents
            >> ffbase.ff_params >> ffbase.cached_energies
            >> ffbase.total_id;
    }
    else
        throw version_error(v, "1", r_ffbase, CODELOC);

    return ds;
}

/** Constructor */
ForceFieldsBase::ForceFieldsBase() : total_id(0)
{}

/** Copy constructor */
ForceFieldsBase::ForceFieldsBase(const ForceFieldsBase &other)
                : index(other.index),
                  ff_equations(other.ff_equations),
                  ff_dependents(other.ff_dependents),
                  ff_params(other.ff_params),
                  cached_energies(other.cached_energies),
                  total_id(other.total_id)
{}

/** Destructor */
ForceFieldsBase::~ForceFieldsBase()
{}

/** Copy assignment */
ForceFieldsBase& ForceFieldsBase::operator=(const ForceFieldsBase &other)
{
    if (this != &other)
    {
        index = other.index;
        ff_equations = other.ff_equations;
        ff_dependents = other.ff_dependents;
        ff_params = other.ff_params;
        cached_energies = other.cached_energies;
        total_id = other.total_id;
    }

    return *this;
}

/** Return all of the parameters used in the forcefield functions, and
    their current values. */
const Values& ForceFieldsBase::parameters() const
{
    return ff_params;
}

/** Return whether or not this contains the function 'function' */
bool ForceFieldsBase::contains(const Function &function) const
{
    return ff_equations.contains(function.ID());
}

/** Return all of the expressions in this set */
QVector<FFExpression> ForceFieldsBase::expressions() const
{
    if (ff_equations.isEmpty())
        return QVector<FFExpression>();

    QVector<FFExpression> allexpressions;

    allexpressions.reserve(ff_equations.count());

    for (QHash<SymbolID,ExpressionInfo>::const_iterator it = ff_equations.begin();
         it != ff_equations.end();
         ++it)
    {
        allexpressions.append( it->expression() );
    }

    return allexpressions;
}

/** Return the expression matching the function 'func'

    \throw SireFF::missing_function
*/
FFExpression ForceFieldsBase::expression(const Function &func) const
{
    QHash<SymbolID,ExpressionInfo>::const_iterator
                                  it = ff_equations.find(func.ID());

    if (it == ff_equations.end())
    {
        QStringList funcs;

        foreach (ExpressionInfo info, ff_equations)
        {
            funcs.append(info.function().toString());
        }

        throw SireFF::missing_function( QObject::tr(
                "There is no expression matching the function \"%1\" in "
                "this set. (functions include [ %2 ])")
                    .arg(func.toString())
                    .arg(funcs.join(", ")), CODELOC );
    }

    return it->expression();
}

/** Return all of the expressions that match the functions in 'funcs'

    \throw SireFF::missing_function
*/
QVector<FFExpression> ForceFieldsBase::expressions(const QSet<Function> &funcs) const
{
    if (funcs.isEmpty())
        return QVector<FFExpression>();

    QVector<FFExpression> exps;
    exps.reserve(funcs.count());

    for (QSet<Function>::const_iterator it = funcs.begin();
         it != funcs.end();
         ++it)
    {
        exps.append( this->expression(*it) );
    }

    return exps;
}

/** Set the parameter with symbol 'param' to the value 'value' */
void ForceFieldsBase::setParameter(const Symbol &param, double value)
{
    ff_params.set(param,value);

    //assume that this changes all of the energies, so clear the cache
    cached_energies.clear();
}

/** Add the molecule 'molecule' to the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
    \throw SireMol::duplicate_molecule
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFieldsBase::add(const Molecule &molecule, ForceFieldID ffid,
                          const ParameterMap &map)
{
    QSet<ForceFieldID> ffids;
    ffids.insert(ffid);

    return this->add(molecule, ffids, map);
}

/** Add the residue 'residue' to the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
    \throw SireMol::duplicate_residue
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFieldsBase::add(const Residue &residue, ForceFieldID ffid,
                          const ParameterMap &map)
{
    QSet<ForceFieldID> ffids;
    ffids.insert(ffid);

    return this->add(residue, ffids, map);
}

/** Add the atom 'atom' to the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
    \throw SireMol::duplicate_atom
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFieldsBase::add(const NewAtom &atom, ForceFieldID ffid,
                          const ParameterMap &map)
{
    QSet<ForceFieldID> ffids;
    ffids.insert(ffid);

    return this->add(atom, ffids, map);
}

/** Add the molecule 'molecule' to all forcefields that have the
    name 'ffname'. This will throw an exception if there are no forcefields
    with this name.

    \throw SireFF::missing_forcefield
    \throw SireMol::duplicate_molecule
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFieldsBase::add(const Molecule &molecule, const QString &ffname,
                          const ParameterMap &map)
{
    return this->add( molecule, this->getFFIDs(ffname), map );
}

/** Add the residue 'residue' to all forcefields that have the
    name 'ffname'. This will throw an exception if there are no forcefields
    with this name.

    \throw SireFF::missing_forcefield
    \throw SireMol::duplicate_residue
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFieldsBase::add(const Residue &residue, const QString &ffname,
                          const ParameterMap &map)
{
    return this->add( residue, this->getFFIDs(ffname), map );
}

/** Add the atom 'atom' to all forcefields that have the
    name 'ffname'. This will throw an exception if there are no forcefields
    with this name.

    \throw SireFF::missing_forcefield
    \throw SireMol::duplicate_atom
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFieldsBase::add(const NewAtom &atom, const QString &ffname,
                          const ParameterMap &map)
{
    return this->add( atom, this->getFFIDs(ffname), map );
}

/** Remove the molecule 'molecule' from all forcefields called
    'ffname'. This will throw an exception if there are no forcefields
    with this name.

    \throw SireFF::missing_forcefield
    \throw SireMol::missing_molecule
*/
bool ForceFieldsBase::remove(const Molecule &molecule, const QString &ffname)
{
    return this->remove( molecule, this->getFFIDs(ffname) );
}

/** Remove the residue 'residue' from all forcefields called
    'ffname'. This will throw an exception if there are no forcefields
    with this name.

    \throw SireFF::missing_forcefield
    \throw SireMol::missing_residue
*/
bool ForceFieldsBase::remove(const Residue &residue, const QString &ffname)
{
    return this->remove( residue, this->getFFIDs(ffname) );
}

/** Remove the atom 'atom' from all forcefields called
    'ffname'. This will throw an exception if there are no forcefields
    with this name.

    \throw SireFF::missing_forcefield
    \throw SireMol::missing_atom
*/
bool ForceFieldsBase::remove(const NewAtom &atom, const QString &ffname)
{
    return this->remove( atom, this->getFFIDs(ffname) );
}

/** Return the energy of the expression described by ExpressionInfo 'expr' */
double ForceFieldsBase::energy(const ExpressionInfo &expr)
{
    //get all of the dependencies of this equation
    const QVector<FFExpression> &deps = expr.dependencies();

    //get all of the IDs of forcefields that contain components
    //of this forcefield
    const QSet<ForceFieldID> &ffids = expr.forceFieldIDs();

    //get all of the energy components for all of the forcefields
    Values nrg_components = this->getEnergies(ffids);

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

/** Return the energy of the forcefield component 'component'

    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
*/
double ForceFieldsBase::energy(const FFComponent &component)
{
    return this->getEnergy(component);
}

/** Return the energy of the component or expression represented
    by the function 'component' */
double ForceFieldsBase::energy(const Function &component)
{
    //look in the cache of calculated energies
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
                  "There is no forcefield component in this set that "
                  "represents the function \"%1\"")
                      .arg(component.toString()), CODELOC );
        }

        //get the energy of this component
        return this->energy(ffcomp);
    }
}

/** Return the energy of the expression 'expression'. This expression
    does not necessarily have to have been added to this set, though
    if must be able to find all of the expressions it depends on
    in this set. */
double ForceFieldsBase::energy(const FFExpression &expression)
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

/** Return the total energy of the system, as calculated from the
    'total()' function. By default, the total is the straight
    sum of the energies of all components */
double ForceFieldsBase::energy()
{
    if (cached_energies.contains(total_id))
        return cached_energies.value(total_id);

    if (total_id != 0)
    {
        //total_id is always valid!
        BOOST_ASSERT( ff_equations.contains(total_id) );

        return this->energy( *(ff_equations.constFind(total_id)) );
    }
    else
    {
        //calculate the sum of all forcefields
        double nrg = this->getTotalEnergy();

        cached_energies.insert(SymbolID(0), nrg);

        return nrg;
    }
}

/** Add the expression 'ff_equation' to the list of expressions in
    this forcefield

    \throw SireFF::duplicate_function
    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
    \throw SireError::dependency_error
*/
void ForceFieldsBase::add(const FFExpression &ff_equation)
{
    if ( ff_equations.contains(ff_equation.function().ID()) )
        throw SireFF::duplicate_function( QObject::tr(
            "There is already a function \"%1\" contained in this set. "
            "(cannot replace \"%1\" with \"%2\")")
                .arg(ff_equation.function().toString())
                .arg(ff_equation.expression().toString(),
                     ff_equations.value(ff_equation.function().ID())
                                                   .expression().toString()),
                    CODELOC );

    //make sure that there are no missing forcefields or components
    this->assertValidComponents(ff_equation);

    //now create an ExpressionInfo for this equation - this resolves
    //its dependencies
    ExpressionInfo exprinfo(ff_equation, ff_equations);

    BOOST_ASSERT( exprinfo.expression().function().ID() == ff_equation.function().ID() );

    //add this to the list of expressions
    ff_equations.insert(ff_equation.function().ID(), exprinfo);

    //now record the dependencies of this function on different
    //forcefields - this is used to help maintain the cache of
    //energies
    foreach (ForceFieldID ffid, exprinfo.forceFieldIDs())
    {
        ff_dependents[ffid].insert( exprinfo.function().ID() );
    }
}

/** Add a whole load of equations to this set.

    \throw SireFF::duplicate_function
    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
    \throw SireError::dependency_error
*/
void ForceFieldsBase::add(const QVector<FFExpression> &ff_equations)
{
    //lots to do here - we need to resolve the correct order in
    //which to do things, and then to ensure they are all
    //added in a way that maintains the invariant!
    #warning TODO!
}

/** Remove the equation 'component' - you can only remove an expression
    if it is not depended on by any other expression!

    \throw SireError::dependency_error
*/
void ForceFieldsBase::remove(const Function &component)
{
    if ( not ff_equations.contains(component.ID()) )
        //nothing to do
        return;

    QSet<SymbolID> dep_by;

    for (QHash<SymbolID,ExpressionInfo>::const_iterator it = ff_equations.constBegin();
         it != ff_equations.constEnd();
         ++it)
    {
        if (it.key() == component.ID())
            //skip this equation
            continue;

        for (QVector<FFExpression>::const_iterator it2 = it->dependencies().begin();
             it2 != it->dependencies().end();
             ++it2)
        {
            if (it2->ID() == component.ID())
            {
                //this depends on 'ff_equation'
                dep_by.insert(it.key());
                break;
            }
        }
    }

    if (not dep_by.isEmpty())
    {
        //this expression is depended on by others - throw an exception
        QStringList deps;

        foreach (SymbolID dep, dep_by)
        {
            const ExpressionInfo &ex = *(ff_equations.constFind(dep));

            deps.append( QString("%1 = %2").arg(ex.function().toString(),
                                                ex.expression().toString()) );
        }

        throw SireError::dependency_error( QObject::tr(
                "Cannot remove the expression \"%1\" as it is "
                "depended on by;\n%2")
                    .arg(component.toString())
                    .arg(deps.join("\n")), CODELOC );
    }
    else
    {
        //it is not depended on by anything else - it is safe to remove
        QSet<ForceFieldID> ffids = ff_equations[component.ID()].forceFieldIDs();

        foreach (ForceFieldID ffid, ffids)
        {
            ff_dependents[ffid].remove(component.ID());

            if (ff_dependents[ffid].isEmpty())
                ff_dependents.remove(ffid);
        }

        ff_equations.remove(component.ID());
        cached_energies.remove(component.ID());

        if (total_id == component.ID())
            //we have just removed the total energy expression!
            total_id = 0;
    }
}

/** Remove a whole load of expressions!

    \throw SireError::dependency_error
*/
void ForceFieldsBase::remove(const QSet<Function> &functions)
{
    #warning TODO
}

/** Remove all expressions from this set */
void ForceFieldsBase::removeAll()
{
    ff_equations.clear();
    cached_energies.clear();
    ff_dependents.clear();

    total_id = 0;
}

/** Take out an expression - this removes the expression and
    returns its value.

    \throw SireCAS::missing_function
    \throw SireError::dependency_error
*/
FFExpression ForceFieldsBase::take(const Function &function)
{
    FFExpression ret = this->expression(function);

    this->remove(function);

    return ret;
}

/** Take out a load of expressions - this removes the expressions
    and returns their values.

    \throw SireCAS::missing_function
    \throw SireError::dependency_error
*/
QVector<FFExpression> ForceFieldsBase::take(const QSet<Function> &functions)
{
    QVector<FFExpression> ret = this->expressions(functions);

    this->remove(functions);

    return ret;
}

/** Take out all of the expressions in this set and return them - this
    remove all of the expressions and returns their values.
*/
QVector<FFExpression> ForceFieldsBase::takeAll()
{
    QVector<FFExpression> ret = this->expressions();

    this->removeAll();

    return ret;
}

/** Internal function used to record that the molecule with ID == molid
    is in the forcefield with ID == ffid (a record is made even if
    only part of the molecule is in the forcefield) */
void ForceFieldsBase::add(MoleculeID molid, ForceFieldID ffid) throw()
{
    index[molid].insert(ffid);
}

/** Internal function used to record that the molecule with ID == molid
    is in the forcefields whose IDs are in ffids (a record is made
    even if only part of the molecule is in the forcefield) */
void ForceFieldsBase::add(MoleculeID molid, const QSet<ForceFieldID> &ffids) throw()
{
    index[molid].unite(ffids);
}

/** Internal function used to record that the molecule with ID == molid
    has been removed from the forcefield with ID == ffid - only call
    this if the molecule has been completely removed from that
    forcefield */
void ForceFieldsBase::remove(MoleculeID molid, ForceFieldID ffid) throw()
{
    index[molid].remove(ffid);

    if (index[molid].isEmpty())
        index.remove(molid);
}

/** Internal function used to record that the forcefield with ID == ffid
    has been removed. */
void ForceFieldsBase::remove(ForceFieldID ffid) throw()
{
    //remove the forcefield from all of the molecules...
    QMutableHashIterator< MoleculeID, QSet<ForceFieldID> > it(index);

    while (it.hasNext())
    {
        it.next();

        if (it.value().contains(ffid))
        {
            it.value().remove(ffid);
            if (it.value().isEmpty())
                it.remove();
        }
    }

    //remove any expressions that depend on this forcefield
    for (QHash< ForceFieldID, QSet<SymbolID> >::const_iterator
                                            it = ff_dependents.constBegin();
         it != ff_dependents.constEnd();
         ++it)
    {
        foreach (SymbolID funcid, *it)
        {
            ff_equations.remove(funcid);
            cached_energies.remove(funcid);

            if (total_id == funcid)
                total_id = 0;
        }
    }

    ff_dependents.remove(ffid);
}

/** Record that the forcefield with ID == ffid has been changed */
void ForceFieldsBase::changed(ForceFieldID ffid) throw()
{
    for (QHash< ForceFieldID, QSet<SymbolID> >::const_iterator
                                            it = ff_dependents.constBegin();
         it != ff_dependents.constEnd();
         ++it)
    {
        foreach (SymbolID funcid, *it)
        {
            cached_energies.remove(funcid);
        }
    }
}

////////
//////// Implementation of ForceFields
////////

/** Empty constructor */
ForceFields::ForceFields() : ForceFieldsBase()
{}

/** Construct from another set of ForceFields */
ForceFields::ForceFields(const ForceFieldsBase &base)
            : ForceFieldsBase(base),
              ffields(base.forceFields())
{}

/** Copy constructor */
ForceFields::ForceFields(const ForceFields &other)
            : ForceFieldsBase(other), ffields(other.ffields)
{}

/** Destructor */
ForceFields::~ForceFields()
{}

/** Copy assignment */
ForceFields& ForceFields::operator=(const ForceFieldsBase &other)
{
    ForceFieldsBase::operator=(other);
    ffields = other.forceFields();

    return *this;
}

/** Copy assignment */
ForceFields& ForceFields::operator=(const ForceFields &other)
{
    ForceFieldsBase::operator=(other);
    ffields = other.ffields;

    return *this;
}

/** Return all of the forcefields in this set, indexed by
    ForceFieldID */
QHash<ForceFieldID,ForceField> ForceFields::forceFields() const
{
    return ffields;
}

/** Add the forcefield 'ffield' to this set. This will synchronise
    all of the molecules in this set so that they are at the latest
    version.

    \throw SireFF::duplicate_forcefield
*/
void ForceFields::add(const ForceField &ffield)
{
    if ( ffields.contains(ffield.ID()) )
        throw SireFF::duplicate_forcefield( QObject::tr(
            "Cannot add the forcefield \"%1\" (%2 %3) to this set "
            "as a copy of this forcefield already exists! (version == %4)")
                .arg(ffield.name()).arg(ffield.ID())
                .arg(ffield.version().toString(),
                     ffields.value(ffield.ID()).version().toString()),
                        CODELOC );

    //take a copy of this set so that we can maintain the invariant
    ForceFields orig = *this;

    try
    {
        ffields.insert(ffield.ID(), ffield);

        //record the location of, and synchronise all of the molecules
        QSet<MoleculeID> molids = ffield.moleculeIDs();

        //record the location of all of the molecules
        ForceFieldsBase::add(molids, ffield.ID());

        foreach (MoleculeID molid, molids)
        {
            this->synchronise(molid);
        }
    }
    catch(...)
    {
        //something went wrong - restore the original
        *this = orig;

        //rethrow the exception
        throw;
    }
}

/** Remove the forcefield with ID == ffid. This will also remove
    any molecules contained in that forcefield, and any expressions
    that depend on that forcefield. */
void ForceFields::remove(ForceFieldID ffid)
{
    ForceFieldsBase::remove(ffid);
    ffields.remove(ffid);
}

/** Remove all forcefields that are called 'ffname' */
void ForceFields::remove(const QString &ffname)
{
    QSet<ForceFieldID> ffids;

    for (QHash<ForceFieldID,ForceField>::const_iterator it = ffields.constBegin();
         it != ffields.constEnd();
         ++it)
    {
        if (it->name() == ffname)
            ffids.insert(it.key());
    }

    foreach( ForceFieldID ffid, ffids )
    {
        this->remove(ffid);
    }
}

/** Add the object 'obj'

    \throw SireMol::duplicate_molecule
    \throw SireFF::missing_forcefield
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
inline bool ForceFields::_pvt_add(const T &obj, const QSet<ForceFieldID> &ffids,
                                  const ParameterMap &map)
{
    //maintain the invariant - take a copy of this object...
    ForceFields orig = *this;

    try
    {
        bool added = false;

        //add the molecule to each forcefield
        for (QSet<ForceFieldID>::const_iterator ffit = ffids.begin();
             ffit != ffids.end();
             ++ffit)
        {
            ForceFieldID ffid = *ffit;

            QHash<ForceFieldID,ForceField>::iterator it = ffields.find(ffid);

            if (it == ffields.constEnd())
                throw SireFF::missing_forcefield( QObject::tr(
                    "There is no forcefield with ID == %1 in this set. Cannot "
                    "therefore add the molecule %2 to it!")
                        .arg(ffid).arg(obj.idString()), CODELOC );

            bool added_to_ff = it->add(obj, map);

            added = added or added_to_ff;
        }

        if (added)
        {
            //ok, all of the molecules added successfully - record the
            //additional forcefields for this molecule
            ForceFieldsBase::add(obj.ID(), ffids);

            //now we must synchronise this molecule across all forcefields...
            this->synchronise(obj.ID());

            //record that the affected forcefields have changed
            ForceFieldsBase::changed(ffids);
        }
    }
    catch(...)
    {
        //something went wrong - restore the original state of
        //this object
        *this = orig;

        //rethrow the exception
        throw;
    }
}

/** Add the molecule 'molecule' to all of the forcefields whose ID numbers
    are in 'ffids', using the parameter map 'map' to find and resolve
    forcefield parameters.

    \throw SireFF::missing_forcefield
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFields::add(const Molecule &molecule, const QSet<ForceFieldID> &ffids,
                      const ParameterMap &map)
{
    return this->_pvt_add<Molecule>(molecule, ffids, map);
}

/** Add the residue 'residue' to all of the forcefields whose ID numbers
    are in 'ffids', using the parameter map 'map' to find and resolve
    forcefield parameters.

    \throw SireFF::missing_forcefield
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFields::add(const Residue &residue, const QSet<ForceFieldID> &ffids,
                      const ParameterMap &map)
{
    return this->_pvt_add<Residue>(residue, ffids, map);
}

/** Add the atom 'atom' to all of the forcefields whose ID numbers
    are in 'ffids', using the parameter map 'map' to find and resolve
    forcefield parameters.

    \throw SireFF::missing_forcefield
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFields::add(const NewAtom &atom, const QSet<ForceFieldID> &ffids,
                      const ParameterMap &map)
{
    return this->_pvt_add<NewAtom>(atom, ffids, map);
}

/** Change the object 'obj'

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
inline bool ForceFields::_pvt_change(const T &obj)
{
    const QSet<ForceFieldID> ffids = this->forceFieldsContaining(obj.ID());

    if (not ffids.isEmpty())
    {
        //we contain this molecule!
        if (ffids.count() == 1)
        {
            //no need to maintain state as only a single forcefield to change...
            ForceFieldID ffid = *(ffids.begin());

            bool changed = ffields[ffid].change(obj);

            //this is guaranteed not to throw
            if (changed)
                ForceFieldsBase::changed(ffid);

            return changed;
        }
        else
        {
            //we need to save ffields so that it can be restored if there
            //is a problem
            QHash<ForceFieldID,ForceField> orig_ffields = ffields;

            bool changed = false;

            try
            {
                foreach( ForceFieldID ffid, ffids )
                {
                    bool ff_changed = ffields[ffid].change(obj);

                    changed = changed or ff_changed;
                }
            }
            catch(...)
            {
                //restore the original forcefields
                ffields = orig_ffields;
                throw;
            }

            //notify the system that the molecule has changed
            //(this is guaranteed not to throw)
            if (changed)
                ForceFieldsBase::changed(ffids);

            return changed;
        }
    }
    else
        return false;
}

/** Change the molecule 'molecule'

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFields::change(const Molecule &molecule)
{
    return this->_pvt_change<Molecule>(molecule);
}

/** Change the residue 'residue'

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFields::change(const Residue &residue)
{
    return this->_pvt_change<Residue>(residue);
}

/** Change the atom 'atom'

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceFields::change(const NewAtom &atom)
{
    return this->_pvt_change<NewAtom>(atom);
}

/** Remove the object 'obj' */
template<class T>
inline bool ForceFields::_pvt_remove(const T &obj, QSet<ForceFieldID> ffids)
{
    ffids.intersect( this->forceFieldsContaining(obj.ID()) );

    if (ffids.isEmpty())
    {
        //it isn't in this set
        return false;
    }
    else if (ffids.count() == 1)
    {
        //there is only one to remove
        ForceFieldID ffid = *(ffids.begin());

        ForceField &ffield = ffields[ffid];

        bool changed = ffield.remove(obj);

        if (changed)
        {
            if ( not ffield.contains(obj.ID()) )
            {
                //this forcefield no longer contains the molecule
                ForceFieldsBase::remove(obj.ID(), ffid);
            }

            ForceFieldsBase::changed(ffid);

            return true;
        }
        else
            return false;
    }
    else
    {
        //take a copy of ffields to maintain the invariant
        QHash<ForceFieldID,ForceField> orig_ffields = ffields;

        bool changed = false;

        try
        {
            foreach (ForceFieldID ffid, ffids)
            {
                //remove the object from the forcefield
                bool this_changed = ffields[ffid].remove(obj);

                changed = changed or this_changed;
            }
        }
        catch(...)
        {
            //restore the original
            ffields = orig_ffields;
            throw;
        }

        if (changed)
        {
            //run through each forcefield again and see if they still
            //contain the molecule
            MoleculeID molid = obj.ID();

            foreach (ForceFieldID ffid, ffids)
            {
                if ( not ffields.constFind(ffid)->contains(molid) )
                    ForceFieldsBase::remove(molid, ffid);
            }

            ForceFieldsBase::changed(ffids);

            return true;
        }
        else
            return false;
    }
}

/** Remove the molecule 'molecule' from the forcefields whose IDs are in ffids */
bool ForceFields::remove(const Molecule &molecule, const QSet<ForceFieldID> &ffids)
{
    return this->_pvt_remove<Molecule>(molecule, ffids);
}

/** Remove the residue 'residue' from the forcefields whose IDs are in ffids */
bool ForceFields::remove(const Residue &residue, const QSet<ForceFieldID> &ffids)
{
    return this->_pvt_remove<Residue>(residue, ffids);
}

/** Remove the atom 'atom' from the forcefields whose IDs are in ffids */
bool ForceFields::remove(const NewAtom &atom, const QSet<ForceFieldID> &ffids)
{
    return this->_pvt_remove<NewAtom>(atom, ffids);
}

/** Completely remove the molecule 'molecule' from this forcefield */
bool ForceFields::remove(const Molecule &molecule)
{
    return this->remove(molecule, this->forceFieldsContaining(molecule));
}

inline QString toString(const QList<ForceFieldID> &ffids)
{
    QStringList strings;

    foreach (ForceFieldID ffid, ffids)
    {
        strings.append( QString::number(ffid) );
    }

    return QString("[ %1 ]").arg(strings.join(", "));
}

/** Return a reference to the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
*/
ForceField& ForceFields::getForceField(ForceFieldID ffid)
{
    QHash<ForceFieldID,ForceField>::iterator it = ffields.find(ffid);

    if (it == ffields.constEnd())
        throw SireFF::missing_forcefield( QObject::tr(
            "There is no forcefield with ID == %1 is this set! "
            "Available forcefields are %2.")
                .arg(ffid).arg(toString(ffields.keys())), CODELOC );

    return *it;
}

/** Return a const reference to the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
*/
const ForceField& ForceFields::getConstForceField(ForceFieldID ffid) const
{
    return this->getForceField(ffid);
}

/** Return a reference to the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
*/
const ForceField& ForceFields::getForceField(ForceFieldID ffid) const
{
    QHash<ForceFieldID,ForceField>::const_iterator it = ffields.find(ffid);

    if (it == ffields.constEnd())
        throw SireFF::missing_forcefield( QObject::tr(
            "There is no forcefield with ID == %1 is this set! "
            "Available forcefields are %2.")
                .arg(ffid).arg(toString(ffields.keys())), CODELOC );

    return *it;
}

/** Return all of the energy components of the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
*/
Values ForceFields::getEnergies(ForceFieldID ffid)
{
    return this->getForceField(ffid).energies();
}

/** Return all of the energy components of the forcefields whose
    IDs are in ffids

    \throw SireFF::missing_forcefield
*/
Values ForceFields::getEnergies(const QSet<ForceFieldID> &ffids)
{
    if (ffids.isEmpty())
        return Values();
    else if (ffids.count() == 1)
        return this->getEnergies( *(ffids.begin()) );
    else
    {
        Values nrgs;

        foreach (ForceFieldID ffid, ffids)
        {
            nrgs += this->getEnergies(ffid);
        }

        return nrgs;
    }
}

/** Return the energy of the component 'component'

    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
*/
double ForceFields::getEnergy(const FFComponent &component)
{
    return this->getForceField(component.forceFieldID()).energy(component);
}

/** Return the total energy of all of the forcefields in this set.

    This calculates the energy as the sum of the total energies
    of all of the forcefields.
*/
double ForceFields::getTotalEnergy()
{
    double nrg = 0;

    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.constEnd();
         ++it)
    {
        nrg += it->energy();
    }

    return nrg;
}

/** Return the values of all of the passed energy components */
Values ForceFields::getEnergies(const QSet<FFComponent> &components)
{
    if (components.isEmpty())
        return Values();

    Values nrgs;
    nrgs.reserve(components.count());

    //sort the components by forcefield...
    QHash< ForceFieldID,QSet<FFComponent> > sorted_components;

    for (QSet<FFComponent>::const_iterator it = components.begin();
         it != components.end();
         ++it)
    {
        sorted_components[it->forceFieldID()].insert(*it);
    }

    //now add the energy of each component
    for (QHash< ForceFieldID,QSet<FFComponent> >::const_iterator
                                    it = sorted_components.constBegin();
         it != sorted_components.constEnd();
         ++it)
    {
        const QSet<FFComponent> &ffcomps = *it;

        if (ffcomps.count() == 1)
        {
            const FFComponent &ffcomp = *(ffcomps.begin());
            nrgs.set( ffcomp, this->getEnergy(ffcomp) );
        }
        else
        {
            nrgs += this->getForceField(it.key()).energies(ffcomps);
        }
    }

    return nrgs;
}

/** Return the ID numbers of all of the forcefields in this set that
    are called 'ffname' */
QSet<ForceFieldID> ForceFields::getFFIDs(const QString &ffname) const
{
    QSet<ForceFieldID> ffids;

    for (QHash<ForceFieldID,ForceField>::const_iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        if (it->name() == ffname)
            ffids.insert(it.key());
    }

    return ffids;
}

/** Assert that all of the FFComponents in the expression 'expression'
    actually exist in this set of forcefields!

    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
*/
void ForceFields::assertValidComponents(const FFExpression &expression) const
{
    QList<FFComponent> ffcomps = this->getComponents(expression);

    foreach( FFComponent ffcomp, ffcomps )
    {
        this->getForceField(ffcomp.forceFieldID()).assertContains(ffcomp);
    }
}

/** Synchronise the added molecule with ID == molid across all of the
    forcefields - this ensures that all of the forcefields have the
    latest version */
void ForceFields::synchronise(MoleculeID molid)
{
    QSet<ForceFieldID> ffids = this->forceFieldsContaining(molid);

    if (ffids.isEmpty() or ffids.count() == 1)
        //there is either no copy of the molecule, or only
        //a single copy, so there is no need to synchronise it
        return;

    //record the version number of each molecule in each of the containing
    //forcefields, and the forcefields that contain the molecule at
    //that version

    QMap< MoleculeVersion, QSet<ForceFieldID> > molversions;

    foreach (ForceFieldID ffid, ffids)
    {
        const ForceField &ffield = *(ffields.constFind(ffid));

        molversions[ ffield.molecule(molid).version() ].insert(ffid);
    }

    if (molversions.isEmpty() or molversions.count() == 1)
    {
        //there is either no copy of the molecule, or there
        //is already only single version of the molecule
        return;
    }

    //there is some disagreement over the version number
    //The latest version is the last item in the map
    QMap< MoleculeVersion, QSet<ForceFieldID> >::const_iterator latest;

    latest = molversions.constEnd();
    --latest;

    //latest now points at the last item in the map
    MoleculeVersion latest_version = latest.key();
    const QSet<ForceFieldID> &latest_ffids = latest.value();

    //get a copy of the latest version of the molecule
    Molecule latest_mol = ffields.constFind(*(latest_ffids.begin()))->molecule(molid);

    //now loop over the forcefields that don't have this version and
    //update them
    molversions.remove(latest_version);

    QSet<ForceFieldID> ffids_needing_updating;

    for (QMap< MoleculeVersion, QSet<ForceFieldID> >::const_iterator
                                                  it = molversions.constBegin();
         it != molversions.constEnd();
         ++it)
    {
        ffids_needing_updating += it.value();
    }

    //change the molecule in the requested forcefields
    foreach (ForceFieldID ffid, ffids_needing_updating)
    {
        ffields[ffid].change(latest_mol);
        ForceFieldsBase::changed(ffid);
    }
}

/** Synchronise all of the molecules that are in the forcefield
    with ID == ffid - this is called when this forcefield has been
    added. */
void ForceFields::synchronise(ForceFieldID ffid)
{
    QSet<MoleculeID> molids = this->getConstForceField(ffid).moleculeIDs();

    foreach (MoleculeID molid, molids)
    {
        this->synchronise(molid);
    }
}

/** Synchronise all of the molecules in this set */
void ForceFields::synchronise()
{
    QSet<MoleculeID> molids = this->moleculeIDs();

    foreach (MoleculeID molid, molids)
    {
        this->synchronise(molid);
    }
}
