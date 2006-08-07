#ifndef SIREDB_TABLEBASE_H
#define SIREDB_TABLEBASE_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
class TableBase;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::TableBase&);
QDataStream& operator>>(QDataStream&, SireDB::TableBase&);

namespace SireMol
{
class Molecule;
}

namespace SireDB
{

using SireMol::Molecule;

/**
This is the virtual base class of all of the parameter tables. This class is not useful on its own, but instead provides an interface for the low-level table classes.

@author Christopher Woods
*/
class SIREDB_EXPORT TableBase
{

friend QDataStream& ::operator<<(QDataStream&, const TableBase&);
friend QDataStream& ::operator>>(QDataStream&, TableBase&);

public:
    TableBase();
    TableBase(const TableBase &other);

    virtual ~TableBase();

    virtual TableBase* clone() const=0;
    virtual const char* what() const=0;
    
    virtual TableBase* create(const Molecule &molecule) const=0;
    
    virtual bool isCompatibleWith(const Molecule &molecule) const=0;
    
    virtual bool isEmpty() const=0;
    
    virtual void add(const TableBase &other)=0;
    
    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }
    
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }
    
    template<class T>
    T& asA()
    {
        return dynamic_cast<T&>(*this);
    }
};

}

SIRE_END_HEADER

#endif
