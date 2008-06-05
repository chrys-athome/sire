#ifndef SIRECAS_FUNCTIONSIGNATURE_H
#define SIRECAS_FUNCTIONSIGNATURE_H

#include <QString>
#include <QSet>
#include <QDataStream>

#include "symbolvalue.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class FunctionSignature;
}

QDataStream& operator<<(QDataStream&, const SireCAS::FunctionSignature&);
QDataStream& operator>>(QDataStream&, SireCAS::FunctionSignature&);

namespace SireCAS
{

class Function;

/** This small class holds a signature for a function. This is ID that will uniquely
    ID a type of function, but not its current differentiation level.
    
    \author Christopher Woods
*/
class SIRECAS_EXPORT FunctionSignature
{
public:
    FunctionSignature()
    {}
    
    FunctionSignature(const QString &name)
             : _name(name)
    {}
    
    FunctionSignature(const QString &name, const QSet<SymbolID> &args)
             : _name(name), _args(args)
    {}
    
    FunctionSignature(const FunctionSignature &other)
             : _name(other._name), _args(other._args)
    {}
    
    ~FunctionSignature()
    {}
    
    void add(SymbolID id)
    {
        _args.insert(id);
    }
    
    void setName(const QString &name)
    {
        _name = name;
    }
    
    bool operator==(const FunctionSignature &other) const
    {
        return _name == other._name and _args == other._args;
    }
    
    bool operator!=(const FunctionSignature &other) const
    {
        return _name != other._name or _args != other._args;
    }

    bool contains(SymbolID id) const
    {
        return _args.contains(id);
    }

    QString name() const
    {
        return _name;
    }

    const QSet<SymbolID>& args() const
    {
        return _args;
    }

private:
    
    QString _name;
    QSet<SymbolID> _args;
};

}

/** Return a hash for this signature */
inline uint qHash(const SireCAS::FunctionSignature &sig)
{
    return qHash(sig.name());
}

Q_DECLARE_METATYPE(SireCAS::FunctionSignature)

SIRE_END_HEADER

#endif
