#ifndef SIRECAS_FACTORY_H
#define SIRECAS_FACTORY_H

#include <QHash>
#include <QString>
#include <QMutex>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

class QDataStream;

namespace SireCAS
{

class RegisterExpressionBase;
class ExpressionBase;
class Expression;
class ExBase;

/**
This class provides a factory that is used to help serialise and deserialise expressions from a datastream, and to allow expressions to be created from strings.

@author Christopher Woods
*/
class Factory
{
public:

    static QDataStream& dump(QDataStream &ds, const ExBase &ex);
    static ExpressionBase load(QDataStream &ds);
    
    static Expression parse(const QString &expression);
    
    static void registerExpression(const RegisterExpressionBase &registra);
    
private:

    Factory();
    ~Factory();
    
    static Factory& factory();

    static RegisterExpressionBase& getRegistra(const char *nme);
    static RegisterExpressionBase& getRegistra(const QString &nme);

    /** Hash of all of the registras, indexed by the class name of the 
        ExBase for which they are responsible */
    QHash<QString, RegisterExpressionBase*> registras;

    /** Mutex to serialise access to the registry */
    QMutex mutex;

    /** Pointer to the application-global factory */
    static Factory *globalfactory;
};

}

SIRE_END_HEADER

#endif
