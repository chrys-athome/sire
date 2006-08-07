#ifndef SIRECAS_REGISTEREXPRESSION_H
#define SIRECAS_REGISTEREXPRESSION_H

#include "expressionbase.h"
#include "exbase.h"
#include "factory.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Base class of the registra used to register ExBase objects

    @author Christopher Woods
*/
class RegisterExpressionBase
{
public:
    RegisterExpressionBase(const char *name);
    RegisterExpressionBase(const RegisterExpressionBase &other);
    
    virtual ~RegisterExpressionBase();
    
    QString what() const
    {
        return nme;
    }
    
    virtual RegisterExpressionBase* clone() const=0;
    
    virtual QDataStream& dump(QDataStream &ds, const ExBase &ex) const=0;
    virtual ExpressionBase load(QDataStream &ds) const=0;

private:
    
    /** The name of the class */
    QString nme;
};

/** Template class that provides the class-specific functionality of the 
    registra
    
    @author Christopher Woods
*/
template<class T>
class RegisterExpression : public RegisterExpressionBase
{
public:
    RegisterExpression() : RegisterExpressionBase(T().what())
    {
        Factory::registerExpression(*this);
    }
    
    RegisterExpression(const RegisterExpression<T> &other) : RegisterExpressionBase(other)
    {}
    
    ~RegisterExpression()
    {}
    
    /** Dump the expression to a binary datastream */
    QDataStream& dump(QDataStream &ds, const ExBase &ex) const
    {
        ds << dynamic_cast<const T&>(ex);
        return ds;
    }

    /** Load the expression from a binary datastream and return 
        as an ExpressionBase */
    ExpressionBase load(QDataStream &ds) const
    {
        T ex;
        
        ds >> ex;
        
        return ExpressionBase(ex);
    }
    
    /** Return a clone of this registra */
    RegisterExpressionBase* clone() const
    {
        return new RegisterExpression<T>(*this);
    }
};

}

SIRE_END_HEADER

#endif
