
#include "factory.h"
#include "registerexpression.h"
#include "expression.h"

#include "SireCAS/errors.h"

#include <QMutex>

#include <iostream>

using namespace SireCAS;

Factory* Factory::globalfactory(0);

Factory& Factory::factory()
{
    if (not globalfactory)
        globalfactory = new Factory();
    
    return *globalfactory;
}

/** Constructor */
Factory::Factory()
{}

/** Destructor */
Factory::~Factory()
{}

/** Return the registra for the expression whose class name is 'nme'. 
    Throws an exception if the class isn't registered. 
    
    \throw SireCAS::unregistered_expression
*/
RegisterExpressionBase& Factory::getRegistra(const QString &nme)
{
    QMutexLocker lkr(&(factory().mutex));

    RegisterExpressionBase *registra = factory().registras.value(nme,0);
    
    if (not registra)
        throw SireCAS::unregistered_expression(QObject::tr(
            "There is no function called \"%1\" registered with the factory!")
                .arg(nme), CODELOC);
                
    return *registra;
}

/** Return the registra for the expression whose class name is 'nme'. 
    Throws an exception if the class isn't registered. 
    
    \throw SireCAS::unregistered_expression
*/
RegisterExpressionBase& Factory::getRegistra(const char *nme)
{
    return getRegistra( QString::fromLatin1(nme) );
}

/** Dump the ExBase 'ex' to a binary datastream */
QDataStream& Factory::dump(QDataStream &ds, const ExBase &ex)
{
    RegisterExpressionBase &registra = getRegistra(ex.what());
    
    ds << registra.what();
    return registra.dump(ds, ex);
}

/** Load an expression from a binary datastream */
ExpressionBase Factory::load(QDataStream &ds)
{
    QString what;
    ds >> what;
    
    RegisterExpressionBase &registra = getRegistra(what);
    return registra.load(ds);
}

/** Return an Expression that results from parsing the string 'expression' */
Expression Factory::parse(const QString&)
{
    #warning Need to write Factory::parse(const QString &expression)
    return Expression();
}

/** Register an expression with the factory */
void Factory::registerExpression(const RegisterExpressionBase &registra)
{
    QMutexLocker lkr(&(factory().mutex));

    factory().registras.insert(registra.what(), registra.clone());
}
