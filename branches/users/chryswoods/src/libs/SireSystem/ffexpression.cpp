
#include "ffexpression.h"

#include "SireFF/ffcomponent.h"

#include "SireError/errors.h"

using namespace SireCAS;
using namespace SireFF;
using namespace SireSystem;

/** Construct a null FFExpression */
FFExpression::FFExpression()
{}

/** Construct an FFExpression for a single energy component or function */
FFExpression::FFExpression(const Function &function)
             : ex(function)
{
    //is this a forcefield component?
    try
    {
        FFComponent ffcomp(function);

        //it is (if it wasn't then an incompatible_error would
        //have been thrown)
        ffids.insert( ffcomp.forceFieldID() );
    }
    catch (const SireError::incompatible_error&)
    {
        //it isn't - this must just be another function
        deps.insert(func);
    }

    func = function;

    ffids.squeeze();
    deps.squeeze();
}

/** Construct an FFExpression called 'name' from the passed Expression. */
FFExpression::FFExpression(const QString &name, const Expression &expression)
             : ex(expression)
{
    if (name.isEmpty())
        throw SireError::invalid_args( QObject::tr(
                "Cannot create an unnamed FFExpression from the expression "
                "\"%1\"").arg(expression.toString()), CODELOC );

    //get all of the dependent functions
    Functions funcs = expression.functions();

    for ( Functions::const_iterator it = funcs.constBegin();
          it != funcs.constEnd();
          ++it )
    {
        try
        {
            //is this function a forcefield component? If not,
            //then the constructor will throw an incompatible_error
            FFComponent ffcomp( *it );

            ffids.insert( ffcomp.forceFieldID() );
        }
        catch (const SireError::incompatible_error&)
        {
            //this isn't a forcefield component - it must
            //be another function on which this function depends
            deps.insert( *it );
        }
    }

    //set the function that represents this expression
    func = Function( name, props );

    //minimize memory usage
    ffids.squeeze();
    deps.squeeze();
}

/** Copy constructor */
FFExpression::FFExpression(const FFExpression &other)
             : ex(other.ex), func(other.func),
               ffids(other.ffids), deps(other.deps)
{}

/** Destructor */
FFExpression::~FFExpression()
{}


