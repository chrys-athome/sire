
#include "registerexpression.h"
#include "factory.h"

using namespace SireCAS;

/** Constructor - this registers this Registra with the factory */
RegisterExpressionBase::RegisterExpressionBase(const char *name)
                       : nme( QString::fromLatin1(name) )
{}

/** Copy constructor */
RegisterExpressionBase::RegisterExpressionBase(const RegisterExpressionBase &other)
                       : nme(other.nme)
{}

/** Destructor */
RegisterExpressionBase::~RegisterExpressionBase()
{}
