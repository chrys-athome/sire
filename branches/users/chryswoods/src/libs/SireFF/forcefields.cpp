/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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

/** Return the number of forcefields in this group */
int ForceFieldsBase::nForceFields() const
{
    return this->forceFieldIDs().count();
}

/** Return the number of molecules in these forcefields */
int ForceFieldsBase::nMolecules() const
{
    return index.count();
}

/** Return whether or not this contains the function 'function' */
bool ForceFieldsBase::contains(const Function &function) const
{
    return ff_equations.contains(function.ID());
}

/** Return the index */
const QHash< MoleculeID,QSet<ForceFieldID> >& ForceFieldsBase::getIndex() const
{
    return index;
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
const FFExpression& ForceFieldsBase::expression(const Function &func) const
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

/** Set the expression used to calculate the total energy of the set
    of forcefields - by default this will be a simple sum of the
    total energies of all of the forcefields. If 'expression' is
    not already in this set then it will be added.

    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
    \throw SireFF::missing_function
*/
void ForceFieldsBase::setTotal(const FFExpression &expression)
{
    if (not ff_equations.contains(expression.function().ID()))
        this->add(expression);

    total_id = expression.function().ID();
}

/** Return the expression used to calculate the total energy of
    the forcefields. This will throw an exception if no
    expression has been set (and instead the energy is
    calculated as a simple sum of forcefields)

    \throw SireFF::missing_function
*/
const FFExpression& ForceFieldsBase::total() const
{
    if (not total_id == 0)
        throw SireFF::missing_function( QObject::tr(
            "No expression to calculate the total energy of the forcefields "
            "has been set, so instead the sum of their total energies is "
            "being calculated."),  CODELOC );

    return ff_equations[total_id].expression();
}

/** Return whether any of the forcefields contain any part of the molecule
    with ID == molid */
bool ForceFieldsBase::contains(MoleculeID molid) const
{
    return index.contains(molid);
}

/** Return whether or not any of the forcefields contain
    any part of any version of the molecule 'molecule' */
bool ForceFieldsBase::contains(const Molecule &molecule) const
{
    return this->contains(molecule.ID());
}

/** Return the complete set of IDs of all molecules that are
    in any way in any of the forcefields in this set. */
QSet<MoleculeID> ForceFieldsBase::moleculeIDs() const
{
    return index.keys().toSet();
}

/** Return the set of IDs of forcefields that in any way contain
    the molecule with ID == molid */
QSet<ForceFieldID>
ForceFieldsBase::forceFieldsContaining(MoleculeID molid) const
{
    return index[molid];
}

/** Return the set of IDs of forcefields that in any way contain
    any version of the molecule 'molecule' */
QSet<ForceFieldID>
ForceFieldsBase::forceFieldsContaining(const Molecule &molecule) const
{
    return this->forceFieldsContaining(molecule.ID());
}

/** Return the energy of the expression described by ExpressionInfo 'expr' */
double ForceFieldsBase::energy(const ExpressionInfo &expr, bool cache_result)
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

        if (cache_result)
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
        if (cache_result)
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

        //return the evaluated energy of this expression, without caching
        //the result
        return this->energy(expr, false);
    }
}

/** Return the energy of the expression 'expression'. This expression
    does not need to have been added to this set, though it must be
    able to resolve all of its dependencies from components in this set

    \throw SireError::dependency_error
*/
double ForceFieldsBase::energy(const Expression &expression)
{
    //create an ExpressionInfo for this expression
    ExpressionInfo expr( FFExpression("E_anon", expression), ff_equations );

    //return the evaluated energy of this expression, without
    //caching the result
    return this->energy(expr, false);
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

/** Internal function used to record that the molecules in 'molids' have
    been added to the forcefield with ID == ffid */
void ForceFieldsBase::add(const QSet<MoleculeID> &molids,
                          ForceFieldID ffid) throw()
{
    foreach (MoleculeID molid, molids)
    {
        index[molid].insert(ffid);
    }
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
    if (cached_energies.isEmpty())
        return;

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

/** Record that all of the forcefields whose IDs are in ffids
    have been changed. */
void ForceFieldsBase::changed(const QSet<ForceFieldID> &ffids) throw()
{
    foreach (ForceFieldID ffid, ffids)
    {
        this->changed(ffid);

        if (cached_energies.isEmpty())
            return;
    }
}

////////
//////// Implementation of ForceFields
////////

static const RegisterMetaType<ForceFields> r_ffields;

/** Serialise to a binary data stream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const ForceFields &ffields)
{
    writeHeader(ds, r_ffields, 1);

    SharedDataStream sds(ds);

    sds << ffields.ffields << static_cast<const ForceFieldsBase&>(ffields);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, ForceFields &ffields)
{
    VersionID v = readHeader(ds, r_ffields);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> ffields.ffields >> static_cast<ForceFieldsBase&>(ffields);
    }
    else
        throw version_error(v, "1", r_ffields, CODELOC);

    return ds;
}

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

/** Return the complete set of IDs of all of the forcefields
    in this set */
QSet<ForceFieldID> ForceFields::forceFieldIDs() const
{
    return ffields.keys().toSet();
}

/** Set this ForceFields object equal to 'forcefields' */
void ForceFields::setEqualTo(const ForceFields &forcefields)
{
    *this = forcefields;
}

/** Perform a major update - this sets this ForceFields object
    equal to 'forcefields' */
void ForceFields::majorUpdate(const ForceFields &forcefields)
{
    *this = forcefields;
}

/** Return whether or not the two hashes (hash0 and hash1) have
    different keys. */
template<class T, class V0, class V1>
bool differentKeys(const QHash<T,V0> &hash0, const QHash<T,V1> &hash1)
{
    if (hash0.count() == hash1.count())
    {
        for (typename QHash<T,V0>::const_iterator it = hash0.begin();
             it != hash0.end();
             ++it)
        {
            if ( not hash1.contains(it.key()) )
                return true;
        }

        return false;
    }
    else
        return true;
}

/** Assert that this set has the same forcefield contents
    as 'other'

    \throw SireError::incompatible_error
*/
void ForceFields::assertSameContents(const ForceFields &other) const
{
    if (this != &other)
    {
        //ensure that we both have the same number of forcefields
        if (differentKeys(ffields, other.ffields))
            throw SireError::incompatible_error( QObject::tr(
                    "This set of forcefields contains different forcefields to the other!"),
                          CODELOC );
    }
}


/** Perform a minor update - this sets this ForceFields object
    equal to 'forcefields', which itself must contain exactly
    the same forcefields as this object, though they
    may have different versions.

    \throw SireError::incompatible_error
*/
void ForceFields::minorUpdate(const ForceFields &forcefields)
{
    this->assertSameContents(forcefields);

    *this = forcefields;
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

/** Change the molecules  in 'mols' */
bool ForceFields::change(const QHash<MoleculeID,Molecule> &molecules)
{
    if (molecules.isEmpty())
        return false;
    else if (molecules.count() == 1)
        return this->change( *(molecules.begin()) );
    else
    {
        //must maintain the invariant
        ForceFields ffields_orig = *this;

        try
        {
            const QHash< MoleculeID,QSet<ForceFieldID> > &index = this->getIndex();

            //find all of the forcefields that don't contain any of these
            //molecules
            QSet<ForceFieldID> all_ffids = this->forceFieldIDs();

            QSet<ForceFieldID> ffids = all_ffids;

            for (QHash<MoleculeID,Molecule>::const_iterator it = molecules.begin();
                 it != molecules.end();
                 ++it)
            {
                //get the forcefields that contain this molecule
                QHash< MoleculeID,QSet<ForceFieldID> >::const_iterator
                                           ffs_with_mol = index.find(it.key());

                //remove all of these forcefields from ffids
                foreach (ForceFieldID ffid, *(ffs_with_mol))
                {
                    ffids.remove(ffid);
                }

                if (ffids.isEmpty())
                    //all of the forcefields contain at least one
                    //of the molecules in 'molecules'
                    break;
            }

            //ffids now contains all of the forcefields that *don't* contain
            //any of 'molecules'. Now loop over all of the forcefields that
            //do at least contain one of the molecules and change them
            ffids = all_ffids.subtract(ffids);

            bool changed = false;

            foreach (ForceFieldID ffid, ffids)
            {
                bool this_changed = ffields[ffid].change(molecules);

                changed = changed or this_changed;
            }

            if (changed)
            {
                ForceFieldsBase::changed(ffids);
                return true;
            }
            else
                return false;
        }
    }
    catch(...)
    {
        //restore the invariant
        *this = ffields_orig;
        throw;

        return false;
    }
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
         it != ffields.end();
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
    QSet<FFComponent> ffcomps = expression.components();

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
        //is already only a single version of the molecule
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
