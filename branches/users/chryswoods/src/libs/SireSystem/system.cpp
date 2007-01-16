
#include "system.h"
#include "simsystem.h"

using namespace SireSystem;

/** Construct a new simulation System. */
System::System(QString name) : nme(name)
{}

/** Destructor. This will delete everything in this system unless it is
    referenced elsewhere. */
System::~System()
{}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This runs the move 'nmoves' times.
*/
void System::run(const Move &move, quint32 nmoves)
{
    LocalSimSystem simsystem(*this, move);

    *this = simsystem.run(nmoves);
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This performs one cycles of the supplied moves
    (e.g. moves.count() moves)
*/
void System::run(const Moves &moves)
{
    LocalSimSystem simsystem(*this, moves);

    *this = simsystem.run(moves.count());
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This performs 'nmoves' moves.
*/
void System::run(const Moves &moves, quint32 nmoves)
{
    LocalSimSystem simsystem(*this, moves);

    *this = simsystem.run(nmoves);
}

/** Evaluate and return the energy of the forcefield component 'component' */
double SystemData::evaluate(const FFComponent &component)
{
    return this->energy(component);
}

/** Evaluate and return the energy of the energy component 'component' */
double SystemData::evaluate(const Function &component)
{
    //look in the cache of calculated energies - this is
    //completely cleared whenever *anything* in the system is changed
    if (cached_energies.contains(component.ID()))
        return cached_energies.value(component.ID());

    QHash<SymbolID,ExpressionInfo>::const_iterator expr =
                                            ff_equations.constFind(component.ID());

    if ( it != exprsns.constEnd() )
    {
        //this is one of the forcefield expressions in this system
        BOOST_ASSERT( expr->expression().function().ID() == component.ID() );

        //get all of the dependencies of this equation
        const QVector<FFExpression> &deps = expr->dependencies();

        //get all of the IDs of forcefields that contain components
        //of this forcefield
        const QSet<ForceFieldID> &ffids = expr->forcefieldIDs();

        //get all of the energy components for all of the forcefields
        Values nrg_components = this->getEnergyComponents(ffids);

        //add on all of the forcefield parameters
        nrg_components += this->parameters();

        if (deps.isEmpty())
        {
            //there are no dependencies - just calculate this expression
            double nrg = expr->expression().evaluate(nrg_components);

            cached_energies.insert( expr->expression().function(), nrg );

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
            double nrg = expr->expression().evaluate(nrg_components);

            //cache this value
            cached_energies.insert( expr->expression().function().ID(), nrg );

            //return the energy :-)
            return nrg;
        }
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
